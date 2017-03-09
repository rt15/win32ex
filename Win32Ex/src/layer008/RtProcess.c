#include "layer008/RtProcess.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer003/RtMemory.h"
#include "layer004/RtChar.h"
#include "layer005/RtStaticHeap.h"
#include "layer007/RtErrorMessage.h"

RT_B RT_CDECL_API RtCreateProcess(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory, RT_CHAR* lpApplicationName, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpApplicationName);
  bResult = RtVCreateProcess(lpProcess, lpVaList, lpCurrentDirectory, RT_NULL, RT_NULL, RT_NULL, lpApplicationName);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_CDECL_API RtCreateProcessWithRedirections(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory,
                                                  RT_FILE* lpStdInput, RT_FILE* lpStdOutput, RT_FILE* lpStdError,
                                                  RT_CHAR* lpApplicationName, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpApplicationName);
  bResult = RtVCreateProcess(lpProcess, lpVaList, lpCurrentDirectory, lpStdInput, lpStdOutput, lpStdError, lpApplicationName);
  va_end(lpVaList);

  return bResult;
}

#ifdef RT_DEFINE_WINDOWS


/**
 * Make an handle inheritable if it is not already.
 */
RT_B RT_CALL RtInheritHandle(HANDLE hHandle)
{
  DWORD unFlags;
  RT_B bResult;

  if (!GetHandleInformation(hHandle, &unFlags)) goto handle_error;
  if (!(unFlags & HANDLE_FLAG_INHERIT))
  {
    if (!SetHandleInformation(hHandle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

#endif


/**
 * Convert given argv so it can be used in command line.<br>
 * See <tt>RtArgVToCommandLine</tt>.
 */
RT_B RT_CALL RtConvertArgToCommandLine(RT_CHAR* lpArg, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
  RT_B bNeedDoubleQuotes;
  RT_UN unWritten;
  RT_UN unAntiSlashes;
  RT_UN unI;
  RT_UN unJ;
  RT_B bResult;

  /* Need double-quotes? */
  bNeedDoubleQuotes = RT_FALSE;
  unI = 0;
  while (lpArg[unI])
  {
    if ((lpArg[unI] == _R(' '))  ||
        (lpArg[unI] == _R('\t')) ||
        (lpArg[unI] == _R('"')))
    {
      bNeedDoubleQuotes = RT_TRUE;
      break;
    }
    unI++;
  }

  unWritten = 0;
  if (bNeedDoubleQuotes)
  {
    /* Open double-quotes. */
    if (!RtCopyStringWithSize(_R("\""), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;

    unI = 0;
    unAntiSlashes = 0;
    while (lpArg[unI])
    {
      if (lpArg[unI] == _R('\\'))
      {
        unAntiSlashes++;
      }
      else
      {
        if (lpArg[unI] == _R('"'))
        {
          for (unJ = 0; unJ < unAntiSlashes; unJ++)
          {
            /* Escape slashes just before the double quote. */
            if (!RtCopyStringWithSize(_R("\\"), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
          }

          /* Escape double-quote. */
          if (!RtCopyStringWithSize(_R("\\"), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
        }
        unAntiSlashes = 0;
      }

      /* Copy character. */
      if (!RtCopyStringWithSize(&lpArg[unI], 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;

      unI++;
    }

    /* Protect possible slashes before added closing double-quote. */
    for (unJ = 0; unJ < unAntiSlashes; unJ++)
    {
      if (!RtCopyStringWithSize(_R("\\"), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
    }

    /* Close double-quotes. */
    if (!RtCopyStringWithSize(_R("\""), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
  }
  else
  {
    if (!RtCopyString(lpArg, lpBuffer, unBufferSize, &unWritten)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  *lpWritten += unWritten;
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

/**
 * Compute a command line size big enough for the given argV.
 *
 * <p>
 * If there are ' ', tab or '"', we need 2 more characters for double-quotes.<br>
 * If double-quotes are needed, add 1 character per anti-slash (Not always required) and '"' (Always required).
 * </p>
 */
RT_UN RT_CALL RtComputeArgSizeInCommandLine(RT_CHAR* lpArg)
{
  RT_UN unAntiSlashes;
  RT_UN unDoubleQuotes;
  RT_UN unSpaces;
  RT_UN unResult;

  unAntiSlashes = 0;
  unDoubleQuotes = 0;
  unSpaces = 0;

  unResult = 0;
  while (lpArg[unResult])
  {
    if (lpArg[unResult] == _R(' ') || lpArg[unResult] == _R('\t'))
    {
      unSpaces++;
    }
    else if (lpArg[unResult] == _R('"'))
    {
      unDoubleQuotes++;
    }
    else if (lpArg[unResult] == _R('\\'))
    {
      unAntiSlashes++;
    }
    unResult++;
  }

  if (unSpaces || unDoubleQuotes)
  {
    /* We need to surround with double-quotes. */
    unResult += 2;
    unResult += unDoubleQuotes;
    unResult += unAntiSlashes;
  }

  return unResult;
}

/**
 * Opposite of CommandLineToArgvW.
 *
 * <p>
 * If an argument contains ' ', tab or '"' then it must be surrounded with double-quotes (No need to surround arguments with anti-slashes only).<br>
 * If an argument needs double-quotes then:<br>
 * Prepend anti-slash before any double-quote.<br>
 * Prepend anti-slash for every anti-slashes just before a double-quote.<br>
 * </p>
 *
 * <ul>
 *   <li>Use "      to start/end a double quoted part</li>
 *   <li>Use \"     to insert a literal "</li>
 *   <li>Use \\"    to insert a \ then start or end a double quoted part</li>
 *   <li>Use \\\"   to insert a literal \"</li>
 *   <li>Use \      to insert a literal \</li>
 * </ul>
 *
 */
RT_B RT_CALL RtArgVToCommandLine(va_list lpVaList, RT_CHAR* lpApplicationName, RT_CHAR* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, RT_CHAR** lpCommandLine)
{
  va_list lpVaList2;
  RT_UN unCommandLineSize;
  RT_CHAR* lpArg;
  RT_UN unCommandLineBufferSize;
  RT_CHAR* lpLocalCommandLine;
  RT_UN unWritten;
  RT_B bResult;

  RT_VA_COPY(lpVaList, lpVaList2);

  /* First, compute the required command line size. */
  unCommandLineSize = RtComputeArgSizeInCommandLine(lpApplicationName);

  while (RT_TRUE)
  {
    lpArg = va_arg(lpVaList, RT_CHAR*);
    if (lpArg)
    {
      unCommandLineSize += RtComputeArgSizeInCommandLine(lpArg);
      unCommandLineSize++; /* Space separator between arguments. */
    }
    else
    {
      break;
    }
  }

  /* Add one for trailing zero. */
  unCommandLineBufferSize = unCommandLineSize + 1;

  if (!RtAllocIfNeeded(lpBuffer, unBufferSize * sizeof(RT_CHAR), lpHeapBuffer, lpHeapBufferSize, (void**)lpCommandLine, unCommandLineBufferSize)) goto handle_error;

  lpLocalCommandLine = *lpCommandLine;

  unWritten = 0;
  if (!RtConvertArgToCommandLine(lpApplicationName, &lpLocalCommandLine[unWritten], unCommandLineBufferSize - unWritten, &unWritten)) goto handle_error;

  while (RT_TRUE)
  {
    lpArg = va_arg(lpVaList2, RT_CHAR*);
    if (lpArg)
    {
      /* Arguments separatory. */
      if (!RtCopyStringWithSize(_R(" "), 1, &lpLocalCommandLine[unWritten], unCommandLineBufferSize - unWritten, &unWritten)) goto handle_error;

      /* Convert argument. */
      if (!RtConvertArgToCommandLine(lpArg, &lpLocalCommandLine[unWritten], unCommandLineBufferSize - unWritten, &unWritten)) goto handle_error;
    }
    else
    {
      break;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  va_end(lpVaList2);
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

#ifdef RT_DEFINE_LINUX

/**
 * Same as RtVCreateProcess but <tt>RT_CHAR** lpPArgs</tt>.has been computed from the va_list.
 */
RT_B RT_CALL RtCreateLinuxProcess(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory,
                                  RT_FILE* lpStdInput, RT_FILE* lpStdOutput, RT_FILE* lpStdError,
                                  RT_CHAR* lpApplicationName, RT_CHAR** lpPArgs)
{
  RT_UN unErrno;
  RT_FILE rtReadPipe;
  RT_FILE rtWritePipe;
  RT_B bReadPipeCreated;
  RT_B bWritePipeCreated;
  pid_t nPid;
  RT_CHAR lpMessage[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unBytesRead;
  RT_B bResult;

  bReadPipeCreated = RT_FALSE;
  bWritePipeCreated = RT_FALSE;

  if (!RtCreatePipe(&rtReadPipe, &rtWritePipe)) goto handle_error;
  bReadPipeCreated = RT_TRUE;
  bWritePipeCreated = RT_TRUE;

  nPid = fork();
  if (nPid == 0)
  {
    /* We are in the child process. */

    /* Close the read pipe, used by the parent. */
    if (!RtFreeFile(&rtReadPipe)) goto handle_child_error;

    /* Attempt to perform redirections. */
    /* dup2 returns -1 in case of error and set errno. */
    if (lpStdInput)
    {
      if (dup2(lpStdInput->nFile, 0) == -1) goto handle_child_error;
    }
    if (lpStdOutput)
    {
      if (dup2(lpStdOutput->nFile, 1) == -1) goto handle_child_error;
    }
    if (lpStdError)
    {
      if (dup2(lpStdError->nFile, 2) == -1) goto handle_child_error;
    }

    /* Change current directory if provided. */
    if (lpCurrentDirectory)
    {
      /* chdir Returns zero in case of success, set errno. */
       if (chdir(lpCurrentDirectory)) goto handle_child_error;
    }

    /* Returns only if an error has occurred. The return value is -1, and errno is set to indicate the error.  */
    execvp(lpApplicationName, lpPArgs);

handle_child_error:

    /* We will write errno in the pipe to be read by the parent. */
    unErrno = errno;
    if (!RtWriteToFile(&rtWritePipe, (RT_CHAR8*)&unErrno, sizeof(unErrno)))
    {
      unWritten = 0;
      if (RtCopyString(_R("Failed to write to parent pipe: "),  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten))
      {
        RtWriteLastErrorMessage(lpMessage);
      }
    }

    /* We do not need the writing pipe anymore so we close it. */
    if (!RtFreeFile(&rtWritePipe))
    {
      unWritten = 0;
      if (RtCopyString(_R("Failed to close writing pipe: "),  &lpMessage[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unWritten))
      {
        RtWriteLastErrorMessage(lpMessage);
      }
    }

    /* Kill forked child process. */
    exit(1);
  }
  else if (nPid > 0)
  {
    /* We are still in the same parent process and the fork is ok. */
    lpProcess->nPid = nPid;

    /* We close writing pipe to avoid a dead lock. It will be used only by the child. */
    bWritePipeCreated = RT_FALSE;
    if (!RtFreeFile(&rtWritePipe)) goto handle_error;

    /* Synchronously read errno or zero bytes from created child. */
    if (!RtReadFromFile(&rtReadPipe, (RT_CHAR8*)&unErrno, sizeof(unErrno), &unBytesRead)) goto handle_error;
    if (unBytesRead)
    {
      /* Copy errno read from child pipe into parent errno. */
      errno = unErrno;
      goto handle_error;
    }
  }
  else
  {
    /* On failure, fork returns -1 and set errno. */
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  if (bWritePipeCreated)
  {
    bWritePipeCreated = RT_FALSE;
    if (!RtFreeFile(&rtWritePipe) && bResult) goto handle_error;
  }
  if (bReadPipeCreated)
  {
    bReadPipeCreated = RT_FALSE;
    if (!RtFreeFile(&rtReadPipe) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

#endif

RT_B RT_API RtVCreateProcess(RT_PROCESS* lpProcess, va_list lpVaList, RT_CHAR* lpCurrentDirectory,
                             RT_FILE* lpStdInput, RT_FILE* lpStdOutput, RT_FILE* lpStdError,
                             RT_CHAR* lpApplicationName)
{
#ifdef RT_DEFINE_WINDOWS
  RT_FILE rtStdInput;
  RT_FILE rtStdOutput;
  RT_FILE rtStdError;
  RT_FILE* lpActualStdInput;
  RT_FILE* lpActualStdOutput;
  RT_FILE* lpActualStdError;
  RT_CHAR lpCommandLineBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_CHAR* lpCommandLine;
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;
  STARTUPINFO rtStartupInfo;
  BOOL bInheritHandles;
#else
  va_list lpVaList2;
  RT_CHAR* lpArg;
  RT_UN unArgsCount;
  void* lpBuffer[240];                             /* 240 * 8 = 1920, to consume half of 4000 under 64 bits. */
  void* lpHeapBuffer;
  RT_CHAR** lpPArgs;
  RT_UN unHeapBufferSize;
  RT_UN unI;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  lpHeapBuffer = RT_NULL;
  unHeapBufferSize = 0;

  RT_MEMORY_ZERO(&rtStartupInfo, sizeof(rtStartupInfo));
  rtStartupInfo.cb = sizeof(rtStartupInfo);
  bInheritHandles = FALSE;

  /* Manage redirection if needed. */
  if (lpStdInput || lpStdOutput || lpStdError)
  {
    rtStartupInfo.dwFlags |= STARTF_USESTDHANDLES;
    /* Required to use redirections. */
    bInheritHandles = TRUE;

    /* Std input. */
    if (lpStdInput)
    {
      lpActualStdInput = lpStdInput;
    }
    else
    {
       if (!RtCreateStdInput(&rtStdInput)) goto handle_error;
       lpActualStdInput = &rtStdInput;
    }
    if (!RtInheritHandle(lpActualStdInput->hFile)) goto handle_error;
    rtStartupInfo.hStdInput = lpActualStdInput->hFile;

    /* Std output. */
    if (lpStdOutput)
    {
      lpActualStdOutput = lpStdOutput;
    }
    else
    {
      if (!RtCreateStdOutput(&rtStdOutput)) goto handle_error;
      lpActualStdOutput = &rtStdOutput;
    }
    if (!RtInheritHandle(lpActualStdOutput->hFile)) goto handle_error;
    rtStartupInfo.hStdOutput = lpActualStdOutput->hFile;

    /* Std error. */
    if (lpStdError)
    {
      lpActualStdError = lpStdError;
    }
    else
    {
      if (!RtCreateStdError(&rtStdError)) goto handle_error;
      lpActualStdError = &rtStdError;
    }
    if (!RtInheritHandle(lpActualStdError->hFile)) goto handle_error;
    rtStartupInfo.hStdError = lpActualStdError->hFile;
  }

  if (!RtArgVToCommandLine(lpVaList, lpApplicationName, lpCommandLineBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &lpHeapBuffer, &unHeapBufferSize, &lpCommandLine)) goto handle_error;

  if (!CreateProcess(RT_NULL,                        /* lpApplicationName.                                                         */
                     lpCommandLine,
                     RT_NULL,                        /* LPSECURITY_ATTRIBUTES lpProcessAttributes.                                 */
                     RT_NULL,                        /* LPSECURITY_ATTRIBUTES lpThreadAttributes.                                  */
                     bInheritHandles,
                     0,                              /* dwCreationFlags.                                                           */
                     RT_NULL,                        /* lpEnvironment.                                                             */
                     lpCurrentDirectory,             /* If NULL, the new process will have the same current directory as this one. */
                     &rtStartupInfo,
                     (PROCESS_INFORMATION*)lpProcess
                     )) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (lpHeapBuffer)
  {
    if (!RtFree(&lpHeapBuffer) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

#else /* NOT RT_DEFINE_WINDOWS */

  RT_VA_COPY(lpVaList, lpVaList2);
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

  if (!RtCreateLinuxProcess(lpProcess, lpCurrentDirectory, lpStdInput, lpStdOutput, lpStdError, lpApplicationName, lpPArgs)) goto handle_error;

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
