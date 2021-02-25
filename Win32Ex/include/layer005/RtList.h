#ifndef RT_LIST_H
#define RT_LIST_H

#include "layer000/rt_types.h"
#include "layer003/rt_heap.h"
#include "layer004/rt_array.h"

/**
 * A list is maintained as a set of arrays.<br>
 * Instead of storing items in a single huge array, they are stored in many small arrays.<br>
 * It permits to add new items fastly as there is no need to copy all existing items in a new array if
 * the current one is full.
 */

typedef struct _RT_LIST_HEADER {
	rt_un size;										 /* Count of items. The array_header contains the count of chunks. */
	rt_un chunk_size;
	rt_un item_size;							 /* Size of an item in the list. The array_header item_size is sizeof(void*).	*/
	struct rt_array_header array_header;
}
RT_LIST_HEADER;

/**
 * Allocate a list using given parameters.
 *
 * @param size Total items count in the list.
 * @param item_size Size of an item.
 * @param chunk_size Maximum items count in a chunk. Must be a power of two.
 */
void *rt_list_Create(void **lpList, struct rt_heap **heap, rt_un size, rt_un item_size, rt_un chunk_size);

/**
 * @return Total items count in the list.
 */
rt_un rt_list_GetSize(void *lpList);

void *rt_list_GetItem(void *lpList, rt_un item_index, void **item);

void *rt_list_SetSize(void **lpList, rt_un size);

/**
 * This function moves the last item if it is not the item to delete.
 * As a result, indexes on the list items might become invalid.
 */
void *rt_list_DeleteItemIndex(void **lpList, rt_un item_index);

void *rt_list_NewItem(void **lpList, void **item);

/**
 *
 * @return RT_TYPE_MAX_UN in case of failure.
 */
rt_un rt_list_NewItemIndex(void **lpList, rt_un *item_index);

/**
 * <p>
 * Set the list to RT_NULL.
 * </p>
 */
rt_s rt_list_Free(void **lpList);

#endif /* RT_LIST_H */
