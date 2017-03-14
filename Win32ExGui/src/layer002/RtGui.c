#include "layer002/RtGui.h"

#include "layer000/RtWin32ExGuiOsDefines.h"
#include "layer001/RtImage.h"

RT_B RT_API RtInitCommonControls(RT_UN32 nClasses)
{
  INITCOMMONCONTROLSEX rtInitCommonControls;
  RT_B bResult;

  rtInitCommonControls.dwSize = sizeof(rtInitCommonControls);
  rtInitCommonControls.dwICC = nClasses;

  if (!InitCommonControlsEx(&rtInitCommonControls)) goto handle_error;
  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  /* InitCommonControlsEx might not call SetLastError. */
  RtSetLastError(RT_ERROR_FUNCTION_FAILED);
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_H RT_API RtGetInstance()
{
  return GetModuleHandle(NULL);
}

void RT_API RtQuitMessageLoop(RT_UN32 unExitCode)
{
  PostQuitMessage(unExitCode);
}

RT_B RT_API RtMoveWindow(RT_H hWindow, RT_GUI_RECT* lpRect)
{
  return MoveWindow(hWindow, lpRect->nX, lpRect->nY, lpRect->nWidth, lpRect->nHeight, TRUE);
}

RT_H RT_API RtCreateStaticWindow(RT_GUI_RECT* lpPosition, RT_H hParentWindow, RT_UN unControlId, RT_H hInstance)
{
  return CreateWindowEx(0, _R("Static"), _R(""),
                        WS_CHILD | WS_VISIBLE,
                        lpPosition->nX, lpPosition->nY,
                        lpPosition->nWidth, lpPosition->nHeight,
                        hParentWindow,
                        (HMENU)unControlId,
                        hInstance,
                        RT_NULL);
}

RT_UN16 RT_CALL RtDefaultMessageLoop()
{
  MSG message;

  while (GetMessage(&message, NULL, 0, 0))
  {
    /* Traduce some messages. */
    TranslateMessage(&message);

    /* Dispatch messages to the windows. */
    DispatchMessage(&message);
  }

  return (RT_UN16)message.wParam;
}

RT_H RT_API RtCreateToolBar(RT_UN unMenuItemsCount, RT_GUI_MENU_ITEM* lpMenuItems[], RT_GUI_TB_BUTTON* lpButtons, RT_H hImageList, RT_H hParentWindow, RT_UN unControlId, RT_H hInstance)
{
  RT_UN unI;
  RT_H hResult;


  hResult = CreateWindowEx(0,                             /* Extended style. */
                            TOOLBARCLASSNAME,             /* Class name. */
                            _R("ToolBar"),                /* Window name. */
                            WS_CHILD | WS_VISIBLE,        /* Style. */
                            0, 0, 0, 0,
                            hParentWindow,                /* Parent. */
                            (HMENU)unControlId,            /* Control id. */
                            hInstance,
                            RT_NULL);

  if (hResult)
  {
    for (unI = 0; unI < unMenuItemsCount; unI++)
    {
      switch (lpMenuItems[unI]->unType)
      {
        case RT_GUI_MENU_ITEM_TYPE_COMMAND:
          lpButtons[unI].iBitmap = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItems[unI])->unImageIndex;
          lpButtons[unI].idCommand = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItems[unI])->unId;
          lpButtons[unI].fsState = TBSTATE_ENABLED;
          lpButtons[unI].fsStyle = TBSTYLE_BUTTON;
          lpButtons[unI].dwData = 0;
          lpButtons[unI].iString = 0;
          break;
        case RT_GUI_MENU_ITEM_TYPE_SEPARATOR:
          lpButtons[unI].iBitmap = ((RT_GUI_SEPARATOR_MENU_ITEM*)lpMenuItems[unI])->nSeparatorSize;;
          lpButtons[unI].idCommand = 0;
          lpButtons[unI].fsState = TBSTATE_ENABLED;
          lpButtons[unI].fsStyle = BTNS_SEP;
          lpButtons[unI].dwData = 0;
          lpButtons[unI].iString = 0;
          break;
        default:
          SetLastError(RT_ERROR_BAD_ARGUMENTS);
          DestroyWindow(hResult);
          hResult = RT_NULL;
          goto the_end;
      }
    }

    SendMessage(hResult, TB_SETIMAGELIST, 0, (LPARAM)hImageList);

    /* Must be called before TB_ADDBUTTONS when we use CreateWindowEx. */
    SendMessage(hResult, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    if (!SendMessage(hResult, TB_ADDBUTTONS, (WPARAM)unMenuItemsCount, (LPARAM)lpButtons))
    {
      DestroyWindow(hResult);
      hResult = RT_NULL;
    }
  }

the_end:
  return hResult;
}

RT_B RT_API RtCreateGuiCommandMenuItemManager(RT_GUI_COMMAND_MENU_ITEM_MANAGER* lpGuiCommandMenuItemManager, RT_GUI_COMMAND_MENU_ITEM* lpCommandMenuItems, RT_UN unCommandMenuItemsCount, RT_H hInstance)
{
  RT_H hDc;
  RT_UN unIconsCount;
  RT_UN32 unImageIndex;
  RT_H hTemporaryToolBar;
  RT_UN32 unSystemImageList;
  RT_UN32 lpSystemImageListIndex[3];
  RT_UN32 unMajorOsVersion;
  RT_UN32 lpMaskBuffer[256];
  RT_UN32 lpColorBuffer[256];
  RT_UN unI;
  RT_B bResult;

  /* Used to load system images. */
  hTemporaryToolBar = RT_NULL;

  hDc = RT_NULL;



  lpGuiCommandMenuItemManager->lpCommandMenuItems = lpCommandMenuItems;
  lpGuiCommandMenuItemManager->unCommandMenuItemsCount = unCommandMenuItemsCount;

  /* Count icons. The result is not accurate as the user can use system image lists, but it will help for initial image list size. */
  unIconsCount = 0;
  for (unI = 0; unI < unCommandMenuItemsCount; unI++)
  {
    if (lpCommandMenuItems[unI].hIcon)
    {
      unIconsCount++;
    }
    lpCommandMenuItems[unI].hBitmap = RT_NULL;
  }

  if (!RtGetOsVersion(&unMajorOsVersion, RT_NULL, RT_NULL)) goto handle_error;

  lpGuiCommandMenuItemManager->hToolBarImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, (int)unIconsCount, 8);
  if (!lpGuiCommandMenuItemManager->hToolBarImageList) goto handle_error;

  /* By default, there is no system image list loaded. */
  for (unI = 0; unI < 3; unI++)
  {
    lpSystemImageListIndex[unI] = RT_TYPE_MAX_UN;
  }

  /* Loop on command menu items. */
  unImageIndex = 0;
  for (unI = 0; unI < unCommandMenuItemsCount; unI++)
  {
    if (lpCommandMenuItems[unI].hIcon)
    {
      /* The user has provided an icon. */
      if (ImageList_AddIcon(lpGuiCommandMenuItemManager->hToolBarImageList, lpCommandMenuItems[unI].hIcon) == -1)
      {
        goto handle_error;
      }
      lpCommandMenuItems[unI].unImageIndex = unImageIndex;
      unImageIndex++;
    }
    else if (lpCommandMenuItems[unI].unSystemImageList != RT_TYPE_MAX_UN)
    {
      /* The user want to use an icon from a system image list. */
      switch (lpCommandMenuItems[unI].unSystemImageList)
      {
        case IDB_STD_SMALL_COLOR:
          unSystemImageList = 0;
          break;
        case IDB_VIEW_SMALL_COLOR:
          unSystemImageList = 1;
          break;
        case IDB_HIST_SMALL_COLOR:
          unSystemImageList = 2;
          break;
        default:
          unSystemImageList = RT_TYPE_MAX_UN;
      }
      if (unSystemImageList == RT_TYPE_MAX_UN)
      {
        RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
        goto handle_error;
      }
      /* If the system list is not already loaded in the  image list. */
      if (lpSystemImageListIndex[unSystemImageList] == RT_TYPE_MAX_UN)
      {
        /* We need a temporary toolbar to load system images in the image list. */
        if (!hTemporaryToolBar)
        {
          hTemporaryToolBar = CreateWindowEx(0,       /* Extended style. */
                          TOOLBARCLASSNAME,           /* Class name. */
                          _R("TemporaryToolBar"),     /* Window name. */
                          0,                          /* Style. */
                          0, 0, 0, 0,
                          RT_NULL,                     /* Parent. */
                          RT_NULL,                     /* Control id. */
                          hInstance,
                          RT_NULL);
          if (!hTemporaryToolBar)
          {
            goto handle_error;
          }
          /* We use our main image list as temporary image list. */
          SendMessage(hTemporaryToolBar, TB_SETIMAGELIST, 0, (LPARAM)lpGuiCommandMenuItemManager->hToolBarImageList);
        }
        SendMessage(hTemporaryToolBar, TB_LOADIMAGES, (WPARAM)lpCommandMenuItems[unI].unSystemImageList, (LPARAM)HINST_COMMCTRL);
        lpSystemImageListIndex[unSystemImageList] = unImageIndex;
        unImageIndex = ImageList_GetImageCount(lpGuiCommandMenuItemManager->hToolBarImageList);
      }
      lpCommandMenuItems[unI].unImageIndex = lpSystemImageListIndex[unSystemImageList] + lpCommandMenuItems[unI].unSystemImageIndex;

      /* Create the menu icon from the image list. */
      lpCommandMenuItems[unI].hIcon = ImageList_ExtractIcon(0, lpGuiCommandMenuItemManager->hToolBarImageList, lpCommandMenuItems[unI].unImageIndex);
      if (!lpCommandMenuItems[unI].hIcon)
      {
        goto handle_error;
      }
    }
    else
    {
      lpCommandMenuItems[unI].unImageIndex = RT_TYPE_MAX_UN;
    }

    /* Under vista, we must use a PARGB bitmap for menus. */
    if (unMajorOsVersion >= 6)
    {
      if (lpCommandMenuItems[unI].hIcon)
      {
        /* Get a reference DC. */
        if (!hDc)
        {
          hDc = GetDC(RT_NULL);
          if (!hDc) goto handle_error;
        }
        /* Create a PARG bitmap from the icon. */
        lpCommandMenuItems[unI].hBitmap = RtConvertIconToPargbBitmap(lpCommandMenuItems[unI].hIcon, 16, 16, lpMaskBuffer, lpColorBuffer, hDc);
        if (!lpCommandMenuItems[unI].hBitmap) goto handle_error;
      }
    }
  }

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  /* Free allocated content in case of error. */
  if (lpGuiCommandMenuItemManager->hToolBarImageList)
  {
    ImageList_Destroy(lpGuiCommandMenuItemManager->hToolBarImageList);
  }
  /* Free allocated icons and bitmaps. */
  for (unI = 0; unI < unCommandMenuItemsCount; unI++)
  {
    if (lpCommandMenuItems[unI].unSystemImageIndex != RT_TYPE_MAX_UN)
    {
      if (lpCommandMenuItems[unI].hIcon)
      {
        DestroyIcon(lpCommandMenuItems[unI].hIcon);
      }
    }
    if (lpCommandMenuItems[unI].hBitmap)
    {
      DeleteObject(lpCommandMenuItems[unI].hBitmap);
    }
  }
  bResult = RT_FAILURE;
free_resources:
  if (hDc)
  {
    if (!ReleaseDC(RT_NULL, hDc) && bResult)
    {
      hDc = RT_NULL;
      goto handle_error;
    }
    hDc = RT_NULL;
  }
  if (hTemporaryToolBar)
  {
    if (!DestroyWindow(hTemporaryToolBar) && bResult)
    {
      hTemporaryToolBar = RT_NULL;
      goto handle_error;
    }
    hTemporaryToolBar = RT_NULL;
  }

  return bResult;
}

RT_B RT_API RtFreeGuiCommandMenuItemManager(RT_GUI_COMMAND_MENU_ITEM_MANAGER* lpGuiCommandMenuItemManager)
{
  RT_GUI_COMMAND_MENU_ITEM* lpGuiCommandMenuItem;
  RT_UN unI;
  RT_B bResult;

  bResult = RT_SUCCESS;

  for (unI = 0; unI < lpGuiCommandMenuItemManager->unCommandMenuItemsCount; unI++)
  {
    lpGuiCommandMenuItem = &lpGuiCommandMenuItemManager->lpCommandMenuItems[unI];

    /* Free icons generated from system image lists. */
    if (lpGuiCommandMenuItem->unSystemImageIndex != RT_TYPE_MAX_UN)
    {
      if (!DestroyIcon(lpGuiCommandMenuItem->hIcon)) bResult = RT_FAILURE;
    }
    if (lpGuiCommandMenuItem->hBitmap)
    {
      if (!DeleteObject(lpGuiCommandMenuItem->hBitmap)) bResult = RT_FAILURE;
    }
  }

  /* Free the image list. */
  if (!ImageList_Destroy(lpGuiCommandMenuItemManager->hToolBarImageList)) bResult = RT_FAILURE;

  return bResult;
}

RT_H RT_API RtCreateMenu(RT_UN unMenuItemsCount, RT_GUI_MENU_ITEM* lpMenuItems[])
{
  MENUITEMINFO menuItemInfo;
  RT_GUI_MENU_ITEM* lpMenuItem;
  RT_UN unMajorOsVersion;
  RT_UN unI;
  RT_H hResult;

  if (RtGetOsVersion(&unMajorOsVersion, RT_NULL, RT_NULL))
  {
    hResult = CreatePopupMenu();
    if (hResult)
    {
      /* Prepare MENUITEMINFO structure. */
      RT_MEMORY_ZERO(&menuItemInfo, sizeof(MENUITEMINFO));
      menuItemInfo.cbSize = sizeof(MENUITEMINFO);

      for (unI = 0; unI < unMenuItemsCount; unI++)
      {
        lpMenuItem = lpMenuItems[unI];
        switch (lpMenuItem->unType)
        {
          case RT_GUI_MENU_ITEM_TYPE_COMMAND:
            menuItemInfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_DATA | MIIM_STRING;
            menuItemInfo.fType = MFT_STRING;
            menuItemInfo.wID = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->unId;
            menuItemInfo.dwTypeData = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->lpText;
            menuItemInfo.cch = (UINT)RtGetStringSize(((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->lpText);

            if (((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->unCommandType == RT_GUI_COMMAND_MENU_ITEM_TYPE_BUTTON)
            {
              if (((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->hIcon)
              {
                menuItemInfo.fMask |= MIIM_BITMAP;
                if (unMajorOsVersion >= 6)
                {
                  /* Use PARGB bitmap under Vista (HBMMENU_CALLBACK is not compatible with fully styled menus). */
                  menuItemInfo.hbmpItem = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->hBitmap;
                }
                else
                {
                  /* Use HBMMENU_CALLBACK under XP. */
                  menuItemInfo.hbmpItem = HBMMENU_CALLBACK;
                }
              }
            }

            break;
          case RT_GUI_MENU_ITEM_TYPE_SUB_MENU:
            menuItemInfo.fMask = MIIM_TYPE | MIIM_SUBMENU;
            menuItemInfo.fType = MFT_STRING;
            menuItemInfo.hSubMenu = ((RT_GUI_SUB_MENU_MENU_ITEM*)lpMenuItem)->hSubMenu;
            menuItemInfo.dwTypeData = ((RT_GUI_SUB_MENU_MENU_ITEM*)lpMenuItem)->lpText;
            menuItemInfo.cch = (UINT)RtGetStringSize(((RT_GUI_SUB_MENU_MENU_ITEM*)lpMenuItem)->lpText);
            break;
          case RT_GUI_MENU_ITEM_TYPE_SEPARATOR:
            menuItemInfo.fMask = MIIM_TYPE;
            menuItemInfo.fType = MFT_SEPARATOR;
            break;
        }
        if (!InsertMenuItem(hResult, (UINT)unI, TRUE, &menuItemInfo))
        {
          DestroyMenu(hResult);
          hResult = RT_NULL;
          break;
        }
      }
    }
  }
  else
  {
    hResult = RT_NULL;
  }
  return hResult;
}

RT_H RT_API RtCreateMenuBar(RT_UN unMenusCount, RT_CHAR* lpTexts[], RT_H* hMenus)
{
  MENUITEMINFO menuItemInfo;
  RT_CHAR* lpText;
  RT_UN unI;
  RT_H hResult;

  hResult = CreateMenu();
  if (hResult)
  {
    /* Prepare MENUITEMINFO structure. */
    menuItemInfo.cbSize = sizeof(MENUITEMINFO);
    menuItemInfo.fMask = MIIM_TYPE | MIIM_SUBMENU;
    menuItemInfo.fType = MFT_STRING;

    for (unI = 0; unI < unMenusCount; unI++)
    {
      lpText = lpTexts[unI];

      /* Fill MENUITEMINFO as sub menu with text. */
      menuItemInfo.hSubMenu = hMenus[unI];
      menuItemInfo.dwTypeData = lpText;
      menuItemInfo.cch = (UINT)RtGetStringSize(lpText);

      if (!InsertMenuItem(hResult, (UINT)unI, TRUE, &menuItemInfo))
      {
        DestroyMenu(hResult);
        hResult = RT_NULL;
        break;
      }
    }
  }

  return hResult;
}

RT_H RT_API RtCreateStatusBar(RT_B bSimple, RT_CHAR* lpText, RT_H hParentWindow, RT_UN unControlId, RT_H hInstance)
{
  RT_H hResult;

  hResult = CreateWindowEx(0,                                        /* No extended style.            */
                           STATUSCLASSNAME,                          /* Status bar class name.        */
                           lpText,                                   /* Initial text.                 */
                           WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,   /* Grip at the right.            */
                           0, 0, 0, 0,                               /* Position and size.            */
                           hParentWindow,                            /* Parent window.                */
                           (HMENU)unControlId,                        /* Child window identifier.      */
                           hInstance,                                /* Instance.                     */
                           RT_NULL);                                 /* No window creation data.      */

  if (hResult)
  {
    if (bSimple)
    {
      SendMessage(hResult, SB_SIMPLE, TRUE, 0);
      RtSetSimpleStatusBarText(hResult, lpText);
    }
    else
    {
      SendMessage(hResult, SB_SIMPLE, FALSE, 0);
    }
  }

  return hResult;
}

void RT_API RtSetSimpleStatusBarText(RT_H hStatusBar, RT_CHAR* lpText)
{
  /* 255 must be used for simple status bars. */
  SendMessage(hStatusBar, SB_SETTEXT, 255, (LPARAM)lpText);
}

RT_H RT_API RtCreateMainWindow(RT_CHAR* lpText, RT_CHAR* lpClassName, RT_UN32 unStyle, RT_UN32 unExStyle, RT_GUI_RECT* lpRect, RT_WINDOW_PROC_CALLBACK lpWindowProc, RT_H hIcon, RT_H hMenuBar, void* lpUserData, RT_H hInstance)
{
  WNDCLASSEX windowClass;
  RT_H hResult;

  hResult = RT_NULL;

  /* Use default icon if hIcon is RT_NULL. */
  if (!hIcon)
  {
    hIcon = LoadIcon(NULL, IDI_APPLICATION);
  }

  /* Window class. */
  windowClass.cbSize = sizeof(WNDCLASSEX);
  windowClass.style = CS_OWNDC;
  windowClass.lpfnWndProc = (WNDPROC)lpWindowProc;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = hIcon;
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
  windowClass.lpszMenuName = RT_NULL;
  windowClass.lpszClassName = lpClassName;
  windowClass.hIconSm = NULL;

  /* Window class registering. */
  if (!RegisterClassEx(&windowClass)) goto handle_error;

  /* Window creation. */
  hResult = CreateWindowEx(unExStyle,
                           lpClassName,
                           lpText,          /* Window title. */
                           unStyle,
                           lpRect->nX,      /* Default X. */
                           lpRect->nY,      /* Not used when X is CW_USEDEFAULT. */
                           lpRect->nWidth,
                           lpRect->nHeight,
                           HWND_DESKTOP,    /* Parent Window.  */
                           hMenuBar,        /* Menu. */
                           hInstance,       /* Application instance. */
                           NULL);           /* lParam of WM_CREATE, used mainly for MDI interfaces. */

  if (!hResult) goto handle_error;

  /* Set user data that can be retrieved later on from window proc. */
  if (!RtSetWindowUserData(hResult, lpUserData)) goto handle_error;

  goto free_resources;
handle_error:
  if (hResult)
  {
    DestroyWindow(hResult);
    hResult = RT_NULL;
  }
free_resources:
  return hResult;
 }

void RT_API RtCenterWindow(RT_H hWindow, int nWidth, int nHeight, RT_H hParentWindow)
{
  RECT parentRect;
  RT_N32 nX;
  RT_N32 nY;

  if (GetWindowRect(hParentWindow, &parentRect))
  {
    nX = (parentRect.left + parentRect.right) / 2 - nWidth / 2;
    if (nX < parentRect.left) nX = parentRect.left;
    nY = (parentRect.top + parentRect.bottom) / 2 - nHeight / 2;
    if (nY < parentRect.top) nY = parentRect.top;
  }
  else
  {
    nX = CW_USEDEFAULT;
    nY = 0; /* Not used when x is CW_USEDEFAULT. */
  }

  SetWindowPos(hWindow, RT_NULL, nX, nY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

RT_H RT_API RtCreateDialogBox(RT_CHAR* lpText, RT_CHAR* lpClassName, RT_B bSizable, RT_B bClosable, RT_N32 nWidth, RT_N32 nHeight, RT_WINDOW_PROC_CALLBACK lpWindowProc, void* lpUserData, RT_H hParentWindow, RT_H hInstance)
{
  WNDCLASSEX windowClass;
  RT_UN32 unStyle;
  HMENU hSystemMenu;
  RT_B bUpdateSystemMenu;
  RT_H hResult;

  hResult = RT_NULL;

  /* Window class. */
  windowClass.cbSize = sizeof(WNDCLASSEX);
  windowClass.style = CS_OWNDC;
  windowClass.lpfnWndProc = (WNDPROC)lpWindowProc;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = RT_NULL;
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
  windowClass.lpszMenuName = RT_NULL;
  windowClass.lpszClassName = lpClassName;
  windowClass.hIconSm = RT_NULL;

  if (bSizable)
  {
    if (bClosable)
    {
      /* All but minimize button. */
      unStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MAXIMIZEBOX;
      bUpdateSystemMenu = RT_FALSE;
    }
    else
    {
      /* All but minimize button and deactivate the red cross (Done by removing close from system menu). */
      unStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MAXIMIZEBOX;
      bUpdateSystemMenu = RT_TRUE;
    }
  }
  else
  {
    if (bClosable)
    {
      /* Only the red cross. */
      unStyle = WS_SYSMENU | WS_CAPTION;
      bUpdateSystemMenu = RT_TRUE;
    }
    else
    {
      /* No window menu, no buttons, only the title. */
      unStyle = 0;
      bUpdateSystemMenu = RT_FALSE;
    }
  }

  /* Window class registering. */
  if (!RegisterClassEx(&windowClass)) goto handle_error;

  /* Window creation. */
  hResult = CreateWindowEx(WS_EX_DLGMODALFRAME,
                           lpClassName,
                           lpText,        /* Window title. */
                           unStyle,
                           CW_USEDEFAULT,
                           0,
                           nWidth,
                           nHeight,
                           hParentWindow, /* Parent Window.  */
                           RT_NULL,       /* No menu. */
                           hInstance,     /* Application instance. */
                           NULL);         /* lParam of WM_CREATE, used mainly for MDI interfaces. */
  if (!hResult) goto handle_error;

  /* Set user data that can be retrieved later on from window proc. */
  if (!RtSetWindowUserData(hResult, lpUserData)) goto handle_error;

  RtCenterWindow(hResult, nWidth, nHeight, hParentWindow);

  /* Update system menu. */
  if (bUpdateSystemMenu)
  {
    hSystemMenu = GetSystemMenu(hResult, FALSE);
    if (!bSizable)
    {
      /* Window is closable but not sizable. */
      RemoveMenu(hSystemMenu, 4, MF_BYPOSITION); /* Maximize. */
      RemoveMenu(hSystemMenu, 3, MF_BYPOSITION); /* Reduce. */
      RemoveMenu(hSystemMenu, 2, MF_BYPOSITION); /* Size. */
      RemoveMenu(hSystemMenu, 0, MF_BYPOSITION); /* Restore. */
    }
    else
    {
       /* Window is sizable but not closable. */
      RemoveMenu(hSystemMenu, 6, MF_BYPOSITION); /* Close. */
      RemoveMenu(hSystemMenu, 5, MF_BYPOSITION); /* Separator. */
      RemoveMenu(hSystemMenu, 3, MF_BYPOSITION); /* Reduce. */
    }
  }

  goto free_resources;
handle_error:
  if (hResult)
  {
    DestroyWindow(hResult);
    hResult = RT_NULL;
  }
free_resources:
  return hResult;
}

RT_B RT_API RtGetWindowSize(RT_H hWindow, RT_GUI_RECT* lpRect)
{
  RT_B bResult;

  bResult = GetWindowRect(hWindow, (RECT*)lpRect);
  if (bResult)
  {
    lpRect->nWidth = lpRect->nWidth - lpRect->nX;
    lpRect->nHeight = lpRect->nHeight - lpRect->nY;
    lpRect->nX = 0;
    lpRect->nY = 0;
  }
  return bResult;
}

void* RT_API RtGetWindowUserData(RT_H hWindow)
{
  return RT_TYPE_MAKE_POINTER(GetWindowLongPtr(hWindow, GWLP_USERDATA));
}

RT_B RT_API RtSetWindowUserData(RT_H hWindow, void* lpUserData)
{
  ULONG_PTR nReturnedValue;
  RT_B bResult;

  /* SetWindowLongPtr returns the previous value and 0 in case of issue. But the previous value can be 0. So there is a check on last error. */
  SetLastError(0);
  nReturnedValue = SetWindowLongPtr(hWindow, GWLP_USERDATA, RT_TYPE_MAKE_UINTEGER(lpUserData));
  if (!nReturnedValue && GetLastError())
  {
    bResult = RT_FAILURE;
  }
  else
  {
    bResult = RT_SUCCESS;
  }
  return bResult;
}
