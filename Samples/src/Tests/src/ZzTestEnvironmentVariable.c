#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestEnvironmentVariable()
{
  RT_CHAR* lpVariableName;
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (RtGetEnvironmentVariable(_R("Non-existing name"), lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;

#ifdef RT_DEFINE_WINDOWS
  lpVariableName = _R("SystemRoot");
#else
  lpVariableName = _R("HOME");
#endif

  unWritten = 0;
  if (!RtGetEnvironmentVariable(lpVariableName, lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;

  if (!RtGetStringSize(lpBuffer)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}