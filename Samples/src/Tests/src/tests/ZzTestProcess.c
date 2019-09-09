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
  RT_PIPE zzPipe;
  RT_IO_DEVICE* lpInputIoDevice;
  RT_IO_DEVICE* lpOutputIoDevice;
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
  if (!RtTempFile_Create(&zzFile, _R("Zz"), lpTempFile, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  bFileCreated = RT_TRUE;
  bDeleteTempFile = RT_TRUE;

  if (!RtPipe_Create(&zzPipe)) goto handle_error;
  lpInputIoDevice = RtPipe_GetInput(&zzPipe);
  lpOutputIoDevice = RtPipe_GetOutput(&zzPipe);
  bInputCreated = RT_TRUE;
  bOutputCreated = RT_TRUE;

  lpApplicationPathAndArgs[0] = lpExecutablePath;
  lpApplicationPathAndArgs[1] = _R("--read-line");
  lpApplicationPathAndArgs[2] = RT_NULL;

  if (!RtProcess_CreateWithRedirections(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, lpInputIoDevice, &zzFile.rtIoDevice, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!ZzWriteLinesToDevice(lpOutputIoDevice, lpHeap, _R("123"), (RT_CHAR*)RT_NULL)) goto handle_error;

  if (!RtProcess_Join(&zzProcess)) goto handle_error;
  if (!RtProcess_GetExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (unExitCode) goto handle_error;

  if (!ZzCheckTextFile(lpTempFile, lpHeap, _R("\"123\""), (RT_CHAR*)RT_NULL)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtProcess_Free(&zzProcess) && bResult) goto handle_error;
  }
  if (bOutputCreated)
  {
    bOutputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpOutputIoDevice) && bResult) goto handle_error;
  }
  if (bInputCreated)
  {
    bInputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpInputIoDevice) && bResult) goto handle_error;
  }
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtIoDevice_Free(&zzFile.rtIoDevice) && bResult) goto handle_error;
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
  if (!RtTempFile_Create(&zzFile, _R("Zz"), lpTempFile, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
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

  if (!RtProcess_CreateWithRedirections(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, RT_NULL, &zzFile.rtIoDevice, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtProcess_Join(&zzProcess)) goto handle_error;
  if (!RtProcess_GetExitCode(&zzProcess, &unExitCode)) goto handle_error;
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
    if (!RtProcess_Free(&zzProcess) && bResult) goto handle_error;
  }
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtIoDevice_Free(&zzFile.rtIoDevice) && bResult) goto handle_error;
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
  if (!RtTempFile_Create(&zzFile, _R("Zz"), lpTempFile, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  bFileCreated = RT_TRUE;
  bDeleteTempFile = RT_TRUE;

  lpApplicationPathAndArgs[0] = lpExecutablePath;
  lpApplicationPathAndArgs[1] = _R("--bad");
  lpApplicationPathAndArgs[2] = RT_NULL;

  if (!RtProcess_CreateWithRedirections(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, RT_NULL, RT_NULL, &zzFile.rtIoDevice, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtProcess_Join(&zzProcess)) goto handle_error;
  if (!RtProcess_GetExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (!unExitCode) goto handle_error;

  if (!RtFileSystem_GetFileSize(lpTempFile, &unFileSize)) goto handle_error;
  if (unFileSize < 10) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtProcess_Free(&zzProcess) && bResult) goto handle_error;
  }
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtIoDevice_Free(&zzFile.rtIoDevice) && bResult) goto handle_error;
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
  RT_IO_DEVICE* lpInputIoDevice;
  RT_IO_DEVICE* lpOutputIoDevice;
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
  lpInputIoDevice = RtPipe_GetInput(&zzPipe);
  lpOutputIoDevice = RtPipe_GetOutput(&zzPipe);
  bInputCreated = RT_TRUE;
  bOutputCreated = RT_TRUE;

  if (!RtEnvVars_Create(&zzEnvVars)) goto handle_error;
  bEnvVarsCreated = RT_TRUE;

  if (!RtEnvVars_MergeEnvVar(&zzEnvVars, _R("RT_PROCESS_VAR"), _R("VAR_VALUE"))) goto handle_error;

  lpApplicationPathAndArgs[0] = lpExecutablePath;
  lpApplicationPathAndArgs[1] = _R("--display-env-var");
  lpApplicationPathAndArgs[2] = _R("RT_PROCESS_VAR");
  lpApplicationPathAndArgs[3] = RT_NULL;

  if (!RtProcess_CreateWithRedirections(&zzProcess, RT_TRUE, RT_NULL, &zzEnvVars, RT_NULL, lpOutputIoDevice, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtProcess_Join(&zzProcess)) goto handle_error;
  if (!RtProcess_GetExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (unExitCode) goto handle_error;

  /* Close writing pipe on parent side. */
  bOutputCreated = RT_FALSE;
  if (!RtIoDevice_Free(lpOutputIoDevice)) goto handle_error;

  if (!RtIoDevice_Read(RtIoDevice_GetInputStream(lpInputIoDevice), lpPipeContent, 256 - 1, &unBytesRead)) goto handle_error;
  lpPipeContent[unBytesRead] = 0;
  if (RtChar8_CompareStrings(lpPipeContent, "RT_PROCESS_VAR=VAR_VALUE\n")) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtProcess_Free(&zzProcess) && bResult) goto handle_error;
  }
  if (bEnvVarsCreated)
  {
    bEnvVarsCreated = RT_FALSE;
    if (!RtEnvVars_Free(&zzEnvVars) && bResult) goto handle_error;
  }
  if (bOutputCreated)
  {
    bOutputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpOutputIoDevice) && bResult) goto handle_error;
  }
  if (bInputCreated)
  {
    bInputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpInputIoDevice) && bResult) goto handle_error;
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

  if (RtProcess_Create(&zzProcess, RT_TRUE,   RT_NULL, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  if (RtProcess_Create(&zzProcess, RT_FALSE,  RT_NULL, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;

  /* Test wrong folder name. */
  lpApplicationPathAndArgs[0] = _R("ping");
  if (RtProcess_Create(&zzProcess, RT_TRUE,   _R("Wrong, wrong directory"), RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  if (RtProcess_Create(&zzProcess, RT_FALSE,  _R("Wrong, wrong directory"), RT_NULL, lpApplicationPathAndArgs)) goto handle_error;

  /* Test bad argument. */
  if (!RtConsole_WriteString(_R("====================================================\n"))) goto handle_error;

  lpApplicationPathAndArgs[1] = _R("-pong");
  if (!RtProcess_Create(&zzProcess, RT_TRUE, RT_NULL, RT_NULL, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtProcess_Join(&zzProcess)) goto handle_error;
  if (!RtConsole_WriteString(_R("====================================================\n"))) goto handle_error;
  if (!RtConsole_WriteString(_R("Joined!\n"))) goto handle_error;
  if (!RtProcess_GetExitCode(&zzProcess, &unExitCode)) goto handle_error;
  if (!unExitCode) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtProcess_Free(&zzProcess) && bResult) goto handle_error;
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
