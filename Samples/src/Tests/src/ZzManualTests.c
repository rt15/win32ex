#include "ZzManualTests.h"
#include "ZzTools.h"

void RT_CALL ZzDisplayFlags()
{
  RtConsole_WriteCString(_R("## Flags:\n\n"));
#ifdef RT_DEFINE_DEBUG
  RtConsole_WriteCString(_R("Debug build.\n"));
#else
  RtConsole_WriteCString(_R("Release build.\n"));
#endif

#ifdef RT_DEFINE_32
  RtConsole_WriteCString(_R("Architecture = 32 bits\n"));
#else
  RtConsole_WriteCString(_R("Architecture = 64 bits\n"));
#endif

#ifdef RT_DEFINE_WINDOWS
  RtConsole_WriteCString(_R("Operating system = Windows\n"));
#else
  RtConsole_WriteCString(_R("Operating system = Linux\n"));
#endif

#ifdef RT_DEFINE_VC
  RtConsole_WriteCString(_R("Compiler = Visual C++\n"));
#else
  RtConsole_WriteCString(_R("Compiler = GCC\n"));
#endif

#ifdef RT_DEFINE_USE_CRT
  RtConsole_WriteCString(_R("Use CRT = true\n"));
#else
  RtConsole_WriteCString(_R("Use CRT = false\n"));
#endif
  RtConsole_WriteStringWithSize(_R("\n"), 1);
}

RT_B RT_CALL ZzMisc()
{
  RT_ARRAY zzMessage;
  RT_CHAR lpMessage[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_ARRAY zzValue;
  RT_CHAR lpValueBuffer[512];
  RT_ARRAY zzConversionBuffer;
  RT_CHAR lpConversionBuffer[64];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_UN unMajorOsVersion;
  RT_UN unMinorOsVersion;
  RT_UN unPatchOsVersion;
  RT_B bResult;

  if (!RtConsole_WriteCString(_R("## Misc:\n\n"))) goto handle_error;

  /* Executable path. */
  unWritten = 0;
  if (!RtChar_CopyString(_R("Executable path = "), &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtFileSystem_GetExecutableFilePath(         &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\n"),                 &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!RtConsole_WriteStringWithSize(lpMessage, unWritten)) goto handle_error;

  /* Environment variables. */
#ifdef RT_DEFINE_WINDOWS
  if (!ZzDisplayEnvVar(_R("SystemRoot"))) goto handle_error;
#else
  if (!ZzDisplayEnvVar(_R("HOME"))) goto handle_error;
#endif

  /* Temporary directory. */
  RtArray_Create(&zzValue, lpValueBuffer, sizeof(RT_CHAR), 512);
  if (!RtFileSystem_GetTempDirectory(&zzValue)) goto handle_error;
  RtArray_Create(&zzMessage, lpMessage, sizeof(RT_CHAR), RT_CHAR_HALF_BIG_STRING_SIZE);
  if (!RtChar_AppendCString(&zzMessage, _R("Temp directory = "))) goto handle_error;
  if (!RtArray_Append(&zzMessage, &zzValue)) goto handle_error;
  if (!RtChar_Append(&zzMessage, _R('\n'))) goto handle_error;
  if (!RtConsole_WriteString(&zzMessage)) goto handle_error;

  /* Application configuration directory. */
  RtArray_Create(&zzValue, lpValueBuffer, sizeof(RT_CHAR), 512);
  if (!RtFileSystem_GetApplicationDataDirectory(_R("Tests"), &zzValue)) goto handle_error;
  RtArray_Create(&zzMessage, lpMessage, sizeof(RT_CHAR), RT_CHAR_HALF_BIG_STRING_SIZE);
  if (!RtChar_AppendCString(&zzMessage, _R("App config dir = "))) goto handle_error;
  if (!RtArray_Append(&zzMessage, &zzValue)) goto handle_error;
  if (!RtChar_Append(&zzMessage, _R('\n'))) goto handle_error;
  if (!RtConsole_WriteString(&zzMessage)) goto handle_error;

  /* OS version. */
  if (!RtSystemInfo_GetOsVersion(&unMajorOsVersion, &unMinorOsVersion, &unPatchOsVersion)) goto handle_error;

  RtArray_Create(&zzMessage, lpMessage, sizeof(RT_CHAR), RT_CHAR_HALF_BIG_STRING_SIZE);
  RtArray_Create(&zzConversionBuffer, lpConversionBuffer, sizeof(RT_CHAR), 64);

  if (!RtChar_AppendCString(&zzMessage, _R("OS version = "))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unMajorOsVersion, &zzConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&zzMessage, &zzConversionBuffer)) goto handle_error;
  if (!RtChar_Append(&zzMessage, _R('.'))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unMinorOsVersion, &zzConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&zzMessage, &zzConversionBuffer)) goto handle_error;
  if (!RtChar_Append(&zzMessage, _R('.'))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unPatchOsVersion, &zzConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&zzMessage, &zzConversionBuffer)) goto handle_error;
  if (!RtChar_Append(&zzMessage, _R('\n'))) goto handle_error;
  if (!RtConsole_WriteString(&zzMessage)) goto handle_error;

  if (!RtConsole_WriteStringWithSize(_R("\n"), 1)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzManuallyTestProcess()
{
  RT_CHAR* lpApplicationPathAndArgs[5];
  RT_PROCESS zzProcess;
  RT_UN32 unExitCode;
  RT_B bResult;

  if (!RtConsole_WriteCString(_R("## Processes:\n\n"))) goto handle_error;

  if (!RtConsole_WriteCString(_R("=========================="))) goto handle_error;

  lpApplicationPathAndArgs[0] = _R("ping");
  lpApplicationPathAndArgs[1] = _R("localhost");

#ifdef RT_DEFINE_WINDOWS
  lpApplicationPathAndArgs[2] = _R("-n");

#else
  lpApplicationPathAndArgs[2] = _R("-c");
#endif

  lpApplicationPathAndArgs[3] = _R("3");
  lpApplicationPathAndArgs[4] = RT_NULL;

  if (!RtProcess_Create(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  if (!RtProcess_Join(&zzProcess)) goto handle_error;
  if (!RtConsole_WriteCString(_R("==========================\n"))) goto handle_error;
  if (!RtConsole_WriteCString(_R("Joined!\n"))) goto handle_error;
  if (!RtProcess_GetExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (unExitCode) goto handle_error;
  if (!RtProcess_Free(&zzProcess)) goto handle_error;

  if (!RtConsole_WriteStringWithSize(_R("\n"), 1)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzManuallyTestInput()
{
  RT_ARRAY zzMessage;
  RT_CHAR lpMessage[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_ARRAY zzConversionBuffer;
  RT_CHAR lpConversionBuffer[64];
  RT_CHAR lpLine[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_CHAR nRead;
  RT_UN unRead;
  RT_B bResult;

  if (!RtConsole_WriteCString(_R("## Input:\n\n"))) goto handle_error;

  if (!RtConsole_WriteCString(_R("Test accented character writing: \"é\"\n"))) goto handle_error;

  if (!RtConsole_WriteCString(_R("Test single character reading:\n"))) goto handle_error;
  nRead = RtConsole_ReadChar();

  RtArray_Create(&zzMessage, lpMessage, sizeof(RT_CHAR), RT_CHAR_HALF_BIG_STRING_SIZE);

  if (!RtChar_AppendCString(&zzMessage, _R("Read character: \""))) goto handle_error;
  if (!RtChar_Append(&zzMessage, nRead)) goto handle_error;
  if (!RtChar_AppendCString(&zzMessage, _R("\", value: "))) goto handle_error;

  RtArray_Create(&zzConversionBuffer, lpConversionBuffer, sizeof(RT_CHAR), 64);
  if (!RtChar_ConvertUnsignedIntegerToString((RT_UCHAR)nRead, &zzConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&zzMessage, &zzConversionBuffer)) goto handle_error;

  if (!RtChar_AppendCString(&zzMessage, _R(".\n"))) goto handle_error;
  if (!RtConsole_WriteString(&zzMessage)) goto handle_error;

  if (!RtConsole_WriteCString(_R("Test line reading:\n"))) goto handle_error;
  unRead = RtConsole_ReadLine(lpLine, RT_CHAR_THIRD_BIG_STRING_SIZE);
  if (unRead == RT_TYPE_MAX_UN) goto handle_error;

  RtArray_Create(&zzMessage, lpMessage, sizeof(RT_CHAR), RT_CHAR_HALF_BIG_STRING_SIZE);
  if (!RtChar_ConvertUnsignedIntegerToString(unRead, &zzMessage)) goto handle_error;
  if (!RtChar_AppendCString(&zzMessage, _R(" characters read: \""))) goto handle_error;
  if (!RtChar_AppendCString(&zzMessage, lpLine)) goto handle_error;
  if (!RtChar_AppendCString(&zzMessage, _R("\".\n"))) goto handle_error;
  if (!RtConsole_WriteString(&zzMessage)) goto handle_error;

  if (!RtConsole_WriteStringWithSize(_R("\n"), 1)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzManualTests()
{
  RT_B bResult;

  RtConsole_WriteStringWithSize(_R("\n"), 1);

  ZzDisplayFlags();
  if (!ZzMisc()) goto handle_error;
  if (!ZzTestErrorMessage()) goto handle_error;
  if (!ZzManuallyTestProcess()) goto handle_error;
  if (!ZzManuallyTestInput()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
