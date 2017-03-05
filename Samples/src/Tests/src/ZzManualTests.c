#include <RtWin32Ex.h>

RT_B RT_CALL ZzDisplayEnvironmentVariable(RT_CHAR* lpName)
{
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (!RtCopyString(lpName,             &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R(" env var = "),  &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtGetEnvironmentVariable(lpName, &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\n"),           &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;

  if (!RtWriteStringToConsoleWithSize(lpBuffer, unWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

void RT_CALL ZzDisplayFlags()
{
  RtWriteStringToConsole(_R("## Flags:\n\n"));
#ifdef RT_DEFINE_DEBUG
  RtWriteStringToConsole(_R("Debug build.\n"));
#else
  RtWriteStringToConsole(_R("Release build.\n"));
#endif

#ifdef RT_DEFINE_32
  RtWriteStringToConsole(_R("Architecture = 32 bits\n"));
#else
  RtWriteStringToConsole(_R("Architecture = 64 bits\n");
#endif

#ifdef RT_DEFINE_WINDOWS
  RtWriteStringToConsole(_R("Operating system = Windows\n"));
#else
  RtWriteStringToConsole(_R("Operating system = Linux\n"));
#endif

#ifdef RT_DEFINE_VC
  RtWriteStringToConsole(_R("Compiler = Visual C++\n"));
#else
  RtWriteStringToConsole(_R("Compiler = GCC\n"));
#endif

#ifdef RT_DEFINE_USE_CRT
  RtWriteStringToConsole(_R("Use CRT = true\n"));
#else
  RtWriteStringToConsole(_R("Use CRT = false\n"));
#endif
  RtWriteStringToConsoleWithSize(_R("\n"), 1);
}

RT_B RT_CALL ZzMisc()
{
  RT_CHAR lpMessage[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  if (!RtWriteStringToConsole(_R("## Misc:\n\n"))) goto handle_error;

  /* Executable path. */
  unWritten = 0;
  if (!RtCopyString(_R("Executable path = "),  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtGetExecutableFilePath(                &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\n"),                  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;

  if (!RtWriteStringToConsoleWithSize(lpMessage, unWritten)) goto handle_error;

  /* Environement variables. */
#ifdef RT_DEFINE_WINDOWS
  if (!ZzDisplayEnvironmentVariable(_R("SystemRoot"))) goto handle_error;
#else
  if (!ZzDisplayEnvironmentVariable(_R("HOME"))) goto handle_error;
#endif


  /* Temporary directory. */
  unWritten = 0;
  if (!RtCopyString(_R("Temp directory = "),  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtGetTempDirectory(                    &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\n"),                 &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;

  if (!RtWriteStringToConsoleWithSize(lpMessage, unWritten)) goto handle_error;

  /* Application configuration directory. */
  unWritten = 0;
  if (!RtGetApplicationDataDirectory(_R("Tests"), lpMessage, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (!RtWriteStringsToConsole(3, _R("App config dir = "), lpMessage, _R("\n"))) goto handle_error;

  if (!RtWriteStringToConsoleWithSize(_R("\n"), 1)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestInput()
{
  RT_CHAR lpMessage[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_CHAR lpLine[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_CHAR nRead;
  RT_UN unRead;
  RT_B bResult;

  if (!RtWriteStringToConsole(_R("## Input:\n\n"))) goto handle_error;

  if (!RtWriteStringToConsole(_R("Test accented character writing: \"�\"\n"))) goto handle_error;

  if (!RtWriteStringToConsole(_R("Test single character reading:\n"))) goto handle_error;
  nRead = RtReadCharFromConsole();

  unWritten = 0;
  if (!RtCopyString(_R("Read   character: \""),    &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyStringWithSize(&nRead, 1,             &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\", value: "),             &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtConvertUIntegerToString((RT_UCHAR)nRead,  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R(".\n"),                     &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;

  if (!RtWriteStringToConsoleWithSize(lpMessage, unWritten)) goto handle_error;

  if (!RtWriteStringToConsole(_R("Test line reading:\n"))) goto handle_error;
  unRead = RtReadLineFromConsole(lpLine, RT_CHAR_THIRD_BIG_STRING_SIZE);
  if (unRead == RT_TYPE_MAX_UN) goto handle_error;
  unWritten = 0;
  if (!RtConvertUIntegerToString(unRead,         &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R(" characters read: \""),  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(lpLine,                      &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\".\n"),                 &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;

  if (!RtWriteStringToConsoleWithSize(lpMessage, unWritten)) goto handle_error;

  if (!RtWriteStringToConsoleWithSize(_R("\n"), 1)) goto handle_error;

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

  RtWriteStringToConsoleWithSize(_R("\n"), 1);

  ZzDisplayFlags();
  if (!ZzMisc()) goto handle_error;
  if (!ZzTestInput()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
