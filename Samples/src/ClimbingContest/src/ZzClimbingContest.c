#define RT_DEFINE_GUI

#include <RtWin32Ex.h>
#include <RtWin32ExGui.h>
#include <RtWin32ExMain.h>

#include "ZzAppContext.h"
#include "ZzResources.h"
#include "ZzStrings.h"
#include "ZzGui.h"
#include "ZzWindows.h"


RT_N RT_CALL ZzMainWindowProc(RT_H hWindow, RT_UN32 unMsg, RT_UN unWParam, RT_N nLParam)
{
  ZZ_APP_CONTEXT* lpAppContext;
  RT_B bCallDefault;
  MINMAXINFO* lpMinMaxInfo;
  RT_GUI_RECT rtRect;
  POINT rtCursorPosition;
  LONG_PTR nSendMessageResult;
  RT_H hChild;
  RT_N nResult;

  lpAppContext = (ZZ_APP_CONTEXT*)RtGetWindowUserData(hWindow);

  bCallDefault = RT_TRUE;
  switch (unMsg)
  {
    case WM_COMMAND:
      if (HIWORD(unWParam) == BN_CLICKED)
      {
        switch (LOWORD(unWParam))
        {
          case ZZ_RESOURCES_ADD_BUTTON_CTRL_ID:
            SendMessage(lpAppContext->hListBox, LB_ADDSTRING, 0, (LPARAM)_R("New item"));
            break;
          case ZZ_RESOURCES_DELETE_BUTTON_CTRL_ID:
            nSendMessageResult = SendMessage(lpAppContext->hListBox, LB_GETCURSEL, 0, 0);
            if (nSendMessageResult != LB_ERR)
            {
              SendMessage(lpAppContext->hListBox, LB_DELETESTRING, nSendMessageResult, 0);
              EnableWindow(lpAppContext->hDeleteButton, FALSE);
            }
            break;
        }
      }
      else if (HIWORD(unWParam) == LBN_SELCHANGE)
      {
        nSendMessageResult = SendMessage(lpAppContext->hListBox, LB_GETCURSEL, 0, 0);
        if (nSendMessageResult != LB_ERR)
        {
          /* An item is selected, enable delete button. */
          EnableWindow(lpAppContext->hDeleteButton, TRUE);
        }
      }
      break;
    case WM_GETMINMAXINFO:
      /* Apply a minimum size to the window. */
      lpMinMaxInfo = (MINMAXINFO*)nLParam;
      lpMinMaxInfo->ptMinTrackSize.x = ZZ_RESOURCES_MIN_WINDOW_WIDTH;
      lpMinMaxInfo->ptMinTrackSize.y = ZZ_RESOURCES_MIN_WINDOW_HEIGHT;
      break;

    case WM_SIZE:
      /* Resize status bar. */
      SendMessage(lpAppContext->hStatusBar, WM_SIZE, unWParam, nLParam);

      RtMoveWindow(lpAppContext->hLeftTab, ZzComputeLeftTabPosition(&rtRect, lpAppContext));
      RtMoveWindow(lpAppContext->hVerticalSplitter, ZzComputeVerticalSplitterPosition(&rtRect, lpAppContext));
      RtMoveWindow(lpAppContext->hListBox, ZzComputeListBoxPosition(&rtRect, lpAppContext));
      break;

    case WM_LBUTTONDOWN:
      rtCursorPosition.x = LOWORD(nLParam);
      rtCursorPosition.y = HIWORD(nLParam);
      hChild = ChildWindowFromPoint(lpAppContext->hMainWindow, rtCursorPosition);
      if (hChild == lpAppContext->hVerticalSplitter)
      {
        ZzComputeVerticalSplitterPosition(&rtRect, lpAppContext);
        lpAppContext->nSplitterCursorOffset = rtCursorPosition.x - rtRect.nX;
        SetCursor(lpAppContext->hWestEastCursor);
        lpAppContext->bUsingVerticalSplitter = RT_TRUE;
        SetCapture(lpAppContext->hMainWindow);
      }
      break;

    case WM_LBUTTONUP:
      if (lpAppContext->bUsingVerticalSplitter)
      {
        ReleaseCapture();
        lpAppContext->bUsingVerticalSplitter = RT_FALSE;
      }
      break;

    case WM_CAPTURECHANGED:
      /* For a reason, we lost the capture. Abandon possible resizing. */
      lpAppContext->bUsingVerticalSplitter = RT_FALSE;
      break;

    case WM_MOUSEMOVE:
      rtCursorPosition.x = LOWORD(nLParam);
      rtCursorPosition.y = HIWORD(nLParam);
      if (lpAppContext->bUsingVerticalSplitter && rtCursorPosition.x < RT_TYPE_MAX_N16)
      {
        lpAppContext->nVerticalSplitterX = rtCursorPosition.x - lpAppContext->nSplitterCursorOffset;
        RtMoveWindow(lpAppContext->hLeftTab, ZzComputeLeftTabPosition(&rtRect, lpAppContext));
        RtMoveWindow(lpAppContext->hVerticalSplitter, ZzComputeVerticalSplitterPosition(&rtRect, lpAppContext));
        RtMoveWindow(lpAppContext->hListBox, ZzComputeListBoxPosition(&rtRect, lpAppContext));
      }
      else
      {
        hChild = ChildWindowFromPoint(lpAppContext->hMainWindow, rtCursorPosition);
        if (hChild == lpAppContext->hVerticalSplitter)
        {
          SetCursor(lpAppContext->hWestEastCursor);
        }
        else
        {
          SetCursor(lpAppContext->hStandardCursor);
        }
      }
      break;

    case WM_DESTROY:
      /* Request end of message loop. */
      RtQuitMessageLoop(lpAppContext->unWindowExitCode);
      break;
  }

  if (bCallDefault)
  {
    /* Default windows treatment. */
    nResult = DefWindowProc(hWindow, unMsg, unWParam, nLParam);
  }
  return nResult;
}

RT_B RT_CALL RtMainWithBoolean(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  ZZ_APP_CONTEXT rtAppContext;
  RT_B bMainWindowCreated;
  RT_B bAppContextCreated;
  RT_GUI_RECT rtRect;
  RT_B bResult;

  bAppContextCreated = RT_FALSE;
  bMainWindowCreated = RT_FALSE;

  if (!RtInitCommonControls(ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES )) goto handle_error;

  if (!ZzCreateAppContext(&rtAppContext)) goto handle_error;
  bAppContextCreated = RT_TRUE;

  rtRect.nX = CW_USEDEFAULT;
  rtRect.nY = 0;
  rtRect.nWidth = ZZ_RESOURCES_DEFAULT_WINDOW_WIDTH;
  rtRect.nHeight = ZZ_RESOURCES_DEFAULT_WINDOW_HEIGHT;

  rtAppContext.hMainWindow = RtCreateMainWindow(ZzGetString(ZZ_STRINGS_APPLICATION_NAME), _R("MainWindow"),
                                                WS_OVERLAPPEDWINDOW, WS_EX_COMPOSITED, &rtRect, &ZzMainWindowProc, RT_NULL, RT_NULL,
                                                &rtAppContext, rtAppContext.hInstance);
  if (!rtAppContext.hMainWindow) goto handle_error;
  bMainWindowCreated = RT_TRUE;

  /* Status bar. */
  rtAppContext.hStatusBar = RtCreateStatusBar(RT_TRUE, _R(""), rtAppContext.hMainWindow, ZZ_RESOURCES_STATUS_BAR_CTRL_ID, rtAppContext.hInstance);
  if (!rtAppContext.hStatusBar) goto handle_error;

  rtAppContext.hLeftTab = ZzCreateLeftTab(ZzComputeLeftTabPosition(&rtRect, &rtAppContext), rtAppContext.hMainWindow, rtAppContext.hInstance, rtAppContext.hFont);
  if (!rtAppContext.hLeftTab) goto handle_error;

  rtAppContext.hVerticalSplitter = RtCreateStaticWindow(ZzComputeVerticalSplitterPosition(&rtRect, &rtAppContext), rtAppContext.hMainWindow, ZZ_RESOURCES_VERTICAL_SPLITTER_CTRL_ID, rtAppContext.hInstance);
  if (!rtAppContext.hVerticalSplitter) goto handle_error;

  rtAppContext.hListBox = ZzCreateListBox(ZzComputeListBoxPosition(&rtRect, &rtAppContext), _R("EntitesListBox"), ZZ_RESOURCES_ENTITIES_LIST_CTRL_ID, rtAppContext.hLeftTab, rtAppContext.hInstance, rtAppContext.hFont);
  if (!rtAppContext.hListBox) goto handle_error;

  rtRect.nX = ZZ_GUI_BORDER;
  rtRect.nY = ZZ_GUI_TAB_HEIGHT + ZZ_GUI_BORDER;
  rtRect.nWidth = ZZ_GUI_BUTTON_WIDTH;
  rtRect.nHeight = ZZ_GUI_BUTTON_HEIGHT;

  rtAppContext.hAddButton = ZzCreateButton(&rtRect, ZzGetString(ZZ_STRINGS_ADD), ZZ_RESOURCES_ADD_BUTTON_CTRL_ID, rtAppContext.hLeftTab, rtAppContext.hInstance, rtAppContext.hFont);
  if (!rtAppContext.hAddButton) goto handle_error;

  rtRect.nX = ZZ_GUI_BORDER * 2 + ZZ_GUI_BUTTON_WIDTH;

  rtAppContext.hDeleteButton = ZzCreateButton(&rtRect, ZzGetString(ZZ_STRINGS_DELETE), ZZ_RESOURCES_DELETE_BUTTON_CTRL_ID, rtAppContext.hLeftTab, rtAppContext.hInstance, rtAppContext.hFont);
  if (!rtAppContext.hDeleteButton) goto handle_error;

  /* Delete button is disabled by default. */
  EnableWindow(rtAppContext.hDeleteButton, FALSE);

  /* No way to check success of ShowWindow. */
  ShowWindow(rtAppContext.hMainWindow, SW_SHOWNORMAL);

  /* The main window is destroyed when it is closed. */
  bMainWindowCreated = RT_FALSE;

  /* RtDefaultMessageLoop returns an exit code. */
  bResult = !RtDefaultMessageLoop();
free_resources:
  if (bMainWindowCreated)
  {
    bMainWindowCreated = RT_FALSE;
    if (!DestroyWindow(rtAppContext.hMainWindow) && bResult) goto handle_error;
  }
  if (bAppContextCreated)
  {
    bAppContextCreated = RT_FALSE;
    if (!ZzFreeAppContext(&rtAppContext) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  return (RtMainWithBoolean(nArgC, lpArgV) ? 0 : 1);
}
