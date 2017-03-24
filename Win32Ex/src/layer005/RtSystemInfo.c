#include "layer005/RtSystemInfo.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer004/RtChar.h"
#include "layer004/RtFastInitialization.h"

RT_FAST_INITIALIZATION rt_osVersionInitialization = RT_FAST_INITIALIZATION_STATIC_INIT;

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
  struct utsname rtUtsName;
  RT_CHAR lpVersion[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_CHAR* lpStart;
  RT_CHAR* lpEnd;
  RT_UN unWritten;
  RT_B bPatchVersion;
#endif
  RT_B bResult;

  if (RtFastInitializationRequired(&rt_osVersionInitialization))
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
    /* On error, -1 is returned, and errno is set appropriately.  */
    if (uname(&rtUtsName) == -1) goto handle_initialization_error;

    /* rtUtsName.release looks like "4.4.0-66-generic". */
    /* But it might look like "2.6" too. */

    if (!RtCopyString(rtUtsName.release, lpVersion, RT_CHAR_THIRD_BIG_STRING_SIZE, &unWritten)) goto handle_initialization_error;
    lpStart = lpVersion;
    lpEnd = lpVersion;

    /* Find first dot. */
    while (*lpEnd != _R('.')) lpEnd++;
    /* Replace dot with null to call RtConvertStringToUInteger. */
    *lpEnd = 0;

    if (!RtConvertStringToUInteger(lpStart, &rt_unMajorOsVersion)) goto handle_initialization_error;

    /* Goto next version number. */
    lpStart = ++lpEnd;

    while ((*lpEnd >= _R('0')) && (*lpEnd <= _R('9'))) lpEnd++;

    /* Is there a patch version? */
    bPatchVersion = (*lpEnd == _R('.'));

    /* Replace dot with null to call RtConvertStringToUInteger. */
    *lpEnd = 0;

    if (!RtConvertStringToUInteger(lpStart, &rt_unMinorOsVersion)) goto handle_initialization_error;

    if (bPatchVersion)
    {
      /* Goto next version number. */
      lpStart = ++lpEnd;

      while ((*lpEnd >= _R('0')) && (*lpEnd <= _R('9'))) lpEnd++;

      /* Replace dot with null to call RtConvertStringToUInteger. */
      *lpEnd = 0;

      if (!RtConvertStringToUInteger(lpStart, &rt_unPatchOsVersion)) goto handle_initialization_error;
    }
    else
    {
      /* No patch version, use 0. */
      rt_unPatchOsVersion = 0;
    }

    rt_bOsVersionInitializationSuccessful = RT_TRUE;
    goto initialized;

handle_initialization_error:
    rt_nOsVersionInitializationError = errno;
    rt_bOsVersionInitializationSuccessful = RT_FALSE;

initialized:

#endif

    RtNotifyFastInitializationDone(&rt_osVersionInitialization);
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

RT_B RT_API RtIsOsVersionGreaterOrEqualTo(RT_UN unMajor, RT_UN unMinor, RT_UN unPatch, RT_B* lpResult)
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
