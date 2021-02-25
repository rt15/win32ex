#include "layer005/rt_list.h"

#include "layer001/rt_memory.h"
#include "layer002/rt_error.h"

void *rt_list_Create(void **lpList, struct rt_heap **heap, rt_un size, rt_un item_size, rt_un chunk_size)
{
	RT_LIST_HEADER *header;
	rt_un unChunksCount;
	void **lpChunks;
	rt_un i;

	lpChunks = RT_NULL;
	unChunksCount = 0;

	if (!RT_MEMORY_IS_POWER_OF_TWO(chunk_size)) {
		rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
		goto error;
	}

	/* Compute chunks count. */
	unChunksCount = RT_MEMORY_GET_CHUNKS_COUNT(size, chunk_size);
	if (unChunksCount == RT_TYPE_MAX_UN) {
		goto error;
	}

	/* Create array of pointers on chunks. */
	if (rt_array_create(lpList, sizeof(RT_LIST_HEADER), heap, unChunksCount, sizeof(void*))) {
		header = *lpList;
		header--;
		header->size = size;
		header->chunk_size = chunk_size;
		header->item_size = item_size;
		lpChunks = (void**)*lpList;
	} else {
		goto error;
	}

	/* Initialize all pointers to be able to clean memory in case of issue. */
	for (i = 0; i < unChunksCount; i++) {
		lpChunks[i] = RT_NULL;
	}

	/* Allocate all chunks. */
	for (i = 0; i < unChunksCount; i++) {
		if (!(*heap)->alloc(heap, (void**)&lpChunks[i], chunk_size * item_size, _R("Chunk"))) {
			goto error;
		}
	}

	goto free;
error:
	if (lpChunks) {
		/* Some chunks might have been allocated. */
		for (i = 0; i < unChunksCount; i++) {
			if (lpChunks[i]) {
				/* Free chunk. */
				(*heap)->free(heap, &lpChunks[i]);
			} else {
				/* All remaining chunks have not been allocated. */
				break;
			}
		}
		/* Free chunks pointers. */
		rt_array_free((void**)&lpChunks);
	}
	*lpList = RT_NULL;
free:
	return *lpList;
}

rt_un rt_list_GetSize(void *lpList)
{
	RT_LIST_HEADER *lpListHeader;

	lpListHeader = lpList;
	lpListHeader--;
	return lpListHeader->size;
}

void *rt_list_GetItem(void *lpList, rt_un item_index, void **item)
{
	RT_LIST_HEADER *lpListHeader;
	void **lpChunks;
	rt_un unChunksCount;
	rt_un unChunkIndex;
	rt_un chunk_size;
	rt_un item_size;
	rt_un unItemIndexInChunk;

	lpListHeader = lpList;
	lpListHeader--;

	unChunksCount = lpListHeader->array_header.size;
	chunk_size = lpListHeader->chunk_size;
	item_size = lpListHeader->item_size;

	/* Find the chunk index. */
	unChunkIndex = item_index / chunk_size;
	if (unChunkIndex >= unChunksCount) {
		rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
		*item = RT_NULL;
		goto free;
	}

	/* Find the item. */
	unItemIndexInChunk = (RT_MEMORY_MODULO_POWER_OF_TWO(item_index, chunk_size));
	lpChunks = (void**)lpList;
	*item = ((rt_uchar8*)lpChunks[unChunkIndex]) + unItemIndexInChunk * item_size;
free:
	return *item;
}

void *rt_list_SetSize(void **lpList, rt_un size)
{
	rt_un unCurrentChunksCount;
	RT_LIST_HEADER *lpListHeader;
	rt_un chunk_size;
	rt_un unNewChunksCount;
	rt_un item_size;
	struct rt_heap **heap;
	void **lpChunks;
	rt_un i;

	lpListHeader = *lpList;
	lpListHeader--;
	unCurrentChunksCount = lpListHeader->array_header.size;
	chunk_size = lpListHeader->chunk_size;
	heap = lpListHeader->array_header.heap;
	item_size = lpListHeader->item_size;

	/* Compute new chunks count. */
	unNewChunksCount = RT_MEMORY_GET_CHUNKS_COUNT(size, chunk_size);
	if (unNewChunksCount == RT_TYPE_MAX_UN) {
		goto error;
	}

	/* If we need to add or remove chunks. */
	if (unCurrentChunksCount != unNewChunksCount) {
		/* Size reduced. */
		if (unNewChunksCount < unCurrentChunksCount) {
			lpChunks = (void**)*lpList;

			/* Remove existing chunks. */
			for (i = unNewChunksCount; i < unCurrentChunksCount; i++) {
				if (!(*heap)->free(heap, &lpChunks[i])) {
					lpChunks[i] = RT_NULL;
					goto error;
				}
				lpChunks[i] = RT_NULL;
			}

			/* Reduce chunks array size. TODO: There can be a memory leak of chunks. */
			if (!rt_array_set_size(lpList, unNewChunksCount))
				goto free;
		} else {
			/* Size increased. We need more chunks. TODO: There can be a memory leak of chunks. */
			if (!rt_array_set_size(lpList, unNewChunksCount))
				goto free;

			lpChunks = (void**)*lpList;

			/* Allocate new chunks. */
			for (i = unCurrentChunksCount; i < unNewChunksCount; i++) {
				if (!(*heap)->alloc(heap, (void**)&lpChunks[i], chunk_size * item_size, _R("Chunk"))) {
					goto error;
				}
			}
		}
	}

	/* Set the new size. */
	if (*lpList) {
		lpListHeader = *lpList;
		lpListHeader--;
		lpListHeader->size = size;
	}
	goto free;
error:
	if (*lpList) {
		rt_list_Free(lpList);
	}
free:
	return *lpList;
}

void *rt_list_DeleteItemIndex(void **lpList, rt_un item_index)
{
	RT_LIST_HEADER *lpListHeader;
	rt_un item_size;
	rt_un last_item_index;
	void *lpLastItem;
	void *lpItemToDelete;

	lpListHeader = *lpList;
	lpListHeader--;
	item_size = lpListHeader->item_size;
	last_item_index = lpListHeader->size - 1;
	if (item_index != last_item_index) {
		/* Copy last item into item to delete. */
		/* TODO: Manage errors in two next lines. */
		rt_list_GetItem(*lpList, last_item_index, &lpLastItem);
		rt_list_GetItem(*lpList, item_index, &lpItemToDelete);
		RT_MEMORY_COPY(lpLastItem, lpItemToDelete, item_size);
	}

	return rt_list_SetSize(lpList, last_item_index);
}

void *rt_list_NewItem(void **lpList, void **item)
{
	RT_LIST_HEADER *lpListHeader;
	rt_un size;

	lpListHeader = *lpList;
	lpListHeader--;

	/* Increase the size of the list. */
	size = lpListHeader->size;
	if (!rt_list_SetSize(lpList, size + 1)) {
		*item = RT_NULL;
		goto free;
	}

	rt_list_GetItem(*lpList, size, item);

free:
	return *item;
}

rt_un rt_list_NewItemIndex(void **lpList, rt_un *item_index)
{
	RT_LIST_HEADER *lpListHeader;
	rt_un size;

	lpListHeader = *lpList;
	lpListHeader--;

	/* Increase the size of the list. */
	size = lpListHeader->size;
	if (!rt_list_SetSize(lpList, size + 1)) {
		*item_index = RT_TYPE_MAX_UN;
		goto free;
	}

	*item_index = size;

free:
	return *item_index;
}


rt_s rt_list_Free(void **lpList)
{
	RT_LIST_HEADER *lpListHeader;
	rt_un unChunksCount;
	void **lpChunks;
	struct rt_heap **heap;
	rt_un i;
	rt_s ret;

	ret = RT_OK;

	if (*lpList) {
		lpListHeader = *lpList;
		lpListHeader--;

		unChunksCount = lpListHeader->array_header.size;
		heap = lpListHeader->array_header.heap;

		/* Free all chunks. */
		lpChunks = (void**)*lpList;
		for (i = 0; i < unChunksCount; i++) {
			if (!(*heap)->free(heap, &lpChunks[i])) {
				ret = RT_FAILED;
			}
		}

		/* Free pointers array. */
		if (!rt_array_free(lpList)) {
			ret = RT_FAILED;
		}
	}

	return ret;
}
