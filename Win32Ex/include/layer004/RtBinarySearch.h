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
 * @param lpItemIndex -1 if the item is not found, the item index otherwise.
 */
RT_B RT_API RtBinarySearchIndex(void* lpArea, void* lpItem, RT_UN32 unSize, RT_UN32 unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext, RT_N* lpItemIndex);

/**
 * Search index corresponding to lpItem in lpArea.<br>
 * lpArea must be sorted.
 *
 * <p>
 * Can fails only if the comparison callback fails.
 * </p>
 *
 * @return The index of searched lpItem or an index to which insert it, -1 in case of issue.
 */
RT_N RT_API RtBinarySearchInsertionIndex(void* lpArea, void* lpItem, RT_UN32 unSize, RT_UN32 unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext);


#endif /* RT_BINARY_SEARCH_H */
