#include "layer005/RtStaticHeap.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer004/RtFastInitialization.h"

#ifdef RT_DEFINE_WINDOWS

RT_FAST_INITIALIZATION rt_staticHeapInitialization = RT_FAST_INITIALIZATION_STATIC_INIT;

/* No need to free heap handle from GetProcessHeap. */
RT_H rt_hStaticHeapProcessHeap;

#endif

void* RT_API RtAlloc(void** lpArea, RT_UN unSize)
{
#ifdef RT_DEFINE_WINDOWS

  if (RtFastInitializationRequired(&rt_staticHeapInitialization))
  {
    rt_hStaticHeapProcessHeap = GetProcessHeap();
    if (!rt_hStaticHeapProcessHeap)
    {
      /* GetProcessHeap calls SetLastError in case of failure. */
      goto handle_error;
    }
    RtNotifyFastInitializationDone(&rt_staticHeapInitialization);
  }

  *lpArea = HeapAlloc(rt_hStaticHeapProcessHeap, 0, unSize);
  if (!*lpArea)
  {
    /* HeapAlloc does not call SetLastError. */
    RtSetLastError(RT_ERROR_NOT_ENOUGH_MEMORY);
  }
  goto free_resources;
handle_error:
  *lpArea = RT_NULL;
free_resources:

#else
  /* Malloc set errno. */
  *lpArea = malloc(unSize);
#endif

  return *lpArea;
}

void* RT_API RtReAlloc(void** lpArea, RT_UN unSize)
{
#ifdef RT_DEFINE_WINDOWS
  /* HeapReAlloc return NULL and call SetLastError in case of failure. */
  *lpArea = HeapReAlloc(rt_hStaticHeapProcessHeap, 0, *lpArea, unSize);
#else
  /* realloc set errno. */
  *lpArea = realloc(*lpArea, unSize);
#endif
  return *lpArea;
}

RT_B RT_API RtFree(void** lpArea)
{
  RT_B bResult;

  if (*lpArea)
  {
#ifdef RT_DEFINE_WINDOWS
    /* HeapFree return 0 and call SetLastError in case of error. */
    bResult = HeapFree(rt_hStaticHeapProcessHeap, 0, *lpArea);
#else
    /* The free function returns void. */
    free(*lpArea);
    bResult = RT_TRUE;
#endif
    *lpArea = RT_NULL;
  }
  else
  {
    bResult = RT_TRUE;
  }
  return bResult;
}

void* RT_API RtAllocIfNeeded(void* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, void** lpArea, RT_UN unSize)
{
  if ((unSize < RT_TYPE_MAX_N) && ((RT_N)unSize <= nBufferSize))
  {
    *lpArea = lpBuffer;
  }
  else
  {
    if ((lpHeapBufferSize) && (unSize <= (RT_UN)*lpHeapBufferSize))
    {
      *lpArea = *lpHeapBuffer;
    }
    else
    {
      if (*lpHeapBuffer)
      {
        *lpArea = RtReAlloc(lpHeapBuffer, unSize);
      }
      else
      {
        *lpArea = RtAlloc(lpHeapBuffer, unSize);
      }
    }
    *lpHeapBufferSize = unSize;
  }
  return *lpArea;
}
