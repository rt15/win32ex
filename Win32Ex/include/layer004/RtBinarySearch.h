#ifndef RT_BINARY_SEARCH_H
#define RT_BINARY_SEARCH_H

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtComparisonCallback.h"

/**
 * @file
 * Binary search implementation.
 */

/**
 * Search for lpItem in lpArea.<br>
 * lpArea must be sorted.
 *
 * <p>
 * Can fails only if the comparison callback fails.
 * </p>
 *
 * @param lpItemIndex RT_TYPE_MAX_UN if the item is not found, the item index otherwise.
 */
RT_B RT_API RtBinarySearchIndex(void* lpArea, void* lpItem, RT_UN unSize, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext, RT_UN* lpItemIndex);

/**
 * Search index corresponding to lpItem in lpArea.<br>
 * lpArea must be sorted.
 *
 * <p>
 * Can fails only if the comparison callback fails.
 * </p>
 *
 * @return The index of searched lpItem or an index to which insert it, RT_TYPE_MAX_UN in case of issue.
 */
RT_UN RT_API RtBinarySearchInsertionIndex(void* lpArea, void* lpItem, RT_UN unSize, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext);


#endif /* RT_BINARY_SEARCH_H */
