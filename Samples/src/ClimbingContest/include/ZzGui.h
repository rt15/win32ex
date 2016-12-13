#ifndef ZZ_GUI_H
#define ZZ_GUI_H

#include <RtWin32Ex.h>
#include <RtWin32ExGui.h>

#include "ZzAppContext.h"

#define ZZ_GUI_BORDER 10
#define ZZ_GUI_TAB_HEIGHT 18
#define ZZ_GUI_BUTTON_WIDTH 80
#define ZZ_GUI_BUTTON_HEIGHT 20

RT_GUI_RECT* RT_CALL ZzComputeVerticalSplitterPosition(RT_GUI_RECT* lpRect, ZZ_APP_CONTEXT* lpAppContext);

RT_GUI_RECT* RT_CALL ZzComputeLeftTabPosition(RT_GUI_RECT* lpRect, ZZ_APP_CONTEXT* lpAppContext);

RT_GUI_RECT* RT_CALL ZzComputeListBoxPosition(RT_GUI_RECT* lpRect, ZZ_APP_CONTEXT* lpAppContext);

RT_H RT_CALL ZzCreateListBox(RT_GUI_RECT* lpPosition, RT_CHAR* lpName, RT_N nControlId, RT_H hParentWindow, RT_H hInstance);

RT_H RT_CALL ZzCreateButton(RT_GUI_RECT* lpPosition, RT_CHAR* lpText, RT_N nControlId, RT_H hParentWindow, RT_H hInstance, RT_H hFont);

RT_H RT_CALL ZzCreateLeftTab(RT_GUI_RECT* lpPosition, RT_H hMainWindow, RT_H hInstance, RT_H hFont);

#endif /* ZZ_GUI_H */
