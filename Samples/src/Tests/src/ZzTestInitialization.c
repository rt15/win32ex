#include <RtWin32Ex.h>

RT_INITIALIZATION tt_Initialization;
RT_B zz_bInitialized;

RT_B RT_CALL ZzTestCommonFunction()
{
  RT_UN unI;
  RT_B bResult;

  RtWriteStringToConsole(_R("Initialize?\n"));
  if (RtInitializationRequired(&tt_Initialization))
  {
    if (zz_bInitialized) goto handle_error;
    zz_bInitialized = RT_TRUE;

    for (unI = 0; unI < 5; unI++)
    {
      RtWriteStringToConsole(_R("Initializing...\n"));
      RtSleep(1);
    }
    RtWriteStringToConsole(_R("Initialized.\n"));
    RtNotifyInitializationDone(&tt_Initialization);
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

  /* RtCreateInitialization cannot fail. */
  RtCreateInitialization(&tt_Initialization);

  zz_bInitialized = RT_FALSE;
 
  if (!RtCreateThread(&zzThread, &ZzTestInitializationThreadCallback, RT_NULL)) goto handle_error;

  if (!ZzTestCommonFunction()) goto handle_error;

  if (!RtJoinThread(&zzThread)) goto handle_error;
  RtWriteStringToConsole(_R("Joined.\n"));
  
  if (!RtGetThreadExitCode(&zzThread, &unExitCode)) goto handle_error;

  if (!zz_bInitialized) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bThreadCreated)
  {
    bThreadCreated = RT_FALSE;
    if (!RtFreeThread(&zzThread) && bResult) goto handle_error;
  }
  /* RtFreeInitialization cannot fail. */
  RtFreeInitialization(&tt_Initialization);
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
