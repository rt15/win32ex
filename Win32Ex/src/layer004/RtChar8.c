#include "layer004/RtChar8.h"

#include "layer002/RtErrorCode.h"

RT_UN RT_API RtGetString8Size(RT_CHAR8* lpInput)
{
  RT_CHAR8* lpInInput;
  RT_UN unResult;

  lpInInput = lpInput;

  while (*lpInInput++);

  unResult = (RT_UN)(lpInInput - lpInput - 1);

  return unResult;
}

RT_N RT_API RtCompareString8s(RT_CHAR8* lpString1, RT_CHAR8* lpString2)
{
  RT_N nResult;

  nResult = 0;

  while(!(nResult = *(RT_UCHAR8*)lpString1 - *(RT_UCHAR8*)lpString2) && *lpString2)
  {
    lpString1++;
    lpString2++;
  }

  return nResult;
}

RT_B RT_API RtCopyString8WithSize(RT_CHAR8* lpSource, RT_UN unSize, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
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

RT_UN RT_API RtSearchString8(RT_CHAR8* lpString, RT_CHAR8* lpSearched)
{
  RT_CHAR8 nChar;
  RT_CHAR8 nSearched;
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

RT_B RT_API RtConvertString8ToInteger(RT_CHAR8* lpInput, RT_N* lpResult)
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

RT_B RT_API RtConvertString8ToUInteger(RT_CHAR8* lpInput, RT_UN* lpResult)
{
  /* TODO: Better implementation. */
  return RtConvertString8ToInteger(lpInput, (RT_N*)lpResult);
}
