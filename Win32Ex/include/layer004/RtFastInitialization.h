#ifndef RT_FAST_INITIALIZATION_H
#define RT_FAST_INITIALIZATION_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Help to manage fast lazy initialization.
 *
 * <p>
 * Sometimes, in a multithreaded environment, some initialization code must be executed once by the first thread that reach this code.<br>
 * Functions provided in this method ensure that only the first thread that executes RtFastInitializationRequired will return RT_TRUE.<br>
 * Then all others threads executing RtFastInitializationRequired will wait in an infinite loop until the first thread calls RtNotifyFastInitializationDone, then RT_FALSE is returned.<br>
 * Beware that the infinite loop will consume a lot of CPU.<br>
 * Call RtInitializationRequired for time consuming initialization code.<br>
 * Be sure to initialize the RT_FAST_INITIALIZATION structure with RT_FAST_INITIALIZATION_STATIC_INIT.
 * </p>
 */

typedef struct _RT_FAST_INITIALIZATION
{
  RT_UN unInitialized;
  RT_UN unInitializing;
}
RT_FAST_INITIALIZATION;

/**
 * Used to initialize a <tt>RT_FAST_INITIALIZATION</tt> structure.
 */
#define RT_FAST_INITIALIZATION_STATIC_INIT {0, 0}

RT_B RT_API RtFastInitializationRequired(RT_FAST_INITIALIZATION* lpFastInitialization);
void RT_API RtNotifyFastInitializationDone(RT_FAST_INITIALIZATION* lpFastInitialization);
RT_B RT_API RtIsFastInitializationDone(RT_FAST_INITIALIZATION* lpFastInitialization);

#endif /* RT_FAST_INITIALIZATION_H */
