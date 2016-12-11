#include "ZzAppContext.h"

#include "ZzResources.h"
#include "ZzWindows.h"

RT_B RT_CALL ZzCreateAppContext(ZZ_APP_CONTEXT* lpAppContext)
{
  RT_B bResult;

  lpAppContext->hInstance = RtGetInstance();
  lpAppContext->unWindowExitCode = 0;

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

  lpAppContext->hMainWindow = RT_NULL;
  lpAppContext->hStatusBar = RT_NULL;
  lpAppContext->hLeftTab = RT_NULL;
  lpAppContext->hVerticalSplitter = RT_NULL;

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_CALL ZzFreeAppContext(ZZ_APP_CONTEXT* lpAppContext)
{
  return RT_TRUE;
}
