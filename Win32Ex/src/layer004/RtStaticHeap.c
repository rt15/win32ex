#include "layer004/RtStaticHeap.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"
#include "layer003/RtFastInitialization.h"

#ifdef RT_DEFINE_WINDOWS

RT_FAST_INITIALIZATION rt_staticHeapInitialization = RT_FAST_INITIALIZATION_STATIC_INIT;

/* No need to free heap handle from GetProcessHeap. */
RT_H rt_hStaticHeapProcessHeap;

#endif

void* RT_API RtAlloc(void** lpArea, RT_UN unSize)
{
#ifdef RT_DEFINE_WINDOWS

  if (RtFastInitialization_IsRequired(&rt_staticHeapInitialization))
  {
    rt_hStaticHeapProcessHeap = GetProcessHeap();
    if (!rt_hStaticHeapProcessHeap)
    {
      /* GetProcessHeap calls SetLastError in case of failure. */
      goto handle_error;
    }
    RtFastInitialization_NotifyDone(&rt_staticHeapInitialization);
  }

  *lpArea = HeapAlloc(rt_hStaticHeapProcessHeap, 0, unSize);
  if (!*lpArea)
  {
    /* HeapAlloc does not call SetLastError. */
    RtError_SetLast(RT_ERROR_NOT_ENOUGH_MEMORY);
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
  void* lpResult;

#ifdef RT_DEFINE_WINDOWS
  /* HeapReAlloc return NULL and call SetLastError in case of failure. Current area is left unchanged. */
  lpResult = HeapReAlloc(rt_hStaticHeapProcessHeap, 0, *lpArea, unSize);
#else
  /* realloc set errno. Current area is left unchanged. */
  lpResult = realloc(*lpArea, unSize);
#endif
  /* If re-alloc has been successful, *lpArea is no more allocated. */
  if (lpResult)
  {
    *lpArea = lpResult;
  }
  return lpResult;
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
    bResult = RT_SUCCESS;
#endif
    *lpArea = RT_NULL;
  }
  else
  {
    bResult = RT_SUCCESS;
  }
  return bResult;
}

void* RT_API RtAllocIfNeeded(void* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, void** lpArea, RT_UN unSize)
{
  if ((unSize < RT_TYPE_MAX_N) && (unSize <= unBufferSize))
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
