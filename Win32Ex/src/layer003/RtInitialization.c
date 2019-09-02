#include "layer003/RtInitialization.h"

#include "layer001/RtAtomic.h"

void RT_API RtInitialization_Create(RT_INITIALIZATION* lpInitialization)
{
  lpInitialization->unInitialized = 0;
  RtCriticalSection_Create(&lpInitialization->rtCriticalSection, RT_FALSE);
}

RT_B RT_API RtInitialization_IsRequired(RT_INITIALIZATION* lpInitialization)
{
  RT_B bResult;

  if (RT_ATOMIC_LOAD(lpInitialization->unInitialized))
  {
    /* Initialized is true and we have put a memory barrier, so we are sure that initialization is done and accessible. */
    bResult = RT_FALSE;
  }
  else
  {
    RtCriticalSection_Enter(&lpInitialization->rtCriticalSection);

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

void RT_API RtInitialization_NotifyDone(RT_INITIALIZATION* lpInitialization)
{
  RT_ATOMIC_STORE(lpInitialization->unInitialized, 1);
  RtCriticalSection_Leave(&lpInitialization->rtCriticalSection);
}

void RT_API RtInitialization_Free(RT_INITIALIZATION* lpInitialization)
{
  RtCriticalSection_Free(&lpInitialization->rtCriticalSection);
}
