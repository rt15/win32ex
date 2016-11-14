#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestHeap(RT_HEAP** lpHeap)
{
  RT_UCHAR8 lpBuffer[50];
  void* lpArea;
  void* lpHeapBuffer;
  RT_N nHeapBufferSize;
  RT_B bResult;

  lpHeapBuffer = RT_NULL;

  if (!RtAllocIfNeededWithHeap(lpBuffer, 50, RT_NULL, RT_NULL, &lpArea, 30, _R("foo"), RT_NULL)) goto handle_error;
  if (lpArea != lpBuffer) goto handle_error;

  /* Alloc. */
  nHeapBufferSize = 0;
  if (!RtAllocIfNeededWithHeap(RT_NULL, 0, &lpHeapBuffer, &nHeapBufferSize, &lpArea, 30, _R("foo"), lpHeap)) goto handle_error;
  if (lpArea != lpHeapBuffer) goto handle_error;
  if (nHeapBufferSize != 30) goto handle_error;

  /* Heap buffer big enough. */
  lpArea = RT_NULL;
  if (!RtAllocIfNeededWithHeap(RT_NULL, 0, &lpHeapBuffer, &nHeapBufferSize, &lpArea, 15, _R("foo"), lpHeap)) goto handle_error;
  if (lpArea != lpHeapBuffer) goto handle_error;
  if (nHeapBufferSize != 30) goto handle_error;

  /* Re-alloc. */
  lpArea = RT_NULL;
  if (!RtAllocIfNeededWithHeap(RT_NULL, 0, &lpHeapBuffer, &nHeapBufferSize, &lpArea, 40, _R("foo"), lpHeap)) goto handle_error;
  if (lpArea != lpHeapBuffer) goto handle_error;
  if (nHeapBufferSize != 40) goto handle_error;

  bResult = RT_TRUE;
free_resources:
  if (lpHeapBuffer)
  {
    if (!(*lpHeap)->lpFree(lpHeap, &lpHeapBuffer))
    {
      lpHeapBuffer = RT_NULL;
      goto handle_error;
    }
    lpHeapBuffer = RT_NULL;
  }
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}
