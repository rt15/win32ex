#include "layer003/RtThread.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"

#ifndef RT_DEFINE_WINDOWS

/**
 * Wrapper callback for Linux that calls the real one.
 */
void* RT_CDECL RtThread_LinuxCallback(void* lpVoidThread)
{
  RT_THREAD* lpThread;

  lpThread = (RT_THREAD*)lpVoidThread;

  /* Call user callback with user parameter and save exit code. */
  lpThread->unExitCode = lpThread->lpThreadCallback(lpThread->lpParameter);
  lpThread->bExitCodeSet = RT_TRUE;

  return RT_NULL;
}

#endif

RT_B RT_API RtThread_Create(RT_THREAD* lpThread, RT_THREAD_CALLBACK lpThreadCallback, void* lpParameter)
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
  bResult = !pthread_create((pthread_t*)&lpThread->nThread, RT_NULL, &RtThread_LinuxCallback, lpThread);
#endif
  return bResult;
}

RT_B RT_API RtThread_Join(RT_THREAD* lpThread)
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
    bResult = RT_FAILURE;
  }
  else
  {
    bResult = RT_SUCCESS;
  }
#endif
  return bResult;
}

RT_B RT_API RtThread_JoinAndCheck(RT_THREAD* lpThread)
{
  RT_UN32 unThreadExitCode;
  RT_B bResult;

  if (!RtThread_Join(lpThread)) goto handle_error;
  if (!RtThread_GetExitCode(lpThread, &unThreadExitCode)) goto handle_error;
  if (!unThreadExitCode)
  {
    /* Unknown error in the joined thread which cannot set last error for this thread. */
    RtError_SetLast(RT_ERROR_FUNCTION_FAILED);
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtThread_GetExitCode(RT_THREAD* lpThread, RT_UN32* lpExitCode)
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

RT_B RT_API RtThread_Free(RT_THREAD* lpThread)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  bResult = CloseHandle(lpThread->hThread);
#else /* NOT RT_DEFINE_WINDOWS */
  bResult = RT_SUCCESS;
#endif
  return bResult;
}
