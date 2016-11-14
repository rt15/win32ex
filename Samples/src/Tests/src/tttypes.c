#include <RtWin32Ex.h>

#define TT_IS_UNSIGNED(a) (a>=0 && ((a=~a)>=0 ? (a=~a, 1) : (a=~a, 0)))

void RT_CALL TtDisplayFlags()
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

RT_UN16 RT_CALL TtTestType(RT_CHAR* lpTypeName, RT_N nSize, RT_B bSigned, RT_N nExpectedSize, RT_B bExpectedSignedness)
{
  RT_CHAR lpBuffer[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_N nWritten;
  RT_UN16 unResult;

  unResult = 1;

  nWritten = 0;
  RtCopyString(lpTypeName,                                        &lpBuffer[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - nWritten, &nWritten);
  RtCopyString(_R(" size = "),                                    &lpBuffer[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - nWritten, &nWritten);
  RtConvertNumberToString(nSize,                                  &lpBuffer[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - nWritten, &nWritten);
  RtCopyString(bSigned ? _R(", signed.\n") : _R(", unsigned.\n"), &lpBuffer[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  if (nSize != nExpectedSize) goto the_end;
  if ((bExpectedSignedness && !bSigned) || (!bExpectedSignedness && bSigned)) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestTypesSizes()
{
  RT_N nExpectedCharSize;
  RT_N nExpectedArchiSize;

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

  RT_UN16 unResult;

  unResult = 1;

#ifdef RT_DEFINE_WINDOWS
  nExpectedCharSize = 16;
#else
  nExpectedCharSize = 8;
#endif

#ifdef RT_DEFINE_32
  nExpectedArchiSize = 32;
#else
  nExpectedArchiSize = 64;
#endif

  cChar8 = -1;
  if (TtTestType(_R("RT_CHAR8"), sizeof(RT_CHAR8) * 8, cChar8 < 0, 8, cChar8 < 0)) goto the_end;

  ucUChar8 = -1;
  if (TtTestType(_R("RT_UCHAR8"), sizeof(RT_UCHAR8) * 8, ucUChar8 < 0, 8, RT_FALSE)) goto the_end;

  unUn16 = -1;
  if (TtTestType(_R("RT_UN16"), sizeof(RT_UN16) * 8, unUn16 < 0, 16, RT_FALSE)) goto the_end;

  cChar = -1;
  if (TtTestType(_R("RT_CHAR"), sizeof(RT_CHAR) * 8, cChar < 0, nExpectedCharSize, cChar < 0)) goto the_end;

  ucUChar = -1;
  if (TtTestType(_R("RT_UCHAR"), sizeof(RT_UCHAR) * 8, ucUChar < 0, nExpectedCharSize, RT_FALSE)) goto the_end;

  nN32 = -1;
  if (TtTestType(_R("RT_N32"), sizeof(RT_N32) * 8, nN32 < 0, 32, RT_TRUE)) goto the_end;

  unUn32 = -1;
  if (TtTestType(_R("RT_UN32"), sizeof(RT_UN32) * 8, unUn32 < 0, 32, RT_FALSE)) goto the_end;

  nN64 = -1;
  if (TtTestType(_R("RT_N64"), sizeof(RT_N64) * 8, nN64 < 0, 64, RT_TRUE)) goto the_end;

  unUn64 = -1;
  if (TtTestType(_R("RT_UN64"), sizeof(RT_UN64) * 8, unUn64 < 0, 64, RT_FALSE)) goto the_end;

  nN = -1;
  if (TtTestType(_R("RT_N"), sizeof(RT_N) * 8, nN < 0, nExpectedArchiSize, RT_TRUE)) goto the_end;

  unUn = -1;
  if (TtTestType(_R("RT_UN"), sizeof(RT_UN) * 8, unUn < 0, nExpectedArchiSize, RT_FALSE)) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestTypes()
{
  RT_UN16 unResult;

  unResult = 1;

  TtDisplayFlags();
  if (TtTestTypesSizes()) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}
