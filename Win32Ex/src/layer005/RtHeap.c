#include "layer005/RtHeap.h"

#include "layer002/RtErrorCode.h"

void* RT_API RtAllocIfNeededWithHeap(void* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, void** lpArea,  RT_UN unSize, RT_CHAR* lpName, RT_HEAP** lpHeap)
{
  /* If a buffer is provided, attempt to use it. */
  if (lpBuffer && nBufferSize > 0)
  {
    if (unSize <= (RT_UN)nBufferSize)
    {
      /* The buffer is enough, don't go any further. */
      *lpArea = lpBuffer;
      goto free_resources;
    }
  }

  /* lpBuffer is RT_NULL or nBufferSize is not enough. Try to use buffer heap. */
  if ((lpHeapBufferSize) && (unSize <= (RT_UN)*lpHeapBufferSize))
  {
    /* Given heap buffer is enough. */
    *lpArea = *lpHeapBuffer;
    goto free_resources;
  }

  /* Both lpBuffer and lpHeapBuffer are null or not enough, use heap. */
  if (!lpHeap)
  {
    /* Buffer is not enough and no heap has been provided. Notify error. */
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    *lpArea = lpBuffer;
  }
  else
  {
    /* If a too small heap buffer exists, realloc. */
    if (*lpHeapBuffer)
    {
      *lpArea = (*lpHeap)->lpReAlloc(lpHeap, lpHeapBuffer, unSize);
    }
    else
    {
      /* Allocate space from the heap. */
      *lpArea = (*lpHeap)->lpAlloc(lpHeap, lpHeapBuffer, unSize, lpName);
    }
    *lpHeapBufferSize = unSize;
  }

free_resources:
  return *lpArea;
}
