#include "layer003/RtChar.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"

RT_B RT_API RtChar_ConvertIntegerToString(RT_N nInput, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  /* TODO: Better implementation. */

#ifdef RT_DEFINE_WINDOWS
  wsprintf(lpBuffer, _R("%d"), nInput);
#else
  sprintf(lpBuffer, "%ld", nInput);
#endif
  *lpOutputSize = RtChar_GetStringSize(lpBuffer);
  return RT_TRUE;
}

RT_B RT_API RtChar_ConvertUIntegerToString(RT_UN unInput, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  /* TODO: Better implementation. */
  return RtChar_ConvertIntegerToString((RT_N)unInput, lpBuffer, unBufferSize, lpOutputSize);
}

RT_B RT_API RtChar_ConvertStringToInteger(RT_CHAR* lpInput, RT_N* lpResult)
{
  RT_UN unI;
  RT_N nResult;
  RT_B bResult;

  nResult = 0;
  unI = 0;
  while (lpInput[unI])
  {
    if ((lpInput[unI] < _R('0')) || (lpInput[unI] > _R('9')))
    {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
    else
    {
      nResult = nResult * 10 + lpInput[unI] - _R('0');
    }
    unI++;
  }
  *lpResult = nResult;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtChar_ConvertStringToIntegerWithSize(RT_CHAR* lpInput, RT_UN unInputSize, RT_N* lpResult)
{
  RT_UN unI;
  RT_N nResult;
  RT_B bResult;

  nResult = 0;
  for (unI = 0; unI < unInputSize; unI++)
  {
    if ((lpInput[unI] < _R('0')) || (lpInput[unI] > _R('9')))
    {
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
    else
    {
      nResult = nResult * 10 + lpInput[unI] - _R('0');
    }
  }
  *lpResult = nResult;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtChar_ConvertStringToUInteger(RT_CHAR* lpInput, RT_UN* lpResult)
{
  /* TODO: Better implementation. */
  return RtChar_ConvertStringToInteger(lpInput, (RT_N*)lpResult);
}

RT_B RT_API RtChar_ConvertStringToUIntegerWithSize(RT_CHAR* lpInput, RT_UN unInputSize, RT_UN* lpResult)
{
  /* TODO: Better implementation. */
  return RtChar_ConvertStringToIntegerWithSize(lpInput, unInputSize, (RT_N*)lpResult);
}

RT_UN RT_API RtChar_GetStringSize(RT_CHAR* lpInput)
{
  RT_CHAR* lpInInput;
  RT_UN unResult;

  lpInInput = lpInput;

  while (*lpInInput++);

  unResult = (RT_UN)(lpInInput - lpInput - 1);

  return unResult;
}

RT_N RT_API RtChar_CompareStrings(RT_CHAR* lpString1, RT_CHAR* lpString2)
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

RT_UN RT_API RtChar_SearchString(RT_CHAR* lpString, RT_CHAR* lpSearched)
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
  unSearchedSize = RtChar_GetStringSize(lpSearched);
  do
  {
    unIndex = RtChar_SearchString(lpInString, lpSearched);
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
    unIndex = RtChar_SearchString(&lpString[unInString], lpSearched);
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
  (*lpOutputSize)++;

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
    /* RtChar_CopyStringWithSize might not have added the zero termination. */
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
  return RtChar_StringEndsWithWithSize(lpString, RtChar_GetStringSize(lpString), lpSearched, RtChar_GetStringSize(lpSearched));
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
      if (!RtChar_CompareStrings(lpStrings[unI], lpSearched))
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

RT_UN RT_API RtChar_Search(RT_CHAR* lpString, RT_CHAR nSearched)
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
