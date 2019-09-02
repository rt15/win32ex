#ifndef RT_FAST_INITIALIZATION_H
#define RT_FAST_INITIALIZATION_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Help to manage fast lazy initialization.
 *
 * <p>
 * Sometimes, in a multithreaded environment, some initialization code must be executed once by the first thread that reach this code.<br>
 * Functions provided in this method ensure that only the first thread that executes RtFastInitialization_IsRequired will return RT_TRUE.<br>
 * Then all others threads executing RtFastInitialization_IsRequired will wait in an infinite loop until the first thread calls RtFastInitialization_NotifyDone, then RT_FALSE is returned.<br>
 * Beware that the infinite loop will consume a lot of CPU.<br>
 * Call RtInitialization_IsRequired for time consuming initialization code.<br>
 * Be sure to initialize the RT_FAST_INITIALIZATION structure with RT_FAST_INITIALIZATION_STATIC_INIT.
 * </p>
 *
 * <p>
 * A big advantage of fast initialization is that there is no need for constructor/destructor.<br>
 * As a result, it can be used for classes without constructor/destructor.
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

RT_B RT_API RtFastInitialization_IsRequired(RT_FAST_INITIALIZATION* lpFastInitialization);
void RT_API RtFastInitialization_NotifyDone(RT_FAST_INITIALIZATION* lpFastInitialization);
RT_B RT_API RtFastInitialization_IsDone(RT_FAST_INITIALIZATION* lpFastInitialization);

#endif /* RT_FAST_INITIALIZATION_H */
