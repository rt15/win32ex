#ifndef RT_RANDOM_H
#define RT_RANDOM_H

#include "layer000/RtWin32ExTypes.h"

/**
 * Fill given <tt>lpArea</tt> with <tt>unSize</tt> random bytes.
 */
RT_B RT_API RtGetRandomBytes(void* lpArea, RT_UN32 unSize);

/**
 * Provide a random integer (Which can be positive or negative.
 */
RT_B RT_API RtGetRandomInteger(RT_N* lpResult);

/**
 * Provide a random positive integer.
 */
RT_B RT_API RtGetRandomUInteger(RT_UN* lpResult);

/**
 * Provide a random positive integer between <tt>unLowerBound</tt> included and <tt>unUpperBound</tt> included.
 */
RT_B RT_API RtGetRandomUIntegerWithBoundaries(RT_UN unLowerBound, RT_UN unUpperBound, RT_UN* lpResult);

/**
 * Provide a random integer between <tt>nLowerBound</tt> included and <tt>nUpperBound</tt> included.
 */
RT_B RT_API RtGetRandomIntegerWithBoundaries(RT_N nLowerBound, RT_N nUpperBound, RT_N* lpResult);

#endif /* RT_RANDOM_H */
