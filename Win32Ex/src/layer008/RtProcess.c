#include "layer008/RtProcess.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer003/RtMemory.h"
#include "layer004/RtChar.h"

RT_B RT_API RtCreateProcess(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory, RT_CHAR* lpApplicationName, ...)
{
  va_list lpVaList;
  RT_CHAR* lpArg;
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpCommandLine[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  STARTUPINFO rtStartupInfo;
#else
  pid_t nPid;
  RT_CHAR* lpPArgs[20];
  RT_UN unI;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  RT_MEMORY_ZERO(&rtStartupInfo, sizeof(rtStartupInfo));
  rtStartupInfo.cb = sizeof(rtStartupInfo);

  unWritten = 0;
  if (!RtCopyString(lpApplicationName, &lpCommandLine[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;

  va_start(lpVaList, lpApplicationName);
  while (RT_TRUE)
  {
    lpArg = va_arg(lpVaList, RT_CHAR*);
    if (lpArg)
    {
      if (!RtCopyStringWithSize(_R(" "), 1, &lpCommandLine[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten))
      {
        va_end(lpVaList);
        goto handle_error;
      }
      if (!RtCopyString(lpArg, &lpCommandLine[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten))
      {
        va_end(lpVaList);
        goto handle_error;
      }
    }
    else
    {
      break;
    }
  }
  va_end(lpVaList);

  if (!CreateProcess(RT_NULL,                        /* lpApplicationName.                                                         */
                     lpCommandLine,
                     RT_NULL,                        /* LPSECURITY_ATTRIBUTES lpProcessAttributes.                                 */
                     RT_NULL,                        /* LPSECURITY_ATTRIBUTES lpThreadAttributes.                                  */
                     RT_FALSE,                       /* bInheritHandles.                                                           */
                     0,                              /* dwCreationFlags.                                                           */
                     RT_NULL,                        /* lpEnvironment.                                                             */
                     lpCurrentDirectory,             /* If NULL, the new process will have the same current directory as this one. */
                     &rtStartupInfo,
                     (PROCESS_INFORMATION*)lpProcess
                     )) goto handle_error;

#else /* NOT RT_DEFINE_WINDOWS */

  lpPArgs[0] = lpApplicationName;

  va_start(lpVaList, lpApplicationName);
  unI = 1;
  while (RT_TRUE)
  {
    lpArg = va_arg(lpVaList, RT_CHAR*);
    if (lpArg)
    {
      lpPArgs[unI] = lpArg;
      unI++;
    }
    else
    {
      break;
    }
  }
  lpPArgs[unI] = RT_NULL;

  va_end(lpVaList);

  nPid = fork();
  if (nPid == 0)
  {
    /* We are in the child process. */

    /* Change current directory if provided. */
    if (lpCurrentDirectory)
    {
      /* chdir Returns zero in case of success, set errno. As we are in the child process, we simply print an error.*/
       if (chdir(lpCurrentDirectory))
       {
         /* TODO: Print error message. */
         fprintf(stderr, "Failed chdir to \"%s\".\n", lpCurrentDirectory);
       }
    }

    /* Returns only if an error has occurred. The return value is -1, and errno is set to indicate the error.  */
    execvp(lpApplicationName, lpPArgs);
    /* TODO: Print error message. */
    fprintf(stderr, "Failed to execvp.\n");
    /* Kill child process. */
    exit(1);
  }
  else if (nPid > 0)
  {
    /* We are still in the same parent process and the fork is ok. */
    lpProcess->nPid = nPid;
  }
  else
  {
    /* On failure, fork returns -1 and set errno. */
    goto handle_error;
  }
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtJoinProcess(RT_PROCESS* lpProcess)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unReturnedValue;
#else
  int nStatus;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  unReturnedValue = WaitForSingleObject(lpProcess->hProcess, INFINITE);
  if (unReturnedValue != WAIT_OBJECT_0)
  {
    /* If returned value is WAIT_FAILED, then last error is set. */
    if (unReturnedValue != WAIT_FAILED)
    {
      /* Set an arbitrary last error for WAIT_TIMEOUT/WAIT_ABANDONED cases. */
      RtSetLastError(RT_ERROR_FUNCTION_FAILED);
    }
    goto handle_error;
  }

#else /* NOT RT_DEFINE_WINDOWS */

  if (waitpid((pid_t)lpProcess->nPid, &nStatus, 0) == -1) goto handle_error;

  /* Exit code can be read only if WIFEXITED is true. */
  if (WIFEXITED(nStatus))
  {
    /* Extract status from exit code. */
    lpProcess->unExitCode = WEXITSTATUS(nStatus);
  }
  else
  {
    /* Process might have been killed, use a generic exit code. */
    lpProcess->unExitCode = 1;
  }
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtGetProcessExitCode(RT_PROCESS* lpProcess, RT_UN32* lpExitCode)
{
#ifdef RT_DEFINE_WINDOWS
  RT_B bResult;

  if (!GetExitCodeProcess(lpProcess->hProcess, lpExitCode)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

#else /* NOT RT_DEFINE_WINDOWS */

  *lpExitCode = lpProcess->unExitCode;
  return RT_SUCCESS;
#endif
}

RT_B RT_API RtFreeProcess(RT_PROCESS* lpProcess)
{
  RT_B bResult;

  bResult = RT_SUCCESS;

#ifdef RT_DEFINE_WINDOWS
  if (!CloseHandle(lpProcess->hThread))  bResult = RT_FAILURE;
  if (!CloseHandle(lpProcess->hProcess)) bResult = RT_FAILURE;
#endif

  return bResult;
}
