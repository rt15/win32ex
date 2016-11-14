#include "layer006/RtEncoding.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer003/RtMemory.h"
#include "layer004/RtChar.h"
#include "layer004/RtFastInitialization.h"

#ifdef RT_DEFINE_WINDOWS

RT_N rt_lpCodePages[] = { CP_ACP, /* System ANSI code page (0).  */
                          37,     /* RT_ENCODING_IBM037.         */
                          437,    /* RT_ENCODING_IBM437.         */
                          500,    /* RT_ENCODING_IBM500.         */
                          708,    /* RT_ENCODING_ASMO_708.       */
                          737,    /* RT_ENCODING_IBM737.         */
                          775,    /* RT_ENCODING_IBM775.         */
                          850,    /* RT_ENCODING_IBM850.         */
                          852,    /* RT_ENCODING_IBM852.         */
                          855,    /* RT_ENCODING_IBM855.         */
                          857,    /* RT_ENCODING_IBM857.         */
                          860,    /* RT_ENCODING_IBM860.         */
                          861,    /* RT_ENCODING_IBM861.         */
                          863,    /* RT_ENCODING_IBM863.         */
                          864,    /* RT_ENCODING_IBM864.         */
                          865,    /* RT_ENCODING_IBM865.         */
                          866,    /* RT_ENCODING_CP866.          */
                          869,    /* RT_ENCODING_IBM869.         */
                          870,    /* RT_ENCODING_IBM870.         */
                          874,    /* RT_ENCODING_WINDOWS_874.    */
                          875,    /* RT_ENCODING_CP875.          */
                          932,    /* RT_ENCODING_SHIFT_JIS.      */
                          936,    /* RT_ENCODING_GB2312.         */
                          949,    /* RT_ENCODING_CP949.          */
                          950,    /* RT_ENCODING_BIG5.           */
                          1026,   /* RT_ENCODING_IBM1026.        */
                          1047,   /* RT_ENCODING_IBM1047.        */
                          1200,   /* RT_ENCODING_UTF_16.         */
                          1200,   /* RT_ENCODING_UTF_16LE.       */
                          1201,   /* RT_ENCODING_UTF_16BE.       */
                          1250,   /* RT_ENCODING_WINDOWS_1250.   */
                          1251,   /* RT_ENCODING_WINDOWS_1251.   */
                          1252,   /* RT_ENCODING_WINDOWS_1252.   */
                          1253,   /* RT_ENCODING_WINDOWS_1253.   */
                          1254,   /* RT_ENCODING_WINDOWS_1254.   */
                          1255,   /* RT_ENCODING_WINDOWS_1255.   */
                          1256,   /* RT_ENCODING_WINDOWS_1256.   */
                          1257,   /* RT_ENCODING_WINDOWS_1257.   */
                          1258,   /* RT_ENCODING_WINDOWS_1258.   */
                          1361,   /* RT_ENCODING_CP1361.         */
                          10000,  /* RT_ENCODING_MAC.            */
                          12000,  /* RT_ENCODING_UTF_32.         */
                          12000,  /* RT_ENCODING_UTF_32LE.       */
                          12001,  /* RT_ENCODING_UTF_32BE.       */
                          20127,  /* RT_ENCODING_US_ASCII.       */
                          20261,  /* RT_ENCODING_T_61.           */
                          20269,  /* RT_ENCODING_ISO_6937.       */
                          20273,  /* RT_ENCODING_IBM273.         */
                          20277,  /* RT_ENCODING_IBM277.         */
                          20278,  /* RT_ENCODING_IBM278.         */
                          20280,  /* RT_ENCODING_IBM280.         */
                          20284,  /* RT_ENCODING_IBM284.         */
                          20285,  /* RT_ENCODING_IBM285.         */
                          20290,  /* RT_ENCODING_IBM290.         */
                          20297,  /* RT_ENCODING_IBM297.         */
                          20420,  /* RT_ENCODING_IBM420.         */
                          20423,  /* RT_ENCODING_IBM423.         */
                          20424,  /* RT_ENCODING_IBM424.         */
                          20866,  /* RT_ENCODING_KOI8_R.         */
                          20871,  /* RT_ENCODING_IBM871.         */
                          20880,  /* RT_ENCODING_IBM880.         */
                          20905,  /* RT_ENCODING_IBM905.         */
                          21866,  /* RT_ENCODING_KOI8_U.         */
                          28591,  /* RT_ENCODING_ISO_8859_1.     */
                          28592,  /* RT_ENCODING_ISO_8859_2.     */
                          28593,  /* RT_ENCODING_ISO_8859_3.     */
                          28594,  /* RT_ENCODING_ISO_8859_4.     */
                          28595,  /* RT_ENCODING_ISO_8859_5.     */
                          28596,  /* RT_ENCODING_ISO_8859_6.     */
                          28597,  /* RT_ENCODING_ISO_8859_7.     */
                          28598,  /* RT_ENCODING_ISO_8859_8.     */
                          28599,  /* RT_ENCODING_ISO_8859_9.     */
                          28603,  /* RT_ENCODING_ISO_8859_13.    */
                          28605,  /* RT_ENCODING_ISO_8859_15.    */
                          50220,  /* RT_ENCODING_ISO_2022_JP.    */
                          50221,  /* RT_ENCODING_CS_ISO_2022_JP. */
                          50225,  /* RT_ENCODING_ISO_2022_KR.    */
                          51932,  /* RT_ENCODING_EUC_JP.         */
                          51936,  /* RT_ENCODING_EUC_CN.         */
                          51949,  /* RT_ENCODING_EUC_KR.         */
                          52936,  /* RT_ENCODING_HZ_GB2312.      */
                          54936,  /* RT_ENCODING_GB18030.        */
                          65000,  /* RT_ENCODING_UTF_7.          */
                          65001   /* RT_ENCODING_UTF_8.          */
                        };

#endif

RT_CHAR8 *rt_lpEncodingNames[] = { "",                  /* Keep original encoding.      */
                                   "IBM037",            /* RT_ENCODING_IBM037.          */
                                   "IBM437",            /* RT_ENCODING_IBM437.          */
                                   "IBM500",            /* RT_ENCODING_IBM500.          */
                                   "ASMO-708",          /* RT_ENCODING_ASMO_708.        */
                                   "CP737",             /* RT_ENCODING_IBM737.          */
                                   "IBM775",            /* RT_ENCODING_IBM775.          */
                                   "IBM850",            /* RT_ENCODING_IBM850.          */
                                   "IBM852",            /* RT_ENCODING_IBM852.          */
                                   "IBM855",            /* RT_ENCODING_IBM855.          */
                                   "IBM857",            /* RT_ENCODING_IBM857.          */
                                   "IBM860",            /* RT_ENCODING_IBM860.          */
                                   "IBM861",            /* RT_ENCODING_IBM861.          */
                                   "IBM863",            /* RT_ENCODING_IBM863.          */
                                   "IBM864",            /* RT_ENCODING_IBM864.          */
                                   "IBM865",            /* RT_ENCODING_IBM865.          */
                                   "CP866",             /* RT_ENCODING_CP866.           */
                                   "IBM869",            /* RT_ENCODING_IBM869.          */
                                   "IBM870",            /* RT_ENCODING_IBM870.          */
                                   "WINDOWS-874",       /* RT_ENCODING_WINDOWS_874.     */
                                   "CP875",             /* RT_ENCODING_CP875.           */
                                   "SHIFT-JIS",         /* RT_ENCODING_SHIFT_JIS.       */
                                   "GB2312",            /* RT_ENCODING_GB2312.          */
                                   "MSCP949",           /* RT_ENCODING_CP949.           */
                                   "BIG5",              /* RT_ENCODING_BIG5.            */
                                   "IBM1026",           /* RT_ENCODING_IBM1026.         */
                                   "IBM1047",           /* RT_ENCODING_IBM1047.         */
                                   "UTF-16",            /* RT_ENCODING_UTF_16.          */
                                   "UTF-16LE",          /* RT_ENCODING_UTF_16LE.        */
                                   "UTF-16BE",          /* RT_ENCODING_UTF_16BE.        */
                                   "WINDOWS-1250",      /* RT_ENCODING_WINDOWS_1250.    */
                                   "WINDOWS-1251",      /* RT_ENCODING_WINDOWS_1251.    */
                                   "WINDOWS-1252",      /* RT_ENCODING_WINDOWS_1252.    */
                                   "WINDOWS-1253",      /* RT_ENCODING_WINDOWS_1253.    */
                                   "WINDOWS-1254",      /* RT_ENCODING_WINDOWS_1254.    */
                                   "WINDOWS-1255",      /* RT_ENCODING_WINDOWS_1255.    */
                                   "WINDOWS-1256",      /* RT_ENCODING_WINDOWS_1256.    */
                                   "WINDOWS-1257",      /* RT_ENCODING_WINDOWS_1257.    */
                                   "WINDOWS-1258",      /* RT_ENCODING_WINDOWS_1258.    */
                                   "MSCP1361",          /* RT_ENCODING_CP1361.          */
                                   "MAC",               /* RT_ENCODING_MAC.             */
                                   "UTF-32",            /* RT_ENCODING_UTF_32.          */
                                   "UTF-32LE",          /* RT_ENCODING_UTF_32LE.        */
                                   "UTF-32BE",          /* RT_ENCODING_UTF_32BE.        */
                                   "ANSI_X3.4-1968",    /* RT_ENCODING_US_ASCII.        */
                                   "T.61",              /* RT_ENCODING_T_61.            */
                                   "ISO6937",           /* RT_ENCODING_ISO_6937.        */
                                   "IBM273",            /* RT_ENCODING_IBM273.          */
                                   "IBM277",            /* RT_ENCODING_IBM277.          */
                                   "IBM278",            /* RT_ENCODING_IBM278.          */
                                   "IBM280",            /* RT_ENCODING_IBM280.          */
                                   "IBM284",            /* RT_ENCODING_IBM284.          */
                                   "IBM285",            /* RT_ENCODING_IBM285.          */
                                   "IBM290",            /* RT_ENCODING_IBM290.          */
                                   "IBM297",            /* RT_ENCODING_IBM297.          */
                                   "IBM420",            /* RT_ENCODING_IBM420.          */
                                   "IBM423",            /* RT_ENCODING_IBM423.          */
                                   "IBM424",            /* RT_ENCODING_IBM424.          */
                                   "KOI8-R",            /* RT_ENCODING_KOI8_R.          */
                                   "IBM871",            /* RT_ENCODING_IBM871.          */
                                   "IBM880",            /* RT_ENCODING_IBM880.          */
                                   "IBM905",            /* RT_ENCODING_IBM905.          */
                                   "KOI8-U",            /* RT_ENCODING_KOI8_U.          */
                                   "ISO-8859-1",        /* RT_ENCODING_ISO_8859_1.      */
                                   "ISO-8859-2",        /* RT_ENCODING_ISO_8859_2.      */
                                   "ISO-8859-3",        /* RT_ENCODING_ISO_8859_3.      */
                                   "ISO-8859-4",        /* RT_ENCODING_ISO_8859_4.      */
                                   "ISO-8859-5",        /* RT_ENCODING_ISO_8859_5.      */
                                   "ISO-8859-6",        /* RT_ENCODING_ISO_8859_6.      */
                                   "ISO-8859-7",        /* RT_ENCODING_ISO_8859_7.      */
                                   "ISO-8859-8",        /* RT_ENCODING_ISO_8859_8.      */
                                   "ISO-8859-9",        /* RT_ENCODING_ISO_8859_9.      */
                                   "ISO-8859-13",       /* RT_ENCODING_ISO_8859_13.     */
                                   "ISO-8859-15",       /* RT_ENCODING_ISO_8859_15.     */
                                   "ISO-2022-JP",       /* RT_ENCODING_ISO_2022_JP.     */
                                   "CSISO2022JP",       /* RT_ENCODING_CS_ISO_2022_JP.  */
                                   "ISO-2022-KR",       /* RT_ENCODING_ISO_2022_KR.     */
                                   "EUC-JP",            /* RT_ENCODING_EUC_JP.          */
                                   "EUC-CN",            /* RT_ENCODING_EUC_CN.          */
                                   "EUC-KR",            /* RT_ENCODING_EUC_KR.          */
                                   "GB2312",            /* RT_ENCODING_HZ_GB2312.       */
                                   "GB18030",           /* RT_ENCODING_GB18030.         */
                                   "UTF-7",             /* RT_ENCODING_UTF_7.           */
                                   "UTF-8"              /* RT_ENCODING_UTF_8.           */
                                 };

#ifndef RT_DEFINE_WINDOWS

RT_FAST_INITIALIZATION rt_encodingSystemInitialization = RT_FAST_INITIALIZATION_STATIC_INIT;

RT_CHAR rt_lpEncodingSystemBuffer[64];

RT_CHAR* rt_lpEncodingSystem;

#endif

#ifdef RT_DEFINE_WINDOWS

/**
 * Use Windows WideCharToMultiByte/MultiByteToWideChar to encode/decode.
 */
RT_N RT_CALL RtEncodeOrDecodeUsingWindows(RT_CHAR8* lpInput, RT_N nInputSize, RT_N nEncoding, RT_CHAR8* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap, RT_B bEncode, RT_N nOutputTerminatingZeroSize)
{
  RT_N nCodePage;
  RT_N nAccurateBufferSize;
  RT_B bUseHeap;
  RT_N nI;
  RT_N nResult;

  nCodePage = rt_lpCodePages[nEncoding];
  nAccurateBufferSize = nBufferSize;
  if (lpBuffer)
  {
    if (nInputSize != -1)
    {
      /* In case nInputSize is provided, we need an extra space for the trailing zero character. */
      nAccurateBufferSize--;
    }
    if (bEncode)
    {
      nResult = WideCharToMultiByte(nCodePage, 0, (RT_CHAR*)lpInput, nInputSize, lpBuffer, nAccurateBufferSize, RT_NULL, RT_NULL);
    }
    else
    {
      nResult = MultiByteToWideChar(nCodePage, MB_ERR_INVALID_CHARS, lpInput, nInputSize, (RT_CHAR*)lpBuffer, nAccurateBufferSize);
    }
    if (nResult)
    {
      if (nInputSize == -1)
      {
        /* The result includes the zero trailing character. */
        nResult--;
      }
      else
      {
        /* In case nInputSize is provided, we need to add the trailing zero character. */
        for (nI = 0; nI < nOutputTerminatingZeroSize; nI++)
        {
          lpBuffer[nResult * nOutputTerminatingZeroSize + nI] = 0;
        }
      }
      /* Buffer was large enough. */
      *lpOutput = lpBuffer;
      bUseHeap = RT_FALSE;
    }
    else
    {
      if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
      {
        bUseHeap = RT_TRUE;
      }
      else
      {
        bUseHeap = RT_FALSE;
        nResult = -1;
      }
    }
  }
  else
  {
    bUseHeap = RT_TRUE;
  }
  if (bUseHeap)
  {
    if (lpHeap)
    {
      if (bEncode)
      {
        nResult = WideCharToMultiByte(nCodePage, 0, (RT_CHAR*)lpInput, nInputSize, RT_NULL, 0, RT_NULL, RT_NULL);
      }
      else
      {
        nResult = MultiByteToWideChar(nCodePage, MB_ERR_INVALID_CHARS, lpInput, nInputSize, RT_NULL, 0);
      }
      if (!nResult)
      {
        *lpOutput = RT_NULL;
        nResult = -1;
      }
      else
      {
        if (nInputSize == -1)
        {
          nAccurateBufferSize = nResult;
        }
        else
        {
          /* As nInputSize is provided, we need place for trailing zero. */
          nAccurateBufferSize = nResult + 1;
        }
        if (RtAllocIfNeededWithHeap(RT_NULL, 0, lpHeapBuffer, lpHeapBufferSize, (void**)lpOutput, (bEncode) ? nAccurateBufferSize : nAccurateBufferSize * sizeof(RT_CHAR), _R("Encoded/decoded stuff"), lpHeap))
        {
          if (bEncode)
          {
            nResult = WideCharToMultiByte(nCodePage, 0, (RT_CHAR*)lpInput, nInputSize, *lpOutput, nResult, RT_NULL, RT_NULL);
          }
          else
          {
             nResult = MultiByteToWideChar(nCodePage, MB_ERR_INVALID_CHARS, lpInput, nInputSize, (RT_CHAR*)*lpOutput, nResult);
          }
          if (nResult)
          {
            if (nInputSize == -1)
            {
              /* The result includes the zero trailing character. */
              nResult--;
            }
            else
            {
              /* In case nInputSize is provided, we need to add the trailing zero character. */
              for (nI = 0; nI < nOutputTerminatingZeroSize; nI++)
              {
                (*lpOutput)[nResult * nOutputTerminatingZeroSize + nI] = 0;
              }
            }
          }
          else
          {
            (*lpHeap)->lpFree(lpHeap, (void**)lpOutput);
            nResult = -1;
          }
        }
      }
    }
    else
    {
      RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
      nResult = -1;
    }
  }
  return nResult;
}

/**
 * Simple affectation, "1 2" -> "0 0 1 2".
 */
void RT_CALL RtCopy16LeTo32Le(RT_UN16* lpSource, RT_UN32* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = lpSource[unI];
  }
}

/**
 * Affectation then 32 bits swap, "1 2" -> "2 1 0 0".
 */
void RT_CALL RtCopy16LeTo32Be(RT_UN16* lpSource, RT_UN32* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = RT_MEMORY_SWAP_BYTES32((RT_UN32)lpSource[unI]);
  }
}

/**
 * Swap then affectation, "2 1" -> "0 0 1 2".
 */
void RT_CALL RtCopy16BeTo32Le(RT_UN16* lpSource, RT_UN32* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = RT_MEMORY_SWAP_BYTES16(lpSource[unI]);
  }
}

/**
 * Affect to high bytes, "2 1" -> "2 1 0 0".
 */
void RT_CALL RtCopy16BeTo32Be(RT_UN16* lpSource, RT_UN32* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    ((RT_UN16*)(&lpDestination[unI]))[1] = lpSource[unI];
  }
}

/**
 * Simple affectation, "0 0 1 2" -> "1 2".
 */
void RT_CALL RtCopy32LeTo16Le(RT_UN32* lpSource, RT_UN16* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = (RT_UN16)lpSource[unI];
  }
}

/**
 * Affectation then swap, "0 0 1 2" -> "2 1".
 */
void RT_CALL RtCopy32LeTo16Be(RT_UN32* lpSource, RT_UN16* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = RT_MEMORY_SWAP_BYTES16((RT_UN16)lpSource[unI]);
  }
}

/**
 * Swap then affectation, "2 1 0 0" -> "1 2".
 */
void RT_CALL RtCopy32BeTo16Le(RT_UN32* lpSource, RT_UN16* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = (RT_UN16)RT_MEMORY_SWAP_BYTES32(lpSource[unI]);
  }
}

/**
 * Affect from high bytes, "2 1 0 0" -> "2 1".
 */
void RT_CALL RtCopy32BeTo16Be(RT_UN32* lpSource, RT_UN16* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = ((RT_UN16*)&lpSource[unI])[1];
  }
}

/**
 * Windows WideCharToMultiByte/MultiByteToWideChar does not manage UTF-16/UTF-32 conversions.<br>
 * This is done by this function.
 *
 * @param lpInput Input without BOM.
 * @param nInputEncoding Input encoding with endianness.
 * @param nOutputEncoding Output encoding with optional endianess.
 * @return Output size in bytes.
 */
RT_N RT_CALL RtEncodeOrDecodeUnicode(RT_CHAR8* lpInput, RT_N nInputCharSize, RT_N nInputEncoding, RT_CHAR8* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap, RT_N nOutputEncoding, RT_N nInputTerminatingZeroSize, RT_N nOutputTerminatingZeroSize)
{
  RT_N nOutputCharSize;
  RT_CHAR8* lpOutputPayload; /* Pointer on output after optional BOM. */
  RT_N nI;
  RT_N nResult;

  if ((nOutputEncoding == RT_ENCODING_UTF_16) ||
      (nOutputEncoding == RT_ENCODING_UTF_32))
  {
    /* There is no more BOM in input, but we need one in output. */
    nOutputCharSize = nInputCharSize + 1;
  }
  else
  {
    nOutputCharSize = nInputCharSize;
  }

  /* Allocate resulting buffer, adding one for zero terminating character. */
  if (!RtAllocIfNeededWithHeap(lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, (void**)lpOutput, (nOutputCharSize + 1) * nOutputTerminatingZeroSize, _R("Encode/decode unicode"), lpHeap))
  {
    goto handle_error;
  }

  /* Add little endian BOM if needed and set output payload pointer. */
  if (nOutputEncoding == RT_ENCODING_UTF_16)
  {
    /* Always assume little endian on Windows. */
    (*lpOutput)[0] = (RT_CHAR8)0xFF;
    (*lpOutput)[1] = (RT_CHAR8)0xFE;
    lpOutputPayload = &(*lpOutput)[2];
  }
  else if (nOutputEncoding == RT_ENCODING_UTF_32)
  {
    /* Always assume little endian on Windows. */
    (*lpOutput)[0] = (RT_CHAR8)0xFF;
    (*lpOutput)[1] = (RT_CHAR8)0xFE;
    (*lpOutput)[2] = (RT_CHAR8)0x00;
    (*lpOutput)[3] = (RT_CHAR8)0x00;
    lpOutputPayload = &(*lpOutput)[4];
  }
  else
  {
    lpOutputPayload = *lpOutput;
  }

  switch (nInputEncoding)
  {
    case RT_ENCODING_UTF_16LE:
      switch (nOutputEncoding)
      {
        case RT_ENCODING_UTF_16:
        case RT_ENCODING_UTF_16LE:
          /* No transformation. */
          RtCopyMemory(lpInput, lpOutputPayload, nInputCharSize * nInputTerminatingZeroSize);
          break;
        case RT_ENCODING_UTF_16BE:
          /* Swap 16. */
          RtSwapBytes16((RT_UN16*)lpInput, (RT_UN16*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_32:
        case RT_ENCODING_UTF_32LE:
          RtCopy16LeTo32Le((RT_UN16*)lpInput, (RT_UN32*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_32BE:
          RtCopy16LeTo32Be((RT_UN16*)lpInput, (RT_UN32*)lpOutputPayload, nInputCharSize);
          break;
      }
      break;
    case RT_ENCODING_UTF_16BE:
      switch (nOutputEncoding)
      {
        case RT_ENCODING_UTF_16:
        case RT_ENCODING_UTF_16LE:
          /* Swap 16. */
          RtSwapBytes16((RT_UN16*)lpInput, (RT_UN16*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_16BE:
          /* No transformation. */
          RtCopyMemory(lpInput, lpOutputPayload, nInputCharSize * nInputTerminatingZeroSize);
          break;
        case RT_ENCODING_UTF_32:
        case RT_ENCODING_UTF_32LE:
          RtCopy16BeTo32Le((RT_UN16*)lpInput, (RT_UN32*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_32BE:
          RtCopy16BeTo32Be((RT_UN16*)lpInput, (RT_UN32*)lpOutputPayload, nInputCharSize);
          break;
      }
      break;
    case RT_ENCODING_UTF_32LE:
      switch (nOutputEncoding)
      {
        case RT_ENCODING_UTF_16:
        case RT_ENCODING_UTF_16LE:
          RtCopy32LeTo16Le((RT_UN32*)lpInput, (RT_UN16*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_16BE:
          RtCopy32LeTo16Be((RT_UN32*)lpInput, (RT_UN16*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_32:
        case RT_ENCODING_UTF_32LE:
          /* No transformation. */
          RtCopyMemory(lpInput, lpOutputPayload, nInputCharSize * nInputTerminatingZeroSize);
          break;
        case RT_ENCODING_UTF_32BE:
          /* Swap 32. */
          RtSwapBytes32((RT_UN32*)lpInput, (RT_UN32*)lpOutputPayload, nInputCharSize);
          break;
      }
      break;
    case RT_ENCODING_UTF_32BE:
      switch (nOutputEncoding)
      {
        case RT_ENCODING_UTF_16:
        case RT_ENCODING_UTF_16LE:
          RtCopy32BeTo16Le((RT_UN32*)lpInput, (RT_UN16*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_16BE:
          RtCopy32BeTo16Be((RT_UN32*)lpInput, (RT_UN16*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_32:
        case RT_ENCODING_UTF_32LE:
          /* Swap 32. */
          RtSwapBytes32((RT_UN32*)lpInput, (RT_UN32*)lpOutputPayload, nInputCharSize);
          break;
        case RT_ENCODING_UTF_32BE:
          /* No transformation. */
          RtCopyMemory(lpInput, lpOutputPayload, nInputCharSize * nInputTerminatingZeroSize);
          break;
      }
      break;
  }

  /* Result is size in bytes without counting the zero terminating bytes. */
  nResult = nOutputCharSize * nOutputTerminatingZeroSize;

  /* Add terminating zero bytes. */
  for (nI = 0; nI < nOutputTerminatingZeroSize; nI++)
  {
    (*lpOutput)[nResult + nI] = 0;
  }

  goto free_resources;
handle_error:
  nResult = -1;
free_resources:
  return nResult;
}

/**
 * Windows WideCharToMultiByte/MultiByteToWideChar does not manage UTF-16/UTF-32 conversions.<br>
 * This is done by this function that manage input BOM and call <tt>RtEncodeOrDecodeUnicode</tt>.
 *
 * @return Output size in bytes.
 */
RT_N RT_CALL RtEncodeOrDecodeUnicodeWithBom(RT_CHAR8* lpInput, RT_N nInputCharSize, RT_N nInputEncoding, RT_CHAR8* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap, RT_N nOutputEncoding, RT_N nInputTerminatingZeroSize, RT_N nOutputTerminatingZeroSize)
{
  RT_CHAR8* lpInputPayload; /* Pointer on input after optional BOM. */
  RT_N nInputPayloadSize;
  RT_N nInputPayloadEncoding; /* Input encoding with endianness. */
  RT_N nResult;

  if (nInputEncoding == RT_ENCODING_UTF_16)
  {
    /* We already checked that size is > 0. */
    if (((RT_UCHAR8)lpInput[0] == 0xFF) && ((RT_UCHAR8)lpInput[1] == 0xFE))
    {
      nInputPayloadEncoding = RT_ENCODING_UTF_16LE;
    }
    else if (((RT_UCHAR8)lpInput[0] == 0xFE) && ((RT_UCHAR8)lpInput[1] == 0xFF))
    {
      nInputPayloadEncoding = RT_ENCODING_UTF_16BE;
    }
    else
    {
      /* Invalid BOM. */
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }

    /* Skip BOM. */
    lpInputPayload = &lpInput[2];

    /* Remove BOM from size. */
    nInputPayloadSize = nInputCharSize - 1;
  }
  else if (nInputEncoding == RT_ENCODING_UTF_32)
  {
    /* We already checked that size is > 0. */
    if (((RT_UCHAR8)lpInput[0] == 0xFF) && ((RT_UCHAR8)lpInput[1] == 0xFE) && ((RT_UCHAR8)lpInput[2] == 0x00) && ((RT_UCHAR8)lpInput[3] == 0x00))
    {
      nInputPayloadEncoding = RT_ENCODING_UTF_32LE;
    }
    else if (((RT_UCHAR8)lpInput[0] == 0x00) && ((RT_UCHAR8)lpInput[1] == 0x00) && ((RT_UCHAR8)lpInput[2] == 0xFE) && ((RT_UCHAR8)lpInput[3] == 0xFF))
    {
      nInputPayloadEncoding = RT_ENCODING_UTF_32BE;
    }
    else
    {
      /* Invalid BOM. */
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }

    /* Skip BOM. */
    lpInputPayload = &lpInput[4];

    /* Remove BOM from size. */
    nInputPayloadSize = nInputCharSize - 1;
  }
  else
  {
    nInputPayloadEncoding = nInputEncoding;
    lpInputPayload = lpInput;
    nInputPayloadSize = nInputCharSize;
  }

  nResult = RtEncodeOrDecodeUnicode(lpInputPayload, nInputPayloadSize, nInputPayloadEncoding, lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, lpOutput, lpHeap, nOutputEncoding, nInputTerminatingZeroSize, nOutputTerminatingZeroSize);

  goto free_resources;
handle_error:
  nResult = -1;
free_resources:
  return nResult;
}

#else /* NOT RT_DEFINE_WINDOWS */

/**
 * Call iconv, manage returned value and add zero terminating characters.
 */
RT_N RT_CALL RtPerformIconvWithBuffer(iconv_t lpIconv, RT_CHAR8* lpInput, RT_N nInputSize, RT_CHAR8* lpBuffer, RT_N nBufferSize, RT_N nOutputTerminatingZeroSize)
{
  RT_CHAR8* lpIconvInput;
  RT_CHAR8* lpIconvBuffer;
  size_t nIconvInputSize;
  size_t nIconvBufferSize;
  size_t nReturnedValue;
  RT_N nI;
  RT_N nResult;

  /* Iconv update input/output sizes. */
  lpIconvInput = lpInput;
  nIconvInputSize = nInputSize;
  lpIconvBuffer = lpBuffer;

  /* Keep bytes for zero terminating characters. */
  nIconvBufferSize = nBufferSize - nOutputTerminatingZeroSize;

  nReturnedValue = iconv(lpIconv, &lpIconvInput, &nIconvInputSize, &lpIconvBuffer, &nIconvBufferSize);
  if (nReturnedValue == 0)
  {
    /* Conversion is ok. */
    nResult = nBufferSize - nOutputTerminatingZeroSize - nIconvBufferSize;
    /* As iconv does not put zero terminating character, it is done here. */
    for (nI = 0; nI < nOutputTerminatingZeroSize; nI++)
    {
      lpBuffer[nResult + nI] = 0;
    }
  }
  else if (nReturnedValue == (size_t)-1)
  {
    /* The errno value has been set by iconv. */
    goto handle_error;
  }
  else
  {
    /* There are non-reversible characters. */
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }

  goto free_resources;
handle_error:
  nResult = -1;
free_resources:
  return nResult;
}

/**
 * Try to perform the convertion increasing the heap buffer size if needed.
 */
RT_N RT_CALL RtPerformIconvWithHeap(iconv_t lpIconv, RT_CHAR8* lpInput, RT_N nInputSize, RT_N nInitialHeapBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap, RT_N nOutputTerminatingZeroSize)
{
  RT_CHAR8* lpIconvInput;
  RT_CHAR8* lpIconvBuffer;
  RT_N nHeapBufferSize;
  RT_N nIncrease;
  size_t nInputBytesLeft;
  size_t nOutBytesLeft;
  size_t nReturnedValue;
  RT_N nLength;
  RT_N nI;
  RT_N nResult;

  /* Iconv update input/output sizes. */
  lpIconvInput = lpInput;
  nInputBytesLeft = nInputSize;
  lpIconvBuffer = *lpOutput;

  /* Keep bytes for zero terminating characters. */
  nHeapBufferSize = nInitialHeapBufferSize - nOutputTerminatingZeroSize;
  nOutBytesLeft = nHeapBufferSize;

  /* Re-allocate buffer until complete conversion. */
  while (RT_TRUE)
  {
    nReturnedValue = iconv(lpIconv, &lpIconvInput, &nInputBytesLeft, &lpIconvBuffer, &nOutBytesLeft);
    if (nReturnedValue == 0)
    {
      /* Conversion is ok. */
      nResult = lpIconvBuffer - *lpOutput;
      /* As iconv does not put zero terminating character, it is done here. */
      for (nI = 0; nI < nOutputTerminatingZeroSize; nI++)
      {
        (*lpOutput)[nResult + nI] = 0;
      }
      /* Conversion done, exiting loop. */
      break;
    }
    else if (nReturnedValue == (size_t)-1)
    {
      if (errno == E2BIG)
      {
        nLength = lpIconvBuffer - *lpOutput;

        /* Should not be zero as nInitialHeapBufferSize is greater than 16. */
        nIncrease = 0.2 * nHeapBufferSize;

        /* Re-allocate more space. */
        nHeapBufferSize += nIncrease;
        nOutBytesLeft += nIncrease;
        if (!RtAllocIfNeededWithHeap(RT_NULL, 0, lpHeapBuffer, lpHeapBufferSize, (void**)lpOutput, nHeapBufferSize, _R("Foo"), lpHeap)) goto handle_error;
        lpIconvBuffer = *lpOutput + nLength;
      }
      else
      {
        /* The errno value has been set by iconv. */
        goto handle_error;
      }
    }
    else
    {
      /* There are non-reversible characters. */
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
  }

  goto free_resources;
handle_error:
  nResult = -1;
free_resources:
  return nResult;
}

/**
 * Call iconv function with given iconv_t.
 *
 * @param nInputSize Must not be -1.
 * @param nTerminatingZeroSize Size of terminating zero. As UTF-16 can contain zero bytes it is cleaner to end them with 2 zero bytes.
 * @return The written bytes count in <tt>lpBuffer</tt>, not including the terminating zero bytes.
 */
RT_N RT_CALL RtIconvWithIconv(iconv_t lpIconv, RT_CHAR8* lpInput, RT_N nInputSize, RT_CHAR8* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap, RT_N nInputTerminatingZeroSize, RT_N nOutputTerminatingZeroSize)
{
  RT_B bUseHeap;
  RT_N nHeapBufferSize;
  size_t nIconvInputSize;
  size_t nIconvBufferSize;
  RT_N nResult;

  if (lpBuffer)
  {
    nResult = RtPerformIconvWithBuffer(lpIconv, lpInput, nInputSize, lpBuffer, nBufferSize, nOutputTerminatingZeroSize);
    if (nResult == -1)
    {
      if (errno == E2BIG)
      {
        /* Buffer was not enough, will try with heap. */
        bUseHeap = RT_TRUE;
      }
      else
      {
        /* Unknown error, do not try with heap. */
        bUseHeap = RT_FALSE;
      }
    }
    else
    {
      /* Buffer was enough, output is ready. */
      bUseHeap = RT_FALSE;
    }
  }
  else
  {
    bUseHeap = RT_TRUE;
  }

  if (bUseHeap)
  {
    if (lpHeap)
    {
      /* Re-initialize iconv_t if used to try to convert with buffer. */
      if (lpBuffer)
      {
        if (iconv(lpIconv, RT_NULL, &nIconvInputSize, RT_NULL, &nIconvBufferSize) == -1)
        {
          goto handle_error;
        }
      }

      /* Compute first heap buffer size based either on given buffer size or input size. */
      if (nBufferSize > 0)
      {
        /* We just failed with nBufferSize so try with twice more. */
        nHeapBufferSize = 16 + nBufferSize * 2;
      }
      else
      {
        /* Base buffer size on characters sizes. */
        nHeapBufferSize = 16 + 1.2 * nInputSize * ((float)nOutputTerminatingZeroSize / (float)nInputTerminatingZeroSize);
      }

      if (!RtAllocIfNeededWithHeap(RT_NULL, 0, lpHeapBuffer, lpHeapBufferSize, (void**)lpOutput, nHeapBufferSize, _R("Encoded/decoded stuff"), lpHeap)) goto handle_error;
      nResult = RtPerformIconvWithHeap(lpIconv, lpInput, nInputSize, nHeapBufferSize, lpHeapBuffer, lpHeapBufferSize, lpOutput, lpHeap, nOutputTerminatingZeroSize);
    }
    else
    {
      RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
      nResult = -1;
    }
  }

  goto free_resources;
handle_error:
  nResult = -1;
free_resources:
  return nResult;
}

/**
 * Open an iconv and call <tt>RtUseIconv</tt>.
 *
 * <p>
 * Beware that an open iconv has a conversion state.<br>
 * To restore it to initial state, use iconv with NULL as inbuf.
 * </p>
 *
 * @param nInputSize If -1, lpInput is considered NULL terminated.
 * @param nTerminatingZeroSize Size of terminating zero. As UTF-16 can contain zero bytes it is cleaner to end them with 2 zero bytes.
 * @return The written bytes count in <tt>lpBuffer</tt>, not including the terminating zero bytes.
 */
RT_N RT_CALL RtIconvWithEncoding(RT_CHAR* lpInputEncoding, RT_CHAR* lpOutputEncoding, RT_CHAR8* lpInput, RT_N nInputSize, RT_CHAR8* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap, RT_N nInputTerminatingZeroSize, RT_N nOutputTerminatingZeroSize)
{
  RT_B bOpen;
  RT_N nActualInputSize;
  iconv_t lpIconv;
  RT_N nResult;

  bOpen = RT_FALSE;

  lpIconv = iconv_open(lpOutputEncoding, lpInputEncoding);
  if (lpIconv == (iconv_t) -1)
  {
    goto handle_error;
  }
  bOpen = RT_TRUE;

  /* Compute input size if not provided. */
  if (nInputSize == -1)
  {
    nActualInputSize = RtGetDataSize(lpInput, nInputTerminatingZeroSize);
  }
  else
  {
    nActualInputSize = nInputSize;
  }

  nResult = RtIconvWithIconv(lpIconv, lpInput, nActualInputSize, lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, lpOutput, lpHeap, nInputTerminatingZeroSize, nOutputTerminatingZeroSize);

  goto free_resources;
handle_error:
  nResult = -1;
free_resources:
  if (bOpen)
  {
    if ((iconv_close(lpIconv) == -1) && (nResult != -1))
    {
      bOpen = RT_FALSE;
      goto handle_error;
    }
    bOpen = RT_FALSE;
  }
  return nResult;
}

/**
 * Retrieve the system encoding.<br>
 * Call setlocale(LC_ALL, "") to avoid "C" locale.
 *
 * Return RT_NULL in case of error.
 */
RT_CHAR* RT_CALL RtGetLinuxSystemEncoding()
{
  RT_CHAR* lpReturnedValue;
  RT_N nWritten;
  RT_N nEncodingNameSize;

  if (RtFastInitializationRequired(&rt_encodingSystemInitialization))
  {
    rt_lpEncodingSystem = RT_NULL;

    lpReturnedValue = nl_langinfo(CODESET);
    if (lpReturnedValue)
    {
      nEncodingNameSize = RtGetStringSize(lpReturnedValue);
      if (nEncodingNameSize)
      {
        /* The pointer nl_langinfo is not thread safe so we make a copy fastly. */
        if (RtCopyString(lpReturnedValue, rt_lpEncodingSystemBuffer, 64, &nWritten))
        {
          rt_lpEncodingSystem = rt_lpEncodingSystemBuffer;
        }
      }
    }

    RtNotifyFastInitializationDone(&rt_encodingSystemInitialization);
  }
  if (!rt_lpEncodingSystem)
  {
    RtSetLastError(RT_ERROR_FUNCTION_FAILED);
  }
  return rt_lpEncodingSystem;
}

#endif

/**
 * Factorize encoding and decoding.
 */
RT_N RT_CALL RtEncodeOrDecode(RT_CHAR8* lpInput, RT_N nInputSize, RT_N nEncoding, RT_CHAR8* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap, RT_B bEncode)
{
  RT_N nInputTerminatingZeroSize;
  RT_N nOutputTerminatingZeroSize;
#ifdef RT_DEFINE_WINDOWS
  RT_N nInputEncoding;
  RT_N nOutputEncoding;
  RT_N nInputCharSize;
#else /* NOT RT_DEFINE_WINDOWS */
  RT_CHAR* lpSystemEncoding;
  RT_CHAR* lpInputEncoding;
  RT_CHAR* lpOutputEncoding;
#endif
  RT_N nI;
  RT_N nResult;

#ifdef RT_DEFINE_LINUX
  /* Retrieve Linux system encoding. */
  lpSystemEncoding = RtGetLinuxSystemEncoding();
  if (!lpSystemEncoding) goto handle_error;
#endif

  /* Compute output terminating zero size. */
  if (bEncode)
  {
    /* Encoding, input size depends on RTCHAR size. */
    nInputTerminatingZeroSize = sizeof(RT_CHAR);

    /* Encoding, output size depends on output encoding. */
    nOutputTerminatingZeroSize = RtGetTerminatingZeroSize(nEncoding);

#ifdef RT_DEFINE_WINDOWS
    nInputEncoding = RT_ENCODING_UTF_16LE;
    nOutputEncoding = nEncoding;
    /* The nInputSize value is given in characters. */
    nInputCharSize = nInputSize;
#else /* NOT RT_DEFINE_WINDOWS */

    lpInputEncoding = lpSystemEncoding;
    lpOutputEncoding = rt_lpEncodingNames[nEncoding];
#endif
  }
  else
  {
    /* Decoding, input size depends on input encoding. */
    nInputTerminatingZeroSize = RtGetTerminatingZeroSize(nEncoding);

    /* Decoding, size depends on RTCHAR size. */
    nOutputTerminatingZeroSize = sizeof(RT_CHAR);

#ifdef RT_DEFINE_WINDOWS
    nInputEncoding = nEncoding;
    nOutputEncoding = RT_ENCODING_UTF_16LE;
    /* The nInputSize value is given in bytes. */
    nInputCharSize = (nInputSize == -1) ? -1 : nInputSize / nInputTerminatingZeroSize;
#else /* NOT RT_DEFINE_WINDOWS */

    lpInputEncoding = rt_lpEncodingNames[nEncoding];
    lpOutputEncoding = lpSystemEncoding;
#endif
  }

  /* If empty input, return empty output. */
  if (!nInputSize)
  {
    /* Allocate a buffer for zero character. */
    if (!RtAllocIfNeededWithHeap(lpBuffer, (bEncode) ? nBufferSize : nBufferSize * sizeof(RT_CHAR), lpHeapBuffer, lpHeapBufferSize, (void**)lpOutput, nOutputTerminatingZeroSize, _R("Null encoding/decoding result"), lpHeap)) goto handle_error;
    for (nI = 0; nI < nOutputTerminatingZeroSize; nI++)
    {
      (*lpOutput)[nI] = 0;
    }
    nResult = 0;
    goto free_resources;
  }
#ifdef RT_DEFINE_WINDOWS
  if ((nEncoding == RT_ENCODING_UTF_16)   ||
      (nEncoding == RT_ENCODING_UTF_16LE) ||
      (nEncoding == RT_ENCODING_UTF_16BE) ||
      (nEncoding == RT_ENCODING_UTF_32)   ||
      (nEncoding == RT_ENCODING_UTF_32LE) ||
      (nEncoding == RT_ENCODING_UTF_32BE))
  {
    /* Result -1 input size. */
    if (nInputCharSize == -1)
    {
      /* The RtGetDataSize return size in bytes. */
      nInputCharSize = RtGetDataSize(lpInput, nInputTerminatingZeroSize) / nInputTerminatingZeroSize;
    }
    nResult = RtEncodeOrDecodeUnicodeWithBom(lpInput, nInputCharSize, nInputEncoding, lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, lpOutput, lpHeap, nOutputEncoding, nInputTerminatingZeroSize, nOutputTerminatingZeroSize);
    if ((nResult != -1) && (!bEncode))
    {
      /* Convert result size from bytes to characters. */
      nResult = nResult / nOutputTerminatingZeroSize;
    }
  }
  else
  {
    nResult = RtEncodeOrDecodeUsingWindows(lpInput, nInputSize, nEncoding, lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, lpOutput, lpHeap, bEncode, nOutputTerminatingZeroSize);
  }
#else /* NOT RT_DEFINE_WINDOWS */

  nResult = RtIconvWithEncoding(lpInputEncoding, lpOutputEncoding, lpInput, nInputSize, lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, lpOutput, lpHeap, nInputTerminatingZeroSize, nOutputTerminatingZeroSize);
#endif
free_resources:
  return nResult;

handle_error:
  nResult = -1;
  goto free_resources;
}

RT_N RT_API RtDecodeWithHeap(RT_CHAR8* lpInput, RT_N nInputSize, RT_N nEncoding, RT_CHAR** lpOutput, RT_HEAP** lpHeap)
{
  void* lpHeapBuffer;
  RT_N nHeapBufferSize;

  lpHeapBuffer = RT_NULL;
  nHeapBufferSize = 0;
  return RtDecode(lpInput, nInputSize, nEncoding, RT_NULL, 0, &lpHeapBuffer, &nHeapBufferSize, lpOutput, lpHeap);
}

RT_N RT_API RtDecode(RT_CHAR8* lpInput, RT_N nInputSize, RT_N nEncoding, RT_CHAR* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR** lpOutput, RT_HEAP** lpHeap)
{
  return RtEncodeOrDecode(lpInput, nInputSize, nEncoding, (RT_CHAR8*)lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, (RT_CHAR8**)lpOutput, lpHeap, RT_FALSE);
}

RT_N RT_API RtEncodeWithHeap(RT_CHAR* lpInput, RT_N nInputSize, RT_N nEncoding, RT_CHAR8** lpOutput, RT_HEAP** lpHeap)
{
  void* lpHeapBuffer;
  RT_N nHeapBufferSize;

  lpHeapBuffer = RT_NULL;
  nHeapBufferSize = 0;
  return RtEncode(lpInput, nInputSize, nEncoding, RT_NULL, 0, &lpHeapBuffer, &nHeapBufferSize, lpOutput, lpHeap);
}

RT_N RT_API RtEncode(RT_CHAR* lpInput, RT_N nInputSize, RT_N nEncoding, RT_CHAR8* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap)
{
  return RtEncodeOrDecode((RT_CHAR8*)lpInput, nInputSize, nEncoding, lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, lpOutput, lpHeap, RT_TRUE);
}

RT_N RT_API RtDecodeWithBuffer(RT_CHAR8* lpInput, RT_N nInputSize, RT_N nEncoding, RT_CHAR* lpBuffer, RT_N nBufferSize)
{
  RT_CHAR* lpOutput;
  return RtDecode(lpInput, nInputSize, nEncoding, lpBuffer, nBufferSize, RT_NULL, RT_NULL, &lpOutput, RT_NULL);
}

RT_N RT_API RtEncodeWithBuffer(RT_CHAR* lpInput, RT_N nInputSize, RT_N nEncoding, RT_CHAR8* lpBuffer, RT_N nBufferSize)
{
  RT_CHAR8* lpOutput;
  return RtEncode(lpInput, nInputSize, nEncoding, lpBuffer, nBufferSize, RT_NULL, RT_NULL, &lpOutput, RT_NULL);
}

RT_N RT_API RtGetTerminatingZeroSize(RT_N nEncoding)
{
  RT_N nResult;

  if ((nEncoding == RT_ENCODING_UTF_16) || (nEncoding == RT_ENCODING_UTF_16BE) || (nEncoding == RT_ENCODING_UTF_16LE))
  {
    nResult = 2;
  }
  else if ((nEncoding == RT_ENCODING_UTF_32) || (nEncoding == RT_ENCODING_UTF_32BE) || (nEncoding == RT_ENCODING_UTF_32LE))
  {
    nResult = 4;
  }
  else
  {
    nResult = 1;
  }
  return nResult;
}

RT_N RT_API RtGetDataSize(RT_CHAR8* lpData, RT_N nTerminatingZeroSize)
{
  RT_CHAR8* lpChars;
  RT_UN16* lpWideChars;
  RT_UN32* lpVeryWideChars;
  RT_N nResult;

  if (nTerminatingZeroSize == 2)
  {
    lpWideChars = (RT_UN16*)lpData;
    while (*lpWideChars++);
    nResult = (RT_N)((RT_CHAR8*)lpWideChars - lpData - 2);
  }
  else if (nTerminatingZeroSize == 4)
  {
    lpVeryWideChars = (RT_UN32*)lpData;
    while (*lpVeryWideChars++);
    nResult = (RT_N)((RT_CHAR8*)lpVeryWideChars - lpData - 4);
  }
  else
  {
    lpChars = lpData;
    while (*lpChars++);
    nResult = (RT_N)(lpChars - lpData - 1);
  }
  return nResult;
}
