#include "layer005/RtSystemInfo.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer004/RtFastInitialization.h"

RT_FAST_INITIALIZATION rt_guiOsVersionInitialization = RT_FAST_INITIALIZATION_STATIC_INIT;

RT_B rt_bOsVersionInitializationSuccessful;

/* Save last error while initializing. */
#ifdef RT_DEFINE_WINDOWS
DWORD rt_unOsVersionInitializationError;
#else
int rt_nOsVersionInitializationError;
#endif

RT_UN rt_unMajorOsVersion;
RT_UN rt_unMinorOsVersion;
RT_UN rt_unPatchOsVersion;

RT_B RT_API RtGetOsVersion(RT_UN* lpMajor, RT_UN* lpMinor, RT_UN* lpPatch)
{
#ifdef RT_DEFINE_WINDOWS
  OSVERSIONINFOEX rtOsVersionInfo;
#else

#endif
  RT_B bResult;

  if (RtFastInitializationRequired(&rt_guiOsVersionInitialization))
  {

#ifdef RT_DEFINE_WINDOWS

    rtOsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (GetVersionEx((OSVERSIONINFO*)&rtOsVersionInfo))
    {
      rt_unMajorOsVersion = rtOsVersionInfo.dwMajorVersion;
      rt_unMinorOsVersion = rtOsVersionInfo.dwMinorVersion;
      rt_unPatchOsVersion = rtOsVersionInfo.wServicePackMajor;
      rt_bOsVersionInitializationSuccessful = RT_TRUE;
    }
    else
    {
      rt_unOsVersionInitializationError = GetLastError();
      rt_bOsVersionInitializationSuccessful = RT_FALSE;
    }

#else
    /* TODO: Implement on Linux. */

#endif

    RtNotifyFastInitializationDone(&rt_guiOsVersionInitialization);
  }

  if (rt_bOsVersionInitializationSuccessful)
  {
    *lpMajor = rt_unMajorOsVersion;
    if (lpMinor)
    {
      *lpMinor = rt_unMinorOsVersion;
    }
    if (lpPatch)
    {
      *lpPatch = rt_unPatchOsVersion;
    }
  }
  else
  {
#ifdef RT_DEFINE_WINDOWS
    SetLastError(rt_unOsVersionInitializationError);
#else
    errno = rt_nOsVersionInitializationError;
#endif
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtIsOsVersionEqualOrGreaterTo(RT_UN unMajor, RT_UN unMinor, RT_UN unPatch, RT_B* lpResult)
{
  RT_UN unCurrentMajor;
  RT_UN unCurrentMinor;
  RT_UN unCurrentPatch;
  RT_B bResult;

  if (!RtGetOsVersion(&unCurrentMajor, &unCurrentMinor, &unCurrentPatch)) goto handle_error;

  if (unCurrentMajor > unMajor)
  {
    *lpResult = RT_TRUE;
  }
  else if (unCurrentMajor == unMajor)
  {
    if (unCurrentMinor > unMinor)
    {
      *lpResult = RT_TRUE;
    }
    else if (unCurrentMinor == unMinor)
    {
      if (unCurrentPatch >= unPatch)
      {
        *lpResult = RT_TRUE;
      }
      else
      {
        *lpResult = RT_FALSE;
      }
    }
    else
    {
      *lpResult = RT_FALSE;
    }
  }
  else
  {
    *lpResult = RT_FALSE;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}