#include "ZzTests.h"

#define ZZ_IS_UNSIGNED(a) (a>=0 && ((a=~a)>=0 ? (a=~a, 1) : (a=~a, 0)))

void RT_CALL ZzTestFlags()
{
#if defined(RT_DEFINE_32) && defined(RT_DEFINE_64)
  #error Both RT_DEFINE_32 and RT_DEFINE_64 are defined.
#endif

#if !defined(RT_DEFINE_32) && !defined(RT_DEFINE_64)
  #error Either RT_DEFINE_32 or RT_DEFINE_64 must be defined.
#endif

#if defined(RT_DEFINE_VC) && defined(RT_DEFINE_GCC)
  #error Both RT_DEFINE_VC and RT_DEFINE_GCC are defined.
#endif

#if !defined(RT_DEFINE_VC) && !defined(RT_DEFINE_GCC)
  #error Either RT_DEFINE_VC or RT_DEFINE_GCC must be defined.
#endif

#if defined(RT_DEFINE_WINDOWS) && defined(RT_DEFINE_LINUX)
  #error Both RT_DEFINE_WINDOWS and RT_DEFINE_LINUX are defined.
#endif

#if !defined(RT_DEFINE_WINDOWS) && !defined(RT_DEFINE_LINUX)
  #error Either RT_DEFINE_WINDOWS or RT_DEFINE_LINUX must be defined.
#endif
}

RT_B RT_CALL ZzTestType(RT_CHAR* lpTypeName, RT_UN unSize, RT_B bSigned, RT_UN unExpectedSize, RT_B bExpectedSignedness)
{
  RT_CHAR lpBuffer[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  RtChar_CopyString(lpTypeName,                                        &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten);
  RtChar_CopyString(_R(" size = "),                                    &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten);
  RtChar_ConvertIntegerToString(unSize,                                &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten);
  RtChar_CopyString(bSigned ? _R(", signed.\n") : _R(", unsigned.\n"), &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten);
  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  if (unSize != unExpectedSize) goto handle_error;
  if ((bExpectedSignedness && !bSigned) || (!bExpectedSignedness && bSigned)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestTypesSizes()
{
  RT_UN unExpectedCharSize;
  RT_UN unExpectedArchiSize;

  RT_CHAR8 cChar8;
  RT_UCHAR8 ucUChar8;
  RT_UN16 unUn16;
  RT_CHAR cChar;
  RT_UCHAR ucUChar;
  RT_N32 nN32;
  RT_UN32 unUn32;
  RT_N64 nN64;
  RT_UN64 unUn64;
  RT_N nN;
  RT_UN unUn;

  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  unExpectedCharSize = 16;
#else
  unExpectedCharSize = 8;
#endif

#ifdef RT_DEFINE_32
  unExpectedArchiSize = 32;
#else
  unExpectedArchiSize = 64;
#endif

  cChar8 = -1;
  if (!ZzTestType(_R("RT_CHAR8"), sizeof(RT_CHAR8) * 8, cChar8 < 0, 8, cChar8 < 0)) goto handle_error;

  ucUChar8 = -1;
  if (!ZzTestType(_R("RT_UCHAR8"), sizeof(RT_UCHAR8) * 8, ucUChar8 < 0, 8, RT_FALSE)) goto handle_error;

  unUn16 = -1;
  if (!ZzTestType(_R("RT_UN16"), sizeof(RT_UN16) * 8, unUn16 < 0, 16, RT_FALSE)) goto handle_error;

  cChar = -1;
  if (!ZzTestType(_R("RT_CHAR"), sizeof(RT_CHAR) * 8, cChar < 0, unExpectedCharSize, cChar < 0)) goto handle_error;

  ucUChar = -1;
  if (!ZzTestType(_R("RT_UCHAR"), sizeof(RT_UCHAR) * 8, ucUChar < 0, unExpectedCharSize, RT_FALSE)) goto handle_error;

  nN32 = -1;
  if (!ZzTestType(_R("RT_N32"), sizeof(RT_N32) * 8, nN32 < 0, 32, RT_TRUE)) goto handle_error;

  unUn32 = -1;
  if (!ZzTestType(_R("RT_UN32"), sizeof(RT_UN32) * 8, unUn32 < 0, 32, RT_FALSE)) goto handle_error;

  nN64 = -1;
  if (!ZzTestType(_R("RT_N64"), sizeof(RT_N64) * 8, nN64 < 0, 64, RT_TRUE)) goto handle_error;

  unUn64 = -1;
  if (!ZzTestType(_R("RT_UN64"), sizeof(RT_UN64) * 8, unUn64 < 0, 64, RT_FALSE)) goto handle_error;

  nN = -1;
  if (!ZzTestType(_R("RT_N"), sizeof(RT_N) * 8, nN < 0, unExpectedArchiSize, RT_TRUE)) goto handle_error;

  unUn = -1;
  if (!ZzTestType(_R("RT_UN"), sizeof(RT_UN) * 8, unUn < 0, unExpectedArchiSize, RT_FALSE)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestTypes()
{
  RT_B bResult;

  /* ZzTestFlags is testing using #error. */
  ZzTestFlags();
  if (!ZzTestTypesSizes()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
