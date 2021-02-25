#include <rpr.h>

typedef struct _ZZ_LIST_ITEM {
	rt_un32 value;
	rt_char value[32];
}
ZZ_LIST_ITEM;

rt_s RT_CALL ZzTestDisplayList(void *lpList)
{
	RT_ARRAY rtMessage;
	rt_char message[512];
	RT_ARRAY rtConversionBuffer;
	rt_char lpConversionBuffer[64];
	RT_ARRAY rtString;
	rt_un size;
	ZZ_LIST_ITEM *item;
	rt_b first;
	rt_un i;
	rt_s ret;

	first = RT_TRUE;
	size = rt_list_GetSize(lpList);

	rt_array_Create(&rtMessage, message, sizeof(rt_char), 512);
	rt_char_CreateString(&rtString, _R(", "));
	rt_array_Create(&rtConversionBuffer, lpConversionBuffer, sizeof(rt_char), 64);

	for (i = 0; i < size; i++) {
		if (first) {
			first = RT_FALSE;
		} else {
			if (!rt_array_Append(&rtMessage, &rtString)) goto error;
		}

		rt_list_GetItem(lpList, i, (void**)&item);
		if (!rt_char_append_un(item->value, &rtConversionBuffer)) goto error;
		if (!rt_array_Append(&rtMessage, &rtConversionBuffer)) goto error;
	}

	if (!rt_char_AppendCString(&rtMessage, _R("\n"))) goto error;

	if (!rt_console_write_string(&rtMessage)) goto error;

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_s RT_CALL ZzTestCheckList(void *lpList, rt_un unExpectedSize, rt_un unExpectedItemSize, rt_un unExpectedChunkSize, rt_un unExpectedChunksCount)
{
	RT_ARRAY rtMessage;
	rt_char message[512];
	RT_ARRAY rtConversionBuffer;
	rt_char lpConversionBuffer[64];
	rt_un size;
	rt_un item_size;
	rt_un chunk_size;
	rt_un unChunksCount;
	RT_LIST_HEADER *lpListHeader;
	rt_s ret;

	lpListHeader = lpList;
	lpListHeader--;

	size = lpListHeader->size;
	item_size = lpListHeader->item_size;
	chunk_size = lpListHeader->chunk_size;
	unChunksCount = lpListHeader->array_header.size;

	rt_array_Create(&rtMessage, message, sizeof(rt_char), 512);
	rt_array_Create(&rtConversionBuffer, lpConversionBuffer, sizeof(rt_char), 64);

	/* Size. */
	if (!rt_char_AppendCString(&rtMessage, _R("List size = "))) goto error;
	if (!rt_char_append_un(size, &rtConversionBuffer)) goto error;
	if (!rt_array_Append(&rtMessage, &rtConversionBuffer)) goto error;

	/* Item size. */
	if (!rt_char_AppendCString(&rtMessage, _R(", itemSize = "))) goto error;
	if (!rt_char_append_un(item_size, &rtConversionBuffer)) goto error;
	if (!rt_array_Append(&rtMessage, &rtConversionBuffer)) goto error;

	/* Chunk size. */
	if (!rt_char_AppendCString(&rtMessage, _R(", chunkSize = "))) goto error;
	if (!rt_char_append_un(chunk_size, &rtConversionBuffer)) goto error;
	if (!rt_array_Append(&rtMessage, &rtConversionBuffer)) goto error;

	/* Chunks count. */
	if (!rt_char_AppendCString(&rtMessage, _R(", chunksCount = "))) goto error;
	if (!rt_char_append_un(unChunksCount, &rtConversionBuffer)) goto error;
	if (!rt_array_Append(&rtMessage, &rtConversionBuffer)) goto error;

	if (!rt_char_AppendCString(&rtMessage, _R("\n"))) goto error;
	if (!rt_console_write_string(&rtMessage)) goto error;

	if (unExpectedSize != size) goto error;
	if (unExpectedItemSize != item_size) goto error;
	if (unExpectedChunkSize != chunk_size) goto error;
	if (unExpectedChunksCount != unChunksCount) goto error;

	ret = RT_OK;
free:
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}

rt_s RT_CALL ZzTestList(struct rt_heap **heap)
{
	void *lpList;
	rt_un32 i;
	ZZ_LIST_ITEM *item;
	rt_un item_index;
	rt_un output_size;
	rt_un item_size;
	rt_s ret;

	lpList = RT_NULL;

	item_size = sizeof(rt_un32) + 32 * sizeof(rt_char);

	if (!rt_list_Create(&lpList, heap, 35, sizeof(ZZ_LIST_ITEM), 16)) goto error;

	if (!ZzTestCheckList(lpList, 35, item_size, 16, 3)) goto error;

	for (i = 0; i < 35; i++) {
		rt_list_GetItem(lpList, i, (void**)&item);
		item->value = i;
		rt_char_CopyString(_R("This is item characters."), item->value, 32, &output_size);
	}

	if (!ZzTestDisplayList(lpList)) goto error;

	/* Reduce the size of the list. */
	if (!rt_list_SetSize(&lpList, 29)) goto error;


	if (!ZzTestCheckList(lpList, 29, item_size, 16, 2)) goto error;
	if (!ZzTestDisplayList(lpList)) goto error;

	/* Increase the size of the list. */
	if (!rt_list_SetSize(&lpList, 49)) goto error;

	if (!ZzTestCheckList(lpList, 49, item_size, 16, 4)) goto error;

	for (i = 0; i < 49; i++) {
		rt_list_GetItem(lpList, i, (void**)&item);
		item->value = i;
	}

	if (!ZzTestDisplayList(lpList)) goto error;

	if (!rt_list_DeleteItemIndex(&lpList, 12)) goto error;

	if (!ZzTestDisplayList(lpList)) goto error;

	if (!rt_list_NewItem(&lpList, (void**)&item)) goto error;
	item->value = 42;

	if (!ZzTestDisplayList(lpList)) goto error;

	if (rt_list_NewItemIndex(&lpList, &item_index) == RT_TYPE_MAX_UN) goto error;

	if (!rt_list_GetItem(lpList, item_index, (void**)&item)) goto error;

	ret = RT_OK;
free:
	if (lpList) {
		if (!rt_list_Free(&lpList) && ret)
			goto error;
	}
	return ret;

error:
	ret = RT_FAILED;
	goto free;
}
