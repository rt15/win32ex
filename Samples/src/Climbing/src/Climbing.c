#include <rpr.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

#include "cbmanager.h"

rt_s RT_CALL CbCompetitionHeaderCallback(void *context)
{
	CbWriteMenuTitle(_R("Competition"));
	return RT_OK;
}

rt_un16 RT_CALL CbCallManager(void *context, rt_n nClass, rt_char *lpClassName)
{
	CB_MANAGER_CONTEXT managerContext;

	managerContext.lpCompetition = context;
	managerContext.nClass = nClass;
	managerContext.lpClassName = lpClassName;

	return CbManageEntities(&managerContext);
}

rt_s RT_CALL CbClimbers(void *context)
{
	return CbCallManager(context, CB_CLIMBERS_INDEX, _R("Climbers"));
}

rt_s RT_CALL CbRoutes(void *context)
{
	return CbCallManager(context, CB_ROUTES_INDEX, _R("Routes"));
}

rt_s RT_CALL CbGrades(void *context)
{
	return CbCallManager(context, CB_GRADES_INDEX, _R("Grades"));
}


CB_MENU_ITEM cb_lpCompetitionMenuItems[3] = {{_R("Climbers"), &CbClimbers},
																						 {_R("Routes"), &CbRoutes},
																						 {_R("Grades"), &CbGrades}};


rt_s RT_CALL CbNameComparisonCallback(void *item1, void *item2, void *context, rt_n *comparison_result)
{
	RT_TABLE *lpTable;
	rt_n *lpIndex1;
	rt_n *lpIndex2;
	RT_ARRAY zzString1;
	RT_ARRAY zzString2;
	rt_char *lpString1;
	rt_char *lpString2;

	lpTable = (RT_TABLE*)context;

	lpIndex1 = (rt_n*)item1;
	lpIndex2 = (rt_n*)item2;

	lpString1 = (rt_char*)(((rt_uchar8*)lpTable->lpTableData) + (lpTable->lpTableMetadata->item_size * *lpIndex1));
	lpString2 = (rt_char*)(((rt_uchar8*)lpTable->lpTableData) + (lpTable->lpTableMetadata->item_size * *lpIndex2));

	rt_char_CreateString(&zzString1, lpString1);
	rt_char_CreateString(&zzString2, lpString2);
	*comparison_result = rt_char_CompareStrings(&zzString1, &zzString2);
	return RT_TRUE;
}

rt_comparison_callback_t cb_lpComparisonCallbacks[] = { &CbNameComparisonCallback };

RT_TABLE_METADATA cb_lpTablesMetadata[] = {
																						{ sizeof(CB_CLIMBER), sizeof(CB_CLIMBER_ITEM), 1, cb_lpComparisonCallbacks },
																						{ sizeof(CB_ROUTE),	 sizeof(CB_ROUTE_ITEM),	 1, cb_lpComparisonCallbacks },
																						{ sizeof(CB_GRADE),	 sizeof(CB_GRADE_ITEM),	 1, cb_lpComparisonCallbacks }
																					};

rt_s RT_CALL ZzMain()
{
	CB_COMPETITION competition;
	rt_b bTableCreated[CB_TABLES_COUNT];
	struct rt_runtime_heap runtimeHeap;
	rt_b bRuntimeHeapCreated;
	rt_un i;
	rt_s ret;

	bRuntimeHeapCreated = RT_FALSE;

	for (i = 0; i < CB_TABLES_COUNT; i++) {
		bTableCreated[i] = RT_FALSE;
	}

	if (!rt_runtime_heap_create(&runtimeHeap)) {
		rt_error_message_write_last(_R("Runtime heap creation failed: "));
		goto error;
	}
	bRuntimeHeapCreated = RT_TRUE;

	for (i = 0; i < CB_TABLES_COUNT; i++) {
		if (!rt_table_Create(&competition.lpTables[i], &cb_lpTablesMetadata[i], &runtimeHeap.heap)) {
			rt_error_message_write_last(_R("Tables creation failed: "));
			goto error;
		}
		bTableCreated[i] = RT_TRUE;
	}

	if (!CbManageMenu(&CbCompetitionHeaderCallback, cb_lpCompetitionMenuItems, 3, &competition)) goto error;

	ret = RT_OK;
free:
	for (i = 0; i < CB_TABLES_COUNT; i++) {
		if (bTableCreated[i]) {
			bTableCreated[i] = RT_FALSE;
			if (!rt_table_Free(&competition.lpTables[i]) && ret) {
				rt_error_message_write_last(_R("Failed to free tables: "));
				goto error;
			}
		}
	}
	if (bRuntimeHeapCreated) {
		bRuntimeHeapCreated = RT_FALSE;
		if (!runtimeHeap.heap->close(&runtimeHeap) && ret) {
			rt_error_message_write_last(_R("Failed to close runtime heap: "));
			goto error;
		}
	}

	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_un16 RT_CALL RtMain(rt_n32 argc, rt_char *argv[])
{
	rt_un16 result;
	if (ZzMain()) {
		result = 0;
	} else {
		result = 1;
	}
	return result;
}
