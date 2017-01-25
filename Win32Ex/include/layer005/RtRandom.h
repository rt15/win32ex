#ifndef RT_RANDOM_H
#define RT_RANDOM_H

#include "layer000/RtWin32ExTypes.h"

/**
 * Fill given <tt>lpArea</tt> with <tt>nSize</tt> random bytes.
 */
RT_B RT_API RtGetRandomBytes(void* lpArea, RT_N32 nSize);

/**
 * Provide a random number (Which can be positive or negative.
 */
RT_B RT_API RtGetRandomNumber(RT_N* lpResult);

/**
 * Provide a random number between <tt>nLowerBound</tt> included and <tt>nUpperBound</tt> included.
 */
RT_B RT_API RtGetRandomNumberWithBoundaries(RT_N nLowerBound, RT_N nUpperBound, RT_N* lpResult);

#endif /* RT_RANDOM_H */