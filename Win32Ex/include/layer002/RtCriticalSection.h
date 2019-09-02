#ifndef RT_CRITICAL_SECTION_H
#define RT_CRITICAL_SECTION_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Critical section implementation for Windows and Linux.
 *
 * <p>
 * A critical section is a portion of instructions that can be executed by only one thread at a given time.
 * </p>
 */

/*
typedef struct _CRITICAL_SECTION {
  PCRITICAL_SECTION_DEBUG DebugInfo;
  LONG LockCount;
  LONG RecursionCount;
  HANDLE OwningThread;
  HANDLE LockSemaphore;
  DWORD SpinCount;
} CRITICAL_SECTION,*PCRITICAL_SECTION,*LPCRITICAL_SECTION;
*/



typedef struct _RT_CRITICAL_SECTION
{
#ifdef RT_DEFINE_WINDOWS
  void*   lpDebugInfo;
  RT_N32  nLockCount;
  RT_N32  nRecursionCount;
  RT_H    hOwningThread;
  RT_H    hLockSemaphore;
  RT_UN32 unSpinCount;
#else
  /* TODO: Fix
  pthread_mutex_t rtCriticalSection;
  */
  RT_CHAR8 lpCriticalSection[200];
#endif
}
RT_CRITICAL_SECTION;

/**
 *
 * <p>
 * A recursive critical section can be recursively executed by the same thread.<br>
 * Under Linux, recursive critical sections are slower than non-recursive critical sections.<br>
 * Windows only manage recursive critical sections.
 * </p>
 */
void RT_API RtCriticalSection_Create(RT_CRITICAL_SECTION* lpCriticalSection, RT_B bRecursive);
void RT_API RtCriticalSection_Enter(RT_CRITICAL_SECTION* lpCriticalSection);
void RT_API RtCriticalSection_Leave(RT_CRITICAL_SECTION* lpCriticalSection);
void RT_API RtCriticalSection_Free(RT_CRITICAL_SECTION* lpCriticalSection);

#endif /* RT_CRITICAL_SECTION_H */
