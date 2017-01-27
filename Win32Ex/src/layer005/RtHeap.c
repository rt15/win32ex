#include "layer005/RtHeap.h"

#include "layer002/RtErrorCode.h"

void* RT_API RtAllocIfNeededWithHeap(void* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, void** lpArea,  RT_UN unSize, RT_CHAR* lpName, RT_HEAP** lpHeap)
{
  /* If a buffer is provided, attempt to use it. */
  if (lpBuffer && unBufferSize > 0)
  {
    if (unSize <= (RT_UN)unBufferSize)
    {
      /* The buffer is enough, don't go any further. */
      *lpArea = lpBuffer;
      goto free_resources;
    }
  }

  /* lpBuffer is RT_NULL or unBufferSize is not enough. Try to use heap buffer. */
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
      if ((*lpHeap)->lpReAlloc(lpHeap, lpArea, *lpHeapBuffer, unSize))
      {
        /* Reallocation was a success, use new area as heap buffer. */
        *lpHeapBuffer = *lpArea;
      }
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
