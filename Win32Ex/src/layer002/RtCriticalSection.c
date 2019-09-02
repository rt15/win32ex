#include "layer002/RtCriticalsection.h"

#include "layer001/RtWin32ExOsHeaders.h"

/**
 *
 * <p>
 * Only Linux provide a static initializer for mutex.<br>
 * There is no static initializer for Windows critical sections.<br>
 * The consequence is that we need a constructor while it can be annoying in some case.
 * </p>
 *
 * <p>
 * Fast initialization could be used in RtCriticalSection_Enter to avoid having to call RtCriticalSection_Create.<br>
 * However, the user would still have to manage the lifecycle of the critical section through RtCriticalSection_Free.<br>
 * So the overhead of fast initialization in RtCriticalSection_Enter would not worth the price.
 * </p>
 *
 * <p>
 * It should not be necessary to optimize RtCriticalSection_Enter with atomic operations to avoid kernel calls.<br>
 * EnterCriticalSection/pthread_mutex_lock should already use equivalent user mode strategies before calling the kernel.
 * </p>
 */
void RT_API RtCriticalSection_Create(RT_CRITICAL_SECTION* lpCriticalSection, RT_B bRecursive)
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

void RT_API RtCriticalSection_Enter(RT_CRITICAL_SECTION* lpCriticalSection)
{
#ifdef RT_DEFINE_WINDOWS
  EnterCriticalSection((PCRITICAL_SECTION)lpCriticalSection);
#else
  /* We don't check returned value but we assume that. */
  pthread_mutex_lock((pthread_mutex_t*)lpCriticalSection);
#endif
}

void RT_API RtCriticalSection_Leave(RT_CRITICAL_SECTION* lpCriticalSection)
{
#ifdef RT_DEFINE_WINDOWS
  LeaveCriticalSection((PCRITICAL_SECTION)lpCriticalSection);
#else
  /* We don't check returned value but we assume that. */
  pthread_mutex_unlock((pthread_mutex_t*)lpCriticalSection);
#endif
}

void RT_API RtCriticalSection_Free(RT_CRITICAL_SECTION* lpCriticalSection)
{
#ifdef RT_DEFINE_WINDOWS
  DeleteCriticalSection((PCRITICAL_SECTION)lpCriticalSection);
#else
  pthread_mutex_destroy((pthread_mutex_t*)lpCriticalSection);
#endif
}
