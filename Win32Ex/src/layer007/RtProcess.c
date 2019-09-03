#include "layer007/RtProcess.h"

#include "layer001/RtMemory.h"
#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"
#include "layer002/RtIoDevice.h"
#include "layer003/RtChar.h"
#include "layer003/RtPipe.h"
#include "layer004/RtStaticHeap.h"
#include "layer006/RtErrorMessage.h"

RT_B RT_API RtProcess_Create(RT_PROCESS* lpProcess, RT_B bChild, RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars, RT_CHAR** lpApplicationPathAndArgs)
{
  return RtProcess_CreateWithRedirections(lpProcess, bChild, lpCurrentDirectory, lpEnvVars, RT_NULL, RT_NULL, RT_NULL, lpApplicationPathAndArgs);
}

RT_B RT_API RtProcess_SpawnSync(RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars, RT_CHAR** lpApplicationPathAndArgs, RT_UN32* lpExitCode)
{
  RT_PROCESS rtProcess;
  RT_B bProcessCreated;
  RT_B bResult;

  bProcessCreated = RT_FALSE;

  if (!RtProcess_Create(&rtProcess, RT_TRUE, lpCurrentDirectory, lpEnvVars, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  if (!RtProcess_Join(&rtProcess)) goto handle_error;
  if (!RtProcess_GetExitCode(&rtProcess, lpExitCode)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtProcess_Free(&rtProcess) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtProcess_SpawnASync(RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars, RT_CHAR** lpApplicationPathAndArgs)
{
  RT_PROCESS rtProcess;
  RT_B bProcessCreated;
  RT_B bResult;

  bProcessCreated = RT_FALSE;

  if (!RtProcess_Create(&rtProcess, RT_FALSE, lpCurrentDirectory, lpEnvVars, lpApplicationPathAndArgs)) goto handle_error;
  bProcessCreated = RT_TRUE;

  bResult = RT_SUCCESS;
free_resources:
  if (bProcessCreated)
  {
    bProcessCreated = RT_FALSE;
    if (!RtProcess_Free(&rtProcess) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

/**
 * Convert given argv so it can be used in command line.<br>
 * See <tt>RtProcess_ArgVToCommandLine</tt>.
 */
RT_B RT_CALL RtProcess_ConvertArgToCommandLine(RT_CHAR* lpArg, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
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
    if (!RtChar_CopyStringWithSize(_R("\""), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;

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
            if (!RtChar_CopyStringWithSize(_R("\\"), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
          }

          /* Escape double-quote. */
          if (!RtChar_CopyStringWithSize(_R("\\"), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
        }
        unAntiSlashes = 0;
      }

      /* Copy character. */
      if (!RtChar_CopyStringWithSize(&lpArg[unI], 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;

      unI++;
    }

    /* Protect possible slashes before added closing double-quote. */
    for (unJ = 0; unJ < unAntiSlashes; unJ++)
    {
      if (!RtChar_CopyStringWithSize(_R("\\"), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
    }

    /* Close double-quotes. */
    if (!RtChar_CopyStringWithSize(_R("\""), 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;
  }
  else
  {
    if (!RtChar_CopyString(lpArg, lpBuffer, unBufferSize, &unWritten)) goto handle_error;
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
RT_UN RT_CALL RtProcess_GetArgSizeInCommandLine(RT_CHAR* lpArg)
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
 * If an argument contains ' ' (space), '	' (tab) or '"' (double quote) then it must be surrounded with double-quotes (No need to surround arguments with anti-slashes only).<br>
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
 * <p>
 * On the other hand, re-implementing CommandLineToArgvW would be pretty hard.<br>
 * See this <a href="https://blogs.msdn.microsoft.com/oldnewthing/20100917-00/?p=12833">blog post</a>.<br>
 * Here are some empirical strange rules:
 * </p>
 *
 * <ul>
 * <li>In a non-quoted argument, double-quotes are ignored (foo"bar -> foobar)</li>
 * <li>In a non-quoted argument, double-quotes can be escaped (foo\"bar -> foo"bar)</li>
 * <li>In a non-quoted argument, several double-quotes are replaced by a single one (foo""""bar -> foo"bar)</li>
 * </ul>
 *
 */
RT_B RT_CALL RtProcess_ArgVToCommandLine(RT_CHAR** lpApplicationPathAndArgs, RT_CHAR* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, RT_CHAR** lpCommandLine)
{
  RT_CHAR** lpInApplicationPathAndArgs;
  RT_UN unCommandLineBufferSize;
  RT_CHAR* lpLocalCommandLine;
  RT_UN unWritten;
  RT_B bFirst;
  RT_B bResult;

  /* First, compute the required command line size. */
  lpInApplicationPathAndArgs = lpApplicationPathAndArgs;
  unCommandLineBufferSize = 0;
  bFirst = RT_TRUE;
  while (*lpInApplicationPathAndArgs)
  {
    if (bFirst)
    {
      bFirst = RT_FALSE;
    }
    else
    {
      unCommandLineBufferSize++; /* Space separator between arguments. */
    }
    unCommandLineBufferSize += RtProcess_GetArgSizeInCommandLine(*lpInApplicationPathAndArgs);

    lpInApplicationPathAndArgs++;
  }

  /* Add one for trailing zero. */
  unCommandLineBufferSize++;

  if (!RtStaticHeap_AllocIfNeeded(lpBuffer, unBufferSize * sizeof(RT_CHAR), lpHeapBuffer, lpHeapBufferSize, (void**)lpCommandLine, unCommandLineBufferSize * sizeof(RT_CHAR))) goto handle_error;

  /* Simplify pointer access. */
  lpLocalCommandLine = *lpCommandLine;

  lpInApplicationPathAndArgs = lpApplicationPathAndArgs;
  unWritten = 0;
  bFirst = RT_TRUE;
  while (*lpInApplicationPathAndArgs)
  {
    if (bFirst)
    {
      bFirst = RT_FALSE;
    }
    else
    {
      /* Arguments separator. */
      if (!RtChar_CopyStringWithSize(_R(" "), 1, &lpLocalCommandLine[unWritten], unCommandLineBufferSize - unWritten, &unWritten)) goto handle_error;
    }
    /* Convert argument. */
    if (!RtProcess_ConvertArgToCommandLine(*lpInApplicationPathAndArgs, &lpLocalCommandLine[unWritten], unCommandLineBufferSize - unWritten, &unWritten)) goto handle_error;

    lpInApplicationPathAndArgs++;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

#ifdef RT_DEFINE_LINUX

/**
 * Called either by the main process or by a forked process if bChild is RT_TRUE.
 */
RT_B RT_CALL RtProcess_CreateActualLinux(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars,
                                        RT_IO_DEVICE* lpStdInput, RT_IO_DEVICE* lpStdOutput, RT_IO_DEVICE* lpStdError,
                                        RT_CHAR** lpApplicationPathAndArgs)
{
  RT_N nErrno;
  RT_PIPE rtPipe;
  RT_IO_DEVICE* lpInput;
  RT_IO_DEVICE* lpOutput;
  RT_B bInputCreated;
  RT_B bOutputCreated;
  pid_t nPid;
  RT_UN unBytesRead;
  RT_CHAR** lpEnvVarsArray;
  RT_B bResult;

  bInputCreated = RT_FALSE;
  bOutputCreated = RT_FALSE;

  /* Not inheritable, only used by the forked process before execvp/execvpe. */
  if (!RtPipe_Create(&rtPipe)) goto handle_error;
  lpInput = RtPipe_GetInput(&rtPipe);
  lpOutput = RtPipe_GetOutput(&rtPipe);
  bInputCreated = RT_TRUE;
  bOutputCreated = RT_TRUE;

  nPid = fork();
  if (nPid == 0)
  {
    /* We are in the child process. */

    /* Close the read pipe, used by the parent/intermediate. */
    if (!RtIoDevice_Free(lpInput))
    {
      RtErrorMessage_WriteLast(_R("Failed to close reading pipe: "));
      goto handle_child_error;
    }

    /* Attempt to perform redirections. */
    /* The copy of the file descriptor does not clone O_CLOEXEC. O_CLOEXEC is false in the copy. */
    /* dup2 returns -1 in case of error and set errno. */
    if (lpStdInput)
    {
      if (dup2(lpStdInput->nFileDescriptor, 0) == -1)
      {
        RtErrorMessage_WriteLast(_R("Failed to duplicate stdin redirection: "));
        goto handle_child_error;
      }
    }
    if (lpStdOutput)
    {
      if (dup2(lpStdOutput->nFileDescriptor, 1) == -1)
      {
        RtErrorMessage_WriteLast(_R("Failed to duplicate stdout redirection: "));
        goto handle_child_error;
      }
    }
    if (lpStdError)
    {
      if (dup2(lpStdError->nFileDescriptor, 2) == -1)
      {
        RtErrorMessage_WriteLast(_R("Failed to duplicate stderr redirection: "));
        goto handle_child_error;
      }
    }

    /* Change current directory if provided. */
    if (lpCurrentDirectory)
    {
      /* chdir Returns zero in case of success, set errno. */
       if (chdir(lpCurrentDirectory))
       {
         RtErrorMessage_WriteLastVariadic(RT_NULL, _R("Failed to change current directory to \""), lpCurrentDirectory, _R("\": "), (RT_CHAR*)RT_NULL);
         goto handle_child_error;
       }
    }

    /* Returns only if an error has occurred. The return value is -1, and errno is set to indicate the error.  */
    if (lpEnvVars)
    {
      if (!RtEnvVars_GetArray(lpEnvVars, &lpEnvVarsArray))
      {
        RtErrorMessage_WriteLast(_R("Failed to compute environment: "));
        goto handle_child_error;
      }
      execvpe(lpApplicationPathAndArgs[0], lpApplicationPathAndArgs, lpEnvVarsArray);
    }
    else
    {
      execvp(lpApplicationPathAndArgs[0], lpApplicationPathAndArgs);
    }
    RtErrorMessage_WriteLastVariadic(RT_NULL, _R("Failed to start \""), lpApplicationPathAndArgs[0], _R("\": "), (RT_CHAR*)RT_NULL);

handle_child_error:

    /* We will write errno in the pipe to be read by the parent. */
    nErrno = errno;
    if (!RtIoDevice_Write(&lpOutput->rtOutputStream, (RT_CHAR8*)&nErrno, sizeof(nErrno)))
    {
      RtErrorMessage_WriteLast(_R("Failed to write to parent pipe: "));
    }

    /* We do not need the writing pipe anymore so we close it. */
    if (!RtIoDevice_Free(lpOutput))
    {
      RtErrorMessage_WriteLast(_R("Failed to close writing pipe: "));
    }

    /* Kill forked child process. */
    exit(1);
  }
  else if (nPid > 0)
  {
    /* We are still in the same parent/intermediate process and the fork is ok. */
    lpProcess->nPid = nPid;

    /* We close writing pipe to avoid a dead lock. It will be used only by the child. */
    bOutputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpOutput)) goto handle_error;

    /* Synchronously read errno or zero bytes (success!) from created child. */
    if (!RtIoDevice_Read(&lpInput->rtInputStream, (RT_CHAR8*)&nErrno, sizeof(nErrno), &unBytesRead)) goto handle_error;
    if (unBytesRead)
    {
      /* Copy errno read from child pipe into parent/intermediate errno. */
      errno = nErrno;
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

/**
 * Fork a process that will fork again to avoid zombification.
 */
RT_B RT_CALL RtProcess_CreateLinuxUsingIntermediate(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars,
                                                   RT_IO_DEVICE* lpStdInput, RT_IO_DEVICE* lpStdOutput, RT_IO_DEVICE* lpStdError,
                                                   RT_CHAR** lpApplicationPathAndArgs)
{
  RT_N nChildPid;
  RT_N nErrno;
  RT_PIPE rtPipe;
  RT_IO_DEVICE* lpInput;
  RT_IO_DEVICE* lpOutput;
  RT_B bInputCreated;
  RT_B bOutputCreated;
  pid_t nPid;
  RT_UN unBytesRead;
  RT_PROCESS rtIntermediateProcess;
  RT_UN32 unExitCode;
  RT_B bResult;

  bInputCreated = RT_FALSE;
  bOutputCreated = RT_FALSE;

  /* Not inheritable, only used by the forked process. */
  if (!RtPipe_Create(&rtPipe)) goto handle_error;
  lpInput = RtPipe_GetInput(&rtPipe);
  lpOutput = RtPipe_GetOutput(&rtPipe);
  bInputCreated = RT_TRUE;
  bOutputCreated = RT_TRUE;

  nPid = fork();
  if (nPid == 0)
  {
    /* We are in the intermediate process. */

    /* Close the read pipe, used by the parent. */
    if (!RtIoDevice_Free(lpInput))
    {
      RtErrorMessage_WriteLast(_R("Failed to close intermediate reading pipe: "));
      goto handle_child_error;
    }

    /* Fork actual process from the current forked process. */
    if (!RtProcess_CreateActualLinux(lpProcess, lpCurrentDirectory, lpEnvVars, lpStdInput, lpStdOutput, lpStdError, lpApplicationPathAndArgs)) goto handle_child_error;

    /* Write child PID into the pipe. */
    nChildPid = lpProcess->nPid;
    if (!RtIoDevice_Write(&lpOutput->rtOutputStream, (RT_CHAR8*)&nChildPid, sizeof(nChildPid)))
    {
      RtErrorMessage_WriteLast(_R("Failed to write the child PID into the pipe: "));
      goto handle_child_error;
    }

    /* Kill forked intermediate process. */
    exit(0);

handle_child_error:

    /* We will write errno in the pipe to be read by the parent. */
    nErrno = errno;
    if (!RtIoDevice_Write(&lpOutput->rtOutputStream, (RT_CHAR8*)&nErrno, sizeof(nErrno)))
    {
      RtErrorMessage_WriteLast(_R("Failed to write to parent pipe: "));
    }

    /* We do not need the writing pipe anymore so we close it. */
    if (!RtIoDevice_Free(lpOutput))
    {
      RtErrorMessage_WriteLast(_R("Failed to close writing pipe: "));
    }

    /* Kill forked intermediate process. */
    exit(1);
  }
  else if (nPid > 0)
  {
    /* We are still in the same parent process and the fork is ok. */

    /* Retrieve intermediate process. */
    rtIntermediateProcess.nPid = nPid;

    /* We close writing pipe to avoid a dead lock. It will be used only by the intermediate. */
    bOutputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpOutput)) goto handle_error;

    /* Retrieve intermediate exit code. */
    if (!RtProcess_Join(&rtIntermediateProcess)) goto handle_error;
    if (!RtProcess_GetExitCode(&rtIntermediateProcess, &unExitCode)) goto handle_error;

    if (unExitCode)
    {
      /* Synchronously read errno or zero bytes (should not happen) from created intermediate. */
      if (!RtIoDevice_Read(&lpInput->rtInputStream, (RT_CHAR8*)&nErrno, sizeof(nErrno), &unBytesRead)) goto handle_error;
      if (unBytesRead)
      {
        /* Copy errno read from child pipe into parent errno. */
        errno = nErrno;
        goto handle_error;
      }
      else
      {
        /* We failed to retrieve errno from intermediate. Use generic error. */
        RtError_SetLast(RT_ERROR_FUNCTION_FAILED);
        goto handle_error;
      }
    }
    else
    {
      /* Synchronously read pid or zero bytes (should not happen) from created intermediate. */
      if (!RtIoDevice_Read(&lpInput->rtInputStream, (RT_CHAR8*)&nChildPid, sizeof(nChildPid), &unBytesRead)) goto handle_error;
      if (unBytesRead)
      {
        /* Great, we have read the child pid from the intermediate pipe. */
        lpProcess->nPid = nChildPid;
      }
      else
      {
        /* We failed to retrieve errno from intermediate. Use generic error. */
        RtError_SetLast(RT_ERROR_FUNCTION_FAILED);
        goto handle_error;
      }
    }
  }
  else
  {
    /* On failure, fork returns -1 and set errno. */
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
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

#endif

RT_B RT_API RtProcess_CreateWithRedirections(RT_PROCESS* lpProcess, RT_B bChild, RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars,
                                            RT_IO_DEVICE* lpStdInput, RT_IO_DEVICE* lpStdOutput, RT_IO_DEVICE* lpStdError,
                                            RT_CHAR** lpApplicationPathAndArgs)
{
#ifdef RT_DEFINE_WINDOWS
  RT_IO_DEVICE rtStdInput;
  RT_IO_DEVICE rtStdOutput;
  RT_IO_DEVICE rtStdError;

  RT_IO_DEVICE* lpActualStdInput;
  RT_IO_DEVICE* lpActualStdOutput;
  RT_IO_DEVICE* lpActualStdError;

  RT_B bInputInheritable;
  RT_B bOutputInheritable;
  RT_B bErrorInheritable;

  RT_B bRestoreInputNonInheritable;
  RT_B bRestoreOutputNonInheritable;
  RT_B bRestoreErrorNonInheritable;

  RT_CHAR lpCommandLineBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_CHAR* lpCommandLine;
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;
  STARTUPINFO rtStartupInfo;
  RT_CHAR* lpEnvVarsBlock;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  lpHeapBuffer = RT_NULL;
  unHeapBufferSize = 0;

  bRestoreInputNonInheritable = RT_FALSE;
  bRestoreOutputNonInheritable = RT_FALSE;
  bRestoreErrorNonInheritable = RT_FALSE;

  RT_MEMORY_ZERO(&rtStartupInfo, sizeof(rtStartupInfo));
  rtStartupInfo.cb = sizeof(rtStartupInfo);

  /* Manage redirections if needed. */
  if (lpStdInput || lpStdOutput || lpStdError)
  {
    rtStartupInfo.dwFlags |= STARTF_USESTDHANDLES;

    /* Std input. */
    if (lpStdInput)
    {
      lpActualStdInput = lpStdInput;
    }
    else
    {
       if (!RtIoDevice_CreateFromStdInput(&rtStdInput)) goto handle_error;
       lpActualStdInput = &rtStdInput;
    }
    rtStartupInfo.hStdInput = lpActualStdInput->hHandle;

    /* Std output. */
    if (lpStdOutput)
    {
      lpActualStdOutput = lpStdOutput;
    }
    else
    {
      if (!RtIoDevice_CreateFromStdOutput(&rtStdOutput)) goto handle_error;
      lpActualStdOutput = &rtStdOutput;
    }
    rtStartupInfo.hStdOutput = lpActualStdOutput->hHandle;

    /* Std error. */
    if (lpStdError)
    {
      lpActualStdError = lpStdError;
    }
    else
    {
      if (!RtIoDevice_CreateFromStdError(&rtStdError)) goto handle_error;
      lpActualStdError = &rtStdError;
    }
    rtStartupInfo.hStdError = lpActualStdError->hHandle;

    /* Temporarily update input inheritability if needed. */
    if (!RtIoDevice_IsInheritable(lpActualStdInput,  &bInputInheritable))  goto handle_error;
    if (!bInputInheritable)
    {
      if (!RtIoDevice_SetInheritable(lpActualStdInput, RT_TRUE)) goto handle_error;
      bRestoreInputNonInheritable = RT_TRUE;
    }

    /* Temporarily update output inheritability if needed. */
    if (!RtIoDevice_IsInheritable(lpActualStdOutput, &bOutputInheritable)) goto handle_error;
    if (!bOutputInheritable)
    {
      if (!RtIoDevice_SetInheritable(lpActualStdOutput, RT_TRUE)) goto handle_error;
      bRestoreOutputNonInheritable = RT_TRUE;
    }

    /* Temporarily update error inheritability if needed. */
    if (!RtIoDevice_IsInheritable(lpActualStdError,  &bErrorInheritable))  goto handle_error;
    if (!bErrorInheritable)
    {
      if (!RtIoDevice_SetInheritable(lpActualStdError, RT_TRUE)) goto handle_error;
      bRestoreErrorNonInheritable = RT_TRUE;
    }
  }

  if (!RtProcess_ArgVToCommandLine(lpApplicationPathAndArgs, lpCommandLineBuffer, RT_CHAR_HALF_BIG_STRING_SIZE, &lpHeapBuffer, &unHeapBufferSize, &lpCommandLine)) goto handle_error;

  if (lpEnvVars)
  {
    if (!RtEnvVars_GetBlock(lpEnvVars, &lpEnvVarsBlock)) goto handle_error;
  }
  else
  {
    lpEnvVarsBlock = RT_NULL;
  }

  if (!CreateProcess(RT_NULL,                        /* lpApplicationName.                                                         */
                     lpCommandLine,
                     RT_NULL,                        /* LPSECURITY_ATTRIBUTES lpProcessAttributes.                                 */
                     RT_NULL,                        /* LPSECURITY_ATTRIBUTES lpThreadAttributes.                                  */
                     TRUE,                           /* Align inheritance with Linux.                                              */
                     CREATE_UNICODE_ENVIRONMENT,     /* dwCreationFlags.                                                           */
                     lpEnvVarsBlock,                 /* lpEnvironment.                                                             */
                     lpCurrentDirectory,             /* If NULL, the new process will have the same current directory as this one. */
                     &rtStartupInfo,
                     (PROCESS_INFORMATION*)lpProcess
                     )) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (lpHeapBuffer)
  {
    if (!RtStaticHeap_Free(&lpHeapBuffer) && bResult) goto handle_error;
  }
  if (bRestoreErrorNonInheritable)
  {
    bRestoreErrorNonInheritable = RT_FALSE;
    if (!RtIoDevice_SetInheritable(lpActualStdError, RT_FALSE) && bResult) goto handle_error;
  }
  if (bRestoreOutputNonInheritable)
  {
    bRestoreOutputNonInheritable = RT_FALSE;
    if (!RtIoDevice_SetInheritable(lpActualStdOutput, RT_FALSE) && bResult) goto handle_error;
  }
  if (bRestoreInputNonInheritable)
  {
    bRestoreInputNonInheritable = RT_FALSE;
    if (!RtIoDevice_SetInheritable(lpActualStdInput, RT_FALSE) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

#else /* NOT RT_DEFINE_WINDOWS */

  if (bChild)
  {
    if (!RtProcess_CreateActualLinux(lpProcess, lpCurrentDirectory, lpEnvVars, lpStdInput, lpStdOutput, lpStdError, lpApplicationPathAndArgs)) goto handle_error;
  }
  else
  {
    if (!RtProcess_CreateLinuxUsingIntermediate(lpProcess, lpCurrentDirectory, lpEnvVars, lpStdInput, lpStdOutput, lpStdError, lpApplicationPathAndArgs)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

#endif
}

RT_B RT_API RtProcess_Join(RT_PROCESS* lpProcess)
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
      RtError_SetLast(RT_ERROR_FUNCTION_FAILED);
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

RT_B RT_API RtProcess_GetExitCode(RT_PROCESS* lpProcess, RT_UN32* lpExitCode)
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

RT_B RT_API RtProcess_Free(RT_PROCESS* lpProcess)
{
  RT_B bResult;

  bResult = RT_SUCCESS;

#ifdef RT_DEFINE_WINDOWS
  if (!CloseHandle(lpProcess->hThread))  bResult = RT_FAILURE;
  if (!CloseHandle(lpProcess->hProcess)) bResult = RT_FAILURE;
#endif

  return bResult;
}
