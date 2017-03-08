#include <RtWin32Ex.h>

#include "ZzTools.h"

RT_B RT_CALL ZzTestRedirectToFile(RT_HEAP** lpHeap)
{
  RT_CHAR lpExecutablePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpTempFile[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_B bFileCreated;
  RT_B bDeleteTempFile;
  RT_FILE zzFile;
  RT_PROCESS zzProcess;
  RT_B bProcessCreated;
  RT_UN32 unExitCode;
  RT_B bResult;

  bFileCreated = RT_FALSE;
  bProcessCreated = RT_FALSE;
  bDeleteTempFile = RT_FALSE;

  unWritten = 0;
  if (!RtGetExecutableFilePath(lpExecutablePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  unWritten = 0;
  if (!RtCreateTempFile(&zzFile, _R("Zz"), lpTempFile, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  bFileCreated = RT_TRUE;
  bDeleteTempFile = RT_TRUE;

  if (!RtCreateProcessWithRedirections(&zzProcess, RT_NULL, RT_NULL, &zzFile, RT_NULL, lpExecutablePath, _R("--args"), _R("foo"), RT_NULL)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (unExitCode) goto handle_error;

  if (!ZzCheckTextFile(lpTempFile, lpHeap, lpExecutablePath,
                                           _R("--args"),
                                           _R("foo"),
                                           RT_NULL)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtFreeProcess(&zzProcess) && bResult) goto handle_error;
  }
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtFreeFile(&zzFile) && bResult) goto handle_error;
  }
  if (bDeleteTempFile)
  {
    bDeleteTempFile = RT_FALSE;
    if (!RtDeleteFile(lpTempFile) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestFailingProcess()
{
  RT_PROCESS zzProcess;
  RT_B bProcessCreated;
  RT_UN32 unExitCode;
  RT_B bResult;

  bProcessCreated = RT_FALSE;

  /* Test wrong process name. */
#ifdef RT_DEFINE_WINDOWS
  if (RtCreateProcess(&zzProcess, RT_NULL, _R("pong"), _R("localhost"), RT_NULL)) goto handle_error;
#else

  /* Under Linux, the fork is ok then execvp fails but there is no way to know it. */

  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;

  if (!RtCreateProcess(&zzProcess, _R("Z:\\fake_directory"), _R("pong"), _R("localhost"), RT_NULL)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;
  if (!RtWriteStringToConsole(_R("Joined!\n"))) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (!unExitCode) goto handle_error;

  bProcessCreated = RT_FALSE;
  if (!RtFreeProcess(&zzProcess)) goto handle_error;

#endif

  /* Test bad argument. */
  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;

  if (!RtCreateProcess(&zzProcess, RT_NULL, _R("ping"), _R("-pong"), RT_NULL)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;
  if (!RtWriteStringToConsole(_R("Joined!\n"))) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (!unExitCode) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtFreeProcess(&zzProcess) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestProcess(RT_HEAP** lpHeap)
{
  RT_B bResult;

  if (!ZzTestRedirectToFile(lpHeap)) goto handle_error;
  if (!ZzTestFailingProcess()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
