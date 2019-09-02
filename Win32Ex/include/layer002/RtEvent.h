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

RT_B RT_API RtEvent_Create(RT_EVENT* lpEvent);

RT_B RT_API RtEvent_Signal(RT_EVENT* lpEvent);

RT_B RT_API RtEvent_WaitFor(RT_EVENT* lpEvent);

RT_B RT_API RtEvent_Free(RT_EVENT* lpEvent);

#endif /* RT_EVENT_H */
