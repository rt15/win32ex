#ifndef RT_INITIALIZATION_H
#define RT_INITIALIZATION_H

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtCriticalsection.h"

/**
 * @file
 * Time consuming initialization management.
 */

typedef struct _RT_INITIALIZATION
{
  RT_UN unInitialized;
  RT_CRITICAL_SECTION rtCriticalSection;
}
RT_INITIALIZATION;

void RT_API RtCreateInitialization(RT_INITIALIZATION* lpInitialization);
RT_B RT_API RtInitializationRequired(RT_INITIALIZATION* lpInitialization);
void RT_API RtNotifyInitializationDone(RT_INITIALIZATION* lpInitialization);
void RT_API RtFreeInitialization(RT_INITIALIZATION* lpInitialization);

#endif /* RT_INITIALIZATION_H */
