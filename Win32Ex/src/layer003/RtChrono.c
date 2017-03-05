#include "layer003/RtChrono.h"

#include "layer001/RtWin32ExOsDefines.h"

#ifdef RT_DEFINE_LINUX

/**
 * Subtract the struct timespec values lpX and lpY, storing the result in lpResult.
 */
void RT_CALL RtSubstractTimespecs(struct timespec *lpX, struct timespec *lpY, struct timespec *lpResult)
{
  struct timespec rtY;
  int nSec;

  /* Perform a copy of Y as we will adjust it. */
  rtY = *lpY;

  /* Ensure that lpX->tv_nsec is greater than rtY.tv_nsec. */
  if (lpX->tv_nsec < rtY.tv_nsec)
  {
    /* Compute seconds that will be removed from rtY.tv_nsec. */
    nSec = (rtY.tv_nsec - lpX->tv_nsec) / 1000000000 + 1;
    /* Remove seconds from rtY.tv_nsec. */
    rtY.tv_nsec -= 1000000000 * nSec;
    /* Add them back to rtY.tv_sec. */
    rtY.tv_sec += nSec;
  }
  /* Ensure that lpResult->tv_nsec will be less than a second. */
  if (lpX->tv_nsec - rtY.tv_nsec > 1000000000)
  {
    /* Compute the surplus seconds. */
    nSec = (lpX->tv_nsec - rtY.tv_nsec) / 1000000000;
    /* Add them to rtY.tv_nsec. */
    rtY.tv_nsec += 1000000000 * nSec;
    /* Remove them of rtY.tv_sec. It might become negative but we don't care. */
    rtY.tv_sec -= nSec;
  }

  lpResult->tv_sec = lpX->tv_sec - rtY.tv_sec;
  lpResult->tv_nsec = lpX->tv_nsec - rtY.tv_nsec;
}

#endif

RT_B RT_API RtCreateChrono(RT_CHRONO* lpChrono)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!QueryPerformanceCounter((LARGE_INTEGER*)lpChrono)) goto handle_error;
#else
  /* clock_gettime returns -1 and set errno in case of failure. */
  if (clock_gettime(CLOCK_MONOTONIC_RAW, (struct timespec*)lpChrono) == -1) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtGetChrono(RT_CHRONO* lpChrono, RT_UN* lpMicroSeconds)
{
#ifdef RT_DEFINE_WINDOWS
  LARGE_INTEGER* lpStartCounter;
  LARGE_INTEGER rtFrequency;
  LARGE_INTEGER rtCounterValue;
#else
  struct timespec* lpStartCounter;
  struct timespec rtCounterValue;
  struct timespec rtDifference;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  lpStartCounter = (LARGE_INTEGER*)lpChrono;

  if (!QueryPerformanceCounter(&rtCounterValue)) goto handle_error;

  /* Beware that returned frequency can be greater than processor frequency and RT_TYPE_MAX_UN32. */
  if (!QueryPerformanceFrequency(&rtFrequency)) goto handle_error;

  *lpMicroSeconds = (RT_UN)((rtCounterValue.QuadPart - lpStartCounter->QuadPart) * 1000000 / rtFrequency.QuadPart);
#else
  lpStartCounter = (struct timespec*)lpChrono;

  /* clock_gettime returns -1 and set errno in case of failure. */
  if (clock_gettime(CLOCK_MONOTONIC_RAW, &rtCounterValue) == -1) goto handle_error;

  RtSubstractTimespecs(&rtCounterValue, lpStartCounter, &rtDifference);

   /* Nano to micro and seconds to micro. */
  *lpMicroSeconds = rtDifference.tv_sec * 1000000 + rtDifference.tv_nsec / 1000;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  *lpMicroSeconds = 0;
  bResult = RT_FAILURE;
  goto free_resources;
}
