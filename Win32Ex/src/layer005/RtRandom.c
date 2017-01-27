#include "layer005/RtRandom.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer004/RtFastInitialization.h"

/**
 *
 * <p>
 * Linux notes:<br>
 * If number generator is not initialized, call RAND_poll (Equivalent of srand).
 * </p>
 *
 * <p>
 * Windows notes:<br>
 * RtlGenRandom is not part of public win32 api but it should remain in Windows as it is used by rand_s of current CRT.<br>
 * If they remove it, all applications statically linked with CRT and using rand_s will fail.<br>
 * The CryptGenRandom public alternative comes with a lot of overhead.<br>
 * Unlike what could be deduced from NTSecAPI.h, calling convention of RtlGenRandom is stdcall (ret 8 at the end).
 * </p>
 */

#ifdef RT_DEFINE_WINDOWS

typedef BOOLEAN (APIENTRY *RT_RANDOM_RTL_GEN_RANDOM)(PVOID RandomBuffer, ULONG RandomBufferLength);

RT_FAST_INITIALIZATION rt_randomInitialization = RT_FAST_INITIALIZATION_STATIC_INIT;

/* Whether initialization of random generator has been successful or not. */
RT_B rt_bRandomInitializationSuccessful;

/* Error code of failed initialization. */
DWORD rt_nRandomInitializationError;

RT_RANDOM_RTL_GEN_RANDOM rt_lpRtlGenRandom;
#endif

#ifdef RT_DEFINE_WINDOWS

RT_B RT_CALL RtInitializeRandom()
{
  RT_H hLibrary;
  RT_B bResult;

  hLibrary = LoadLibrary(_R("advapi32.dll"));
  if (!hLibrary) goto handle_error;
  rt_lpRtlGenRandom = (RT_RANDOM_RTL_GEN_RANDOM)GetProcAddress(hLibrary, "SystemFunction036");
  if (!rt_lpRtlGenRandom) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  rt_nRandomInitializationError = GetLastError();
  bResult = RT_FAILURE;
  goto free_resources;
}

#endif

RT_B RT_API RtGetRandomBytes(void* lpArea, RT_UN32 unSize)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (RtFastInitializationRequired(&rt_randomInitialization))
  {
    rt_bRandomInitializationSuccessful = RtInitializeRandom();
    RtNotifyFastInitializationDone(&rt_randomInitialization);
  }
  if (!rt_bRandomInitializationSuccessful)
  {
    /* Set last error as when the initialization has failed. */
    SetLastError(rt_nRandomInitializationError);
    goto handle_error;
  }
#endif

#ifdef RT_DEFINE_WINDOWS
  if (!rt_lpRtlGenRandom(lpArea, unSize)) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */
  if (!RAND_bytes(lpArea, unSize))
  {
    /* RAND_bytes uses err_get_error and does not set errno so we use RtSetLastError. */
    RtSetLastError(RT_ERROR_FUNCTION_FAILED);
    goto handle_error;
  }
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtGetRandomInteger(RT_N* lpResult)
{
  return RtGetRandomBytes(lpResult, sizeof(RT_N));
}

RT_B RT_API RtGetRandomUInteger(RT_UN* lpResult)
{
  return RtGetRandomBytes(lpResult, sizeof(RT_UN));
}

RT_B RT_API RtGetRandomUIntegerWithBoundaries(RT_UN unLowerBound, RT_UN unUpperBound, RT_UN* lpResult)
{
  RT_UN unUnsigned;
  RT_B bResult;

  if (!RtGetRandomUInteger(&unUnsigned)) goto handle_error;
  unUnsigned = unUnsigned % (unUpperBound + 1 - unLowerBound);
  *lpResult = unUnsigned + unLowerBound;

  bResult = RT_TRUE;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtGetRandomIntegerWithBoundaries(RT_N nLowerBound, RT_N nUpperBound, RT_N* lpResult)
{
  RT_UN unUnsigned;
  RT_B bResult;

  if (!RtGetRandomUInteger(&unUnsigned)) goto handle_error;
  unUnsigned = unUnsigned % (RT_UN)(nUpperBound + 1 - nLowerBound);
  *lpResult = unUnsigned + nLowerBound;

  bResult = RT_TRUE;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}
