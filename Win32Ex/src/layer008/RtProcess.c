#include "layer008/RtProcess.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer003/RtMemory.h"
#include "layer004/RtChar.h"
#include "layer005/RtStaticHeap.h"
#include "layer007/RtErrorMessage.h"

RT_B RT_API RtCreateProcess(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory, RT_CHAR* lpApplicationName, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpApplicationName);
  bResult = RtVCreateProcess(lpProcess, lpVaList, lpCurrentDirectory, lpApplicationName);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_API RtVCreateProcess(RT_PROCESS* lpProcess, va_list lpVaList, RT_CHAR* lpCurrentDirectory, RT_CHAR* lpApplicationName)
{
  RT_CHAR* lpArg;
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpCommandLine[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  STARTUPINFO rtStartupInfo;
#else
  va_list lpVaList2;
  pid_t nPid;
  RT_UN unArgsCount;
  void* lpBuffer[240];                             /* 240 * 8 = 1920, to consume half of 4000 under 64 bits. */
  void* lpHeapBuffer;
  RT_CHAR** lpPArgs;
  RT_UN unHeapBufferSize;
  RT_CHAR lpMessage[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unI;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  RT_MEMORY_ZERO(&rtStartupInfo, sizeof(rtStartupInfo));
  rtStartupInfo.cb = sizeof(rtStartupInfo);

  unWritten = 0;
  if (!RtCopyString(lpApplicationName, &lpCommandLine[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;

  while (RT_TRUE)
  {
    lpArg = va_arg(lpVaList, RT_CHAR*);
    if (lpArg)
    {
      if (!RtCopyStringWithSize(_R(" "), 1,  &lpCommandLine[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
      if (!RtCopyString(lpArg,               &lpCommandLine[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
    }
    else
    {
      break;
    }
  }

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

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

#else /* NOT RT_DEFINE_WINDOWS */

  RT_VA_COPY(lpVaList2, lpVaList);
  lpHeapBuffer = RT_NULL;
  unHeapBufferSize = 0;

  /* Count the size of the array of pointers to arguments. First is application name, last is null. */

  /* lpApplicationName and trailing RT_NULL. */
  unArgsCount = 2;

  while (RT_TRUE)
  {
    lpArg = va_arg(lpVaList, RT_CHAR*);
    if (lpArg)
    {
      unArgsCount++;
    }
    else
    {
      break;
    }
  }

  /* Ensure to have an array of pointers of the right size. */
  if (!RtAllocIfNeeded(lpBuffer, 240 * sizeof(RT_CHAR*), &lpHeapBuffer, &unHeapBufferSize, (void**)&lpPArgs, unArgsCount * sizeof(RT_CHAR*))) goto handle_error;

  /* First "arg" is application name. */
  lpPArgs[0] = lpApplicationName;

  unI = 1;
  while (RT_TRUE)
  {
    lpArg = va_arg(lpVaList2, RT_CHAR*);
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
  /* We must have RT_NULL as last "arg". */
  lpPArgs[unI] = RT_NULL;

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
         unWritten = 0;
         if (!RtCopyString(_R("Failed to chdir to \""),  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto write_message_failed;
         if (!RtCopyString(lpCurrentDirectory,           &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto write_message_failed;
         if (!RtCopyString(_R("\": "),                   &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto write_message_failed;
         RtWriteLastErrorMessage(lpMessage);
       }
    }

write_message_failed:

    /* Returns only if an error has occurred. The return value is -1, and errno is set to indicate the error.  */
    execvp(lpApplicationName, lpPArgs);

    /* We execute following code only if execvp failed. */
    unWritten = 0;
    if (!RtCopyString(_R("Failed to execute \""),  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto write_message_failed;
    if (!RtCopyString(lpApplicationName,           &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto write_message_failed;
    if (!RtCopyString(_R("\": "),                  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten)) goto write_message_failed;
    RtWriteLastErrorMessage(lpMessage);

    /* Kill forked child process. */
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

  bResult = RT_SUCCESS;
free_resources:
  if (lpHeapBuffer)
  {
    if (!RtFree(&lpHeapBuffer)) goto handle_error;
  }
  /* va_end must be the last resource. */
  va_end(lpVaList2);
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

#endif
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
