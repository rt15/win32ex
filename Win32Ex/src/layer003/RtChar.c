#include "layer003/RtChar.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer001/RtMemory.h"
#include "layer002/RtError.h"

RT_ARRAY* RT_API RtChar_CreateString(RT_ARRAY* lpString, RT_CHAR* lpCString)
{
  lpString->unGenericTypeSize = sizeof(RT_CHAR);
  lpString->lpData = lpCString;
  lpString->unCapacity = RtChar_GetCStringSize(lpCString);
  lpString->unSize = lpString->unCapacity;
  lpString->unFlags = 0;
  return lpString;
}

RT_ARRAY* RT_API RtChar_CreateStringWithSize(RT_ARRAY* lpString, RT_CHAR* lpCString, RT_UN unSize)
{
  lpString->unGenericTypeSize = sizeof(RT_CHAR);
  lpString->lpData = lpCString;
  lpString->unCapacity = unSize;
  lpString->unSize = unSize;
  lpString->unFlags = 0;
  return lpString;
}

RT_UN RT_API RtChar_GetStringSize(RT_ARRAY* lpString)
{
  return lpString->unSize;
}

RT_B RT_API RtChar_CopyCString(RT_ARRAY* lpString, RT_CHAR* lpCString)
{
  RT_ARRAY rtString;

  lpString->unSize = 0;
  return RtArray_Append(lpString, RtChar_CreateString(&rtString, lpCString));
}

RT_B RT_API RtChar_AppendCString(RT_ARRAY* lpString, RT_CHAR* lpCString)
{
  RT_ARRAY rtString;

  return RtArray_Append(lpString, RtChar_CreateString(&rtString, lpCString));
}

RT_B RT_API RtChar_Append(RT_ARRAY* lpString, RT_CHAR nChar)
{
  RT_UN unSize;
  RT_CHAR* lpBuffer;
  RT_B bResult;

  unSize = lpString->unSize;
  if (!RtArray_SetSize(lpString, unSize + 1)) goto handle_error;
  lpBuffer = (RT_CHAR*)lpString->lpData;
  lpBuffer[unSize] = nChar;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_UN RT_API RT_API RtChar_GetCStringSize(RT_CHAR* lpInput)
{
  RT_CHAR* lpInInput;
  RT_UN unResult;

  /* TODO: Use strlen in case of Linux? */

  lpInInput = lpInput;

  while (*lpInInput++);

  unResult = (RT_UN)(lpInInput - lpInput - 1);

  return unResult;
}

RT_UN RT_API RtChar_SearchString(RT_ARRAY* lpString, RT_ARRAY* lpSearched)
{
  RT_CHAR* lpStringChars;
  RT_CHAR* lpSearchedChars;
  RT_UN unI, unJ, unK;
  RT_UN unResult;

  unResult = RT_TYPE_MAX_UN;

  if (lpSearched->unSize)
  {
    lpStringChars = (RT_CHAR*)lpString->lpData;
    lpSearchedChars = (RT_CHAR*)lpSearched->lpData;

    for (unI = 0; unI < lpString->unSize; unI++)
    {
      if (lpStringChars[unI] == lpSearchedChars[0])
      {
        for (unJ = 1; unJ < lpSearched->unSize; unJ++)
        {
          unK = unI + unJ;
          if (unK >= lpString->unSize)
          {
            break;
          }
          if (lpStringChars[unK] != lpSearchedChars[unJ])
          {
            break;
          }
        }
        if (unJ == lpSearched->unSize)
        {
          unResult = unI;
          break;
        }
      }
    }
  }

  return unResult;
}

RT_UN RT_API RtChar_Search(RT_ARRAY* lpString, RT_CHAR nSearched)
{
  RT_CHAR* lpStringChars;
  RT_UN unResult;

  lpStringChars = lpString->lpData;
  for (unResult = 0; unResult < lpString->unSize; unResult++)
  {
    if (lpStringChars[unResult] == nSearched)
    {
      break;
    }
  }
  if (unResult == lpString->unSize)
  {
    unResult = RT_TYPE_MAX_UN;
  }
  return unResult;
}

RT_B RT_API RtChar_StringEqualsString(RT_ARRAY* lpString, RT_ARRAY* lpOtherString)
{
  RT_B bResult;
  if (lpString->unSize != lpOtherString->unSize)
  {
    bResult = RT_FALSE;
  }
  else
  {
    /* RT_MEMORY_COMPARE returns an INT32 so it is safe to put the negation in an RT_B. */
    bResult = !RT_MEMORY_COMPARE(lpString->lpData, lpOtherString->lpData, lpString->unSize * sizeof(RT_CHAR));
  }

  return bResult;
}

RT_B RT_API RtChar_StringEqualsCString(RT_ARRAY* lpString, RT_CHAR* lpCString)
{
  RT_ARRAY rtOtherString;

  return RtChar_StringEqualsString(lpString, RtChar_CreateString(&rtOtherString, lpCString));
}

RT_B RT_API RtChar_ConvertIntegerToString(RT_N nInput, RT_ARRAY* lpString)
{
  RT_CHAR* lpCString;

  lpCString = (RT_CHAR*)lpString->lpData;

  /* TODO: Better implementation. Possible buffer overflow with current implementation. */

#ifdef RT_DEFINE_WINDOWS
  wsprintf(lpCString, _R("%d"), nInput);
#else
  sprintf(lpCString, "%ld", nInput);
#endif
  lpString->unSize = RtChar_GetCStringSize(lpCString);
  return RT_TRUE;
}

RT_B RT_API RtChar_ConvertUnsignedIntegerToString(RT_UN unInput, RT_ARRAY* lpString)
{
  /* TODO: Specific implementation. */
  return RtChar_ConvertIntegerToString(unInput, lpString);
}

/* TODO: Watch out for overflows!? */
RT_B RT_API RtChar_ConvertStringToInteger(RT_ARRAY* lpInput, RT_N* lpResult)
{
  RT_CHAR cChar;
  RT_UN unI;
  RT_N nResult;
  RT_B bNegative;
  RT_B bResult;

  nResult = 0;

  if (lpInput->unSize == 0)
  {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }

  bNegative = (((RT_CHAR*)lpInput->lpData)[0] == _R('-'));
  if (bNegative && lpInput->unSize == 1)
  {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }

  for (unI = (bNegative) ? 1 : 0; unI < lpInput->unSize; unI++)
  {
    cChar = ((RT_CHAR*)lpInput->lpData)[unI];

    if ((cChar < _R('0')) || (cChar > _R('9')))
    {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
    else
    {
      nResult = nResult * 10 + cChar - _R('0');
    }
  }
  if (bNegative)
  {
    nResult = -nResult;
  }
  *lpResult = nResult;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

/* TODO: Watch out for overflows!? */
RT_B RT_API RtChar_ConvertStringToUnsignedInteger(RT_ARRAY* lpInput, RT_UN* lpResult)
{
  RT_CHAR cChar;
  RT_UN unI;
  RT_UN unResult;
  RT_B bResult;

  unResult = 0;

  if (lpInput->unSize == 0)
  {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }

  for (unI = 0; unI < lpInput->unSize; unI++)
  {
    cChar = ((RT_CHAR*)lpInput->lpData)[unI];

    if ((cChar < _R('0')) || (cChar > _R('9')))
    {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
    else
    {
      unResult = unResult * 10 + cChar - _R('0');
    }
  }
  *lpResult = unResult;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_N RT_API RtChar_CompareStrings(RT_ARRAY* lpString1, RT_ARRAY* lpString2)
{
  RT_UCHAR* lpChars1;
  RT_UCHAR* lpChars2;
  RT_UN unI;
  RT_N nResult;

  lpChars1 = (RT_UCHAR*)lpString1->lpData;
  lpChars2 = (RT_UCHAR*)lpString2->lpData;

  for (unI = 0; unI < lpString1->unSize; unI++)
  {
    /* We found the end of string2, lpString1[unI] - "0" = 1. */
    if (unI >= lpString2->unSize)
    {
      nResult = 1;
      break;
    }
    nResult = lpChars1[unI] - lpChars2[unI];
    if (nResult)
    {
      break;
    }
  }
  if (unI == lpString1->unSize)
  {
    if (unI == lpString2->unSize)
    {
      nResult = 0;
    }
    else
    {
      /* We reached the end of lpString1 but not the end of lpString2. */
      nResult = -1;
    }
  }
  return nResult;
}

RT_N RT_API RtChar_CompareCStrings(RT_CHAR* lpString1, RT_CHAR* lpString2)
{
  RT_N nResult;

  nResult = 0;

  while(!(nResult = *(RT_UCHAR*)lpString1 - *(RT_UCHAR*)lpString2) && *lpString2)
  {
    lpString1++;
    lpString2++;
  }

  return nResult;
}

RT_N RT_API RtChar1337_CompareStrings(RT_CHAR* lpString1, RT_CHAR* lpString2)
{
  RT_N nResult;

  nResult = 0;

  while(!(nResult = *(RT_UCHAR*)lpString1 - *(RT_UCHAR*)lpString2) && *lpString2)
  {
    lpString1++;
    lpString2++;
  }

  return nResult;
}

RT_N RT_API RtChar_CompareStringsWithSize(RT_CHAR* lpString1, RT_CHAR* lpString2, RT_UN unSize)
{
  RT_UN unI;
  RT_N nResult;

  for (unI = 0; unI < unSize; unI++)
  {
    nResult = ((RT_UCHAR)lpString1[unI]) - ((RT_UCHAR)lpString2[unI]);
    if (nResult) break;
  }

  return nResult;
}

RT_N RT_API RtChar_CompareNullStrings(RT_CHAR* lpString1, RT_CHAR* lpString2)
{
  RT_N nResult;

  if (!lpString1)
  {
    if (!lpString2)
    {
      nResult = 0;
    }
    else
    {
      nResult = 1;
    }
  }
  else
  {
    if (!lpString2)
    {
      nResult = -1;
    }
    else
    {
      while(!(nResult = *(RT_UCHAR*)lpString1 - *(RT_UCHAR*)lpString2) && *lpString2)
      {
        lpString1++;
        lpString2++;
      }
    }
  }

  return nResult;
}

RT_UN RT_API RtChar1337_SearchString(RT_CHAR* lpString, RT_CHAR* lpSearched)
{
  RT_CHAR nChar;
  RT_CHAR nSearched;
  RT_UN unInSearched;
  RT_UN unI, unJ;
  RT_UN unResult;

  unResult = RT_TYPE_MAX_UN;
  unI = 0;
  while (1)
  {
    nChar = lpString[unI];
    if (!nChar)
    {
      break;
    }
    if (lpString[unI] == lpSearched[0])
    {
      unInSearched = 1;
      unJ = unI + 1;
      while (1)
      {
        nSearched = lpSearched[unInSearched];
        if (!nSearched)
        {
          unResult = unI;
          break;
        }
        nChar = lpString[unJ];
        if ((!nChar) || (nChar != nSearched))
        {
          break;
        }
        unJ++;
        unInSearched++;
      }
      if (unResult != RT_TYPE_MAX_UN)
      {
        /* Searched found, breaking. */
        break;
      }
    }
    unI++;
  }

  return unResult;
}

RT_UN RT_API RtChar_CountStringOccurrences(RT_CHAR* lpString, RT_CHAR* lpSearched)
{
  RT_CHAR* lpInString;
  RT_UN unIndex;
  RT_UN unSearchedSize;
  RT_UN unResult;

  unResult = 0;
  lpInString = lpString;
  unSearchedSize = RtChar_GetCStringSize(lpSearched);
  do
  {
    unIndex = RtChar1337_SearchString(lpInString, lpSearched);
    if (unIndex != RT_TYPE_MAX_UN)
    {
      unResult++;
      lpInString = &lpInString[unIndex + unSearchedSize];
    }
  }
  while (unIndex != RT_TYPE_MAX_UN);

  return unResult;
}

RT_B RT_API RtChar_ReplaceString(RT_CHAR* lpString, RT_UN unStringSize,
                                 RT_CHAR* lpSearched, RT_UN unSearchedSize,
                                 RT_CHAR* lpReplacement, RT_UN unReplacementSize,
                                 RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unIndex;
  RT_UN unOutputSize;
  RT_UN unInString;
  RT_B bResult;

  unInString = 0;
  *lpOutputSize = 0;
  while (1)
  {
    unIndex = RtChar1337_SearchString(&lpString[unInString], lpSearched);
    if (unIndex == RT_TYPE_MAX_UN) break;
    /* Copy all characters until found string. */
    if (!RtChar_CopyStringWithSize(&lpString[unInString], unIndex,   &lpBuffer[*lpOutputSize], unBufferSize - *lpOutputSize, &unOutputSize)) goto handle_error; *lpOutputSize += unOutputSize;
    /* Copy replacement string. */
    if (!RtChar_CopyStringWithSize(lpReplacement, unReplacementSize, &lpBuffer[*lpOutputSize], unBufferSize - *lpOutputSize, &unOutputSize)) goto handle_error; *lpOutputSize += unOutputSize;

    /* Continue reading the input String. */
    unInString += unIndex + unSearchedSize;
  }

  /* Copy remaining characters. */
  if (!RtChar_CopyStringWithSize(&lpString[unInString], unStringSize - unInString, &lpBuffer[*lpOutputSize], unBufferSize - *lpOutputSize, &unOutputSize)) goto handle_error; *lpOutputSize += unOutputSize;

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_B RT_API RtChar_CopyStringWithSize(RT_CHAR* lpSource, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_B bResult;
  RT_UN unI;

  /* Manage negative size. */
  if (unSize < 0)
  {
    RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }

  /* Manage negative or zero buffer sizes. */
  if (unBufferSize <= 0)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  /* Copy only unBufferSize - 1 if the buffer is too small. */
  if (unSize > unBufferSize - 1)
  {
    bResult = RT_FAILURE;
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    unSize = unBufferSize - 1;
  }
  else
  {
    bResult = RT_SUCCESS;
  }

  /* Manage overlapping. */
  if (lpBuffer <= lpSource)
  {
    for (unI = 0; unI < unSize; unI++)
    {
      lpBuffer[unI] = lpSource[unI];
    }
  }
  else
  {
    for (unI = unSize - 1; unI < RT_TYPE_MAX_UN; unI--)
    {
      lpBuffer[unI] = lpSource[unI];
    }
  }
  lpBuffer[unSize] = 0;

  *lpOutputSize = unSize;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtChar_Copy(RT_CHAR nChar, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_B bResult;
  if (unBufferSize < 2)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  lpBuffer[0] = nChar;
  lpBuffer[1] = 0;
  *lpOutputSize = 1;

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_B RT_API RtChar_CopyString(RT_CHAR* lpSource, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unI;
  RT_B bResult;

  /* Manage negative or zero buffer sizes. */
  if (unBufferSize <= 0)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    *lpOutputSize = 0;
    goto handle_error;
  }

  /* Manage all characters until zero trailing character. */
  unI = 0;
  while (lpSource[unI])
  {
    if (unI >= unBufferSize)
    {
      RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
      lpBuffer[unBufferSize - 1] = 0;
      *lpOutputSize = unBufferSize - 1;
      goto handle_error;
    }
    lpBuffer[unI] = lpSource[unI];
    unI++;
  }

  /* Manage zero trailing character. */
  if (unI >= unBufferSize)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    lpBuffer[unBufferSize - 1] = 0;
    *lpOutputSize = unBufferSize - 1;
    goto handle_error;
  }
  lpBuffer[unI] = 0;

  *lpOutputSize = unI;
  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_UN RT_API RtChar_FastLowerString(RT_CHAR* lpString)
{
  RT_CHAR nCurrentChar;
  RT_UN unResult;

  unResult = 0;
  while (lpString[unResult])
  {
    nCurrentChar = lpString[unResult];
    if (nCurrentChar >= _R('A') && nCurrentChar <= _R('Z'))
    {
      lpString[unResult] = nCurrentChar + 32;
    }
    unResult++;
  }
  return unResult;
}

RT_CHAR RT_API RtChar_FastLower(RT_CHAR nChar)
{
  RT_CHAR nResult;

  if (nChar >= _R('A') && nChar <= _R('Z'))
  {
    nResult = nChar + 32;
  }
  else
  {
    nResult = nChar;
  }
  return nResult;
}

RT_UN RT_API RtChar_FastUpperString(RT_CHAR* lpString)
{
  RT_CHAR nCurrentChar;
  RT_UN unResult;

  unResult = 0;
  while (lpString[unResult])
  {
    nCurrentChar = lpString[unResult];
    if (nCurrentChar >= _R('a') && nCurrentChar <= _R('z'))
    {
      lpString[unResult] = nCurrentChar - 32;
    }
    unResult++;
  }
  return unResult;
}

RT_CHAR RT_API RtChar_FastUpper(RT_CHAR nChar)
{
  RT_CHAR nResult;

  if (nChar >= _R('a') && nChar <= _R('z'))
  {
    nResult = nChar - 32;
  }
  else
  {
    nResult = nChar;
  }
  return nResult;
}

RT_B RT_CDECL_API RtChar_ConcatStrings(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpOutputSize);
  bResult = RtChar_VConcatStrings(lpVaList, lpBuffer, unBufferSize, lpOutputSize);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_API RtChar_VConcatStrings(va_list lpVaList, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_CHAR* lpString;
  RT_UN unOutputSize;
  RT_B bResult;

  *lpOutputSize = 0;
  while (RT_TRUE)
  {
    lpString = va_arg(lpVaList, RT_CHAR*);
    if (lpString)
    {
      if (!RtChar_CopyString(lpString, &lpBuffer[*lpOutputSize], unBufferSize - *lpOutputSize, &unOutputSize)) goto handle_error; *lpOutputSize += unOutputSize;
    }
    else
    {
      break;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtChar_LeftPadString(RT_CHAR* lpInput, RT_UN unInputSize, RT_CHAR nChar, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unPaddingSize;
  RT_UN unI;
  RT_UN unOutputSize;
  RT_B bResult;

  if (unBufferSize <= 0)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  /* Should we add some padding characters? */
  if (unInputSize < unSize)
  {
    unPaddingSize = unSize - unInputSize;

    /* Copy lpInput after the padding characters. */
    RtChar_CopyStringWithSize(lpInput, unInputSize, &lpBuffer[unPaddingSize], unBufferSize - unPaddingSize, &unOutputSize);
    if (unPaddingSize > unBufferSize - 1)
    {
      unPaddingSize = unBufferSize - 1;
    }
    /* Add padding characters at the left of lpInput. Must be done in a second time to avoid overwriting. */
    for (unI = 0; unI < unPaddingSize; unI++)
    {
      lpBuffer[unI] = nChar;
    }
    /* RtChar_CopyStringWithSize might not have added the null termination. */
    if (unOutputSize == 0)
    {
      lpBuffer[unI] = 0;
    }
    unOutputSize = unOutputSize + unPaddingSize;
  }
  else
  {
    /* The result is the input. */
    if (lpBuffer == lpInput)
    {
      /* If the same buffer is used for input and result, nothing to do. */
      unOutputSize = unInputSize;
    }
    else
    {
      /* We must copy the complete input to the result. */
      RtChar_CopyString(lpInput, lpBuffer, unBufferSize, &unOutputSize);
    }
  }

  *lpOutputSize = unOutputSize;
  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtChar_StringEndsWith(RT_CHAR* lpString, RT_CHAR* lpSearched)
{
  return RtChar_StringEndsWithWithSize(lpString, RtChar_GetCStringSize(lpString), lpSearched, RtChar_GetCStringSize(lpSearched));
}

RT_B RT_API RtChar_StringEndsWithWithSize(RT_CHAR* lpString, RT_UN unStringSize, RT_CHAR* lpSearched, RT_UN unSearchedSize)
{
  RT_CHAR* lpStringEnd;
  RT_UN unI;
  RT_B bResult;

  if (unStringSize >= unSearchedSize)
  {
    lpStringEnd = &lpString[unStringSize - unSearchedSize];
    bResult = RT_TRUE;
    for (unI = 0; unI < unSearchedSize; unI++)
    {
      if (lpStringEnd[unI] != lpSearched[unI])
      {
        bResult = RT_FALSE;
        break;
      }
    }
  }
  else
  {
    bResult = RT_FALSE;
  }
  return bResult;
}

RT_UN RT_API RtChar_GetStringHash(RT_CHAR* lpString)
{
  /* TODO: Make a better implementation. */
  return lpString[0];
}

void RT_API RtChar_RightTrimString(RT_CHAR* lpString, RT_UN unStringSize, RT_UN *lpOutputSize)
{
  RT_UN unWritten;

  if (!unStringSize)
  {
    unWritten = 0;
  }
  else
  {
    unWritten = unStringSize - 1;
    while (unWritten < RT_TYPE_MAX_UN)
    {
      if (lpString[unWritten] <= _R(' '))
      {
        lpString[unWritten] = 0;
      }
      else
      {
        break;
      }
      unWritten--;
    }
    unWritten++;
  }
  *lpOutputSize = unWritten;
}

RT_UN RT_API RtChar_SearchStringInStrings(RT_CHAR* lpStrings[], RT_CHAR* lpSearched)
{
  RT_UN unResult;
  RT_UN unI;

  unResult = RT_TYPE_MAX_UN;

  if (lpStrings)
  {
    unI = 0;
    while (lpStrings[unI])
    {
      if (!RtChar1337_CompareStrings(lpStrings[unI], lpSearched))
      {
        /* We found the string. */
        unResult = unI;
        break;
      }
      unI++;
    }
  }

  return unResult;
}

RT_UN RT_API RtChar1337_Search(RT_CHAR* lpString, RT_CHAR nSearched)
{
  RT_UN unResult;

  if (lpString)
  {
    unResult = 0;
    while (lpString[unResult] && lpString[unResult] != nSearched) unResult++;
    if (!lpString[unResult])
    {
      unResult = RT_TYPE_MAX_UN;
    }
  }
  else
  {
    unResult = RT_TYPE_MAX_UN;
  }
  return unResult;
}
