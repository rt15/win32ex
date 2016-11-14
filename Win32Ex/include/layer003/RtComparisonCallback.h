#ifndef RT_COMPARISON_CALLBACK_H
#define RT_COMPARISON_CALLBACK_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Comparison callback used in some other headers.
 */

typedef RT_B (RT_CALL *RT_COMPARISON_CALLBACK)(void* lpItem1, void* lpItem2, void* lpContext, RT_N* lpComparisonResult);

/**
 * A comparison callback implementation that assume that items are starting with a RT_N and that it must be used to sort them.
 */
RT_B RT_API RtDefaultCompareCallback(void* lpItem1, void* lpItem2, void* lpContext, RT_N* lpComparisonResult);

#endif /* RT_COMPARISON_CALLBACK_H */
