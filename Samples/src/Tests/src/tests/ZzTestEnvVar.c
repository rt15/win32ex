#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestEnvVar(RT_HEAP** lpHeap)
{
  RT_DYNAMIC_ARRAY zzValue;
  RT_CHAR lpBuffer[2];
  RT_ARRAY zzMessage;
  RT_CHAR lpMessageBuffer[512];
  RT_B bValueCreated;
  RT_CHAR* lpVariableName;
  RT_B bResult;

  bValueCreated = RT_FALSE;

  RtDynamicArray_Create(&zzValue, lpBuffer, sizeof(RT_CHAR), 2, lpHeap);
  bValueCreated = RT_TRUE;

  /* Asking for the value of an unknown variable should result in error. */
  if (RtEnvVar_Get(_R("Non-Existing name"), &zzValue.rtArray)) goto handle_error;

#ifdef RT_DEFINE_WINDOWS
  lpVariableName = _R("SystemRoot");
#else
  lpVariableName = _R("HOME");
#endif

  if (!RtEnvVar_Get(lpVariableName, &zzValue.rtArray)) goto handle_error;

  /* Display the environment variable value. */
  RtArray_Create(&zzMessage, lpMessageBuffer, sizeof(RT_CHAR), 512);
  if (!RtChar_AppendCString(&zzMessage, lpVariableName)) goto handle_error;
  if (!RtChar_Append(&zzMessage, _R('='))) goto handle_error;
  if (!RtArray_Append(&zzMessage, &zzValue.rtArray)) goto handle_error;
  if (!RtChar_Append(&zzMessage, _R('\n'))) goto handle_error;
  if (!RtConsole_WriteString(&zzMessage)) goto handle_error;

  /* Add a new environment variable and get it. */
  if (!RtEnvVar_Set(_R("RT_FOO"), _R("BAR"))) goto handle_error;
  if (!RtEnvVar_Get(_R("RT_FOO"), &zzValue.rtArray)) goto handle_error;
  if (!RtChar_StringEqualsCString(&zzValue.rtArray, _R("BAR"))) goto handle_error;

  /* Remove the new environment variable and check that it cannot be retrieved anymore. */
  if (!RtEnvVar_Delete(_R("RT_FOO"))) goto handle_error;
  if (RtEnvVar_Get(_R("RT_FOO"), &zzValue.rtArray)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bValueCreated)
  {
    bValueCreated = RT_FALSE;
    if (!RtDynamicArray_Free(&zzValue) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
