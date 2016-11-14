#include "layer003/RtSleep.h"

#include "layer001/RtWin32ExOsDefines.h"

void RT_API RtYeild()
{
#ifdef RT_DEFINE_WINDOWS
  Sleep(0);
#else
  /* Result is not checked as the Linux implementation always succeeds. */
  sched_yield();
#endif
}

void RT_API RtSleep(RT_UN32 unMilliseconds)
{
#ifndef RT_DEFINE_WINDOWS
  struct timespec requested;
#endif

#ifdef RT_DEFINE_WINDOWS
  Sleep(unMilliseconds);
#else
  /* Normalize, tv_nsec must be less than 999999999 (1 second). */
  requested.tv_sec = unMilliseconds / 1000;
  requested.tv_nsec = (unMilliseconds % 1000) * 1000000;
loop:
  /* Second argument is the remaining time if the function is interrupted. */
  if (nanosleep(&requested, &requested))
  {
    /* The nanosleep can be interrupted by a signal. Other errors are ignored. */
    if (errno == EINTR)
    {
      /* Interrupted, wait for remaining time. */
      goto loop;
    }
  }
#endif
}
