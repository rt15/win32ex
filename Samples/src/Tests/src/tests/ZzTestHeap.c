#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestHeap(RT_HEAP** lpHeap)
{
  RT_UCHAR8 lpBuffer[50];
  void* lpArea;
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;
  RT_B bResult;

  lpHeapBuffer = RT_NULL;

  if (!RtHeapHelper_AllocIfNeeded(lpBuffer, 50, RT_NULL, RT_NULL, &lpArea, 30, _R("foo"), RT_NULL)) goto handle_error;
  if (lpArea != lpBuffer) goto handle_error;

  /* Alloc. */
  unHeapBufferSize = 0;
  if (!RtHeapHelper_AllocIfNeeded(RT_NULL, 0, &lpHeapBuffer, &unHeapBufferSize, &lpArea, 30, _R("foo"), lpHeap)) goto handle_error;
  if (lpArea != lpHeapBuffer) goto handle_error;
  if (unHeapBufferSize != 30) goto handle_error;

  /* Big enough heap buffer . */
  lpArea = RT_NULL;
  if (!RtHeapHelper_AllocIfNeeded(RT_NULL, 0, &lpHeapBuffer, &unHeapBufferSize, &lpArea, 15, _R("foo"), lpHeap)) goto handle_error;
  if (lpArea != lpHeapBuffer) goto handle_error;
  if (unHeapBufferSize != 30) goto handle_error;

  /* Re-alloc. */
  lpArea = RT_NULL;
  if (!RtHeapHelper_AllocIfNeeded(RT_NULL, 0, &lpHeapBuffer, &unHeapBufferSize, &lpArea, 40, _R("foo"), lpHeap)) goto handle_error;
  if (lpArea != lpHeapBuffer) goto handle_error;
  if (unHeapBufferSize != 40) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (lpHeapBuffer)
  {
    if (!(*lpHeap)->lpFree(lpHeap, &lpHeapBuffer)) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
