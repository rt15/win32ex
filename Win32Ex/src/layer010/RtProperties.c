#include "layer010/RtProperties.h"

#include "layer004/RtBinarySearch.h"
#include "layer004/RtChar.h"
#include "layer006/RtEncoding.h"
#include "layer008/RtSmallFile.h"
#include "layer009/RtSortableArray.h"

RT_B RT_CALL RtComputePropertiesEntries(RT_PROPERTY_ENTRY** lpPropertiesEntries, RT_CHAR* lpData)
{
  RT_PROPERTY_ENTRY* lpPropertyEntry;
  RT_CHAR* lpKey;
  RT_CHAR* lpValue;
  RT_N nCurrentIndex;
  RT_CHAR nCurrentChar;
  RT_N nContext; /* 0 = in end of lines, 1 = in key, 2 = in separator, 3 = in value. */
  RT_B bResult;

  bResult = RT_FALSE;

  nCurrentIndex = 0;
  nContext = 0;
  while (lpData[nCurrentIndex])
  {
    nCurrentChar = lpData[nCurrentIndex];
    if (nContext == 0)
    {
      /* Manage end of lines. */
      switch (nCurrentChar)
      {
        case _R('\r'):
        case _R('\n'):
        case _R(' '):
          /* Skip all end of line until something interesting is found. */
          lpData[nCurrentIndex] = 0;
          break;
        case _R('='):
          nContext = 2;
          lpData[nCurrentIndex] = 0;
          lpKey = &lpData[nCurrentIndex];
          break;
        default:
          nContext = 1;
          lpKey = &lpData[nCurrentIndex];
      }
    }
    else if (nContext == 1)
    {
      /* Manage key and spaces until '='. */
      switch (nCurrentChar)
      {
        case _R('='):
          nContext = 2;
        case _R(' '):
          lpData[nCurrentIndex] = 0;
          break;
        case _R('\n'):
        case _R('\r'):
          if (!RtNewArrayItem((void**)lpPropertiesEntries, (void**)&lpPropertyEntry))
          {
            goto the_end;
          }
          /* Fill the new entry. */
          lpData[nCurrentIndex] = 0;
          lpValue = &lpData[nCurrentIndex];
          lpPropertyEntry->nHash = RtComputeStringHash(lpKey);
          lpPropertyEntry->lpKey = lpKey;
          lpPropertyEntry->lpValue = lpValue;
          nContext = 0;
          break;
      }
    }
    else if (nContext == 2)
    {
      /* Manage spaces between '=' and value. */
      switch (nCurrentChar)
      {
        case _R(' '):
          /* Skip all spaces after '='. */
          lpData[nCurrentIndex] = 0;
          break;
        case _R('\r'):
        case _R('\n'):
          lpData[nCurrentIndex] = 0;
        default:
          /* Beginning of the value. */
          lpValue = &lpData[nCurrentIndex];
          nContext = 3;
      }
    }
    else if (nContext == 3)
    {
      /* Manage value. */
      switch (nCurrentChar)
      {
        case _R('\r'):
        case _R('\n'):
          if (!RtNewArrayItem((void**)lpPropertiesEntries, (void**)&lpPropertyEntry))
          {
            goto the_end;
          }
          /* Fill the new entry. */
          lpPropertyEntry->nHash = RtComputeStringHash(lpKey);
          lpPropertyEntry->lpKey = lpKey;
          lpPropertyEntry->lpValue = lpValue;
          lpData[nCurrentIndex] = 0;
          nContext = 0;
          break;
      }
    }

    nCurrentIndex++;
  }

  switch (nContext)
  {
    case 1:
    case 2:
      lpValue = &lpData[nCurrentIndex];
    case 3:
      if (!RtNewArrayItem((void**)lpPropertiesEntries, (void**)&lpPropertyEntry))
      {
        goto the_end;
      }
      /* Fill the new entry. */
      lpPropertyEntry->nHash = RtComputeStringHash(lpKey);
      lpPropertyEntry->lpKey = lpKey;
      lpPropertyEntry->lpValue = lpValue;
      break;
  }

  bResult = RtSortSortableArray(*lpPropertiesEntries);
the_end:
  return bResult;
}

RT_B RT_CALL RtCreatePropertiesFromString(RT_PROPERTIES* lpProperties, RT_CHAR* lpData, RT_HEAP** lpHeap)
{
  RT_PROPERTY_ENTRY* lpPropertiesEntries;
  RT_B bResult;

  if (!RtCreateSortableArray((void**)&lpPropertiesEntries, lpHeap, 0, sizeof(RT_PROPERTY_ENTRY), &RtDefaultCompareCallback, RT_NULL))
  {
    bResult = RT_FALSE;
    goto the_end;
  }

  if (RtComputePropertiesEntries(&lpPropertiesEntries, lpData))
  {
    /* Fill the structure. */
    lpProperties->lpPropertiesEntries = lpPropertiesEntries;
    lpProperties->lpData = lpData;
    lpProperties->lpHeap = lpHeap;
    bResult = RT_TRUE;
  }
  else
  {
    RtFreeArray((void**)&lpPropertiesEntries);
    bResult = RT_FALSE;
  }

the_end:
  return bResult;
}

RT_B RT_CALL RtCreatePropertiesFromBuffer(RT_PROPERTIES* lpProperties, RT_CHAR8* lpBuffer, RT_N nBufferSize, RT_N nEncoding, RT_HEAP** lpHeap)
{
  RT_CHAR* lpData;
  RT_B bResult;

  bResult = RT_FALSE;

  if (RtDecodeWithHeap(lpBuffer, nBufferSize, nEncoding, &lpData, lpHeap) == -1)
  {
    goto the_end;
  }

  bResult = RtCreatePropertiesFromString(lpProperties, lpData, lpHeap);
  if (!bResult)
  {
    (*lpHeap)->lpFree(lpHeap, (void**)&lpData);
  }
the_end:
  return bResult;
}

RT_B RT_API RtCreateProperties(RT_PROPERTIES* lpProperties, RT_CHAR* lpFilePath, RT_N nEncoding, RT_HEAP** lpHeap)
{
  RT_CHAR8* lpFileContent;
  RT_N nFileSize;
  RT_B bResult;

  bResult = RT_FALSE;

  nFileSize = RtReadFromSmallFile(lpFilePath, &lpFileContent, lpHeap);
  if (nFileSize == -1)
  {
    goto the_end;
  }

  bResult = RtCreatePropertiesFromBuffer(lpProperties, lpFileContent, nFileSize, nEncoding, lpHeap);

  bResult = bResult && (*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent);
the_end:
  return bResult;
}

RT_CHAR* RT_API RtGetStringProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_CHAR* lpDefaultValue)
{
  RT_N nHash;
  RT_PROPERTY_ENTRY* lpPropertiesEntries;
  RT_N nIndex;
  RT_UN32 unSize;
  RT_CHAR* lpResult;

  lpResult = lpDefaultValue;

  nHash = RtComputeStringHash(lpKey);
  lpPropertiesEntries = lpProperties->lpPropertiesEntries;

  unSize = RtGetArraySize(lpPropertiesEntries);

  /* No need to check result as the comparison function cannot trigger error. */
  RtSearchSortableArrayItemIndex(lpPropertiesEntries, &nHash, &nIndex);
  if (nIndex != -1)
  {
    /* Find the first item with the given hash. */
    while (nIndex > 0 && (lpPropertiesEntries[nIndex - 1].nHash == nHash))
    {
      nIndex--;
    }

    /* Find the right item by checking the key. */
    while ((nIndex < RT_TYPE_MAX_N) && (nIndex < unSize) && (lpPropertiesEntries[nIndex].nHash == nHash))
    {
      if (!RtCompareStrings(lpPropertiesEntries[nIndex].lpKey, lpKey))
      {
        lpResult = lpPropertiesEntries[nIndex].lpValue;
        break;
      }
      nIndex++;
    }
  }

  return lpResult;
}

RT_N RT_API RtGetNumberProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_N nDefaultValue)
{
  RT_CHAR* lpStringProperty;
  RT_N nResult;

  lpStringProperty = RtGetStringProperty(lpProperties, lpKey, RT_NULL);
  if (!lpStringProperty) goto handle_error;
  if (!RtConvertStringToNumber(lpStringProperty, &nResult)) goto handle_error;

  goto free_resources;
handle_error:
  nResult = nDefaultValue;
free_resources:
  return nResult;
}

RT_B RT_API RtGetBooleanProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_B bDefaultValue)
{
  RT_CHAR* lpStringProperty;
  RT_CHAR lpLowerCaseValue[8];
  RT_N nWritten;
  RT_B bResult;

  lpStringProperty = RtGetStringProperty(lpProperties, lpKey, RT_NULL);
  if (!lpStringProperty) goto handle_error;
  if (RtGetStringSize(lpStringProperty) > 4)
  {
    bResult = RT_FALSE;
    goto free_resources;
  }
  if (!RtCopyString(lpStringProperty, lpLowerCaseValue, 8, &nWritten)) goto handle_error;
  RtFastLowerString(lpLowerCaseValue);
  bResult = (!RtCompareStrings(lpLowerCaseValue, _R("true")) ||
             !RtCompareStrings(lpLowerCaseValue, _R("yes")) ||
             !RtCompareStrings(lpLowerCaseValue, _R("y")) ||
             !RtCompareStrings(lpLowerCaseValue, _R("t")) ||
             !RtCompareStrings(lpLowerCaseValue, _R("1"))
            );

  goto free_resources;
handle_error:
  bResult = bDefaultValue;
free_resources:
  return bResult;
}

RT_B RT_API RtFreeProperties(RT_PROPERTIES* lpProperties)
{
  RT_HEAP** lpHeap;
  RT_CHAR* lpData;
  RT_B bResult;

  bResult = RtFreeArray((void**)&lpProperties->lpPropertiesEntries);
  lpHeap = lpProperties->lpHeap;
  lpData = lpProperties->lpData;

  bResult = bResult && (*lpHeap)->lpFree(lpHeap, (void**)&lpData);

  return bResult;
}
