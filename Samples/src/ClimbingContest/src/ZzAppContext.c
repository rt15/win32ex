#include "ZzAppContext.h"

#include "ZzResources.h"
#include "ZzWindows.h"

RT_B RT_CALL ZzCreateAppContext(ZZ_APP_CONTEXT* lpAppContext)
{
  RT_UN unI;
  RT_B bResult;

  lpAppContext->bHeapCreated = RT_FALSE;
  for (unI = 0; unI < ZZ_RESOURCES_ENTITIES_COUNT; unI++)
  {
    lpAppContext->lpLists[unI] = RT_NULL;
  }

  if (!RtRuntimeHeapCreate(&lpAppContext->rtRuntimeHeap)) goto handle_error;
  lpAppContext->bHeapCreated = RT_TRUE;

  for (unI = 0; unI < ZZ_RESOURCES_ENTITIES_COUNT; unI++)
  {
    if (!RtCreateList(&lpAppContext->lpLists[unI], &lpAppContext->rtRuntimeHeap.lpHeap, 0, ZZ_RESOURCES_NAME_SIZE, 3000 / ZZ_RESOURCES_NAME_SIZE)) goto handle_error;
  }

  lpAppContext->hInstance = RtGetInstance();
  lpAppContext->unWindowExitCode = 0;
  lpAppContext->unCurrentEntity = 0;

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

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  ZzFreeAppContext(lpAppContext);
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzFreeAppContext(ZZ_APP_CONTEXT* lpAppContext)
{
  RT_UN unI;
  RT_B bResult;

  bResult = RT_SUCCESS;
free_resources:
  for (unI = 0; unI < ZZ_RESOURCES_ENTITIES_COUNT; unI++)
  {
    if (lpAppContext->lpLists[unI])
    {
      if (!RtFreeList(&lpAppContext->lpLists[unI]) && bResult) goto handle_error;
    }
  }
  if (lpAppContext->bHeapCreated)
  {
    lpAppContext->bHeapCreated = RT_FALSE;
    if (!lpAppContext->rtRuntimeHeap.lpHeap->lpClose(&lpAppContext->rtRuntimeHeap) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
