#ifndef RT_SLEEP_H
#define RT_SLEEP_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Allow to pause a thread or request a context switch.
 */

/**
 * Request a context switch.
 *
 * <p>
 * Ask the scheduler to execute another thread.
 * </p>
 */
void RT_API RtYeild();

void RT_API RtSleep(RT_UN32 unMilliseconds);

#endif /* RT_SLEEP_H */
