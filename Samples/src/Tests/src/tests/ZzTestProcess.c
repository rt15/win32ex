#include "ZzTests.h"

#include "ZzTools.h"

RT_B RT_CALL ZzTestRedirectStdInToPipe(RT_HEAP** lpHeap)
{
  RT_CHAR lpExecutablePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpTempFile[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_FILE zzFile;
  RT_B bFileCreated;
  RT_B bDeleteTempFile;
  RT_IO_DEVICE zzFileIoDevice;
  RT_PIPE zzPipe;
  RT_IO_DEVICE* lpInput;
  RT_IO_DEVICE* lpOutput;
  RT_B bInputCreated;
  RT_B bOutputCreated;
  RT_CHAR* lpApplicationPathAndArgs[3];

  RT_PROCESS zzProcess;
  RT_B bProcessCreated;
  RT_UN32 unExitCode;
  RT_B bResult;

  bInputCreated = RT_FALSE;
  bOutputCreated = RT_FALSE;
  bProcessCreated = RT_FALSE;

  unWritten = 0;
  if (!RtFileSystem_GetExecutableFilePath(lpExecutablePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  unWritten = 0;
  if (!RtCreateTempFile(&zzFile, _R("Zz"), lpTempFile, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  bFileCreated = RT_TRUE;
  bDeleteTempFile = RT_TRUE;

  if (!RtPipe_Create(&zzPipe)) goto handle_error;
  lpInput = RtPipe_GetInput(&zzPipe);
  lpOutput = RtPipe_GetOutput(&zzPipe);
  bInputCreated = RT_TRUE;
  bOutputCreated = RT_TRUE;

  lpApplicationPathAndArgs[0] = lpExecutablePath;
  lpApplicationPathAndArgs[1] = _R("--read-line");
  lpApplicationPathAndArgs[2] = RT_NULL;

#ifdef RT_DEFINE_WINDOWS
  RtIoDevice_CreateFromHandle(&zzFileIoDevice, zzFile.hFile);
#else
  RtIoDevice_CreateFromFileDescriptor(&zzFileIoDevice, zzFile.nFile);
#endif

  if (!RtCreateProcessWithRedirections(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, lpInput, &zzFileIoDevice, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!ZzWriteLinesToDevice(lpOutput, lpHeap, _R("123"), (RT_CHAR*)RT_NULL)) goto handle_error;

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
  if (bOutputCreated)
  {
    bOutputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpOutput) && bResult) goto handle_error;
  }
  if (bInputCreated)
  {
    bInputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpInput) && bResult) goto handle_error;
  }
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtFreeFile(&zzFile) && bResult) goto handle_error;
  }
  if (bDeleteTempFile)
  {
    bDeleteTempFile = RT_FALSE;
    if (!RtFileSystem_DeleteFile(lpTempFile) && bResult) goto handle_error;
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
  RT_IO_DEVICE zzFileIoDevice;
  RT_CHAR* lpApplicationPathAndArgs[13];
  RT_PROCESS zzProcess;
  RT_B bProcessCreated;
  RT_UN32 unExitCode;
  RT_B bResult;

  bFileCreated = RT_FALSE;
  bProcessCreated = RT_FALSE;
  bDeleteTempFile = RT_FALSE;

  unWritten = 0;
  if (!RtFileSystem_GetExecutableFilePath(lpExecutablePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  unWritten = 0;
  if (!RtCreateTempFile(&zzFile, _R("Zz"), lpTempFile, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  bFileCreated = RT_TRUE;
  bDeleteTempFile = RT_TRUE;

  lpApplicationPathAndArgs[0]  = lpExecutablePath;
  lpApplicationPathAndArgs[1]  = _R("--args");
  lpApplicationPathAndArgs[2]  = _R("foo");
  lpApplicationPathAndArgs[3]  = _R("a b");
  lpApplicationPathAndArgs[4]  = _R("c:\\");
  lpApplicationPathAndArgs[5]  = _R("c:\\Program Files\\");
  lpApplicationPathAndArgs[6]  = _R("c:\\\\Program Files\\\\");
  lpApplicationPathAndArgs[7]  = _R("\"bar\"");
  lpApplicationPathAndArgs[8]  = _R("b\\a\\r");
  lpApplicationPathAndArgs[9]  = _R("b\\\t\\r");
  lpApplicationPathAndArgs[10] = _R("b\"r");
  lpApplicationPathAndArgs[11] = _R("b\\\"r");
  lpApplicationPathAndArgs[12] = RT_NULL;

#ifdef RT_DEFINE_WINDOWS
  RtIoDevice_CreateFromHandle(&zzFileIoDevice, zzFile.hFile);
#else
  RtIoDevice_CreateFromFileDescriptor(&zzFileIoDevice, zzFile.nFile);
#endif

  if (!RtCreateProcessWithRedirections(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, RT_NULL, &zzFileIoDevice, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
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
    if (!RtFileSystem_DeleteFile(lpTempFile) && bResult) goto handle_error;
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
  RT_IO_DEVICE zzFileIoDevice;
  RT_CHAR* lpApplicationPathAndArgs[3];
  RT_PROCESS zzProcess;
  RT_B bProcessCreated;
  RT_UN32 unExitCode;
  RT_UN64 unFileSize;
  RT_B bResult;

  bFileCreated = RT_FALSE;
  bProcessCreated = RT_FALSE;
  bDeleteTempFile = RT_FALSE;

  unWritten = 0;
  if (!RtFileSystem_GetExecutableFilePath(lpExecutablePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  unWritten = 0;
  if (!RtCreateTempFile(&zzFile, _R("Zz"), lpTempFile, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  bFileCreated = RT_TRUE;
  bDeleteTempFile = RT_TRUE;

  lpApplicationPathAndArgs[0] = lpExecutablePath;
  lpApplicationPathAndArgs[1] = _R("--bad");
  lpApplicationPathAndArgs[2] = RT_NULL;

#ifdef RT_DEFINE_WINDOWS
  RtIoDevice_CreateFromHandle(&zzFileIoDevice, zzFile.hFile);
#else
  RtIoDevice_CreateFromFileDescriptor(&zzFileIoDevice, zzFile.nFile);
#endif
  if (!RtCreateProcessWithRedirections(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, RT_NULL, RT_NULL, &zzFileIoDevice, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (!unExitCode) goto handle_error;

  if (!RtFileSystem_GetFileSize(lpTempFile, &unFileSize)) goto handle_error;
  if (unFileSize < 10) goto handle_error;

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
    if (!RtFileSystem_DeleteFile(lpTempFile) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCreateProcessEnv()
{
  RT_CHAR lpExecutablePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_PIPE zzPipe;
  RT_IO_DEVICE* lpInput;
  RT_IO_DEVICE* lpOutput;
  RT_B bInputCreated;
  RT_B bOutputCreated;
  RT_ENV_VARS zzEnvVars;
  RT_B bEnvVarsCreated;
  RT_UN unWritten;
  RT_CHAR* lpApplicationPathAndArgs[4];
  RT_PROCESS zzProcess;
  RT_B bProcessCreated;
  RT_UN32 unExitCode;
  RT_CHAR8 lpPipeContent[256];
  RT_UN unBytesRead;
  RT_B bResult;

  bInputCreated = RT_FALSE;
  bOutputCreated = RT_FALSE;
  bEnvVarsCreated = RT_FALSE;
  bProcessCreated = RT_FALSE;

  unWritten = 0;
  if (!RtFileSystem_GetExecutableFilePath(lpExecutablePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  if (!RtPipe_Create(&zzPipe)) goto handle_error;
  lpInput = RtPipe_GetInput(&zzPipe);
  lpOutput = RtPipe_GetOutput(&zzPipe);
  bInputCreated = RT_TRUE;
  bOutputCreated = RT_TRUE;

  if (!RtCreateEnvVars(&zzEnvVars)) goto handle_error;
  bEnvVarsCreated = RT_TRUE;

  if (!RtMergeEnvVarIntoEnvVars(&zzEnvVars, _R("RT_PROCESS_VAR"), _R("VAR_VALUE"))) goto handle_error;

  lpApplicationPathAndArgs[0] = lpExecutablePath;
  lpApplicationPathAndArgs[1] = _R("--display-env-var");
  lpApplicationPathAndArgs[2] = _R("RT_PROCESS_VAR");
  lpApplicationPathAndArgs[3] = RT_NULL;

  if (!RtCreateProcessWithRedirections(&zzProcess, RT_TRUE, RT_NULL, &zzEnvVars, RT_NULL, lpOutput, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtJoinProcess(&zzProcess)) goto handle_error;
  if (!RtGetProcessExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (unExitCode) goto handle_error;

  /* Close writing pipe on parent side. */
  bOutputCreated = RT_FALSE;
  if (!RtIoDevice_Free(lpOutput)) goto handle_error;

  if (!RtIoDevice_Read(RtIoDevice_GetInputStream(lpInput), lpPipeContent, 256 - 1, &unBytesRead)) goto handle_error;
  lpPipeContent[unBytesRead] = 0;
  if (RtChar8_CompareStrings(lpPipeContent, "RT_PROCESS_VAR=VAR_VALUE\n")) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtFreeProcess(&zzProcess) && bResult) goto handle_error;
  }
  if (bEnvVarsCreated)
  {
    bEnvVarsCreated = RT_FALSE;
    if (!RtFreeEnvVars(&zzEnvVars) && bResult) goto handle_error;
  }
  if (bOutputCreated)
  {
    bOutputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpOutput) && bResult) goto handle_error;
  }
  if (bInputCreated)
  {
    bInputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpInput) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestFailingProcess()
{
  RT_CHAR* lpApplicationPathAndArgs[3];
  RT_PROCESS zzProcess;
  RT_B bProcessCreated;
  RT_UN32 unExitCode;
  RT_B bResult;

  bProcessCreated = RT_FALSE;

  /* Test wrong process name. */
  lpApplicationPathAndArgs[0] = _R("pong");
  lpApplicationPathAndArgs[1] = _R("localhost");
  lpApplicationPathAndArgs[2] = RT_NULL;

  if (RtCreateProcess(&zzProcess, RT_TRUE,   RT_NULL, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  if (RtCreateProcess(&zzProcess, RT_FALSE,  RT_NULL, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;

  /* Test wrong folder name. */
  lpApplicationPathAndArgs[0] = _R("ping");
  if (RtCreateProcess(&zzProcess, RT_TRUE,   _R("Wrong, wrong directory"), RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  if (RtCreateProcess(&zzProcess, RT_FALSE,  _R("Wrong, wrong directory"), RT_NULL, lpApplicationPathAndArgs)) goto handle_error;

  /* Test bad argument. */
  if (!RtWriteStringToConsole(_R("====================================================\n"))) goto handle_error;

  lpApplicationPathAndArgs[1] = _R("-pong");
  if (!RtCreateProcess(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
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
  if (!ZzTestCreateProcessEnv()) goto handle_error;
  if (!ZzTestFailingProcess()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
