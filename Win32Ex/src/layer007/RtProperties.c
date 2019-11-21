#include "layer007/RtProperties.h"

#include "layer002/RtBinarySearch.h"
#include "layer003/RtChar.h"
#include "layer004/RtEncoding.h"
#include "layer005/RtSortableArray.h"
#include "layer006/RtSmallFile.h"

RT_B RT_CALL RtProperties_GetEntries(RT_PROPERTY_ENTRY** lpPropertiesEntries, RT_CHAR* lpData)
{
  RT_PROPERTY_ENTRY* lpPropertyEntry;
  RT_CHAR* lpKey;
  RT_CHAR* lpValue;
  RT_UN unCurrentIndex;
  RT_CHAR nCurrentChar;
  RT_UN unContext; /* 0 = in end of lines, 1 = in key, 2 = in separator, 3 = in value. */
  RT_B bResult;

  lpKey = RT_NULL;
  lpValue = RT_NULL;

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
          if (!RtArray_NewItem((void**)lpPropertiesEntries, (void**)&lpPropertyEntry))
          {
            goto the_end;
          }
          /* Fill the new entry. */
          lpData[unCurrentIndex] = 0;
          lpValue = &lpData[unCurrentIndex];
          lpPropertyEntry->unHash = RtChar_GetStringHash(lpKey);
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
          if (!RtArray_NewItem((void**)lpPropertiesEntries, (void**)&lpPropertyEntry))
          {
            goto the_end;
          }
          /* Fill the new entry. */
          lpPropertyEntry->unHash = RtChar_GetStringHash(lpKey);
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
      if (!RtArray_NewItem((void**)lpPropertiesEntries, (void**)&lpPropertyEntry))
      {
        goto the_end;
      }
      /* Fill the new entry. */
      lpPropertyEntry->unHash = RtChar_GetStringHash(lpKey);
      lpPropertyEntry->lpKey = lpKey;
      lpPropertyEntry->lpValue = lpValue;
      break;
  }

  bResult = RtSortableArray_Sort(*lpPropertiesEntries);
the_end:
  return bResult;
}

RT_B RT_CALL RtProperties_CreateFromString(RT_PROPERTIES* lpProperties, RT_CHAR* lpData, RT_HEAP** lpHeap)
{
  RT_PROPERTY_ENTRY* lpPropertiesEntries;
  RT_B bResult;

  if (!RtSortableArray_Create((void**)&lpPropertiesEntries, lpHeap, 0, sizeof(RT_PROPERTY_ENTRY), &RtComparison_DefaultCallback, RT_NULL))
  {
    bResult = RT_FAILURE;
    goto the_end;
  }

  if (RtProperties_GetEntries(&lpPropertiesEntries, lpData))
  {
    /* Fill the structure. */
    lpProperties->lpPropertiesEntries = lpPropertiesEntries;
    lpProperties->lpData = lpData;
    lpProperties->lpHeap = lpHeap;
    bResult = RT_SUCCESS;
  }
  else
  {
    RtArray1337_Free((void**)&lpPropertiesEntries);
    bResult = RT_FAILURE;
  }

the_end:
  return bResult;
}

RT_B RT_CALL RtProperties_CreateFromBuffer(RT_PROPERTIES* lpProperties, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN unEncoding, RT_HEAP** lpHeap)
{
  RT_CHAR* lpData;
  RT_B bResult;

  bResult = RT_FAILURE;

  if (RtEncoding_DecodeWithHeap(lpBuffer, unBufferSize, unEncoding, &lpData, lpHeap) == RT_TYPE_MAX_UN)
  {
    goto the_end;
  }

  bResult = RtProperties_CreateFromString(lpProperties, lpData, lpHeap);
  if (!bResult)
  {
    (*lpHeap)->lpFree(lpHeap, (void**)&lpData);
  }
the_end:
  return bResult;
}

RT_B RT_API RtProperties_Create(RT_PROPERTIES* lpProperties, RT_CHAR* lpFilePath, RT_UN unEncoding, RT_HEAP** lpHeap)
{
  RT_CHAR8* lpFileContent;
  RT_UN unFileSize;
  RT_B bResult;

  bResult = RT_FAILURE;

  unFileSize = RtSmallFile_Read(lpFilePath, &lpFileContent, lpHeap);
  if (unFileSize == RT_TYPE_MAX_UN)
  {
    goto the_end;
  }

  bResult = RtProperties_CreateFromBuffer(lpProperties, lpFileContent, unFileSize, unEncoding, lpHeap);

  bResult = bResult && (*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent);
the_end:
  return bResult;
}

RT_CHAR* RT_API RtProperties_GetString(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_CHAR* lpDefaultValue)
{
  RT_UN unHash;
  RT_PROPERTY_ENTRY* lpPropertiesEntries;
  RT_UN unIndex;
  RT_UN unSize;
  RT_CHAR* lpResult;

  lpResult = lpDefaultValue;

  unHash = RtChar_GetStringHash(lpKey);
  lpPropertiesEntries = lpProperties->lpPropertiesEntries;

  unSize = RtArray_GetSize(lpPropertiesEntries);

  /* No need to check result as the comparison function cannot trigger error. */
  RtSortableArray_SearchItemIndex(lpPropertiesEntries, &unHash, &unIndex);
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
      if (!RtChar1337_CompareStrings(lpPropertiesEntries[unIndex].lpKey, lpKey))
      {
        lpResult = lpPropertiesEntries[unIndex].lpValue;
        break;
      }
      unIndex++;
    }
  }

  return lpResult;
}

RT_UN RT_API RtProperties_GetUnsignedInteger(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_UN unDefaultValue)
{
  RT_ARRAY rtString;
  RT_CHAR* lpStringProperty;
  RT_UN unResult;

  lpStringProperty = RtProperties_GetString(lpProperties, lpKey, RT_NULL);
  if (!lpStringProperty) goto handle_error;
  if (!RtChar_ConvertStringToUnsignedInteger(RtChar_CreateString(&rtString, lpStringProperty), &unResult)) goto handle_error;

  goto free_resources;
handle_error:
  unResult = unDefaultValue;
free_resources:
  return unResult;
}

RT_B RT_API RtProperties_GetBoolean(RT_PROPERTIES* lpProperties, RT_CHAR* lpKey, RT_B bDefaultValue)
{
  RT_CHAR* lpStringProperty;
  RT_CHAR lpLowerCaseValue[8];
  RT_UN unOutputSize;
  RT_B bResult;

  lpStringProperty = RtProperties_GetString(lpProperties, lpKey, RT_NULL);
  if (!lpStringProperty) goto handle_error;
  if (RtChar_GetCStringSize(lpStringProperty) > 4)
  {
    bResult = RT_FALSE;
    goto free_resources;
  }
  if (!RtChar_CopyString(lpStringProperty, lpLowerCaseValue, 8, &unOutputSize)) goto handle_error;
  RtChar_FastLowerString(lpLowerCaseValue);
  bResult = (!RtChar1337_CompareStrings(lpLowerCaseValue, _R("true")) ||
             !RtChar1337_CompareStrings(lpLowerCaseValue, _R("yes")) ||
             !RtChar1337_CompareStrings(lpLowerCaseValue, _R("y")) ||
             !RtChar1337_CompareStrings(lpLowerCaseValue, _R("t")) ||
             !RtChar1337_CompareStrings(lpLowerCaseValue, _R("1"))
            );

  goto free_resources;
handle_error:
  bResult = bDefaultValue;
free_resources:
  return bResult;
}

RT_B RT_API RtProperties_Free(RT_PROPERTIES* lpProperties)
{
  RT_HEAP** lpHeap;
  RT_CHAR* lpData;
  RT_B bResult;

  bResult = RtArray1337_Free((void**)&lpProperties->lpPropertiesEntries);
  lpHeap = lpProperties->lpHeap;
  lpData = lpProperties->lpData;

  bResult = bResult && (*lpHeap)->lpFree(lpHeap, (void**)&lpData);

  return bResult;
}
