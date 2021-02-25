#include "layer002/RtGui.h"

#include "layer000/RtWin32ExGuiOsDefines.h"
#include "layer001/RtImage.h"

rt_s RtInitCommonControls(rt_un32 nClasses)
{
	INITCOMMONCONTROLSEX rtInitCommonControls;
	rt_s ret;

	rtInitCommonControls.dwSize = sizeof(rtInitCommonControls);
	rtInitCommonControls.dwICC = nClasses;

	if (!InitCommonControlsEx(&rtInitCommonControls)) goto error;
	ret = RT_OK;
free:
	return ret;

error:
	/* InitCommonControlsEx might not call SetLastError. */
	rt_error_set_last(RT_ERROR_FUNCTION_FAILED);
	ret = RT_FAILED;
	goto free;
}

rt_h RtGetInstance()
{
	return GetModuleHandle(NULL);
}

void RtQuitMessageLoop(rt_un32 exit_code)
{
	PostQuitMessage(exit_code);
}

rt_s RtMoveWindow(rt_h hWindow, RT_GUI_RECT *lpRect)
{
	return MoveWindow(hWindow, lpRect->nX, lpRect->nY, lpRect->nWidth, lpRect->nHeight, TRUE);
}

rt_h RtCreateStaticWindow(RT_GUI_RECT *lpPosition, rt_h hParentWindow, rt_un unControlId, rt_h hInstance)
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

rt_un16 RT_CALL RtDefaultMessageLoop()
{
	MSG message;

	while (GetMessage(&message, NULL, 0, 0)) {
		/* Traduce some messages. */
		TranslateMessage(&message);

		/* Dispatch messages to the windows. */
		DispatchMessage(&message);
	}

	return (rt_un16)message.wParam;
}

rt_h RtCreateToolBar(rt_un unMenuItemsCount, RT_GUI_MENU_ITEM *lpMenuItems[], RT_GUI_TB_BUTTON *lpButtons, rt_h hImageList, rt_h hParentWindow, rt_un unControlId, rt_h hInstance)
{
	rt_un i;
	rt_h result_handle;


	result_handle = CreateWindowEx(0,														 /* Extended style. */
														TOOLBARCLASSNAME,						 /* Class name. */
														_R("ToolBar"),								/* Window name. */
														WS_CHILD | WS_VISIBLE,				/* Style. */
														0, 0, 0, 0,
														hParentWindow,								/* Parent. */
														(HMENU)unControlId,						/* Control id. */
														hInstance,
														RT_NULL);

	if (result_handle) {
		for (i = 0; i < unMenuItemsCount; i++) {
			switch (lpMenuItems[i]->type)
			{
				case RT_GUI_MENU_ITEM_TYPE_COMMAND:
					lpButtons[i].iBitmap = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItems[i])->unImageIndex;
					lpButtons[i].idCommand = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItems[i])->unId;
					lpButtons[i].fsState = TBSTATE_ENABLED;
					lpButtons[i].fsStyle = TBSTYLE_BUTTON;
					lpButtons[i].dwData = 0;
					lpButtons[i].iString = 0;
					break;
				case RT_GUI_MENU_ITEM_TYPE_SEPARATOR:
					lpButtons[i].iBitmap = ((RT_GUI_SEPARATOR_MENU_ITEM*)lpMenuItems[i])->nSeparatorSize;;
					lpButtons[i].idCommand = 0;
					lpButtons[i].fsState = TBSTATE_ENABLED;
					lpButtons[i].fsStyle = BTNS_SEP;
					lpButtons[i].dwData = 0;
					lpButtons[i].iString = 0;
					break;
				default:
					SetLastError(RT_ERROR_BAD_ARGUMENTS);
					DestroyWindow(result_handle);
					result_handle = RT_NULL;
					goto the_end;
			}
		}

		SendMessage(result_handle, TB_SETIMAGELIST, 0, (LPARAM)hImageList);

		/* Must be called before TB_ADDBUTTONS when we use CreateWindowEx. */
		SendMessage(result_handle, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

		if (!SendMessage(result_handle, TB_ADDBUTTONS, (WPARAM)unMenuItemsCount, (LPARAM)lpButtons)) {
			DestroyWindow(result_handle);
			result_handle = RT_NULL;
		}
	}

the_end:
	return result_handle;
}

rt_s RtCreateGuiCommandMenuItemManager(RT_GUI_COMMAND_MENU_ITEM_MANAGER *lpGuiCommandMenuItemManager, RT_GUI_COMMAND_MENU_ITEM *lpCommandMenuItems, rt_un unCommandMenuItemsCount, rt_h hInstance)
{
	rt_h hDc;
	rt_un unIconsCount;
	rt_un32 unImageIndex;
	rt_h hTemporaryToolBar;
	rt_un32 unSystemImageList;
	rt_un32 lpSystemImageListIndex[3];
	rt_un32 unMajorOsVersion;
	rt_un32 lpMaskBuffer[256];
	rt_un32 lpColorBuffer[256];
	rt_un i;
	rt_s ret;

	/* Used to load system images. */
	hTemporaryToolBar = RT_NULL;

	hDc = RT_NULL;



	lpGuiCommandMenuItemManager->lpCommandMenuItems = lpCommandMenuItems;
	lpGuiCommandMenuItemManager->unCommandMenuItemsCount = unCommandMenuItemsCount;

	/* Count icons. The result is not accurate as the user can use system image lists, but it will help for initial image list size. */
	unIconsCount = 0;
	for (i = 0; i < unCommandMenuItemsCount; i++) {
		if (lpCommandMenuItems[i].hIcon) {
			unIconsCount++;
		}
		lpCommandMenuItems[i].hBitmap = RT_NULL;
	}

	if (!rt_os_version_get(&unMajorOsVersion, RT_NULL, RT_NULL)) goto error;

	lpGuiCommandMenuItemManager->hToolBarImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, (int)unIconsCount, 8);
	if (!lpGuiCommandMenuItemManager->hToolBarImageList) goto error;

	/* By default, there is no system image list loaded. */
	for (i = 0; i < 3; i++) {
		lpSystemImageListIndex[i] = RT_TYPE_MAX_UN;
	}

	/* Loop on command menu items. */
	unImageIndex = 0;
	for (i = 0; i < unCommandMenuItemsCount; i++) {
		if (lpCommandMenuItems[i].hIcon) {
			/* The user has provided an icon. */
			if (ImageList_AddIcon(lpGuiCommandMenuItemManager->hToolBarImageList, lpCommandMenuItems[i].hIcon) == -1) {
				goto error;
			}
			lpCommandMenuItems[i].unImageIndex = unImageIndex;
			unImageIndex++;
		} else if (lpCommandMenuItems[i].unSystemImageList != RT_TYPE_MAX_UN) {
			/* The user want to use an icon from a system image list. */
			switch (lpCommandMenuItems[i].unSystemImageList)
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
			if (unSystemImageList == RT_TYPE_MAX_UN) {
				rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
				goto error;
			}
			/* If the system list is not already loaded in the	image list. */
			if (lpSystemImageListIndex[unSystemImageList] == RT_TYPE_MAX_UN) {
				/* We need a temporary toolbar to load system images in the image list. */
				if (!hTemporaryToolBar) {
					hTemporaryToolBar = CreateWindowEx(0,			 /* Extended style. */
													TOOLBARCLASSNAME,					 /* Class name. */
													_R("TemporaryToolBar"),		 /* Window name. */
													0,													/* Style. */
													0, 0, 0, 0,
													RT_NULL,										 /* Parent. */
													RT_NULL,										 /* Control id. */
													hInstance,
													RT_NULL);
					if (!hTemporaryToolBar) {
						goto error;
					}
					/* We use our main image list as temporary image list. */
					SendMessage(hTemporaryToolBar, TB_SETIMAGELIST, 0, (LPARAM)lpGuiCommandMenuItemManager->hToolBarImageList);
				}
				SendMessage(hTemporaryToolBar, TB_LOADIMAGES, (WPARAM)lpCommandMenuItems[i].unSystemImageList, (LPARAM)HINST_COMMCTRL);
				lpSystemImageListIndex[unSystemImageList] = unImageIndex;
				unImageIndex = ImageList_GetImageCount(lpGuiCommandMenuItemManager->hToolBarImageList);
			}
			lpCommandMenuItems[i].unImageIndex = lpSystemImageListIndex[unSystemImageList] + lpCommandMenuItems[i].unSystemImageIndex;

			/* Create the menu icon from the image list. */
			lpCommandMenuItems[i].hIcon = ImageList_ExtractIcon(0, lpGuiCommandMenuItemManager->hToolBarImageList, lpCommandMenuItems[i].unImageIndex);
			if (!lpCommandMenuItems[i].hIcon) {
				goto error;
			}
		} else {
			lpCommandMenuItems[i].unImageIndex = RT_TYPE_MAX_UN;
		}

		/* Under vista, we must use a PARGB bitmap for menus. */
		if (unMajorOsVersion >= 6) {
			if (lpCommandMenuItems[i].hIcon) {
				/* Get a reference DC. */
				if (!hDc) {
					hDc = GetDC(RT_NULL);
					if (!hDc) goto error;
				}
				/* Create a PARG bitmap from the icon. */
				lpCommandMenuItems[i].hBitmap = RtConvertIconToPargbBitmap(lpCommandMenuItems[i].hIcon, 16, 16, lpMaskBuffer, lpColorBuffer, hDc);
				if (!lpCommandMenuItems[i].hBitmap) goto error;
			}
		}
	}

	ret = RT_OK;
	goto free;
error:
	/* Free allocated content in case of error. */
	if (lpGuiCommandMenuItemManager->hToolBarImageList) {
		ImageList_Destroy(lpGuiCommandMenuItemManager->hToolBarImageList);
	}
	/* Free allocated icons and bitmaps. */
	for (i = 0; i < unCommandMenuItemsCount; i++) {
		if (lpCommandMenuItems[i].unSystemImageIndex != RT_TYPE_MAX_UN) {
			if (lpCommandMenuItems[i].hIcon) {
				DestroyIcon(lpCommandMenuItems[i].hIcon);
			}
		}
		if (lpCommandMenuItems[i].hBitmap) {
			DeleteObject(lpCommandMenuItems[i].hBitmap);
		}
	}
	ret = RT_FAILED;
free:
	if (hDc) {
		if (!ReleaseDC(RT_NULL, hDc) && ret) {
			hDc = RT_NULL;
			goto error;
		}
		hDc = RT_NULL;
	}
	if (hTemporaryToolBar) {
		if (!DestroyWindow(hTemporaryToolBar) && ret) {
			hTemporaryToolBar = RT_NULL;
			goto error;
		}
		hTemporaryToolBar = RT_NULL;
	}

	return ret;
}

rt_s RtFreeGuiCommandMenuItemManager(RT_GUI_COMMAND_MENU_ITEM_MANAGER *lpGuiCommandMenuItemManager)
{
	RT_GUI_COMMAND_MENU_ITEM *lpGuiCommandMenuItem;
	rt_un i;
	rt_s ret;

	ret = RT_OK;

	for (i = 0; i < lpGuiCommandMenuItemManager->unCommandMenuItemsCount; i++) {
		lpGuiCommandMenuItem = &lpGuiCommandMenuItemManager->lpCommandMenuItems[i];

		/* Free icons generated from system image lists. */
		if (lpGuiCommandMenuItem->unSystemImageIndex != RT_TYPE_MAX_UN) {
			if (!DestroyIcon(lpGuiCommandMenuItem->hIcon)) ret = RT_FAILED;
		}
		if (lpGuiCommandMenuItem->hBitmap) {
			if (!DeleteObject(lpGuiCommandMenuItem->hBitmap)) ret = RT_FAILED;
		}
	}

	/* Free the image list. */
	if (!ImageList_Destroy(lpGuiCommandMenuItemManager->hToolBarImageList)) ret = RT_FAILED;

	return ret;
}

rt_h RtCreateMenu(rt_un unMenuItemsCount, RT_GUI_MENU_ITEM *lpMenuItems[])
{
	MENUITEMINFO menuItemInfo;
	RT_GUI_MENU_ITEM *lpMenuItem;
	rt_un unMajorOsVersion;
	rt_un i;
	rt_h result_handle;

	if (rt_os_version_get(&unMajorOsVersion, RT_NULL, RT_NULL)) {
		result_handle = CreatePopupMenu();
		if (result_handle) {
			/* Prepare MENUITEMINFO structure. */
			RT_MEMORY_ZERO(&menuItemInfo, sizeof(MENUITEMINFO));
			menuItemInfo.cbSize = sizeof(MENUITEMINFO);

			for (i = 0; i < unMenuItemsCount; i++) {
				lpMenuItem = lpMenuItems[i];
				switch (lpMenuItem->type)
				{
					case RT_GUI_MENU_ITEM_TYPE_COMMAND:
						menuItemInfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_DATA | MIIM_STRING;
						menuItemInfo.fType = MFT_STRING;
						menuItemInfo.wID = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->unId;
						menuItemInfo.dwTypeData = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->lpText;
						menuItemInfo.cch = (UINT)rt_char_GetCStringSize(((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->lpText);

						if (((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->unCommandType == RT_GUI_COMMAND_MENU_ITEM_TYPE_BUTTON) {
							if (((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->hIcon) {
								menuItemInfo.fMask |= MIIM_BITMAP;
								if (unMajorOsVersion >= 6) {
									/* Use PARGB bitmap under Vista (HBMMENU_CALLBACK is not compatible with fully styled menus). */
									menuItemInfo.hbmpItem = ((RT_GUI_COMMAND_MENU_ITEM*)lpMenuItem)->hBitmap;
								} else {
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
						menuItemInfo.cch = (UINT)rt_char_GetCStringSize(((RT_GUI_SUB_MENU_MENU_ITEM*)lpMenuItem)->lpText);
						break;
					case RT_GUI_MENU_ITEM_TYPE_SEPARATOR:
						menuItemInfo.fMask = MIIM_TYPE;
						menuItemInfo.fType = MFT_SEPARATOR;
						break;
				}
				if (!InsertMenuItem(result_handle, (UINT)i, TRUE, &menuItemInfo)) {
					DestroyMenu(result_handle);
					result_handle = RT_NULL;
					break;
				}
			}
		}
	} else {
		result_handle = RT_NULL;
	}
	return result_handle;
}

rt_h RtCreateMenuBar(rt_un unMenusCount, rt_char *lpTexts[], rt_h *hMenus)
{
	MENUITEMINFO menuItemInfo;
	rt_char *lpText;
	rt_un i;
	rt_h result_handle;

	result_handle = CreateMenu();
	if (result_handle) {
		/* Prepare MENUITEMINFO structure. */
		menuItemInfo.cbSize = sizeof(MENUITEMINFO);
		menuItemInfo.fMask = MIIM_TYPE | MIIM_SUBMENU;
		menuItemInfo.fType = MFT_STRING;

		for (i = 0; i < unMenusCount; i++) {
			lpText = lpTexts[i];

			/* Fill MENUITEMINFO as sub menu with text. */
			menuItemInfo.hSubMenu = hMenus[i];
			menuItemInfo.dwTypeData = lpText;
			menuItemInfo.cch = (UINT)rt_char_GetCStringSize(lpText);

			if (!InsertMenuItem(result_handle, (UINT)i, TRUE, &menuItemInfo)) {
				DestroyMenu(result_handle);
				result_handle = RT_NULL;
				break;
			}
		}
	}

	return result_handle;
}

rt_h RtCreateStatusBar(rt_b bSimple, rt_char *lpText, rt_h hParentWindow, rt_un unControlId, rt_h hInstance)
{
	rt_h result_handle;

	result_handle = CreateWindowEx(0,																				/* No extended style.						*/
													 STATUSCLASSNAME,													/* Status bar class name.				*/
													 lpText,																	 /* Initial text.								 */
													 WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,	 /* Grip at the right.						*/
													 0, 0, 0, 0,															 /* Position and size.						*/
													 hParentWindow,														/* Parent window.								*/
													 (HMENU)unControlId,												/* Child window identifier.			*/
													 hInstance,																/* Instance.										 */
													 RT_NULL);																 /* No window creation data.			*/

	if (result_handle) {
		if (bSimple) {
			SendMessage(result_handle, SB_SIMPLE, TRUE, 0);
			RtSetSimpleStatusBarText(result_handle, lpText);
		} else {
			SendMessage(result_handle, SB_SIMPLE, FALSE, 0);
		}
	}

	return result_handle;
}

void RtSetSimpleStatusBarText(rt_h hStatusBar, rt_char *lpText)
{
	/* 255 must be used for simple status bars. */
	SendMessage(hStatusBar, SB_SETTEXT, 255, (LPARAM)lpText);
}

rt_h RtCreateMainWindow(rt_char *lpText, rt_char *lpClassName, rt_un32 unStyle, rt_un32 unExStyle, RT_GUI_RECT *lpRect, RT_WINDOW_PROC_CALLBACK lpWindowProc, rt_h hIcon, rt_h hMenuBar, void *lpUserData, rt_h hInstance)
{
	WNDCLASSEX windowClass;
	rt_h result_handle;

	result_handle = RT_NULL;

	/* Use default icon if hIcon is RT_NULL. */
	if (!hIcon) {
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
	if (!RegisterClassEx(&windowClass)) goto error;

	/* Window creation. */
	result_handle = CreateWindowEx(unExStyle,
													 lpClassName,
													 lpText,					/* Window title. */
													 unStyle,
													 lpRect->nX,			/* Default X. */
													 lpRect->nY,			/* Not used when X is CW_USEDEFAULT. */
													 lpRect->nWidth,
													 lpRect->nHeight,
													 HWND_DESKTOP,		/* Parent Window.	*/
													 hMenuBar,				/* Menu. */
													 hInstance,			 /* Application instance. */
													 NULL);					 /* lParam of WM_CREATE, used mainly for MDI interfaces. */

	if (!result_handle) goto error;

	/* Set user data that can be retrieved later on from window proc. */
	if (!RtSetWindowUserData(result_handle, lpUserData)) goto error;

	goto free;
error:
	if (result_handle) {
		DestroyWindow(result_handle);
		result_handle = RT_NULL;
	}
free:
	return result_handle;
 }

void RtCenterWindow(rt_h hWindow, int nWidth, int nHeight, rt_h hParentWindow)
{
	RECT parentRect;
	rt_n32 nX;
	rt_n32 nY;

	if (GetWindowRect(hParentWindow, &parentRect)) {
		nX = (parentRect.left + parentRect.right) / 2 - nWidth / 2;
		if (nX < parentRect.left) nX = parentRect.left;
		nY = (parentRect.top + parentRect.bottom) / 2 - nHeight / 2;
		if (nY < parentRect.top) nY = parentRect.top;
	} else {
		nX = CW_USEDEFAULT;
		nY = 0; /* Not used when x is CW_USEDEFAULT. */
	}

	SetWindowPos(hWindow, RT_NULL, nX, nY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

rt_h RtCreateDialogBox(rt_char *lpText, rt_char *lpClassName, rt_b bSizable, rt_b bClosable, rt_n32 nWidth, rt_n32 nHeight, RT_WINDOW_PROC_CALLBACK lpWindowProc, void *lpUserData, rt_h hParentWindow, rt_h hInstance)
{
	WNDCLASSEX windowClass;
	rt_un32 unStyle;
	HMENU hSystemMenu;
	rt_b bUpdateSystemMenu;
	rt_h result_handle;

	result_handle = RT_NULL;

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

	if (bSizable) {
		if (bClosable) {
			/* All but minimize button. */
			unStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MAXIMIZEBOX;
			bUpdateSystemMenu = RT_FALSE;
		} else {
			/* All but minimize button and deactivate the red cross (Done by removing close from system menu). */
			unStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MAXIMIZEBOX;
			bUpdateSystemMenu = RT_TRUE;
		}
	} else {
		if (bClosable) {
			/* Only the red cross. */
			unStyle = WS_SYSMENU | WS_CAPTION;
			bUpdateSystemMenu = RT_TRUE;
		} else {
			/* No window menu, no buttons, only the title. */
			unStyle = 0;
			bUpdateSystemMenu = RT_FALSE;
		}
	}

	/* Window class registering. */
	if (!RegisterClassEx(&windowClass)) goto error;

	/* Window creation. */
	result_handle = CreateWindowEx(WS_EX_DLGMODALFRAME,
													 lpClassName,
													 lpText,				/* Window title. */
													 unStyle,
													 CW_USEDEFAULT,
													 0,
													 nWidth,
													 nHeight,
													 hParentWindow, /* Parent Window.	*/
													 RT_NULL,			 /* No menu. */
													 hInstance,		 /* Application instance. */
													 NULL);				 /* lParam of WM_CREATE, used mainly for MDI interfaces. */
	if (!result_handle) goto error;

	/* Set user data that can be retrieved later on from window proc. */
	if (!RtSetWindowUserData(result_handle, lpUserData)) goto error;

	RtCenterWindow(result_handle, nWidth, nHeight, hParentWindow);

	/* Update system menu. */
	if (bUpdateSystemMenu) {
		hSystemMenu = GetSystemMenu(result_handle, FALSE);
		if (!bSizable) {
			/* Window is closable but not sizable. */
			RemoveMenu(hSystemMenu, 4, MF_BYPOSITION); /* Maximize. */
			RemoveMenu(hSystemMenu, 3, MF_BYPOSITION); /* Reduce. */
			RemoveMenu(hSystemMenu, 2, MF_BYPOSITION); /* Size. */
			RemoveMenu(hSystemMenu, 0, MF_BYPOSITION); /* Restore. */
		} else {
			 /* Window is sizable but not closable. */
			RemoveMenu(hSystemMenu, 6, MF_BYPOSITION); /* Close. */
			RemoveMenu(hSystemMenu, 5, MF_BYPOSITION); /* Separator. */
			RemoveMenu(hSystemMenu, 3, MF_BYPOSITION); /* Reduce. */
		}
	}

	goto free;
error:
	if (result_handle) {
		DestroyWindow(result_handle);
		result_handle = RT_NULL;
	}
free:
	return result_handle;
}

rt_s RtGetWindowSize(rt_h hWindow, RT_GUI_RECT *lpRect)
{
	rt_s ret;

	ret = GetWindowRect(hWindow, (RECT*)lpRect);
	if (ret) {
		lpRect->nWidth = lpRect->nWidth - lpRect->nX;
		lpRect->nHeight = lpRect->nHeight - lpRect->nY;
		lpRect->nX = 0;
		lpRect->nY = 0;
	}
	return ret;
}

void *RtGetWindowUserData(rt_h hWindow)
{
	return RT_TYPE_MAKE_POINTER(GetWindowLongPtr(hWindow, GWLP_USERDATA));
}

rt_s RtSetWindowUserData(rt_h hWindow, void *lpUserData)
{
	ULONG_PTR returned_value;
	rt_s ret;

	/* SetWindowLongPtr returns the previous value and 0 in case of issue. But the previous value can be 0. So there is a check on last error. */
	SetLastError(0);
	returned_value = SetWindowLongPtr(hWindow, GWLP_USERDATA, RT_TYPE_MAKE_UINTEGER(lpUserData));
	if (!returned_value && GetLastError()) {
		ret = RT_FAILED;
	} else {
		ret = RT_OK;
	}
	return ret;
}
