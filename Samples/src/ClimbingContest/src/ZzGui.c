#include "ZzGui.h"

#include "ZzResources.h"
#include "ZzStrings.h"
#include "ZzWindows.h"

RT_GUI_RECT* RT_CALL ZzComputeVerticalSplitterPosition(RT_GUI_RECT* lpRect, ZZ_APP_CONTEXT* lpAppContext)
{
  RT_GUI_RECT rtRect;

  RtGetWindowSize(lpAppContext->hLeftTab, &rtRect);
  lpRect->nX = rtRect.nWidth; /* After tree view. */
  lpRect->nY = 0;
  lpRect->nWidth = ZZ_RESOURCES_SPLITTER_SIZE; /* Fixed width. */

  /* Available height in main window. */
  GetClientRect(lpAppContext->hMainWindow, (RECT*)&rtRect);
  lpRect->nHeight = rtRect.nHeight;

  /* Remove status bar height. */
  RtGetWindowSize(lpAppContext->hStatusBar, &rtRect);
  lpRect->nHeight = lpRect->nHeight - rtRect.nHeight;

  return lpRect;
}

RT_GUI_RECT* RT_CALL ZzComputeListBoxPosition(RT_GUI_RECT* lpRect, ZZ_APP_CONTEXT* lpAppContext)
{
  RT_GUI_RECT rtRect;

  /* We base size on tab control client area. */
  GetClientRect(lpAppContext->hLeftTab, (RECT*)&rtRect);

  /* Skeep tabs. */
  rtRect.nY += ZZ_GUI_TAB_HEIGHT;

  lpRect->nX = rtRect.nX + ZZ_GUI_BORDER;
  lpRect->nY = rtRect.nY + ZZ_GUI_BORDER * 2 + ZZ_GUI_BUTTON_HEIGHT;
  lpRect->nWidth = rtRect.nWidth - rtRect.nX - ZZ_GUI_BORDER * 2;
  lpRect->nHeight = rtRect.nHeight - rtRect.nY - ZZ_GUI_BORDER * 3 - ZZ_GUI_BUTTON_HEIGHT;

  return lpRect;
}

RT_GUI_RECT* RT_CALL ZzComputeLeftTabPosition(RT_GUI_RECT* lpRect, ZZ_APP_CONTEXT* lpAppContext)
{
  RT_GUI_RECT rtRect;
  RECT rtRect2;

  /* Top-left aligned. */
  lpRect->nX = 0;
  lpRect->nY = 0;

  /* Width depends of vertical splitter. */
  lpRect->nWidth = lpAppContext->nVerticalSplitterX;

 /* Available height in main window. */
  GetClientRect(lpAppContext->hMainWindow, (RECT*)&rtRect);
  GetClientRect(lpAppContext->hMainWindow, &rtRect2);
  lpRect->nHeight = rtRect.nHeight;
  if (lpAppContext->nVerticalSplitterX < ZZ_RESOURCES_MIN_LEFT_TAB_WIDTH)
  {
    lpAppContext->nVerticalSplitterX = ZZ_RESOURCES_MIN_LEFT_TAB_WIDTH;
  }
  else if (lpAppContext->nVerticalSplitterX > rtRect.nWidth - ZZ_RESOURCES_SPLITTER_SIZE)
  {
    lpAppContext->nVerticalSplitterX = rtRect.nWidth - ZZ_RESOURCES_SPLITTER_SIZE;
  }
  lpRect->nWidth = lpAppContext->nVerticalSplitterX;

  /* Remove status bar height. */
  RtGetWindowSize(lpAppContext->hStatusBar, &rtRect);
  lpRect->nHeight = lpRect->nHeight - rtRect.nHeight;

  return lpRect;
}

RT_H RT_CALL ZzCreateListBox(RT_GUI_RECT* lpPosition, RT_CHAR* lpName, RT_UN unControlId, RT_H hParentWindow, RT_H hInstance, RT_H hFont)
{
  RT_H hResult;

  hResult = CreateWindowEx(0,                                         /* ExStyle. */
                           _R("LISTBOX"),                             /* List box class name. */
                           lpName,                                    /* Window name. */
                           WS_VISIBLE | WS_CHILD |                    /* Style. */
                           WS_VSCROLL | WS_BORDER |
                           LBS_NOTIFY | LBS_DISABLENOSCROLL |
                           LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT,
                           lpPosition->nX, lpPosition->nY,            /* Position. */
                           lpPosition->nWidth, lpPosition->nHeight,   /* Size. */
                           hParentWindow,                             /* Parent Window. */
                           (HMENU)unControlId,                         /* Control id. */
                           hInstance,                                 /* Application instance. */
                           RT_NULL);

  if (!hResult) goto handle_error;

  /* Cannot check errors. */
  SendMessage(hResult, WM_SETFONT, (WPARAM)hFont, TRUE);

/*
LBN_SELCHANGE
*/

free_resources:
  return hResult;

handle_error:
  if (hResult)
  {
    DestroyWindow(hResult);
    hResult = RT_NULL;
  }
  goto free_resources;
}

RT_H RT_CALL ZzCreateButton(RT_GUI_RECT* lpPosition, RT_CHAR* lpText, RT_UN unControlId, RT_H hParentWindow, RT_H hInstance, RT_H hFont)
{
  DWORD unStyle;
  RT_H hResult;

  hResult = CreateWindowEx(0,                                         /* ExStyle. */
                           _R("BUTTON"),                              /* Button class name. */
                           lpText,                                    /* Window name. */
                           WS_VISIBLE | WS_CHILD |                    /* Style. */
                           BS_PUSHBUTTON | BS_TEXT |
                           BS_CENTER | BS_VCENTER | BS_NOTIFY,
                           lpPosition->nX, lpPosition->nY,            /* Position. */
                           lpPosition->nWidth, lpPosition->nHeight,   /* Size. */
                           hParentWindow,                             /* Parent Window. */
                           (HMENU)unControlId,               /* Control id. */
                           hInstance,                                 /* Application instance. */
                           RT_NULL);

  if (!hResult) goto handle_error;

  /* Cannot check errors. */
  SendMessage(hResult, WM_SETFONT, (WPARAM)hFont, TRUE);

  /* Remove CS_DBLCLKS style to not care about double clicks. */
  unStyle = GetClassLong(hResult, GCL_STYLE);
  if (!unStyle) goto handle_error;
  unStyle &= (~CS_DBLCLKS);
  if (!SetClassLong(hResult, GCL_STYLE, unStyle)) goto handle_error;

free_resources:
  return hResult;

handle_error:
  if (hResult)
  {
    DestroyWindow(hResult);
    hResult = RT_NULL;
  }
  goto free_resources;
}

/**
 * Forward left tab messages to main window.
 */
RT_N RT_CALL ZzLeftTabSubclassProc(HWND hWindow, UINT unMsg, WPARAM unWParam, LPARAM nLParam, UINT_PTR unIdSubclass, DWORD_PTR unRefData)
{
  RT_H hParent;

  switch (unMsg)
  {
    case WM_COMMAND:
      hParent = GetParent(hWindow);
      SendMessage(hParent, unMsg, unWParam, nLParam);
      break;
  }
  return DefSubclassProc(hWindow, unMsg, unWParam, nLParam);
}

RT_H RT_CALL ZzCreateLeftTab(RT_GUI_RECT* lpPosition, RT_H hMainWindow, RT_H hInstance, RT_H hFont)
{
  TC_ITEM rtItem;
  RT_H hResult;

  hResult = CreateWindowEx(0,                                         /* ExStyle. */
                           WC_TABCONTROL,                             /* Tab class name. */
                           _R("LeftTab"),                             /* Window name. */
                           WS_VISIBLE | WS_CHILD |                    /* Style. */
                           TCS_FIXEDWIDTH,
                           lpPosition->nX, lpPosition->nY,            /* Position. */
                           lpPosition->nWidth, lpPosition->nHeight,   /* Size. */
                           hMainWindow,                               /* Parent Window. */
                           (HMENU)ZZ_RESOURCES_LEFT_TAB_CTRL_ID,      /* Control id. */
                           hInstance,                                 /* Application instance. */
                           RT_NULL);

  if (!hResult) goto handle_error;

  /* Cannot check errors. */
  TabCtrl_SetItemSize(hResult, 60, ZZ_GUI_TAB_HEIGHT);
  /* Cannot check errors. */
  SendMessage(hResult, WM_SETFONT, (WPARAM)hFont, TRUE);

  rtItem.mask = TCIF_TEXT | TCIF_PARAM;

  rtItem.pszText = ZzGetString(ZZ_STRINGS_CLIMBERS);
  rtItem.lParam = ZZ_RESOURCES_CLIMBERS;
  if (SendMessage(hResult, TCM_INSERTITEM, 1, (LPARAM)&rtItem) == -1) goto handle_error;

  rtItem.pszText = ZzGetString(ZZ_STRINGS_ROUTES);
  rtItem.lParam = ZZ_RESOURCES_ROUTES;
  if (SendMessage(hResult, TCM_INSERTITEM, 2, (LPARAM)&rtItem) == -1) goto handle_error;

  rtItem.pszText = ZzGetString(ZZ_STRINGS_GRADES);
  rtItem.lParam = ZZ_RESOURCES_GRADES;
  if (SendMessage(hResult, TCM_INSERTITEM, 3, (LPARAM)&rtItem) == -1) goto handle_error;

  if (!SetWindowSubclass(hResult, &ZzLeftTabSubclassProc, 0, 0)) goto handle_error;

free_resources:
  return hResult;

handle_error:
  if (hResult)
  {
    DestroyWindow(hResult);
    hResult = RT_NULL;
  }
  goto free_resources;
}
