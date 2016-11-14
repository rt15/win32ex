#ifndef RT_GUI_H
#define RT_GUI_H

#include <RtWin32Ex.h>

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
typedef struct _RT_GUI_MENU_ITEM
{
  RT_UN unType;
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
typedef struct _RT_GUI_COMMAND_MENU_ITEM
{
  RT_GUI_MENU_ITEM menuItem;
  RT_CHAR* lpText;
  RT_UN32 unCommandType;   /* RT_GUI_COMMAND_MENU_ITEM_TYPE_BUTTON/RT_GUI_COMMAND_MENU_ITEM_TYPE_CHECK_BOX. */
  RT_UN32 unId;            /* WM_COMMAND id. */
  RT_H hIcon;             /* Optional icon. If RT_NULL, and nSystemImageList == -1, then lpText is used in toolbars. */
  RT_H hBitmap;           /* PARGB bitmap for menu rendering in version above XP. */
  RT_N nSystemImageList;  /* -1/IDB_STD_SMALL_COLOR/IDB_VIEW_SMALL_COLOR/IDB_HIST_SMALL_COLOR. */
  RT_N nSystemImageIndex; /* Index in nSystemImageList. */
  RT_UN32 unImageIndex;    /* Index of the image in the imagelist. Filled by RtCreateToolBarImageList. */
}
RT_GUI_COMMAND_MENU_ITEM;

/**
 * A RT_GUI_MENU_ITEM that contains other RT_GUI_MENU_ITEMs.
 */
typedef struct _RT_GUI_SUB_MENU_MENU_ITEM
{
  RT_GUI_MENU_ITEM menuItem;
  RT_CHAR* lpText;
  RT_H hSubMenu;
}
RT_GUI_SUB_MENU_MENU_ITEM;

/**
 * A RT_GUI_MENU_ITEM that separates RT_GUI_MENU_ITEMs.
 */
typedef struct _RT_GUI_SEPARATOR_MENU_ITEM
{
  RT_GUI_MENU_ITEM menuItem;
  RT_N nSeparatorSize;   /* Used for toolbars only. */
}
RT_GUI_SEPARATOR_MENU_ITEM;

/**
 * RT_GUI_COMMAND_MENU_ITEM are managed as a pool that can be used for menus and toolbars.
 */
typedef struct _RT_GUI_COMMAND_MENU_ITEM_MANAGER
{
  RT_GUI_COMMAND_MENU_ITEM* lpCommandMenuItems;
  RT_N nCommandMenuItemsCount;
  RT_H hToolBarImageList;
}
RT_GUI_COMMAND_MENU_ITEM_MANAGER;

/**
 * Center hWindow in hParentWindow;
 */
void RT_API RtCenterWindow(RT_H hWindow, int nWidth, int nHeight, RT_H hParentWindow);

/**
 * Used as a buffer in RtCreateToolBar.
 */
typedef struct _RT_GUI_TB_BUTTON {
	int iBitmap;
	int idCommand;
	RT_UCHAR8 fsState;
	RT_UCHAR8 fsStyle;
	RT_UCHAR8 bReserved[2];
	RT_UN32 dwData;
	int iString;
} RT_GUI_TB_BUTTON;

typedef struct _RT_GUI_RECT {
	RT_N32 nX;
	RT_N32 nY;
	RT_N32 nWidth;
	RT_N32 nHeight;
} RT_GUI_RECT;

/**
 * Initialize a command menu item manager given an array of RT_GUI_COMMAND_MENU_ITEM.
 *
 * <p>
 * Must be called prior to create menus and toolbars.
 * </p>
 */
RT_B RT_API RtCreateGuiCommandMenuItemManager(RT_GUI_COMMAND_MENU_ITEM_MANAGER* lpGuiCommandMenuItemManager, RT_GUI_COMMAND_MENU_ITEM* lpCommandMenuItems, RT_UN unCommandMenuItemsCount, RT_H hInstance);

RT_B RT_API RtFreeGuiCommandMenuItemManager(RT_GUI_COMMAND_MENU_ITEM_MANAGER* lpGuiCommandMenuItemManager);

/**
 * @param lpButtons Buffer.
 */
RT_H RT_API RtCreateToolBar(RT_UN unMenuItemsCount, RT_GUI_MENU_ITEM* lpMenuItems[], RT_GUI_TB_BUTTON* lpButtons, RT_H hImageList, RT_H hParentWindow, RT_N32 nControlId, RT_H hInstance);

/**
 * GetClientRect returns the size of the client area.<br>
 * GetWindowRect returns the coordinates of a window.<br>
 * RtGetWindowSize returns the size of the complete window including borders.
 *
 * <p>
 * Zero is returned for nX and nY.
 * </p>
 */
RT_B RT_API RtGetWindowSize(RT_H hWindow, RT_GUI_RECT* lpRect);

/**
 * <tt>nRight</tt> is width and <tt>nBottom</tt> is height.
 */
RT_B RT_API RtMoveWindow(RT_H hWindow, RT_GUI_RECT* lpRect);

RT_H RT_API RtCreateStaticWindow(RT_GUI_RECT* lpPosition, RT_H hParentWindow, RT_N32 nControlId, RT_H hInstance);

/**
 * Returns an handle to the application instance (hInstance).
 *
 * <p>
 * This is also an handle to the main module (.exe) of the application.
 * </p>
 */
RT_H RT_API RtGetInstance();

RT_H RT_API RtCreateMenu(RT_UN unMenuItemsCount, RT_GUI_MENU_ITEM* lpMenuItems[]);

typedef RT_N (RT_CALL *RT_WINDOW_PROC_CALLBACK)(RT_H hWindow, RT_UN32 unMsg, RT_UN unWParam, RT_N nLParam);

typedef RT_B (RT_CALL *RT_DIALOG_PROC_CALLBACK)(RT_H hWindow, RT_UN32 unMsg, RT_UN unWParam, RT_N nLParam);

/**
 *
 * <p>
 * When Associated to a Window, the created menu will be destroyed when the window is destroyed.<br>
 * In other cases, call DestroyMenu.
 * </p>
 */
RT_H RT_API RtCreateMenuBar(RT_UN unMenusCount, RT_CHAR* lpTexts[], RT_H* hMenus);

/**
 *
 * @param nStyle See CreateWindow documentation. WS_OVERLAPPEDWINDOW is a classical values.
 * @param nExStyle See CreateWindowEx documentation. O is a classical value.
 * @param lpRect Use CW_USEDEFAULT 0x80000000 for nX or default position (nY is not used in that case).
 * @param nY Not used if nX is CW_USEDEFAULT.
 * @param hIcon If RT_NULL, a default application icon is used.
 * @param hMenuBar If RT_NULL, the window won't have a menu bar.
 * @param lpUserData Associated to the window using SetWindowLongPtr/GWLP_USERDATA.
 * @return RT_NULL in case of error, the window handle otherwise.
 */
RT_H RT_API RtCreateMainWindow(RT_CHAR* lpText, RT_CHAR* lpClassName, RT_UN32 unStyle, RT_UN32 unExStyle, RT_GUI_RECT* lpRect, RT_WINDOW_PROC_CALLBACK lpWindowProc, RT_H hIcon, RT_H hMenuBar, void* lpUserData, RT_H hInstance);

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
RT_H RT_API RtCreateDialogBox(RT_CHAR* lpText, RT_CHAR* lpClassName, RT_B bSizable, RT_B bClosable, RT_N32 nWidth, RT_N32 nHeight, RT_WINDOW_PROC_CALLBACK lpWindowProc, void* lpUserData, RT_H hParentWindow, RT_H hInstance);


/**
 * Be sure to dispatch the WM_SIZE to the status bar in the parent window proc.
 */
RT_H RT_API RtCreateStatusBar(RT_B bSimple, RT_CHAR* lpText, RT_H hParentWindow, RT_N32 nControlId, RT_H hInstance);

void RT_API RtSetSimpleStatusBarText(RT_H hStatusBar, RT_CHAR* lpText);

/**
 * Post a WM_QUIT so that GetMessage will returns 0.
 */
void RT_API RtQuitMessageLoop(RT_UN32 unExitCode);

/**
 * Perform default message loop.<br>
 * Returns when a WM_QUIT message is posted.<br>
 * This message can be posted using RtQuitMessageLoop.
 */
RT_UN16 RT_API RtDefaultMessageLoop();

/**
 * Returns the major version and optionally the minor version.
 *
 * @param lpMinor Can be null.
 */
RT_UN32 RT_API RtGetOsVersion(RT_UN32* lpMinor);

/**
 * Retrieve the lpUserData pointer given to RtCreateMainWindow or RtCreateDialogBox.
 *
 * <p>
 * Use SetWindowLongPtr/GWLP_USERDATA internally.
 * </p>
 */
void* RT_API RtGetWindowUserData(RT_H hWindow);

/**
 * Set the user data associated with the given window.
 *
 * <p>
 * Use SetWindowLongPtr/GWLP_USERDATA internally.
 * </p>
 */
RT_B RT_API RtSetWindowUserData(RT_H hWindow, void* lpUserData);

#endif /* RT_GUI_H */
