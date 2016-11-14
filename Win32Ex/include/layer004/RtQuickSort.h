#ifndef RT_QUICK_SORT_H
#define RT_QUICK_SORT_H

/**
 * Quick sort implementation based on Microsoft qsort.
 */

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtComparisonCallback.h"

/**
 * <p>
 * Can fails only if the comparison callback fails.
 * </p>
 */
RT_B RT_API RtQuickSort(void* lpArea, RT_UN unSize, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext);

#endif /* RT_QUICK_SORT_H */
