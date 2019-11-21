#ifndef RT_LIST_H
#define RT_LIST_H

#include "layer000/RtWin32ExTypes.h"
#include "layer001/RtHeap.h"
#include "layer004/RtArray.h"

/**
 * A list is maintained as a set of arrays.<br>
 * Instead of storing items in a single huge array, they are stored in many small arrays.<br>
 * It permits to add new items fastly as there is no need to copy all existing items in a new array if
 * the current one is full.
 */

typedef struct _RT_LIST_HEADER
{
  RT_UN unSize;                     /* Count of items. The rtArrayHeader contains the count of chunks. */
  RT_UN unChunkSize;
  RT_UN unItemSize;               /* Size of an item in the list. The rtArrayHeader unItemSize is sizeof(void*).  */
  RT_ARRAY_HEADER rtArrayHeader;
}
RT_LIST_HEADER;

/**
 * Allocate a list using given parameters.
 *
 * @param unSize Total items count in the list.
 * @param unItemSize Size of an item.
 * @param unChunkSize Maximum items count in a chunk. Must be a power of two.
 */
void* RT_API RtList_Create(void** lpList, RT_HEAP** lpHeap, RT_UN unSize, RT_UN unItemSize, RT_UN unChunkSize);

/**
 * @return Total items count in the list.
 */
RT_UN RT_API RtList_GetSize(void* lpList);

void* RT_API RtList_GetItem(void* lpList, RT_UN unItemIndex, void** lpItem);

void* RT_API RtList_SetSize(void** lpList, RT_UN unSize);

/**
 * This function moves the last item if it is not the item to delete.
 * As a result, indexes on the list items might become invalid.
 */
void* RT_API RtList_DeleteItemIndex(void** lpList, RT_UN unItemIndex);

void* RT_API RtList_NewItem(void** lpList, void** lpItem);

/**
 *
 * @return RT_TYPE_MAX_UN in case of failure.
 */
RT_UN RT_API RtList_NewItemIndex(void** lpList, RT_UN* lpItemIndex);

/**
 * <p>
 * Set the list to RT_NULL.
 * </p>
 */
RT_B RT_API RtList_Free(void** lpList);

#endif /* RT_LIST_H */
