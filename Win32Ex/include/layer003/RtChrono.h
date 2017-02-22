#ifndef RT_CHRONO_H
#define RT_CHRONO_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Can be used to measure time durations.
 */


typedef struct _RT_CHRONO
{
#ifdef RT_DEFINE_WINDOWS
  /* LARGE_INTEGER. */
  RT_UN64 nStartCounter;
#else
  /* timespec (time_t and long). */
  RT_N nSeconds;
  RT_N nNanoSeconds;
#endif
}
RT_CHRONO;

RT_B RT_API RtCreateChrono(RT_CHRONO* lpChrono);
RT_B RT_API RtGetChrono(RT_CHRONO* lpChrono, RT_UN* lpMicroSeconds);

#endif /* RT_CHRONO_H */
