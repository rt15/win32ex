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
  RT_UN unCurrentIndex;
  RT_CHAR nCurrentChar;
  RT_UN unContext; /* 0 = in end of lines, 1 = in key, 2 = in separator, 3 = in value. */
  RT_B bResult;

  bResult = RT_FAILURE;

  unCurrentIndex = 0;
  unContext = 0;
  while (lpData[unCurrentIndex])
  {
    nCurrentChar = lpData[unCurrentIndex];
    if (unContext == 0)
    {
      /* Manage end of lines. */
      switch (nCurrentChar)
      {
        case _R('\r'):
        case _R('\n'):
        case _R(' '):
          /* Skip all end of line until something interesting is found. */
          lpData[unCurrentIndex] = 0;
          break;
        case _R('='):
          unContext = 2;
          lpData[unCurrentIndex] = 0;
          lpKey = &lpData[unCurrentIndex];
          break;
        default:
          unContext = 1;
          lpKey = &lpData[unCurrentIndex];
      }
    }
    else if (unContext == 1)
    {
      /* Manage key and spaces until '='. */
      switch (nCurrentChar)
      {
        case _R('='):
          unContext = 2;
        case _R(' '):
          lpData[unCurrentIndex] = 0;
          break;
        case _R('\n'):
        case _R('\r'):
          if (!RtNewArrayItem((void**)lpPropertiesEntries, (void**)&lpPropertyEntry))
          {
            goto the_end;
          }
          /* Fill the new entry. */
          lpData[unCurrentIndex] = 0;
          lpValue = &lpData[unCurrentIndex];
          lpPropertyEntry->unHash = RtComputeStringHash(lpKey);
          lpPropertyEntry->lpKey = lpKey;
          lpPropertyEntry->lpValue = lpValue;
          unContext = 0;
          break;
      }
    }
    else if (unContext == 2)
    {
      /* Manage spaces between '=' and value. */
      switch (nCurrentChar)
      {
        case _R(' '):
          /* Skip all spaces after '='. */
          lpData[unCurrentIndex] = 0;
          break;
        case _R('\r'):
        case _R('\n'):
          lpData[unCurrentIndex] = 0;
        default:
          /* Beginning of the value. */
          lpValue = &lpData[unCurrentIndex];
          unContext = 3;
      }
    }
    else if (unContext == 3)
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
          lpPropertyEntry->unHash = RtComputeStringHash(lpKey);
          lpPropertyEntry->lpKey = lpKey;
          lpPropertyEntry->lpValue = lpValue;
          lpData[unCurrentIndex] = 0;
          unContext = 0;
          break;
      }
    }

    unCurrentIndex++;
  }

  switch (unContext)
  {
    case 1:
    case 2:
      lpValue = &lpData[unCurrentIndex];
    case 3:
      if (!RtNewArrayItem((void**)lpPropertiesEntries, (void**)&lpPropertyEntry))
      {
        goto the_end;
      }
      /* Fill the new entry. */
      lpPropertyEntry->unHash = RtComputeStringHash(lpKey);
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
    bResult = RT_FAILURE;
    goto the_end;
  }

  if (RtComputePropertiesEntries(&lpPropertiesEntries, lpData))
  {
    /* Fill the structure. */
    lpProperties->lpPropertiesEntries = lpPropertiesEntries;
    lpProperties->lpData = lpData;
    lpProperties->lpHeap = lpHeap;
    bResult = RT_SUCCESS;
  }
  else
  {
    RtFreeArray((void**)&lpPropertiesEntries);
    bResult = RT_FAILURE;
  }

the_end:
  return bResult;
}

RT_B RT_CALL RtCreatePropertiesFromBuffer(RT_PROPERTIES* lpProperties, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN unEncoding, RT_HEAP** lpHeap)
{
  RT_CHAR* lpData;
  RT_B bResult;

  bResult = RT_FAILURE;

  if (RtDecodeWithHeap(lpBuffer, unBufferSize, unEncoding, &lpData, lpHeap) == RT_TYPE_MAX_UN)
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

RT_B RT_API RtCreateProperties(RT_PROPERTIES* lpProperties, RT_CHAR* lpFilePath, RT_UN unEncoding, RT_HEAP** lpHeap)
{
  RT_CHAR8* lpFileContent;
  RT_UN unFileSize;
  RT_B bResult;

  bResult = RT_FAILURE;

  unFileSize = RtReadFromSmallFile(lpFilePath, &lpFileContent, lpHeap);
  if (unFileSize == RT_TYPE_MAX_UN)
  {
    goto the_end;
  }

  bResult = RtCreatePropertiesFromBuffer(lpProperties, lpFileContent, unFileSize, unEncoding, lpHeap);

  bResult = bResult && (*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent);
the_end:
  return bResult;
}

RT_CHAR* RT_API RtGetStringProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_CHAR* lpDefaultValue)
{
  RT_UN unHash;
  RT_PROPERTY_ENTRY* lpPropertiesEntries;
  RT_UN unIndex;
  RT_UN unSize;
  RT_CHAR* lpResult;

  lpResult = lpDefaultValue;

  unHash = RtComputeStringHash(lpKey);
  lpPropertiesEntries = lpProperties->lpPropertiesEntries;

  unSize = RtGetArraySize(lpPropertiesEntries);

  /* No need to check result as the comparison function cannot trigger error. */
  RtSearchSortableArrayItemIndex(lpPropertiesEntries, &unHash, &unIndex);
  if (unIndex != RT_TYPE_MAX_UN)
  {
    /* Find the first item with the given hash. */
    while (unIndex > 0 && (lpPropertiesEntries[unIndex - 1].unHash == unHash))
    {
      unIndex--;
    }

    /* Find the right item by checking the key. */
    while ((unIndex < RT_TYPE_MAX_N) && (unIndex < unSize) && (lpPropertiesEntries[unIndex].unHash == unHash))
    {
      if (!RtCompareStrings(lpPropertiesEntries[unIndex].lpKey, lpKey))
      {
        lpResult = lpPropertiesEntries[unIndex].lpValue;
        break;
      }
      unIndex++;
    }
  }

  return lpResult;
}

RT_N RT_API RtGetIntegerProperty(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_N nDefaultValue)
{
  RT_CHAR* lpStringProperty;
  RT_N nResult;

  lpStringProperty = RtGetStringProperty(lpProperties, lpKey, RT_NULL);
  if (!lpStringProperty) goto handle_error;
  if (!RtConvertStringToInteger(lpStringProperty, &nResult)) goto handle_error;

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
  RT_UN unWritten;
  RT_B bResult;

  lpStringProperty = RtGetStringProperty(lpProperties, lpKey, RT_NULL);
  if (!lpStringProperty) goto handle_error;
  if (RtGetStringSize(lpStringProperty) > 4)
  {
    bResult = RT_FALSE;
    goto free_resources;
  }
  if (!RtCopyString(lpStringProperty, lpLowerCaseValue, 8, &unWritten)) goto handle_error;
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
