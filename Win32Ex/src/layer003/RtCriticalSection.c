#include "layer003/RtCriticalsection.h"

#include "layer001/RtWin32ExOsDefines.h"

/**
 *
 * <p>
 * Only Linux provide a static initializer for mutex.<br>
 * There is no static initializer for Windows critical sections.
 * </p>
 */
void RT_API RtCreateCriticalSection(RT_CRITICAL_SECTION* lpCriticalSection, RT_B bRecursive)
{
#ifndef RT_DEFINE_WINDOWS
  pthread_mutexattr_t mutexAttributes;
#endif

#ifdef RT_DEFINE_WINDOWS
  InitializeCriticalSection((PCRITICAL_SECTION)lpCriticalSection);
#else
  pthread_mutexattr_init(&mutexAttributes);
  pthread_mutexattr_settype(&mutexAttributes, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init((pthread_mutex_t*)lpCriticalSection, &mutexAttributes);
#endif
}

void RT_API RtEnterCriticalSection(RT_CRITICAL_SECTION* lpCriticalSection)
{
#ifdef RT_DEFINE_WINDOWS
  EnterCriticalSection((PCRITICAL_SECTION)lpCriticalSection);
#else
  /* We don't check returned value but we assume that. */
  pthread_mutex_lock((pthread_mutex_t*)lpCriticalSection);
#endif
}

void RT_API RtLeaveCriticalSection(RT_CRITICAL_SECTION* lpCriticalSection)
{
#ifdef RT_DEFINE_WINDOWS
  LeaveCriticalSection((PCRITICAL_SECTION)lpCriticalSection);
#else
  /* We don't check returned value but we assume that. */
  pthread_mutex_unlock((pthread_mutex_t*)lpCriticalSection);
#endif
}

void RT_API RtFreeCriticalSection(RT_CRITICAL_SECTION* lpCriticalSection)
{
#ifdef RT_DEFINE_WINDOWS
  DeleteCriticalSection((PCRITICAL_SECTION)lpCriticalSection);
#else
  pthread_mutex_destroy((pthread_mutex_t*)lpCriticalSection);
#endif
}
