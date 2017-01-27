#include <RtWin32Ex.h>

RT_INITIALIZATION tt_Initialization;

void RT_CALL TtCommonFunction()
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

RT_UN32 RT_CALL TtInitializationThreadCallback(void* lpParameter)
{
  TtCommonFunction();
  return 0;
}

RT_UN16 RT_CALL TtTestInitialization()
{
  RT_THREAD thread;
  RT_UN16 unResult;

  unResult = 1;

  RtCreateInitialization(&tt_Initialization);

  if (!RtCreateThread(&thread, &TtInitializationThreadCallback, RT_NULL)) goto free_initialization;

  TtCommonFunction();

  if (!RtJoinThread(&thread)) goto free_thread;
  RtWriteStringToConsole(_R("Joined.\n"));

  unResult = 0;
free_thread:
  if (!RtFreeThread(&thread)) unResult = 1;
free_initialization:
  RtFreeInitialization(&tt_Initialization);
  return unResult;
}
