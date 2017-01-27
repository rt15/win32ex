#include "layer003/RtThread.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"

#ifndef RT_DEFINE_WINDOWS

/**
 * Wrapper callback for Linux that calls the real one.
 */
void* RT_CDECL RtLinuxThreadCallback(void* lpVoidThread)
{
  RT_THREAD* lpThread;

  lpThread = (RT_THREAD*)lpVoidThread;

  /* Call user callback with user parameter and save exit code. */
  lpThread->unExitCode = lpThread->lpThreadCallback(lpThread->lpParameter);
  lpThread->bExitCodeSet = RT_TRUE;

  return RT_NULL;
}

#endif

RT_B RT_API RtCreateThread(RT_THREAD* lpThread, RT_THREAD_CALLBACK lpThreadCallback, void* lpParameter)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  RT_UN32 unThreadId;
  lpThread->hThread = CreateThread(RT_NULL, 0, lpThreadCallback, lpParameter, 0, &unThreadId);
  bResult = (lpThread->hThread != RT_NULL);
#else /* NOT RT_DEFINE_WINDOWS */

  /* Save user callback and parameter. */
  lpThread->lpThreadCallback = lpThreadCallback;
  lpThread->lpParameter = lpParameter;

  /* Initialize bExitCodeSet */
  lpThread->bExitCodeSet = RT_FALSE;

  /* Start the thread. The pthread_create function return -1 in case of failure and set errno. */
  bResult = !pthread_create((pthread_t*)&lpThread->nThread, RT_NULL, &RtLinuxThreadCallback, lpThread);
#endif
  return bResult;
}

RT_B RT_API RtJoinThread(RT_THREAD* lpThread)
{
#ifndef RT_DEFINE_WINDOWS
  int nReturnedValue;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  bResult = (WaitForSingleObject(lpThread->hThread, INFINITE) != WAIT_FAILED);
#else /* NOT RT_DEFINE_WINDOWS */

  /* The pthread_join function returns an errno. */
  nReturnedValue = pthread_join(lpThread->nThread, RT_NULL);
  if (nReturnedValue)
  {
    errno = nReturnedValue;
    bResult = RT_FALSE;
  }
  else
  {
    bResult = RT_TRUE;
  }
#endif
  return bResult;
}

RT_B RT_API RtJoinAndCheckThread(RT_THREAD* lpThread)
{
  RT_UN32 unThreadExitCode;
  RT_B bResult;

  if (!RtJoinThread(lpThread)) goto handle_error;
  if (!RtGetThreadExitCode(lpThread, &unThreadExitCode)) goto handle_error;
  if (!unThreadExitCode)
  {
    /* Unknown error in the joined thread which cannot set last error for this thread. */
    RtSetLastError(RT_ERROR_FUNCTION_FAILED);
    goto handle_error;
  }

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtGetThreadExitCode(RT_THREAD* lpThread, RT_UN32* lpExitCode)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  bResult = GetExitCodeThread(lpThread->hThread, lpExitCode);
#else /* NOT RT_DEFINE_WINDOWS */

  bResult = lpThread->bExitCodeSet;
  if (bResult)
  {
    *lpExitCode = lpThread->unExitCode;
  }
  else
  {
    errno = ENODATA;
  }
#endif
  return bResult;
}

RT_B RT_API RtFreeThread(RT_THREAD* lpThread)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  bResult = CloseHandle(lpThread->hThread);
#else /* NOT RT_DEFINE_WINDOWS */
  bResult = RT_TRUE;
#endif
  return bResult;
}
