#include "layer003/RtHeapHelper.h"

#include "layer002/RtError.h"

void* RT_API RtHeapHelper_AllocIfNeeded(void* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, void** lpArea,  RT_UN unSize, RT_CHAR* lpName, RT_HEAP** lpHeap)
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
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
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
