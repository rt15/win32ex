#include "layer004/RtChar.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"

RT_B RT_API RtConvertIntegerToString(RT_N nInput, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  /* TODO: Better implementation. */

#ifdef RT_DEFINE_WINDOWS
  wsprintf(lpBuffer, _R("%d"), nInput);
#else
  sprintf(lpBuffer, "%ld", nInput);
#endif
  *lpWritten += RtGetStringSize(lpBuffer);
  return RT_TRUE;
}

RT_B RT_API RtConvertUIntegerToString(RT_UN unInput, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  /* TODO: Better implementation. */
  return RtConvertIntegerToString((RT_N)unInput, lpBuffer, unBufferSize, lpWritten);
}

RT_B RT_API RtConvertStringToInteger(RT_CHAR* lpInput, RT_N* lpResult)
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
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
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

RT_B RT_API RtConvertStringToIntegerWithSize(RT_CHAR* lpInput, RT_UN unInputSize, RT_N* lpResult)
{
  RT_UN unI;
  RT_N nResult;
  RT_B bResult;

  nResult = 0;
  for (unI = 0; unI < unInputSize; unI++)
  {
    if ((lpInput[unI] < _R('0')) || (lpInput[unI] > _R('9')))
    {
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
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

RT_B RT_API RtConvertStringToUInteger(RT_CHAR* lpInput, RT_UN* lpResult)
{
  /* TODO: Better implementation. */
  return RtConvertStringToInteger(lpInput, (RT_N*)lpResult);
}

RT_B RT_API RtConvertStringToUIntegerWithSize(RT_CHAR* lpInput, RT_UN unInputSize, RT_UN* lpResult)
{
  /* TODO: Better implementation. */
  return RtConvertStringToIntegerWithSize(lpInput, unInputSize, (RT_N*)lpResult);
}

RT_UN RT_API RtGetStringSize(RT_CHAR* lpInput)
{
  RT_CHAR* lpInInput;
  RT_UN unResult;

  lpInInput = lpInput;

  while (*lpInInput++);

  unResult = (RT_UN)(lpInInput - lpInput - 1);

  return unResult;
}

RT_N RT_API RtCompareStrings(RT_CHAR* lpString1, RT_CHAR* lpString2)
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

RT_N RT_API RtCompareNullStrings(RT_CHAR* lpString1, RT_CHAR* lpString2)
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

RT_UN RT_API RtSearchString(RT_CHAR* lpString, RT_CHAR* lpSearched)
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

RT_UN RT_API RtCountStringOccurrences(RT_CHAR* lpString, RT_CHAR* lpSearched)
{
  RT_CHAR* lpInString;
  RT_UN unIndex;
  RT_UN unSearchedSize;
  RT_UN unResult;

  unResult = 0;
  lpInString = lpString;
  unSearchedSize = RtGetStringSize(lpSearched);
  do
  {
    unIndex = RtSearchString(lpInString, lpSearched);
    if (unIndex != RT_TYPE_MAX_UN)
    {
      unResult++;
      lpInString = &lpInString[unIndex + unSearchedSize];
    }
  }
  while (unIndex != RT_TYPE_MAX_UN);

  return unResult;
}

RT_B RT_API RtReplaceString(RT_CHAR* lpString, RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  RT_UN unIndex;
  RT_UN unWritten;
  RT_CHAR* lpInString;
  RT_UN unSearchedSize;
  RT_B bResult;

  lpInString = lpString;
  unWritten = 0;
  unSearchedSize = RtGetStringSize(lpSearched);
  while (1)
  {
    unIndex = RtSearchString(lpInString, lpSearched);
    if (unIndex == RT_TYPE_MAX_UN) break;
    /* Copy all characters until found string. */
    if (!RtCopyStringWithSize(lpInString, unIndex, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
    /* Copy replacement string. */
    if (!RtCopyString(lpReplacement, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;

    /* Continue reading the input String. */
    lpInString = &lpInString[unIndex + unSearchedSize];
  }

  /* Copy remaining characters. */
  if (!RtCopyString(lpInString, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  *lpWritten += unWritten;
  return bResult;
}

RT_B RT_API RtCopyStringWithSize(RT_CHAR* lpSource, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  RT_B bResult;
  RT_UN unI;

  /* Manage negative size. */
  if (unSize < 0)
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }

  /* Manage negative or zero buffer sizes. */
  if (unBufferSize <= 0)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  /* Copy only unBufferSize - 1 if the buffer is too small. */
  if (unSize > unBufferSize - 1)
  {
    bResult = RT_FAILURE;
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
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

  *lpWritten += unSize;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtCopyChar(RT_CHAR nChar, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  RT_B bResult;
  if (unBufferSize < 2)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  lpBuffer[0] = nChar;
  lpBuffer[1] = 0;
  (*lpWritten)++;

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_B RT_API RtCopyString(RT_CHAR* lpSource, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  RT_UN unI;
  RT_B bResult;

  /* Manage negative or zero buffer sizes. */
  if (unBufferSize <= 0)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  /* Manage all characters until zero trailing character. */
  unI = 0;
  while (lpSource[unI])
  {
    if (unI >= unBufferSize)
    {
      RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
      lpBuffer[unBufferSize - 1] = 0;
      *lpWritten += unBufferSize - 1;
      goto handle_error;
    }
    lpBuffer[unI] = lpSource[unI];
    unI++;
  }

  /* Manage zero trailing character. */
  if (unI >= unBufferSize)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    lpBuffer[unBufferSize - 1] = 0;
    *lpWritten += unBufferSize - 1;
    goto handle_error;
  }
  lpBuffer[unI] = 0;

  *lpWritten += unI;
  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_UN RT_API RtFastLowerString(RT_CHAR* lpString)
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

RT_CHAR RT_API RtFastLowerChar(RT_CHAR nChar)
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

RT_UN RT_API RtFastUpperString(RT_CHAR* lpString)
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

RT_CHAR RT_API RtFastUpperChar(RT_CHAR nChar)
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

RT_B RT_CDECL_API RtConcatStrings(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpWritten);
  bResult = RtVConcatStrings(lpVaList, lpBuffer, unBufferSize, lpWritten);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_API RtVConcatStrings(va_list lpVaList, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  RT_CHAR* lpString;
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  while (RT_TRUE)
  {
    lpString = va_arg(lpVaList, RT_CHAR*);
    if (lpString)
    {
      if (!RtCopyString(lpString, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
    }
    else
    {
      break;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  *lpWritten += unWritten;
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtLeftPadString(RT_CHAR* lpInput, RT_CHAR nChar, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  return RtLeftPadStringWithSize(lpInput, RtGetStringSize(lpInput), nChar, unSize, lpBuffer, unBufferSize, lpWritten);
}

RT_B RT_API RtLeftPadStringWithSize(RT_CHAR* lpInput, RT_UN unInputSize, RT_CHAR nChar, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  RT_UN unPaddingSize;
  RT_UN unI;
  RT_UN unWritten;
  RT_B bResult;

  if (unBufferSize <= 0)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  if (unInputSize < unSize)
  {
    unPaddingSize = unSize - unInputSize;

    /* Copy lpInput after the padding characters. */
    unWritten = 0;
    RtCopyStringWithSize(lpInput, unInputSize, &lpBuffer[unPaddingSize], unBufferSize - unPaddingSize, &unWritten);
    if (unPaddingSize > unBufferSize - 1)
    {
      unPaddingSize = unBufferSize - 1;
    }
    /* Add padding characters at the left of lpInput. Must be done in a second time to avoid overwriting. */
    for (unI = 0; unI < unPaddingSize; unI++)
    {
      lpBuffer[unI] = nChar;
    }
    /* RtCopyStringWithSize might not have added the zero termination. */
    if (unWritten == 0)
    {
      lpBuffer[unI] = 0;
    }
    unWritten = unWritten + unPaddingSize;
  }
  else
  {
    /* The result is the input. */
    if (lpBuffer == lpInput)
    {
      /* If the same buffer is used for input and result, nothing to do. */
      unWritten = unInputSize;
    }
    else
    {
      /* We must copy the complete input to the result. */
      RtCopyString(lpInput, lpBuffer, unBufferSize, &unWritten);
    }
  }

  *lpWritten = unWritten;
  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtStringEndsWith(RT_CHAR* lpString, RT_CHAR* lpSearched)
{
  return RtStringEndsWithWithSize(lpString, RtGetStringSize(lpString), lpSearched, RtGetStringSize(lpSearched));
}

RT_B RT_API RtStringEndsWithWithSize(RT_CHAR* lpString, RT_UN unStringSize, RT_CHAR* lpSearched, RT_UN unSearchedSize)
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

RT_UN RT_API RtGetStringHash(RT_CHAR* lpString)
{
  /* TODO: Make a better implementation. */
  return lpString[0];
}

RT_B RT_API RtRightTrimString(RT_CHAR* lpString, RT_UN *lpWritten)
{
  return RtRightTrimStringWithSize(lpString, RtGetStringSize(lpString), lpWritten);
}

RT_B RT_API RtRightTrimStringWithSize(RT_CHAR* lpString, RT_UN unStringSize, RT_UN *lpWritten)
{
  if (!unStringSize)
  {
    *lpWritten = 0;
  }
  else
  {
    *lpWritten = unStringSize - 1;
    while (*lpWritten < RT_TYPE_MAX_UN)
    {
      if (lpString[*lpWritten] <= _R(' '))
      {
        lpString[*lpWritten] = 0;
      }
      else
      {
        break;
      }
      (*lpWritten)--;
    }
    (*lpWritten)++;
  }

  return RT_TRUE;
}

RT_UN RT_API RtSearchStringInStrings(RT_CHAR* lpStrings[], RT_CHAR* lpSearched)
{
  RT_UN unResult;
  RT_UN unI;

  unResult = RT_TYPE_MAX_UN;

  if (lpStrings)
  {
    unI = 0;
    while (lpStrings[unI])
    {
      if (!RtCompareStrings(lpStrings[unI], lpSearched))
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

RT_UN RT_API RtSearchChar(RT_CHAR* lpString, RT_CHAR nSearched)
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
