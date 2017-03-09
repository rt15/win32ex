#include <RtWin32Ex.h>

#include "ZzTools.h"

RT_B RT_CALL ZzTestRedirectStdInToPipe(RT_HEAP** lpHeap)
{
  RT_CHAR lpExecutablePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpTempFile[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_FILE zzFile;
  RT_B bFileCreated;
  RT_B bDeleteTempFile;
  RT_FILE zzReadPipe;
  RT_FILE zzWritePipe;
  RT_B bReadPipeCreated;
  RT_B bWritePipeCreated;
  RT_PROCESS zzProcess;
  RT_B bProcessCreated;
  RT_UN32 unExitCode;
  RT_B bResult;

  bReadPipeCreated = RT_FALSE;
  bWritePipeCreated = RT_FALSE;
  bProcessCreated = RT_FALSE;

  unWritten = 0;
  if (!RtGetExecutableFilePath(lpExecutablePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  unWritten = 0;
  if (!RtCreateTempFile(&zzFile, _R("Zz"), lpTempFile, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  bFileCreated = RT_TRUE;
  bDeleteTempFile = RT_TRUE;

  if (!RtCreatePipe(&zzReadPipe, &zzWritePipe)) goto handle_error;
  bReadPipeCreated = RT_TRUE;
  bWritePipeCreated = RT_TRUE;

  if (!RtCreateProcessWithRedirections(&zzProcess, RT_TRUE, RT_NULL, &zzReadPipe, &zzFile, RT_NULL, lpExecutablePath, _R("--read-line"), (RT_CHAR*)RT_NULL)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!ZzWriteLinesToFile(&zzWritePipe, lpHeap, _R("123"), (RT_CHAR*)RT_NULL)) goto handle_error;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (unExitCode) goto handle_error;

  if (!ZzCheckTextFile(lpTempFile, lpHeap, _R("\"123\""), (RT_CHAR*)RT_NULL)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtFreeProcess(&zzProcess) && bResult) goto handle_error;
  }
  if (bWritePipeCreated)
  {
    bWritePipeCreated = RT_FALSE;
    if (!RtFreeFile(&zzWritePipe) && bResult) goto handle_error;
  }
  if (bReadPipeCreated)
  {
    bReadPipeCreated = RT_FALSE;
    if (!RtFreeFile(&zzReadPipe) && bResult) goto handle_error;
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

RT_B RT_CALL ZzTestRedirectStdOutToFile(RT_HEAP** lpHeap)
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

  if (!RtCreateProcessWithRedirections(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, &zzFile, RT_NULL, lpExecutablePath,
                                                                                                _R("--args"),
                                                                                                _R("foo"),
                                                                                                _R("a b"),
                                                                                                _R("c:\\"),
                                                                                                _R("c:\\Program Files\\"),
                                                                                                _R("c:\\\\Program Files\\\\"),
                                                                                                _R("\"bar\""),
                                                                                                _R("b\\a\\r"),
                                                                                                _R("b\\\t\\r"),
                                                                                                _R("b\"r"),
                                                                                                _R("b\\\"r"),
                                                                                                (RT_CHAR*)RT_NULL)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (unExitCode) goto handle_error;

  if (!ZzCheckTextFile(lpTempFile, lpHeap, lpExecutablePath,
                                           _R("--args"),
                                           _R("foo"),
                                           _R("a b"),
                                           _R("c:\\"),
                                           _R("c:\\Program Files\\"),
                                           _R("c:\\\\Program Files\\\\"),
                                           _R("\"bar\""),
                                           _R("b\\a\\r"),
                                           _R("b\\\t\\r"),
                                           _R("b\"r"),
                                           _R("b\\\"r"),
                                           (RT_CHAR*)RT_NULL)) goto handle_error;

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

RT_B RT_CALL ZzTestRedirectStdErrToFile()
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
  RT_UN unFileSize;
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

  if (!RtCreateProcessWithRedirections(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, RT_NULL, &zzFile, lpExecutablePath, _R("--bad"), (RT_CHAR*)RT_NULL)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (!unExitCode) goto handle_error;

  unFileSize = RtGetFileSize(lpTempFile);
  if (unFileSize == RT_TYPE_MAX_UN || unFileSize < 10) goto handle_error;

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
  if (RtCreateProcess(&zzProcess, RT_TRUE, RT_NULL, _R("pong"), _R("localhost"), (RT_CHAR*)RT_NULL)) goto handle_error;
  if (RtCreateProcess(&zzProcess, RT_FALSE, RT_NULL, _R("pong"), _R("localhost"), (RT_CHAR*)RT_NULL)) goto handle_error;

  /* Test wrong folder name. */
  if (RtCreateProcess(&zzProcess, RT_TRUE, _R("Wrong, wrong directory"), _R("ping"), _R("localhost"), (RT_CHAR*)RT_NULL)) goto handle_error;
  if (RtCreateProcess(&zzProcess, RT_FALSE, _R("Wrong, wrong directory"), _R("ping"), _R("localhost"), (RT_CHAR*)RT_NULL)) goto handle_error;

  /* Test bad argument. */
  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;

  if (!RtCreateProcess(&zzProcess, RT_TRUE, RT_NULL, _R("ping"), _R("-pong"), (RT_CHAR*)RT_NULL)) goto handle_error;
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

  if (!ZzTestRedirectStdInToPipe(lpHeap)) goto handle_error;
  if (!ZzTestRedirectStdOutToFile(lpHeap)) goto handle_error;
  if (!ZzTestRedirectStdErrToFile()) goto handle_error;
  if (!ZzTestFailingProcess()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
