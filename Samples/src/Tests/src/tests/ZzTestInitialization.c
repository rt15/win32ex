#include "ZzTests.h"

RT_INITIALIZATION zz_Initialization;
RT_B zz_bInitialized;

RT_B RT_CALL ZzTestCommonFunction()
{
  RT_UN unI;
  RT_B bResult;

  RtWriteStringToConsole(_R("Initialize?\n"));
  if (RtInitialization_IsRequired(&zz_Initialization))
  {
    if (zz_bInitialized) goto handle_error;
    zz_bInitialized = RT_TRUE;

    for (unI = 0; unI < 5; unI++)
    {
      RtWriteStringToConsole(_R("Initializing...\n"));
      RtSleep_Sleep(1);
    }
    RtWriteStringToConsole(_R("Initialized.\n"));
    RtInitialization_NotifyDone(&zz_Initialization);
  }
  else
  {
    if (!zz_bInitialized) goto handle_error;
    RtWriteStringToConsole(_R("Already initialized.\n"));
  }

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_UN32 RT_CALL ZzTestInitializationThreadCallback(void* lpParameter)
{
  return ZzTestCommonFunction();
}

RT_B RT_CALL ZzTestInitialization()
{
  RT_B bThreadCreated;
  RT_THREAD zzThread;
  RT_UN32 unExitCode;
  RT_B bResult;

  bThreadCreated = RT_FALSE;

  /* RtInitialization_Create cannot fail. */
  RtInitialization_Create(&zz_Initialization);

  zz_bInitialized = RT_FALSE;

  if (!RtThread_Create(&zzThread, &ZzTestInitializationThreadCallback, RT_NULL)) goto handle_error;

  if (!ZzTestCommonFunction()) goto handle_error;

  if (!RtThread_Join(&zzThread)) goto handle_error;
  RtWriteStringToConsole(_R("Joined.\n"));

  if (!RtThread_GetExitCode(&zzThread, &unExitCode)) goto handle_error;

  if (!zz_bInitialized) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bThreadCreated)
  {
    bThreadCreated = RT_FALSE;
    if (!RtThread_Free(&zzThread) && bResult) goto handle_error;
  }
  /* RtInitialization_Free cannot fail. */
  RtInitialization_Free(&zz_Initialization);
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
