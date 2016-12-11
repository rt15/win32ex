#ifndef ZZ_GUI_H
#define ZZ_GUI_H

#include <RtWin32Ex.h>
#include <RtWin32ExGui.h>

#include "ZzAppContext.h"

RT_GUI_RECT* RT_CALL ZzComputeVerticalSplitterPosition(RT_GUI_RECT* lpRect, ZZ_APP_CONTEXT* lpAppContext);

RT_GUI_RECT* RT_CALL ZzComputeLeftTabPosition(RT_GUI_RECT* lpRect, ZZ_APP_CONTEXT* lpAppContext);

RT_H RT_CALL ZzCreateLeftTab(RT_GUI_RECT* lpPosition, RT_H hMainWindow, RT_H hInstance, RT_H hFont);

#endif /* ZZ_GUI_H */
