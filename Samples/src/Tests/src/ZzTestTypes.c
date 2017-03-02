#include <RtWin32Ex.h>

#define TT_IS_UNSIGNED(a) (a>=0 && ((a=~a)>=0 ? (a=~a, 1) : (a=~a, 0)))

void RT_CALL ZzTestDisplayFlags()
{
#ifdef RT_DEFINE_32
  RtWriteStringToConsole(_R("Architecture = 32 bits.\n"));
#else
  RtWriteStringToConsole(_R("Architecture = 64 bits\n");
#endif

#ifdef RT_DEFINE_WINDOWS
  RtWriteStringToConsole(_R("Operating system = Windows.\n"));
#else
  RtWriteStringToConsole(_R("Operating system = Linux.\n"));
#endif

#ifdef RT_DEFINE_VC
  RtWriteStringToConsole(_R("Compiler = Visual C++.\n"));
#else
  RtWriteStringToConsole(_R("Compiler = GCC.\n"));
#endif

#ifdef RT_DEFINE_USE_CRT
  RtWriteStringToConsole(_R("Use CRT = true.\n"));
#else
  RtWriteStringToConsole(_R("Use CRT = false.\n"));
#endif
}

RT_B RT_CALL ZzTestType(RT_CHAR* lpTypeName, RT_UN unSize, RT_B bSigned, RT_UN unExpectedSize, RT_B bExpectedSignedness)
{
  RT_CHAR lpBuffer[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  RtCopyString(lpTypeName,                                        &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten);
  RtCopyString(_R(" size = "),                                    &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten);
  RtConvertIntegerToString(unSize,                                &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten);
  RtCopyString(bSigned ? _R(", signed.\n") : _R(", unsigned.\n"), &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten);
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

  ZzTestDisplayFlags();
  if (!ZzTestTypesSizes()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
