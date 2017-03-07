#include <RtWin32Ex.h>


/**
 * A successful process is tested in manual tests.
 */
RT_B RT_CALL ZzTestProcess()
{
  RT_PROCESS zzProcess;
  RT_UN32 unExitCode;
  RT_B bResult;

  /* Test wrong process name. */
#ifdef RT_DEFINE_WINDOWS
  if (RtCreateProcess(&zzProcess, RT_NULL, _R("pong"), _R("localhost"), RT_NULL)) goto handle_error;
#else

  /* Under Linux, the fork is ok then execvp fails but there is no way to know it. */

  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;

  if (!RtCreateProcess(&zzProcess, _R("Z:\\fake_directory"), _R("pong"), _R("localhost"), RT_NULL)) goto handle_error;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;
  if (!RtWriteStringToConsole(_R("Joined!\n"))) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (!unExitCode) goto handle_error;
  if (!RtFreeProcess(&zzProcess)) goto handle_error;
#endif

  /* Test bad argument. */
  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;

  if (!RtCreateProcess(&zzProcess, RT_NULL, _R("ping"), _R("-pong"), RT_NULL)) goto handle_error;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;
  if (!RtWriteStringToConsole(_R("Joined!\n"))) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (!unExitCode) goto handle_error;
  if (!RtFreeProcess(&zzProcess)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
