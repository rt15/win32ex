#include "layer004/RtFastInitialization.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer003/RtAtomic.h"
#include "layer003/RtSleep.h"

RT_B RT_API RtFastInitializationRequired(RT_FAST_INITIALIZATION* lpFastInitialization)
{
  RT_B bResult;

  if (RT_ATOMIC_LOAD(lpFastInitialization->unInitialized))
  {
    /* Initialized is true and we have put a memory barrier, so we are sure that initialization is done and accessible. */
    bResult = RT_FALSE;
  }
  else
  {
    if (RT_ATOMIC_TRY_TO_ACQUIRE(lpFastInitialization->unInitializing))
    {
      /* We are the first and the only one that has set 1 in initializing, so we are responsible for initialization. */
      bResult = RT_TRUE;
    }
    else
    {
      /* Another thread is already performing initialization. */
      while (RT_ATOMIC_LOAD(lpFastInitialization->unInitialized))
      {
        RtYeild();
      }
      bResult = RT_FALSE;
    }
  }
  return bResult;
}

void RT_API RtNotifyFastInitializationDone(RT_FAST_INITIALIZATION* lpFastInitialization)
{
  RT_ATOMIC_STORE(lpFastInitialization->unInitialized, 1);
}

RT_B RT_API RtIsFastInitializationDone(RT_FAST_INITIALIZATION* lpFastInitialization)
{
  RT_B bResult;

  if (RT_ATOMIC_LOAD(lpFastInitialization->unInitialized))
  {
    bResult = RT_TRUE;
  }
  else
  {
    bResult = RT_FALSE;
  }
  return bResult;
}

