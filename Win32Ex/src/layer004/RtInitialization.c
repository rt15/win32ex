#include "layer004/RtInitialization.h"

#include "layer003/RtAtomic.h"

void RT_API RtCreateInitialization(RT_INITIALIZATION* lpInitialization)
{
  lpInitialization->unInitialized = 0;
  RtCreateCriticalSection(&lpInitialization->rtCriticalSection, RT_FALSE);
}

RT_B RT_API RtInitializationRequired(RT_INITIALIZATION* lpInitialization)
{
  RT_B bResult;

  if (RT_ATOMIC_LOAD(lpInitialization->unInitialized))
  {
    /* Initialized is true and we have put a memory barrier, so we are sure that initialization is done and accessible. */
    bResult = RT_FALSE;
  }
  else
  {
    RtEnterCriticalSection(&lpInitialization->rtCriticalSection);

    if (!RT_ATOMIC_LOAD(lpInitialization->unInitialized))
    {
      bResult = RT_TRUE;
    }
    else
    {
      bResult = RT_FALSE;
    }
  }
  return bResult;
}

void RT_API RtNotifyInitializationDone(RT_INITIALIZATION* lpInitialization)
{
  RT_ATOMIC_STORE(lpInitialization->unInitialized, 1);
  RtLeaveCriticalSection(&lpInitialization->rtCriticalSection);
}

void RT_API RtFreeInitialization(RT_INITIALIZATION* lpInitialization)
{
  RtFreeCriticalSection(&lpInitialization->rtCriticalSection);
}
