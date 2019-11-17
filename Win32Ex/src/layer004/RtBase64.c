#include "layer004/RtBase64.h"

#include "layer001/RtMemory.h"
#include "layer002/RtError.h"
#include "layer003/RtChar8.h"

/*
https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
*/

#define RT_BASE64_WHITESPACE 64
#define RT_BASE64_EQUALS     65
#define RT_BASE64_INVALID    66

RT_CHAR8 rt_lpBase64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

RT_UCHAR8 rt_lpDecodeBase64[] = {
  66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
  66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
  54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
  29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
  66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
  66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
  66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
  66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
  66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
  66,66,66,66,66,66
};

RT_B RT_API RtBase64_Encode(RT_CHAR8* lpString, RT_UN unStringSize, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UCHAR8* lpUString;
  RT_UN32 unNumber;
  RT_UN unIndexInBuffer;
  RT_UCHAR8 un0;
  RT_UCHAR8 un1;
  RT_UCHAR8 un2;
  RT_UCHAR8 un3;
  RT_UN unI;
  RT_B bResult;

  lpUString = (RT_UCHAR8*)lpString;
  unIndexInBuffer = 0;

  /* Increment over the length of the string, three characters at a time. */
  for (unI = 0; unI < unStringSize; unI += 3)
  {
    /* These three 8-bit characters become one 24-bit number. */
    unNumber = ((RT_UN32)lpUString[unI]) << 16;

    /* Is there a second character available? */
    if ((unI + 1) < unStringSize)
    {
      unNumber += ((RT_UN32)lpUString[unI + 1]) << 8;
    }
    /* Is there a third character available? */
    if ((unI + 2) < unStringSize)
    {
      unNumber += lpUString[unI + 2];
    }

    /* This 24-bit number gets separated into four 6-bit numbers. */
    un0 = (RT_UCHAR8)(unNumber >> 18) & 63;
    un1 = (RT_UCHAR8)(unNumber >> 12) & 63;
    un2 = (RT_UCHAR8)(unNumber >> 6) & 63;
    un3 = (RT_UCHAR8)unNumber & 63;

    /* If we have one byte available (8 bits), then its encoding is spread out over two characters (2 * 6 bits). */
    if (unIndexInBuffer >= unBufferSize)
    {
      RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
      goto handle_error;
    }
    lpBuffer[unIndexInBuffer++] = rt_lpBase64[un0];
    if (unIndexInBuffer >= unBufferSize)
    {
      RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
      goto handle_error;
    }
    lpBuffer[unIndexInBuffer++] = rt_lpBase64[un1];

    /* If we have only two bytes available (16 bits), then their encoding is spread out over three characters (3 * 6 bits) . */
    if ((unI + 1) < unStringSize)
    {
      if (unIndexInBuffer >= unBufferSize)
      {
        RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
        goto handle_error;
      }
      lpBuffer[unIndexInBuffer++] = rt_lpBase64[un2];
    }

    /* If we have all three bytes available (24 bits), then their encoding is spread out over four characters (4 * 6 bits). */
    if ((unI + 2) < unStringSize)
    {
      if (unIndexInBuffer >= unBufferSize)
      {
        RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
        goto handle_error;
      }
      lpBuffer[unIndexInBuffer++] = rt_lpBase64[un3];
    }
  }

  /* Add padding that is required if we did not have a multiple of 4 number of characters in output. */
  while (RT_MEMORY_MODULO_POWER_OF_TWO(unIndexInBuffer, 4))
  {
    if (unIndexInBuffer >= unBufferSize)
    {
      RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
      goto handle_error;
    }
    lpBuffer[unIndexInBuffer++] = '=';
  }

  /* Null trailing character. */
  if (unIndexInBuffer >= unBufferSize)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
  lpBuffer[unIndexInBuffer] = 0;

  bResult = RT_SUCCESS;
free_resources:
  *lpOutputSize = unIndexInBuffer;
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtBase64_Decode(RT_CHAR8* lpBase64, RT_UN unBase64Size, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_CHAR8* lpEnd;
  RT_UN32 unBuffer;
  RT_UN unOutputSize;
  RT_UCHAR8 unChar;
  RT_UN unI;
  RT_B bResult;

  lpEnd = lpBase64 + unBase64Size;
  unBuffer = 0;
  unOutputSize = 0;
  unI = 0;

  while (lpBase64 < lpEnd)
  {
    unChar = rt_lpDecodeBase64[(RT_UCHAR8)*lpBase64++];

    switch (unChar)
    {
      case RT_BASE64_WHITESPACE:
        /* Skip whitespace. */
        continue;
      case RT_BASE64_INVALID:
        /* Invalid input. */
        RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
        goto handle_error;
      case RT_BASE64_EQUALS:
        /* Pad character, end of data. */
        lpBase64 = lpEnd;
        continue;
      default:
        unBuffer = unBuffer << 6 | unChar;
         /* Increment the number of iterations. */
        unI++;
        /* If the buffer is full, split it into bytes */
        if (unI == 4)
        {
          if ((unOutputSize += 3) > unBufferSize)
          {
            RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
            goto handle_error;
          }
          *(lpBuffer++) = (RT_UCHAR8)((unBuffer >> 16) & 255);
          *(lpBuffer++) = (RT_UCHAR8)((unBuffer >> 8) & 255);
          *(lpBuffer++) = (RT_UCHAR8)(unBuffer & 255);
          unBuffer = 0;
          unI = 0;
        }
    }
  }

  if (unI == 3)
  {
    if ((unOutputSize += 2) > unBufferSize)
    {
      RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
      goto handle_error;
    }
    *(lpBuffer++) = (RT_UCHAR8)((unBuffer >> 10) & 255);
    *(lpBuffer++) = (RT_UCHAR8)((unBuffer >> 2) & 255);
  }
  else if (unI == 2)
  {
    if (++unOutputSize > unBufferSize)
    {
      RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
      goto handle_error;
    }
    *(lpBuffer++) = (RT_UCHAR8)((unBuffer >> 4) & 255);
  }

  /* Null trailing character. */
  if (unOutputSize + 1 > unBufferSize)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
  *(lpBuffer++) = 0;

  bResult = RT_SUCCESS;
free_resources:
  *lpOutputSize = unOutputSize;
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
