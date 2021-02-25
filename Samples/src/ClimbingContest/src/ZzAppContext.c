#include "ZzAppContext.h"

#include "ZzResources.h"
#include "ZzWindows.h"

rt_s RT_CALL ZzCreateAppContext(ZZ_APP_CONTEXT *lpAppContext)
{
	rt_un i;
	rt_s ret;

	lpAppContext->bHeapCreated = RT_FALSE;
	for (i = 0; i < ZZ_RESOURCES_ENTITIES_COUNT; i++) {
		lpAppContext->lpLists[i] = RT_NULL;
	}

	if (!rt_runtime_heap_create(&lpAppContext->rtRuntimeHeap)) goto error;
	lpAppContext->bHeapCreated = RT_TRUE;

	for (i = 0; i < ZZ_RESOURCES_ENTITIES_COUNT; i++) {
		if (!rt_list_Create(&lpAppContext->lpLists[i], &lpAppContext->rtRuntimeHeap.heap, 0, ZZ_RESOURCES_NAME_SIZE, 128)) goto error;
	}

	lpAppContext->hInstance = RtGetInstance();
	lpAppContext->unWindowExitCode = 0;
	lpAppContext->unCurrentEntity = 0;

	/* Initial position of the vertical splitter. */
	lpAppContext->nVerticalSplitterX = ZZ_RESOURCES_MIN_LEFT_TAB_WIDTH;
	lpAppContext->bUsingVerticalSplitter = RT_FALSE;

	/* No need to destroy cursors from LoadCursor. */
	lpAppContext->hStandardCursor = (rt_h)LoadCursor(RT_NULL, IDC_ARROW);
	if (!lpAppContext->hStandardCursor) goto error;
	lpAppContext->hWestEastCursor = (rt_h)LoadCursor(RT_NULL, IDC_SIZEWE);
	if (!lpAppContext->hWestEastCursor) goto error;

	/* No need to call DeleteObject on stock objects. */
	lpAppContext->hFont = GetStockObject(DEFAULT_GUI_FONT);
	if (!lpAppContext->hFont) goto error;

	ret = RT_OK;
free:
	return ret;

error:
	ZzFreeAppContext(lpAppContext);
	ret = RT_FAILED;
	goto free;
}

rt_s RT_CALL ZzFreeAppContext(ZZ_APP_CONTEXT *lpAppContext)
{
	rt_un i;
	rt_s ret;

	ret = RT_OK;
free:
	for (i = 0; i < ZZ_RESOURCES_ENTITIES_COUNT; i++) {
		if (lpAppContext->lpLists[i]) {
			if (!rt_list_Free(&lpAppContext->lpLists[i]) && ret)
				goto error;
		}
	}
	if (lpAppContext->bHeapCreated) {
		lpAppContext->bHeapCreated = RT_FALSE;
		if (!lpAppContext->rtRuntimeHeap.heap->close(&lpAppContext->rtRuntimeHeap) && ret)
			goto error;
	}
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}
