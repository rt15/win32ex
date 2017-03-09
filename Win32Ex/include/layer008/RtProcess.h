#ifndef RT_PROCESS_H
#define RT_PROCESS_H

#include "layer000/RtWin32ExTypes.h"
#include "layer006/RtFile.h"

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
 * If successful, ensure to call RtFreeProcess (And optionally RtJoinProcess to avoid zombies).
 * </p>
 *
 * @param lpCurrentDirectory Current directory of the started process. Same as current process if RT_NULL.
 * @param ... Process arguments (RT_CHAR*). Must end with RT_NULL.
 */
RT_B RT_CDECL_API RtCreateProcess(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory, RT_CHAR* lpApplicationName, ...);

RT_B RT_CDECL_API RtCreateProcessWithRedirections(RT_PROCESS* lpProcess, RT_CHAR* lpCurrentDirectory,
                                                  RT_FILE* lpStdInput, RT_FILE* lpStdOutput, RT_FILE* lpStdError,
                                                  RT_CHAR* lpApplicationName, ...);

RT_B RT_API RtVCreateProcess(RT_PROCESS* lpProcess, va_list lpVaList, RT_CHAR* lpCurrentDirectory,
                             RT_FILE* lpStdInput, RT_FILE* lpStdOutput, RT_FILE* lpStdError,
                             RT_CHAR* lpApplicationName);

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
