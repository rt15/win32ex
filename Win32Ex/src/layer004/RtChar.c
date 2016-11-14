#include "layer004/RtChar.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"

RT_B RT_API RtConvertNumberToString(RT_N nInput, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
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

RT_B RT_API RtConvertStringToNumber(RT_CHAR* lpInput, RT_N* lpResult)
{
  RT_N nI;
  RT_N nResult;
  RT_B bResult;

  nResult = 0;
  nI = 0;
  while (lpInput[nI])
  {
    if ((lpInput[nI] < _R('0')) || (lpInput[nI] > _R('9')))
    {
      goto handle_error;
    }
    else
    {
      nResult = nResult * 10 + lpInput[nI] - _R('0');
    }
    nI++;
  }
  *lpResult = nResult;

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_N RT_API RtGetStringSize(RT_CHAR* lpInput)
{
  RT_CHAR* lpInInput;
  RT_UN unResult;

  lpInInput = lpInput;

  while (*lpInInput++);

  unResult = (RT_N)(lpInInput - lpInput - 1);

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

RT_N RT_API RtSearchString(RT_CHAR* lpString, RT_CHAR* lpSearched)
{
  RT_CHAR nChar;
  RT_CHAR nSearched;
  RT_N nInSearched;
  RT_N nI, nJ;
  RT_N nResult;

  nResult = -1;
  nI = 0;
  while (1)
  {
    nChar = lpString[nI];
    if (!nChar)
    {
      break;
    }
    if (lpString[nI] == lpSearched[0])
    {
      nInSearched = 1;
      nJ = nI + 1;
      while (1)
      {
        nSearched = lpSearched[nInSearched];
        if (!nSearched)
        {
          nResult = nI;
          break;
        }
        nChar = lpString[nJ];
        if ((!nChar) || (nChar != nSearched))
        {
          break;
        }
        nJ++;
        nInSearched++;
      }
      if (nResult != -1)
      {
        /* Searched found, breaking. */
        break;
      }
    }
    nI++;
  }

  return nResult;
}

RT_N RT_API RtCountStringOccurrences(RT_CHAR* lpString, RT_CHAR* lpSearched)
{
  RT_N nResult;
  RT_CHAR* lpInString;
  RT_N nIndex;
  RT_N nSearchedSize;

  nResult = 0;
  lpInString = lpString;
  nSearchedSize = RtGetStringSize(lpSearched);
  do
  {
    nIndex = RtSearchString(lpInString, lpSearched);
    if (nIndex != -1)
    {
      nResult++;
      lpInString = &lpInString[nIndex + nSearchedSize];
    }
  }
  while (nIndex != -1);

  return nResult;
}

RT_B RT_API RtReplaceString(RT_CHAR* lpString, RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_N nIndex;
  RT_N nWritten;
  RT_CHAR* lpInString;
  RT_N nSearchedSize;
  RT_B bResult;

  lpInString = lpString;
  nWritten = 0;
  nSearchedSize = RtGetStringSize(lpSearched);
  while (1)
  {
    nIndex = RtSearchString(lpInString, lpSearched);
    if (nIndex == -1) break;
    /* Copy all characters until found string. */
    if (!RtCopyStringWithSize(lpInString, nIndex, &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;
    /* Copy replacement string. */
    if (!RtCopyString(lpReplacement, &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;

    /* Continue reading the input String. */
    lpInString = &lpInString[nIndex + nSearchedSize];
  }

  /* Copy remaining characters. */
  if (!RtCopyString(lpInString, &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  *lpWritten += nWritten;
  return bResult;
}

RT_B RT_API RtCopyStringWithSize(RT_CHAR* lpSource, RT_N nSize, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_B bResult;
  RT_N nI;

  /* Manage negative size. */
  if (nSize < 0)
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }

  /* Manage negative or zero buffer sizes. */
  if (nBufferSize <= 0)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  /* Copy only nBufferSize - 1 if the buffer is too small. */
  if (nSize > nBufferSize - 1)
  {
    bResult = RT_FALSE;
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    nSize = nBufferSize - 1;
  }
  else
  {
    bResult = RT_TRUE;
  }

  /* Manage overlapping. */
  if (lpBuffer <= lpSource)
  {
    for (nI = 0; nI < nSize; nI++)
    {
      lpBuffer[nI] = lpSource[nI];
    }
  }
  else
  {
    for (nI = nSize - 1; nI >= 0; nI--)
    {
      lpBuffer[nI] = lpSource[nI];
    }
  }
  lpBuffer[nSize] = 0;

  *lpWritten += nSize;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtCopyChar(RT_CHAR nChar, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_B bResult;
  if (nBufferSize < 2)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  lpBuffer[0] = nChar;
  lpBuffer[1] = 0;
  (*lpWritten)++;

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_B RT_API RtCopyString(RT_CHAR* lpSource, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_N nI;
  RT_B bResult;

  /* Manage negative or zero buffer sizes. */
  if (nBufferSize <= 0)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  /* Manage all characters until zero trailing character. */
  nI = 0;
  while (lpSource[nI])
  {
    if (nI >= nBufferSize)
    {
      RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
      lpBuffer[nBufferSize - 1] = 0;
      *lpWritten += nBufferSize - 1;
      goto handle_error;
    }
    lpBuffer[nI] = lpSource[nI];
    nI++;
  }

  /* Manage zero trailing character. */
  if (nI >= nBufferSize)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    lpBuffer[nBufferSize - 1] = 0;
    *lpWritten += nBufferSize - 1;
    goto handle_error;
  }
  lpBuffer[nI] = 0;

  *lpWritten += nI;
  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_N RT_API RtFastLowerString(RT_CHAR* lpString)
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

RT_B RT_CDECL_API RtConcatStrings(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten, RT_UN unStringsCount, ...)
{
  va_list vaList;
  RT_B bResult;

  va_start(vaList, unStringsCount);
  bResult = RtVConcatStrings(vaList, unStringsCount, lpBuffer, nBufferSize, lpWritten);
  va_end(vaList);

  return bResult;
}

RT_B RT_CDECL_API RtVConcatStrings(va_list vaList, RT_UN unStringsCount, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_CHAR* lpString;
  RT_N nWritten;
  RT_B bResult;
  RT_UN unI;

  nWritten = 0;
  bResult = RT_TRUE;
  for (unI = 0; unI < unStringsCount; unI++)
  {
    lpString = va_arg(vaList, RT_CHAR*);
    bResult = RtCopyString(lpString, &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten);
    if (!bResult)
    {
      break;
    }
  }

  *lpWritten += nWritten;

  return bResult;
}

RT_B RT_API RtLeftPadString(RT_CHAR* lpInput, RT_CHAR nChar, RT_N nSize, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  return RtLeftPadStringWithSize(lpInput, RtGetStringSize(lpInput), nChar, nSize, lpBuffer, nBufferSize, lpWritten);
}

RT_B RT_API RtLeftPadStringWithSize(RT_CHAR* lpInput, RT_N nInputSize, RT_CHAR nChar, RT_N nSize, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_N nPaddingSize;
  RT_N nI;
  RT_N nWritten;
  RT_B bResult;

  if (nBufferSize <= 0)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  if (nInputSize < nSize)
  {
    nPaddingSize = nSize - nInputSize;

    /* Copy lpInput after the padding characters. */
    nWritten = 0;
    RtCopyStringWithSize(lpInput, nInputSize, &lpBuffer[nPaddingSize], nBufferSize - nPaddingSize, &nWritten);
    if (nPaddingSize > nBufferSize - 1)
    {
      nPaddingSize = nBufferSize - 1;
    }
    /* Add padding characters at the left of lpInput. Must be done in a second time to avoid overwriting. */
    for (nI = 0; nI < nPaddingSize; nI++)
    {
      lpBuffer[nI] = nChar;
    }
    /* RtCopyStringWithSize might not have added the zero termination. */
    if (nWritten == 0)
    {
      lpBuffer[nI] = 0;
    }
    nWritten = nWritten + nPaddingSize;
  }
  else
  {
    /* The result is the input. */
    if (lpBuffer == lpInput)
    {
      /* If the same buffer is used for input and result, nothing to do. */
      nWritten = nInputSize;
    }
    else
    {
      /* We must copy the complete input to the result. */
      RtCopyString(lpInput, lpBuffer, nBufferSize, &nWritten);
    }
  }

  *lpWritten = nWritten;
  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtStringEndsWith(RT_CHAR* lpString, RT_CHAR* lpSearched)
{
  return RtStringEndsWithWithSize(lpString, RtGetStringSize(lpString), lpSearched, RtGetStringSize(lpSearched));
}

RT_B RT_API RtStringEndsWithWithSize(RT_CHAR* lpString, RT_N nStringSize, RT_CHAR* lpSearched, RT_N nSearchedSize)
{
  RT_CHAR* lpStringEnd;
  RT_N nI;
  RT_B bResult;

  if (nStringSize >= nSearchedSize)
  {
    lpStringEnd = &lpString[nStringSize - nSearchedSize];
    bResult = RT_TRUE;
    for (nI = 0; nI < nSearchedSize; nI++)
    {
      if (lpStringEnd[nI] != lpSearched[nI])
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

RT_N RT_API RtComputeStringHash(RT_CHAR* lpString)
{
  /* TODO: Make a better implementation. */
  return lpString[0];
}

RT_B RT_API RtRightTrimString(RT_CHAR* lpString, RT_N *lpWritten)
{
  return RtRightTrimStringWithSize(lpString, RtGetStringSize(lpString), lpWritten);
}

RT_B RT_API RtRightTrimStringWithSize(RT_CHAR* lpString, RT_N nStringSize, RT_N *lpWritten)
{
  if (nStringSize <= 0)
  {
    *lpWritten = 0;
  }
  else
  {
    *lpWritten = nStringSize - 1;
    while (*lpWritten >= 0)
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
