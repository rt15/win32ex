#include "ZzAppContext.h"

#include "ZzResources.h"
#include "ZzWindows.h"

RT_B RT_CALL ZzCreateAppContext(ZZ_APP_CONTEXT* lpAppContext)
{
  RT_N nI;
  RT_B bResult;

  lpAppContext->bHeapCreated = RT_FALSE;
  for (nI = 0; nI < ZZ_RESOURCES_ENTITIES_COUNT; nI++)
  {
    lpAppContext->lpLists[nI] = RT_NULL;
  }

  if (!RtRuntimeHeapCreate(&lpAppContext->rtRuntimeHeap)) goto handle_error;
  lpAppContext->bHeapCreated = RT_TRUE;

  for (nI = 0; nI < ZZ_RESOURCES_ENTITIES_COUNT; nI++)
  {
    if (!RtCreateList(&lpAppContext->lpLists[nI], &lpAppContext->rtRuntimeHeap.lpHeap, 0, ZZ_RESOURCES_NAME_SIZE, 3000 / ZZ_RESOURCES_NAME_SIZE)) goto handle_error;
  }

  lpAppContext->hInstance = RtGetInstance();
  lpAppContext->unWindowExitCode = 0;
  lpAppContext->nCurrentEntity = 0;

  /* Initial position of the vertical splitter. */
  lpAppContext->nVerticalSplitterX = ZZ_RESOURCES_MIN_LEFT_TAB_WIDTH;
  lpAppContext->bUsingVerticalSplitter = RT_FALSE;

  /* No need to destroy cursors from LoadCursor. */
  lpAppContext->hStandardCursor = (RT_H)LoadCursor(RT_NULL, IDC_ARROW);
  if (!lpAppContext->hStandardCursor) goto handle_error;
  lpAppContext->hWestEastCursor = (RT_H)LoadCursor(RT_NULL, IDC_SIZEWE);
  if (!lpAppContext->hWestEastCursor) goto handle_error;

  /* No need to call DeleteObject on stock objects. */
  lpAppContext->hFont = GetStockObject(DEFAULT_GUI_FONT);
  if (!lpAppContext->hFont) goto handle_error;

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  ZzFreeAppContext(lpAppContext);
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_CALL ZzFreeAppContext(ZZ_APP_CONTEXT* lpAppContext)
{
  RT_N nI;
  RT_B bResult;

  bResult = RT_TRUE;
free_resources:
  for (nI = 0; nI < ZZ_RESOURCES_ENTITIES_COUNT; nI++)
  {
    if (lpAppContext->lpLists[nI])
    {
      if (!RtFreeList(&lpAppContext->lpLists[nI]) && bResult) goto handle_error;
    }
  }
  if (lpAppContext->bHeapCreated)
  {
    lpAppContext->bHeapCreated = RT_FALSE;
    if (!lpAppContext->rtRuntimeHeap.lpHeap->lpClose(&lpAppContext->rtRuntimeHeap) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}
