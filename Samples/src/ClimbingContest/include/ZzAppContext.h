#ifndef ZZ_APP_CONTEXT_H
#define ZZ_APP_CONTEXT_H

#include <rpr.h>
#include <RtWin32ExGui.h>

#include "ZzResources.h"

typedef struct _ZZ_APP_CONTEXT {
	rt_h hInstance;
	rt_un32 unWindowExitCode;

	rt_n32 nVerticalSplitterX;								/* Position of the vertical splitter. */
	rt_n32 nSplitterCursorOffset;						 /* Used when the a splitter is in use to store the offset where the user clicked. */
	rt_b bUsingVerticalSplitter;							/* Track usage of vertical splitter. */

	rt_h hStandardCursor;
	rt_h hWestEastCursor;

	rt_h hFont;

	/* Current entity tab. */
	rt_un unCurrentEntity;

	void *lpLists[ZZ_RESOURCES_ENTITIES_COUNT];

	struct rt_runtime_heap rtRuntimeHeap;
	rt_b bHeapCreated;

	rt_h hMainWindow;
	rt_h hStatusBar;
	rt_h hLeftTab;
	rt_h hVerticalSplitter;
	rt_h hListBox;
	rt_h hAddButton;
	rt_h hDeleteButton;
}
ZZ_APP_CONTEXT;

rt_s RT_CALL ZzCreateAppContext(ZZ_APP_CONTEXT *lpAppContext);

rt_s RT_CALL ZzFreeAppContext(ZZ_APP_CONTEXT *lpAppContext);

#endif /* ZZ_APP_CONTEXT_H */
