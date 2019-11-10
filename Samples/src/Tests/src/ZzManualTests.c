#include "ZzManualTests.h"

RT_B RT_CALL ZzManualDisplayEnvVar(RT_CHAR* lpName)
{
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  unWritten = 0;
  if (!RtChar_CopyString(lpName,            &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R(" env var = "), &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtEnvVar_Get(lpName,                 &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\n"),          &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!RtConsole_WriteStringWithSize(lpBuffer, unWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

void RT_CALL ZzDisplayFlags()
{
  RtConsole_WriteString(_R("## Flags:\n\n"));
#ifdef RT_DEFINE_DEBUG
  RtConsole_WriteString(_R("Debug build.\n"));
#else
  RtConsole_WriteString(_R("Release build.\n"));
#endif

#ifdef RT_DEFINE_32
  RtConsole_WriteString(_R("Architecture = 32 bits\n"));
#else
  RtConsole_WriteString(_R("Architecture = 64 bits\n");
#endif

#ifdef RT_DEFINE_WINDOWS
  RtConsole_WriteString(_R("Operating system = Windows\n"));
#else
  RtConsole_WriteString(_R("Operating system = Linux\n"));
#endif

#ifdef RT_DEFINE_VC
  RtConsole_WriteString(_R("Compiler = Visual C++\n"));
#else
  RtConsole_WriteString(_R("Compiler = GCC\n"));
#endif

#ifdef RT_DEFINE_USE_CRT
  RtConsole_WriteString(_R("Use CRT = true\n"));
#else
  RtConsole_WriteString(_R("Use CRT = false\n"));
#endif
  RtConsole_WriteStringWithSize(_R("\n"), 1);
}

RT_B RT_CALL ZzMisc()
{
  RT_CHAR lpMessage[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_UN unMajorOsVersion;
  RT_UN unMinorOsVersion;
  RT_UN unPatchOsVersion;
  RT_B bResult;

  if (!RtConsole_WriteString(_R("## Misc:\n\n"))) goto handle_error;

  /* Executable path. */
  unWritten = 0;
  if (!RtChar_CopyString(_R("Executable path = "), &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtFileSystem_GetExecutableFilePath(         &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\n"),                 &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!RtConsole_WriteStringWithSize(lpMessage, unWritten)) goto handle_error;

  /* Environement variables. */
#ifdef RT_DEFINE_WINDOWS
  if (!ZzManualDisplayEnvVar(_R("SystemRoot"))) goto handle_error;
#else
  if (!ZzManualDisplayEnvVar(_R("HOME"))) goto handle_error;
#endif

  /* Temporary directory. */
  unWritten = 0;
  if (!RtChar_CopyString(_R("Temp directory = "), &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtFileSystem_GetTempDirectory(             &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\n"),                &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!RtConsole_WriteStringWithSize(lpMessage, unWritten)) goto handle_error;

  /* Application configuration directory. */
  if (!RtFileSystem_GetApplicationDataDirectory(_R("Tests"), lpMessage, RT_CHAR_HALF_BIG_STRING_SIZE, &unOutputSize)) goto handle_error;
  if (unOutputSize != RtChar_GetStringSize(lpMessage)) goto handle_error;
  if (!RtConsole_WriteStringsOrErrors(RT_TRUE, _R("App config dir = "), lpMessage, _R("\n"), (RT_CHAR*)RT_NULL)) goto handle_error;

  /* OS version. */
  if (!RtSystemInfo_GetOsVersion(&unMajorOsVersion, &unMinorOsVersion, &unPatchOsVersion)) goto handle_error;
  unWritten = 0;
  if (!RtChar_CopyString(_R("OS version = "),            &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_ConvertUIntegerToString(unMajorOsVersion,  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyStringWithSize(_R("."), 1,             &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_ConvertUIntegerToString(unMinorOsVersion,  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyStringWithSize(_R("."), 1,             &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_ConvertUIntegerToString(unPatchOsVersion,  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyStringWithSize(_R("\n"), 1,            &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtConsole_WriteStringWithSize(lpMessage, unWritten)) goto handle_error;

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

  if (!RtConsole_WriteString(_R("## Processes:\n\n"))) goto handle_error;

  if (!RtConsole_WriteString(_R("=========================="))) goto handle_error;

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
  if (!RtConsole_WriteString(_R("==========================\n"))) goto handle_error;
  if (!RtConsole_WriteString(_R("Joined!\n"))) goto handle_error;
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
  RT_CHAR lpMessage[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_CHAR lpLine[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_CHAR nRead;
  RT_UN unRead;
  RT_B bResult;

  if (!RtConsole_WriteString(_R("## Input:\n\n"))) goto handle_error;

  if (!RtConsole_WriteString(_R("Test accented character writing: \"é\"\n"))) goto handle_error;

  if (!RtConsole_WriteString(_R("Test single character reading:\n"))) goto handle_error;
  nRead = RtConsole_ReadChar();

  unWritten = 0;
  if (!RtChar_CopyString(_R("Read character: \""),     &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyStringWithSize(&nRead, 1,            &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\", value: "),            &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_ConvertUIntegerToString((RT_UCHAR)nRead, &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R(".\n"),                    &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!RtConsole_WriteStringWithSize(lpMessage, unWritten)) goto handle_error;

  if (!RtConsole_WriteString(_R("Test line reading:\n"))) goto handle_error;
  unRead = RtConsole_ReadLine(lpLine, RT_CHAR_THIRD_BIG_STRING_SIZE);
  if (unRead == RT_TYPE_MAX_UN) goto handle_error;
  unWritten = 0;
  if (!RtChar_ConvertUIntegerToString(unRead,         &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R(" characters read: \""),  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(lpLine,                      &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\".\n"),                 &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!RtConsole_WriteStringWithSize(lpMessage, unWritten)) goto handle_error;

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
