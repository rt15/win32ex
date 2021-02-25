#ifndef ZZ_GUI_H
#define ZZ_GUI_H

#include <rpr.h>
#include <RtWin32ExGui.h>

#include "ZzAppContext.h"

#define ZZ_GUI_BORDER 10
#define ZZ_GUI_TAB_HEIGHT 18
#define ZZ_GUI_BUTTON_WIDTH 80
#define ZZ_GUI_BUTTON_HEIGHT 20

RT_GUI_RECT *RT_CALL ZzGetVerticalSplitterPosition(RT_GUI_RECT *lpRect, ZZ_APP_CONTEXT *lpAppContext);

RT_GUI_RECT *RT_CALL ZzGetLeftTabPosition(RT_GUI_RECT *lpRect, ZZ_APP_CONTEXT *lpAppContext);

RT_GUI_RECT *RT_CALL ZzGetListBoxPosition(RT_GUI_RECT *lpRect, ZZ_APP_CONTEXT *lpAppContext);

rt_h RT_CALL ZzCreateListBox(RT_GUI_RECT *lpPosition, rt_char *lpName, rt_un unControlId, rt_h hParentWindow, rt_h hInstance, rt_h hFont);

rt_h RT_CALL ZzCreateButton(RT_GUI_RECT *lpPosition, rt_char *lpText, rt_un unControlId, rt_h hParentWindow, rt_h hInstance, rt_h hFont);

rt_h RT_CALL ZzCreateLeftTab(RT_GUI_RECT *lpPosition, rt_h hMainWindow, rt_h hInstance, rt_h hFont);

#endif /* ZZ_GUI_H */
