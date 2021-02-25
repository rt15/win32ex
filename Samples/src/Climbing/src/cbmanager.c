#include "cbmanager.h"

#include "cbmenu.h"
#include "cbtools.h"

#include "cbcompetition.h"

#define ZZ_MENU_ITEMS_COUNT 2

rt_s RT_CALL CbDisplayList(CB_MANAGER_CONTEXT *lpManagerContext, rt_b bWithIndex)
{
	RT_ARRAY rtMessage;
	rt_char buffer[256];
	RT_TABLE *lpTable;
	rt_n nObjectsCount;
	CB_OBJECT *lpObject;
	rt_n i;
	rt_s ret;

	lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

	rt_array_Create(&rtMessage, buffer, sizeof(rt_char), 256);

	CbWriteSeparator();
	nObjectsCount = rt_array_get_size(lpTable->lpTableData);
	for (i = 0; i < nObjectsCount; i++) {
		rt_array_get_item(lpTable->lpTableData, lpTable->lpTableIndexes[0].lpIndexes[i], (void**)&lpObject);
		if (bWithIndex) {
			if (!rt_char_append_n(i + 1, &rtMessage)) goto error;
			if (!rt_char_AppendCString(&rtMessage, _R(": "))) goto error;
		}

		if (!rt_char_AppendCString(&rtMessage, lpObject->lpName)) goto error;
		if (!rt_char_AppendCString(&rtMessage, _R("\n"))) goto error;

		if (!rt_console_write_string(&rtMessage)) goto error;
		if (!rt_array_SetSize(&rtMessage, 0)) goto error;
	}
	CbWriteSeparator();

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_s RT_CALL CbAddObject(void *context)
{
	CB_MANAGER_CONTEXT *lpManagerContext;
	RT_TABLE *lpTable;
	void **lpTableData;
	CB_OBJECT *lpObject;
	rt_char lpName[256];
	rt_un uread;
	rt_un output_size;
	rt_s ret;

	while (RT_TRUE) {
		rt_console_WriteCString(_R("\nEnter a name or nothing to quit:\n"));
		uread = rt_console_read_line(lpName, 256);

		/* Error. */
		if (uread == RT_TYPE_MAX_UN) goto error;

		/* Exit. */
		if (uread == 0) {
			break;
		}

		/* Too long name. */
		if (uread >= CB_NAME_SIZE) {
			rt_console_WriteCString(_R("Too long name.\n"));
		} else {
			lpManagerContext = (CB_MANAGER_CONTEXT*)context;
			lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];
			lpTableData = &lpTable->lpTableData;
			if (!rt_array_new_item(lpTableData, (void**)&lpObject)) goto error;
			rt_char_CopyStringWithSize(lpName, uread, lpObject->lpName, CB_NAME_SIZE, &output_size);
			if (!rt_table_IndexNewItem(lpTable)) goto error;
		}
	}

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_s RT_CALL CbDeleteObject(void *context)
{
	RT_ARRAY zzString;
	CB_MANAGER_CONTEXT *lpManagerContext;
	RT_TABLE *lpTable;
	rt_n nObjectsCount;
	rt_char lpIndex[256];
	rt_un index;
	rt_un uread;
	rt_s ret;

	lpManagerContext = (CB_MANAGER_CONTEXT*)context;
	lpTable = &lpManagerContext->lpCompetition->lpTables[lpManagerContext->nClass];

	while (RT_TRUE) {
		nObjectsCount = rt_array_get_size(lpTable->lpTableData);

		rt_console_WriteCString(_R("\nEnter an index or nothing to quit\n"));

		CbDisplayList(lpManagerContext, RT_TRUE);

		uread = rt_console_read_line(lpIndex, 256);

		/* Error. */
		if (uread == RT_TYPE_MAX_UN) goto error;

		/* Exit. */
		if (uread == 0) {
			break;
		}

		/* Parse index. */
		if (rt_char_convert_to_un(rt_char_CreateString(&zzString, lpIndex), &index)) {
			if (!index) {
				break;
			}
			if (index <= nObjectsCount) {
				/* Retrieve data index from table index. */
				index = lpTable->lpTableIndexes[0].lpIndexes[index - 1];

				if (!rt_table_DeleteItem(lpTable, index)) goto error;
			}
		}
	}

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

CB_MENU_ITEM cb_lpMenuItems[ZZ_MENU_ITEMS_COUNT] = {{_R("Add"), &CbAddObject}, {_R("Delete"), &CbDeleteObject}};

rt_s RT_CALL CbHeaderCallback(void *context)
{
	CB_MANAGER_CONTEXT *lpManagerContext;

	lpManagerContext = (CB_MANAGER_CONTEXT*)context;

	CbWriteMenuTitle(lpManagerContext->lpClassName);

	CbDisplayList(lpManagerContext, RT_FALSE);

	return RT_OK;
}

rt_s RT_CALL CbManageEntities(CB_MANAGER_CONTEXT *context)
{
	return CbManageMenu(&CbHeaderCallback, cb_lpMenuItems, ZZ_MENU_ITEMS_COUNT, context);
}
