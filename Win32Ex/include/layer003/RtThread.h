#ifndef RT_THREAD_H
#define RT_THREAD_H

#include "layer000/RtWin32ExTypes.h"

/*
Thread creation, with void* parameter.
Thread suspension is not supported under linux (And not much interesting under windows.

Under windows, I might call the callback directly.
Under linux, I might need a wrapper (And as a consequence an additional parameter).

The result of a linux thread seems to be a pointer, and is get by join.

Under windows, it is a dword and it is retrieved using GetExitCodeThread.

*/

typedef RT_UN32 (RT_CALL *RT_THREAD_CALLBACK)(void* lpParameter);

typedef struct _RT_THREAD
{
#ifdef RT_DEFINE_WINDOWS
  RT_H hThread;
#else
  /* 25.08.2016. */
  unsigned long int nThread;
  /* User callback. */
  RT_THREAD_CALLBACK lpThreadCallback;
  /* User parameter. */
  void* lpParameter;
  RT_UN32 unExitCode;
  RT_B bExitCodeSet;
#endif
}
RT_THREAD;


RT_B RT_API RtCreateThread(RT_THREAD* lpThread, RT_THREAD_CALLBACK lpThreadCallback, void* lpParameter);

RT_B RT_API RtJoinThread(RT_THREAD* lpThread);

/**
 * Join with a thread and check that exit code is non-zero.
 */
RT_B RT_API RtJoinAndCheckThread(RT_THREAD* lpThread);

/**
 * <p>
 * Must be called before <tt>RtFreeThread</tt>.
 * </p>
 */
RT_B RT_API RtGetThreadExitCode(RT_THREAD* lpThread, RT_UN32* lpExitCode);

RT_B RT_API RtFreeThread(RT_THREAD* lpThread);

#endif /* RT_THREAD_H */
