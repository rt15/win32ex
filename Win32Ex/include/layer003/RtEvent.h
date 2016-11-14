#ifndef RT_EVENT_H
#define RT_EVENT_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Simple events usable within a single process.<br>
 * Only a single thread can wait for a single event.
 */

typedef struct _RT_EVENT
{
#ifdef RT_DEFINE_WINDOWS
  RT_H hEvent;
#else
  RT_N32 nFileDescriptor;
#endif
}
RT_EVENT;

RT_B RT_API RtCreateEvent(RT_EVENT* lpEvent);

RT_B RT_API RtSignalEvent(RT_EVENT* lpEvent);

RT_B RT_API RtWaitForEvent(RT_EVENT* lpEvent);

RT_B RT_API RtFreeEvent(RT_EVENT* lpEvent);

#endif /* RT_EVENT_H */
