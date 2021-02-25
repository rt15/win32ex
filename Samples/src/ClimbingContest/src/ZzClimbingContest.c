#include <rpr.h>
#include <RtWin32ExGui.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

#include "ZzAppContext.h"
#include "ZzResources.h"
#include "ZzStrings.h"
#include "ZzGui.h"
#include "ZzWindows.h"

/**
 * TODO
 * The display of the list should be sorted.
 * Put references in place.
 */

/**
 * Generate a new resource name.
 */
rt_s RT_CALL ZzGenerateResourceName(rt_char *buffer)
{
	RT_ARRAY zzResourceName;
	RT_ARRAY zzConversionBuffer;
	rt_char lpConversionBuffer[64];
	rt_un unResourceIndex;
	rt_s ret;

	rt_array_Create(&zzResourceName, buffer, sizeof(rt_char), ZZ_RESOURCES_NAME_SIZE);

	if (!rt_random_get_unsigned_integer_with_boundaries(1, 20, &unResourceIndex)) goto error;

	rt_array_Create(&zzConversionBuffer, lpConversionBuffer, sizeof(rt_char), 64);
	if (!rt_char_append_un(unResourceIndex, &zzConversionBuffer)) goto error;

	if (!rt_char_AppendCString(&zzResourceName, _R("New item "))) goto error;
	if (!rt_array_Append(&zzResourceName, &zzConversionBuffer)) goto error;
	if (!rt_char_Append(&zzResourceName, 0)) goto error;

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

/**
 * Load into the list-box the content of the rt_list.
 */
void RT_CALL ZzRefreshList(ZZ_APP_CONTEXT *lpAppContext)
{
	rt_un unListSize;
	rt_char *item;
	rt_un i;

	/* Clear the list. */
	SendMessage(lpAppContext->hListBox, LB_RESETCONTENT, 0, 0);

	/* Fill the list. */
	unListSize = rt_list_GetSize(lpAppContext->lpLists[lpAppContext->unCurrentEntity]);
	for (i = 0; i < unListSize; i++) {
		rt_list_GetItem(lpAppContext->lpLists[lpAppContext->unCurrentEntity], i, (void**)&item);
		SendMessage(lpAppContext->hListBox, LB_ADDSTRING, 0, (LPARAM)item);
	}

	/* Nothing selected, disable delete button. */
	EnableWindow(lpAppContext->hDeleteButton, FALSE);
}

void RT_CALL ZzItemSelected(ZZ_APP_CONTEXT *lpAppContext)
{
	/* An item is selected, enable delete button. */
	EnableWindow(lpAppContext->hDeleteButton, TRUE);
}

rt_n RT_CALL ZzMainWindowProc(rt_h hWindow, rt_un32 unMsg, rt_un unWParam, rt_n nLParam)
{
	ZZ_APP_CONTEXT *lpAppContext;
	rt_b bCallDefault;
	MINMAXINFO *lpMinMaxInfo;
	RT_GUI_RECT rtRect;
	POINT rtCursorPosition;
	LONG_PTR nSelectedItem;
	rt_un unListSize;
	rt_char *item;
	NMHDR *lpNotifyMessageInfo;
	rt_h hChild;
	rt_n ret;

	lpAppContext = (ZZ_APP_CONTEXT*)RtGetWindowUserData(hWindow);

	bCallDefault = RT_TRUE;
	switch (unMsg)
	{
		case WM_NOTIFY:
			switch (unWParam)
			{
				case ZZ_RESOURCES_LEFT_TAB_CTRL_ID:
					lpNotifyMessageInfo = (NMHDR*)nLParam;
					switch (lpNotifyMessageInfo->code)
					{
						case TCN_SELCHANGE:
							/* Set current entity in context. */
							lpAppContext->unCurrentEntity = TabCtrl_GetCurSel(lpAppContext->hLeftTab);

							/* Load list content. */
							ZzRefreshList(lpAppContext);
							break;
					}
					break;
			}
			break;
		case WM_COMMAND:
			switch (HIWORD(unWParam))
			{
				case BN_CLICKED:
					switch (LOWORD(unWParam))
					{
						case ZZ_RESOURCES_ADD_BUTTON_CTRL_ID:
							rt_list_NewItem(&lpAppContext->lpLists[lpAppContext->unCurrentEntity], (void**)&item);
							ZzGenerateResourceName(item);
							ZzRefreshList(lpAppContext);

							/* Select created entity. */
							unListSize = rt_list_GetSize(lpAppContext->lpLists[lpAppContext->unCurrentEntity]);
							SendMessage(lpAppContext->hListBox, LB_SETCURSEL, unListSize - 1, 0);
							ZzItemSelected(lpAppContext);
							break;
						case ZZ_RESOURCES_DELETE_BUTTON_CTRL_ID:
							nSelectedItem = SendMessage(lpAppContext->hListBox, LB_GETCURSEL, 0, 0);
							if (nSelectedItem != LB_ERR) {
								rt_list_DeleteItemIndex(&lpAppContext->lpLists[lpAppContext->unCurrentEntity], nSelectedItem);
								ZzRefreshList(lpAppContext);

								unListSize = rt_list_GetSize(lpAppContext->lpLists[lpAppContext->unCurrentEntity]);

								/* Select last item if it has been removed. */
								if ((rt_un32)nSelectedItem >= unListSize) nSelectedItem = unListSize - 1;

								/* Select next/last item if available. */
								if (nSelectedItem >= 0) {
									SendMessage(lpAppContext->hListBox, LB_SETCURSEL, nSelectedItem, 0);
									ZzItemSelected(lpAppContext);
								}
							}
							break;
					}
					break;
				case LBN_SELCHANGE:
					nSelectedItem = SendMessage(lpAppContext->hListBox, LB_GETCURSEL, 0, 0);
					if (nSelectedItem != LB_ERR) {
						ZzItemSelected(lpAppContext);
					}
					break;
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

			RtMoveWindow(lpAppContext->hLeftTab, ZzGetLeftTabPosition(&rtRect, lpAppContext));
			RtMoveWindow(lpAppContext->hVerticalSplitter, ZzGetVerticalSplitterPosition(&rtRect, lpAppContext));
			RtMoveWindow(lpAppContext->hListBox, ZzGetListBoxPosition(&rtRect, lpAppContext));
			break;

		case WM_LBUTTONDOWN:
			rtCursorPosition.x = LOWORD(nLParam);
			rtCursorPosition.y = HIWORD(nLParam);
			hChild = ChildWindowFromPoint(lpAppContext->hMainWindow, rtCursorPosition);
			if (hChild == lpAppContext->hVerticalSplitter) {
				ZzGetVerticalSplitterPosition(&rtRect, lpAppContext);
				lpAppContext->nSplitterCursorOffset = rtCursorPosition.x - rtRect.nX;
				SetCursor(lpAppContext->hWestEastCursor);
				lpAppContext->bUsingVerticalSplitter = RT_TRUE;
				SetCapture(lpAppContext->hMainWindow);
			}
			break;

		case WM_LBUTTONUP:
			if (lpAppContext->bUsingVerticalSplitter) {
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
			if (lpAppContext->bUsingVerticalSplitter && rtCursorPosition.x < RT_TYPE_MAX_N16) {
				lpAppContext->nVerticalSplitterX = rtCursorPosition.x - lpAppContext->nSplitterCursorOffset;
				RtMoveWindow(lpAppContext->hLeftTab, ZzGetLeftTabPosition(&rtRect, lpAppContext));
				RtMoveWindow(lpAppContext->hVerticalSplitter, ZzGetVerticalSplitterPosition(&rtRect, lpAppContext));
				RtMoveWindow(lpAppContext->hListBox, ZzGetListBoxPosition(&rtRect, lpAppContext));
			} else {
				hChild = ChildWindowFromPoint(lpAppContext->hMainWindow, rtCursorPosition);
				if (hChild == lpAppContext->hVerticalSplitter) {
					SetCursor(lpAppContext->hWestEastCursor);
				} else {
					SetCursor(lpAppContext->hStandardCursor);
				}
			}
			break;

		case WM_DESTROY:
			/* Request end of message loop. */
			RtQuitMessageLoop(lpAppContext->unWindowExitCode);
			break;
	}

	if (bCallDefault) {
		/* Default windows treatment. */
		ret = DefWindowProc(hWindow, unMsg, unWParam, nLParam);
	}
	return ret;
}

rt_s RT_CALL ZzMain(rt_n32 argc, rt_char *argv[])
{
	ZZ_APP_CONTEXT rtAppContext;
	rt_b bMainWindowCreated;
	rt_b bAppContextCreated;
	RT_GUI_RECT rtRect;
	rt_s ret;

	bAppContextCreated = RT_FALSE;
	bMainWindowCreated = RT_FALSE;

	if (!RtInitCommonControls(ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES )) goto error;

	if (!ZzCreateAppContext(&rtAppContext)) goto error;
	bAppContextCreated = RT_TRUE;

	rtRect.nX = CW_USEDEFAULT;
	rtRect.nY = 0;
	rtRect.nWidth = ZZ_RESOURCES_DEFAULT_WINDOW_WIDTH;
	rtRect.nHeight = ZZ_RESOURCES_DEFAULT_WINDOW_HEIGHT;

	rtAppContext.hMainWindow = RtCreateMainWindow(ZzGetString(ZZ_STRINGS_APPLICATION_NAME), _R("MainWindow"),
																								WS_OVERLAPPEDWINDOW, WS_EX_COMPOSITED, &rtRect, &ZzMainWindowProc, RT_NULL, RT_NULL,
																								&rtAppContext, rtAppContext.hInstance);
	if (!rtAppContext.hMainWindow) goto error;
	bMainWindowCreated = RT_TRUE;

	/* Status bar. */
	rtAppContext.hStatusBar = RtCreateStatusBar(RT_TRUE, _R(""), rtAppContext.hMainWindow, ZZ_RESOURCES_STATUS_BAR_CTRL_ID, rtAppContext.hInstance);
	if (!rtAppContext.hStatusBar) goto error;

	rtAppContext.hLeftTab = ZzCreateLeftTab(ZzGetLeftTabPosition(&rtRect, &rtAppContext), rtAppContext.hMainWindow, rtAppContext.hInstance, rtAppContext.hFont);
	if (!rtAppContext.hLeftTab) goto error;

	rtAppContext.hVerticalSplitter = RtCreateStaticWindow(ZzGetVerticalSplitterPosition(&rtRect, &rtAppContext), rtAppContext.hMainWindow, ZZ_RESOURCES_VERTICAL_SPLITTER_CTRL_ID, rtAppContext.hInstance);
	if (!rtAppContext.hVerticalSplitter) goto error;

	rtAppContext.hListBox = ZzCreateListBox(ZzGetListBoxPosition(&rtRect, &rtAppContext), _R("EntitesListBox"), ZZ_RESOURCES_ENTITIES_LIST_CTRL_ID, rtAppContext.hLeftTab, rtAppContext.hInstance, rtAppContext.hFont);
	if (!rtAppContext.hListBox) goto error;

	rtRect.nX = ZZ_GUI_BORDER;
	rtRect.nY = ZZ_GUI_TAB_HEIGHT + ZZ_GUI_BORDER;
	rtRect.nWidth = ZZ_GUI_BUTTON_WIDTH;
	rtRect.nHeight = ZZ_GUI_BUTTON_HEIGHT;

	rtAppContext.hAddButton = ZzCreateButton(&rtRect, ZzGetString(ZZ_STRINGS_ADD), ZZ_RESOURCES_ADD_BUTTON_CTRL_ID, rtAppContext.hLeftTab, rtAppContext.hInstance, rtAppContext.hFont);
	if (!rtAppContext.hAddButton) goto error;

	rtRect.nX = ZZ_GUI_BORDER * 2 + ZZ_GUI_BUTTON_WIDTH;

	rtAppContext.hDeleteButton = ZzCreateButton(&rtRect, ZzGetString(ZZ_STRINGS_DELETE), ZZ_RESOURCES_DELETE_BUTTON_CTRL_ID, rtAppContext.hLeftTab, rtAppContext.hInstance, rtAppContext.hFont);
	if (!rtAppContext.hDeleteButton) goto error;

	/* Delete button is disabled by default. */
	EnableWindow(rtAppContext.hDeleteButton, FALSE);

	/* No way to check success of ShowWindow. */
	ShowWindow(rtAppContext.hMainWindow, SW_SHOWNORMAL);

	/* The main window is destroyed when it is closed. */
	bMainWindowCreated = RT_FALSE;

	/* RtDefaultMessageLoop returns an exit code. */
	ret = !RtDefaultMessageLoop();
free:
	if (bMainWindowCreated) {
		bMainWindowCreated = RT_FALSE;
		if (!DestroyWindow(rtAppContext.hMainWindow) && ret)
			goto error;
	}
	if (bAppContextCreated) {
		bAppContextCreated = RT_FALSE;
		if (!ZzFreeAppContext(&rtAppContext) && ret)
			goto error;
	}
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_un16 RT_CALL RtMain(rt_n32 argc, rt_char *argv[])
{
	rt_un16 result;
	if (ZzMain(argc, argv)) {
		result = 0;
	} else {
		result = 1;
	}
	return result;
}
