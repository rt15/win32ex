#ifndef RT_PROCESS_H
#define RT_PROCESS_H

#include "layer000/RtWin32ExTypes.h"
#include "layer002/RtIoDevice.h"
#include "layer005/RtEnvVars.h"
#include "layer005/RtFile.h"

/**
 * @file
 * Processes functions.
 *
 * <p>
 * After a successful call to RtCreateProcess, RtJoinProcess and RtFreeProcess are mandatory.
 * </p>
 */

typedef struct _RT_PROCESS
{
#ifdef RT_DEFINE_WINDOWS
  /* RT_PROCESS first fields must be exactly the same fields as PROCESS_INFORMATION. */
  RT_H hProcess;
  RT_H hThread;
  RT_UN32  unProcessId;
  RT_UN32  unThreadId;
#else
  RT_N32 nPid;
  RT_UN32 unExitCode;
#endif
}
RT_PROCESS;

/**
 * Start a process.
 *
 * <p>
 * If successful, ensure to call RtFreeProcess (And optionally RtJoinProcess to avoid zombies if bChild is RT_TRUE).
 * </p>
 *
 * <p>
 * Watch out for handles/file descriptors leaks.<br>
 * Under Linux, any file descriptor without FD_CLOEXEC/O_CLOEXEC will "leak".<br>
 * Under Windows, any handle with HANDLE_FLAG_INHERIT/bInheritHandle of SECURITY_ATTRIBUTES will "leak".
 * </p>
 *
 * @param bChild Set to RT_TRUE if and only if you will call RtJoinProcess. No effect under Windows.
 * @param lpCurrentDirectory Current directory of the started process. Same as current process if RT_NULL.
 * @param ... Process arguments (RT_CHAR*). Must end with RT_NULL.
 */
RT_B RT_API RtCreateProcess(RT_PROCESS* lpProcess, RT_B bChild, RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars, RT_CHAR** lpApplicationPathAndArgs);

/**
 * Spawn a child process and wait for the end of it.
 */
RT_B RT_API RtSpawnProcessSync(RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars, RT_CHAR** lpApplicationPathAndArgs, RT_UN32* lpExitCode);

/**
 * Spawn a non-child process.
 */
RT_B RT_API RtSpawnProcessASync(RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars, RT_CHAR** lpApplicationPathAndArgs);

/**
 * Start a process with redirections.
 *
 * </p>
 * Beware that there is no need for the redirected files to be inheritable.<br>
 * Under Linux, dup2 is used for the redirection, and dup2 does not copy the FD_CLOEXEC into the duplicated file descriptor.<br>
 * Under Windows, RtSetFileInheritable is used to temporarily configure the handle as inheritable if it is not already.<br>
 * Be careful as this can cause a race conditions with other calls to CreateProcess from other threads.<br>
 * Be sure to use a critical section to avoid handles leaks if needed.
 * </p>
 *
 * @param lpStdInput Can be RT_NULL.
 * @param lpStdOutput Can be RT_NULL.
 * @param lpStdError Can be RT_NULL.
 */
RT_B RT_API RtCreateProcessWithRedirections(RT_PROCESS* lpProcess, RT_B bChild, RT_CHAR* lpCurrentDirectory, RT_ENV_VARS* lpEnvVars,
                                            RT_IO_DEVICE* lpStdInput, RT_IO_DEVICE* lpStdOutput, RT_IO_DEVICE* lpStdError,
                                            RT_CHAR** lpApplicationPathAndArgs);

/**
 * Can be used only if bChild was RT_TRUE while creating the process.<br>
 * Called to read the exit code to kill a zombified process.
 */
RT_B RT_API RtJoinProcess(RT_PROCESS* lpProcess);

/**
 * <p>
 * Under Linux, exit code is 8 unsigned bits, 0 -> 255.<br>
 * Under Windows, exit code is a DWORD (32 unsigned bits) although command line interpreter treats them as signed.<br>
 * </p>
 */
RT_B RT_API RtGetProcessExitCode(RT_PROCESS* lpProcess, RT_UN32* lpExitCode);

RT_B RT_API RtFreeProcess(RT_PROCESS* lpProcess);

#endif /* RT_PROCESS_H */
