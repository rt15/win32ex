#ifndef RT_GUI_H
#define RT_GUI_H

#include <rpr.h>

/**
 * @file
 * Windows GUI utility functions.
 */

/* Type of RT_GUI_MENU_ITEM menu item. */
#define RT_GUI_MENU_ITEM_TYPE_COMMAND 0
#define RT_GUI_MENU_ITEM_TYPE_SUB_MENU 1
#define RT_GUI_MENU_ITEM_TYPE_SEPARATOR 2

/**
 * Abstract super class for RT_GUI_COMMAND_MENU_ITEM/RT_GUI_SUB_MENU_MENU_ITEM/RT_GUI_SEPARATOR_MENU_ITEM.
 */
typedef struct _RT_GUI_MENU_ITEM {
	rt_un type;
}
RT_GUI_MENU_ITEM;

/**
 * There are 3 types of RT_GUI_COMMAND_MENU_ITEM: regular buttons, check boxes and radio buttons.
 *
 * <p>
 * Check boxes and radio buttons cannot have an icon.<br>
 * There is no difference in a tool bar between a check box and a radio button.<br>
 * Radio buttons should be placed between separators.
 * </p>
 */
#define RT_GUI_COMMAND_MENU_ITEM_TYPE_BUTTON 1
#define RT_GUI_COMMAND_MENU_ITEM_TYPE_CHECK_BOX 2
#define RT_GUI_COMMAND_MENU_ITEM_TYPE_RADIO_BUTTON 3

/**
 * A RT_GUI_MENU_ITEM that allows the user to perform a command.
 */
typedef struct _RT_GUI_COMMAND_MENU_ITEM {
	RT_GUI_MENU_ITEM menuItem;
	rt_char *lpText;
	rt_un32 unCommandType;			 /* RT_GUI_COMMAND_MENU_ITEM_TYPE_BUTTON/RT_GUI_COMMAND_MENU_ITEM_TYPE_CHECK_BOX. */
	rt_un32 unId;								/* WM_COMMAND id. */
	rt_h hIcon;									/* Optional icon. If RT_NULL, and unSystemImageList == RT_TYPE_MAX_UN, then lpText is used in toolbars. */
	rt_h hBitmap;								/* PARGB bitmap for menu rendering in version above XP. */
	rt_un32 unSystemImageList;	 /* RT_TYPE_MAX_UN/IDB_STD_SMALL_COLOR/IDB_VIEW_SMALL_COLOR/IDB_HIST_SMALL_COLOR. */
	rt_un32 unSystemImageIndex;	/* Index in nSystemImageList. */
	rt_un32 unImageIndex;				/* Index of the image in the imagelist. Filled by RtCreateToolBarImageList. */
}
RT_GUI_COMMAND_MENU_ITEM;

/**
 * A RT_GUI_MENU_ITEM that contains other RT_GUI_MENU_ITEMs.
 */
typedef struct _RT_GUI_SUB_MENU_MENU_ITEM {
	RT_GUI_MENU_ITEM menuItem;
	rt_char *lpText;
	rt_h hSubMenu;
}
RT_GUI_SUB_MENU_MENU_ITEM;

/**
 * A RT_GUI_MENU_ITEM that separates RT_GUI_MENU_ITEMs.
 */
typedef struct _RT_GUI_SEPARATOR_MENU_ITEM {
	RT_GUI_MENU_ITEM menuItem;
	rt_n32 nSeparatorSize;	 /* Used for toolbars only. */
}
RT_GUI_SEPARATOR_MENU_ITEM;

/**
 * RT_GUI_COMMAND_MENU_ITEM are managed as a pool that can be used for menus and toolbars.
 */
typedef struct _RT_GUI_COMMAND_MENU_ITEM_MANAGER {
	RT_GUI_COMMAND_MENU_ITEM *lpCommandMenuItems;
	rt_un unCommandMenuItemsCount;
	rt_h hToolBarImageList;
}
RT_GUI_COMMAND_MENU_ITEM_MANAGER;

/**
 * Center hWindow in hParentWindow;
 */
void RtCenterWindow(rt_h hWindow, int nWidth, int nHeight, rt_h hParentWindow);

/**
 * Used as a buffer in RtCreateToolBar.
 */
typedef struct _RT_GUI_TB_BUTTON {
	int iBitmap;
	int idCommand;
	rt_uchar8 fsState;
	rt_uchar8 fsStyle;
	rt_uchar8 bReserved[2];
	rt_un32 dwData;
	int iString;
} RT_GUI_TB_BUTTON;

typedef struct _RT_GUI_RECT {
	rt_n32 nX;
	rt_n32 nY;
	rt_n32 nWidth;
	rt_n32 nHeight;
} RT_GUI_RECT;

/**
 * Must be called before using particular control classes.
 *
 * <p>
 * <tt>nClasses</tt> is a combination of:
 * </p>
 *
 * <li>
 *   <ul>ICC_ANIMATE_CLASS			Load animate control class.</ul>
 *   <ul>ICC_BAR_CLASSES				Load toolbar, status bar, trackbar, and tooltip control classes.</ul>
 *   <ul>ICC_COOL_CLASSES			 Load rebar control class.</ul>
 *   <ul>ICC_DATE_CLASSES			 Load date and time picker control class.</ul>
 *   <ul>ICC_HOTKEY_CLASS			 Load hot key control class.</ul>
 *   <ul>ICC_INTERNET_CLASSES	 Load IP address class.</ul>
 *   <ul>ICC_LINK_CLASS				 Load a hyperlink control class.</ul>
 *   <ul>ICC_LISTVIEW_CLASSES	 Load list-view and header control classes.</ul>
 *   <ul>ICC_NATIVEFNTCTL_CLASS Load a native font control class.</ul>
 *   <ul>ICC_PAGESCROLLER_CLASS Load pager control class.</ul>
 *   <ul>ICC_PROGRESS_CLASS		 Load progress bar control class.</ul>
 *   <ul>ICC_STANDARD_CLASSES	 Load one of the intrinsic User32 control classes. The user controls include button, edit, static, listbox, combobox, and scroll bar.</ul>
 *   <ul>ICC_TAB_CLASSES				Load tab and tooltip control classes.</ul>
 *   <ul>ICC_TREEVIEW_CLASSES	 Load tree-view and tooltip control classes.</ul>
 *   <ul>ICC_UPDOWN_CLASS			 Load up-down control class.</ul>
 *   <ul>ICC_USEREX_CLASSES		 Load ComboBoxEx class.</ul>
 *   <ul>ICC_WIN95_CLASSES			Load animate control, header, hot key, list-view, progress bar, status bar, tab, tooltip, toolbar, trackbar, tree-view, and up-down control classes.</ul>
 * </li>
 *
 * <p>
 * A classical cause of failure seems to be the manifest file.
 * </p>
 *
 */
rt_s RtInitCommonControls(rt_un32 nClasses);

/**
 * Initialize a command menu item manager given an array of RT_GUI_COMMAND_MENU_ITEM.
 *
 * <p>
 * Must be called prior to create menus and toolbars.
 * </p>
 */
rt_s RtCreateGuiCommandMenuItemManager(RT_GUI_COMMAND_MENU_ITEM_MANAGER *lpGuiCommandMenuItemManager, RT_GUI_COMMAND_MENU_ITEM *lpCommandMenuItems, rt_un unCommandMenuItemsCount, rt_h hInstance);

rt_s RtFreeGuiCommandMenuItemManager(RT_GUI_COMMAND_MENU_ITEM_MANAGER *lpGuiCommandMenuItemManager);

/**
 * @param lpButtons Buffer.
 */
rt_h RtCreateToolBar(rt_un unMenuItemsCount, RT_GUI_MENU_ITEM *lpMenuItems[], RT_GUI_TB_BUTTON *lpButtons, rt_h hImageList, rt_h hParentWindow, rt_un unControlId, rt_h hInstance);

/**
 * GetClientRect returns the size of the client area.<br>
 * GetWindowRect returns the coordinates of a window.<br>
 * RtGetWindowSize returns the size of the complete window including borders.
 *
 * <p>
 * Zero is returned for nX and nY.
 * </p>
 */
rt_s RtGetWindowSize(rt_h hWindow, RT_GUI_RECT *lpRect);

/**
 * <tt>nRight</tt> is width and <tt>nBottom</tt> is height.
 */
rt_s RtMoveWindow(rt_h hWindow, RT_GUI_RECT *lpRect);

rt_h RtCreateStaticWindow(RT_GUI_RECT *lpPosition, rt_h hParentWindow, rt_un unControlId, rt_h hInstance);

/**
 * Returns an handle to the application instance (hInstance).
 *
 * <p>
 * This is also an handle to the main module (.exe) of the application.
 * </p>
 */
rt_h RtGetInstance();

rt_h RtCreateMenu(rt_un unMenuItemsCount, RT_GUI_MENU_ITEM *lpMenuItems[]);

typedef rt_n (RT_CALL *RT_WINDOW_PROC_CALLBACK)(rt_h hWindow, rt_un32 unMsg, rt_un unWParam, rt_n nLParam);

typedef rt_b (RT_CALL *RT_DIALOG_PROC_CALLBACK)(rt_h hWindow, rt_un32 unMsg, rt_un unWParam, rt_n nLParam);

/**
 *
 * <p>
 * When Associated to a Window, the created menu will be destroyed when the window is destroyed.<br>
 * In other cases, call DestroyMenu.
 * </p>
 */
rt_h RtCreateMenuBar(rt_un unMenusCount, rt_char *lpTexts[], rt_h *hMenus);

/**
 *
 * <p>
 * All window classes that an application registers are unregistered when it terminates.<br>
 * No window classes registered by a DLL are unregistered when the DLL is unloaded. A DLL must explicitly unregister its classes when it is unloaded.
 * </p>
 *
 * @param nStyle See CreateWindow documentation. WS_OVERLAPPEDWINDOW is a classical values.
 * @param nExStyle See CreateWindowEx documentation. O is a classical value.
 * @param lpRect Use CW_USEDEFAULT 0x80000000 as nX for default position (nY is not used in that case).
 * @param hIcon If RT_NULL, a default application icon is used.
 * @param hMenuBar If RT_NULL, the window won't have a menu bar.
 * @param lpUserData Associated to the window using SetWindowLongPtr/GWLP_USERDATA.
 * @return RT_NULL in case of error, the window handle otherwise. Not need to call CloseHandle on it.
 */
rt_h RtCreateMainWindow(rt_char *lpText, rt_char *lpClassName, rt_un32 unStyle, rt_un32 unExStyle, RT_GUI_RECT *lpRect, RT_WINDOW_PROC_CALLBACK lpWindowProc, rt_h hIcon, rt_h hMenuBar, void *lpUserData, rt_h hInstance);

/**
 * Create a dialog box, that is a secondary window.
 *
 * <p>
 * To create a modal dialog box, call EnableWindow FALSE on the parent window while displaying the dialog box.<br>
 * Call EnableWindow TRUE on the parent window before destroying or hiding the dialog box, for example when WM_CLOSE is received.
 * </p>
 *
 * <p>
 * The dialog box is centered in its parent.<br>
 * It cannot be minimized.<br>
 * If it is not closable, you will have to intercept WM_CLOSE.
 * </p>
 */
rt_h RtCreateDialogBox(rt_char *lpText, rt_char *lpClassName, rt_b bSizable, rt_b bClosable, rt_n32 nWidth, rt_n32 nHeight, RT_WINDOW_PROC_CALLBACK lpWindowProc, void *lpUserData, rt_h hParentWindow, rt_h hInstance);


/**
 * Be sure to dispatch the WM_SIZE to the status bar in the parent window proc.
 */
rt_h RtCreateStatusBar(rt_b bSimple, rt_char *lpText, rt_h hParentWindow, rt_un unControlId, rt_h hInstance);

void RtSetSimpleStatusBarText(rt_h hStatusBar, rt_char *lpText);

/**
 * Post a WM_QUIT so that GetMessage will returns 0.
 */
void RtQuitMessageLoop(rt_un32 exit_code);

/**
 * Perform default message loop.<br>
 * Returns when a WM_QUIT message is posted.<br>
 * This message can be posted using RtQuitMessageLoop.
 */
rt_un16 RtDefaultMessageLoop();

/**
 * Retrieve the lpUserData pointer given to RtCreateMainWindow or RtCreateDialogBox.
 *
 * <p>
 * Use SetWindowLongPtr/GWLP_USERDATA internally.
 * </p>
 */
void *RtGetWindowUserData(rt_h hWindow);

/**
 * Set the user data associated with the given window.
 *
 * <p>
 * Use SetWindowLongPtr/GWLP_USERDATA internally.
 * </p>
 */
rt_s RtSetWindowUserData(rt_h hWindow, void *lpUserData);

#endif /* RT_GUI_H */
