#include "ZzTests.h"

RT_B RT_CALL ZzTestEnvVar()
{
  RT_CHAR* lpVariableName;
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (RtEnvVar_Get(_R("Non-existing name"), lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;

#ifdef RT_DEFINE_WINDOWS
  lpVariableName = _R("SystemRoot");
#else
  lpVariableName = _R("HOME");
#endif

  unWritten = 0;
  if (!RtEnvVar_Get(lpVariableName, lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;

  if (!RtChar_GetStringSize(lpBuffer)) goto handle_error;

  if (!RtEnvVar_Set(_R("RT_FOO"), _R("BAR"))) goto handle_error;

  unWritten = 0;
  if (!RtEnvVar_Get(_R("RT_FOO"), lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpBuffer, _R("BAR"))) goto handle_error;

  if (!RtEnvVar_Delete(_R("RT_FOO"))) goto handle_error;
  unWritten = 0;
  if (RtEnvVar_Get(_R("RT_FOO"), lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &unWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
