#include <RtWin32Ex.h>

RT_INITIALIZATION tt_Initialization;

void RT_CALL ZzTestCommonFunction()
{
  RT_UN unI;

  RtWriteStringToConsole(_R("Initialize?\n"));
  if (RtInitializationRequired(&tt_Initialization))
  {
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
    RtWriteStringToConsole(_R("Already initialized.\n"));
  }
}

RT_UN32 RT_CALL ZzTestInitializationThreadCallback(void* lpParameter)
{
  ZzTestCommonFunction();
  return 0;
}

RT_B RT_CALL ZzTestInitialization()
{
  RT_B bThreadCreated;
  RT_THREAD rtThread;
  RT_B bResult;

  bThreadCreated = RT_FALSE;

  /* RtCreateInitialization cannot fail. */
  RtCreateInitialization(&tt_Initialization);

  if (!RtCreateThread(&rtThread, &ZzTestInitializationThreadCallback, RT_NULL)) goto handle_error;

  ZzTestCommonFunction();

  if (!RtJoinThread(&rtThread)) goto handle_error;
  RtWriteStringToConsole(_R("Joined.\n"));

  bResult = RT_SUCCESS;
free_resources:
  if (bThreadCreated)
  {
    bThreadCreated = RT_FALSE;
    if (!RtFreeThread(&rtThread) && bResult) goto handle_error;
  }
  /* RtFreeInitialization cannot fail. */
  RtFreeInitialization(&tt_Initialization);
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
