#include "ZzTests.h"

RT_UN32 RT_CALL ZzTestSimpleThreadCallback(void* lpParameter)
{
  RT_UN unI;

  for (unI = 0; unI < 5; unI++)
  {
    RtWriteStringToConsole((RT_CHAR*)lpParameter);
    RtSleep(1);
  }

  return 12;
}

RT_B RT_CALL ZzTestThread()
{
  RT_B bThreadCreated;
  RT_THREAD zzThread;
  RT_UN32 unExitCode;
  RT_B bResult;

  bThreadCreated = RT_FALSE;

  if (!RtCreateThread(&zzThread, &ZzTestSimpleThreadCallback, _R("Hello from thread\n"))) goto handle_error;
  bThreadCreated = RT_TRUE;

  if (!RtJoinThread(&zzThread)) goto handle_error;
  RtWriteStringToConsole(_R("Joined!\n"));
  if (!RtGetThreadExitCode(&zzThread, &unExitCode)) goto handle_error;
  if (unExitCode != 12) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bThreadCreated)
  {
    bThreadCreated = RT_FALSE;
    if ((!RtFreeThread(&zzThread)) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
