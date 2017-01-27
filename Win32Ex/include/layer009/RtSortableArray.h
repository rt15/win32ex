#ifndef RT_SORTABLE_ARRAY_H
#define RT_SORTABLE_ARRAY_H

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtComparisonCallback.h"
#include "layer008/RtArray.h"

typedef struct _RT_SORTABLE_ARRAY_HEADER
{
  RT_B bSorted;
  RT_COMPARISON_CALLBACK lpComparisonCallback;
  void* lpContext;
  RT_ARRAY_HEADER rtArrayHeader;
}
RT_SORTABLE_ARRAY_HEADER;

void* RT_API RtCreateSortableArray(void** lpSortableArray, RT_HEAP** lpHeap, RT_UN unSize, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext);

/**
 * Add a new item to the sortable array, in its right place.<br>
 * The items placed after the insertion point are moved forward.
 *
 * <p>
 * Giving the item outside of the array is not an issue as there is a copy in most cases.
 * </p>
 *
 * @return The index at which the item has been inserted, RT_TYPE_MAX_UN in case of failure.
 */
RT_UN RT_API RtAddItemToSortableArray(void** lpSortableArray, void* lpNewItem);

/**
 * Delete an item of the array by overwriting it with the end of the array.<br>
 * The array remains sorted.
 */
void* RT_API RtDeleteSortableArrayItemIndex(void** lpSortableArray, RT_UN unItemIndex);

/**
 * <p>
 * Can fails only if the comparison callback fails.
 * </p>
 */
RT_B RT_API RtSortSortableArray(void* lpSortableArray);

/**
 * Mark the array as not sorted anymore.
 *
 * <p>
 * Must be called when performing an operation that breaks the sorting.
 * </p>
 */
RT_B RT_API RtUnsortSortableArray(void* lpSortableArray);

/**
 * Search lpItem in lpSortableArray.<br>
 * Sort the array if it is not already sorted.
 *
 * <p>
 * Can fails only if the comparison callback fails.
 * </p>
 *
 */
RT_B RT_API RtSearchSortableArrayItemIndex(void* lpSortableArray, void* lpItem, RT_UN* lpItemIndex);

#endif /* RT_SORTABLE_ARRAY_H */
