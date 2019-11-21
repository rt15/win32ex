#include <RtWin32Ex.h>

RT_UN32 RT_CALL ZzTestSimpleThreadCallback(void* lpParameter)
{
  RT_UN unI;

  for (unI = 0; unI < 5; unI++)
  {
    RtConsole_WriteCString((RT_CHAR*)lpParameter);
    RtSleep_Sleep(1);
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

  if (!RtThread_Create(&zzThread, &ZzTestSimpleThreadCallback, _R("Hello from thread\n"))) goto handle_error;
  bThreadCreated = RT_TRUE;

  if (!RtThread_Join(&zzThread)) goto handle_error;
  RtConsole_WriteCString(_R("Joined!\n"));
  if (!RtThread_GetExitCode(&zzThread, &unExitCode)) goto handle_error;
  if (unExitCode != 12) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bThreadCreated)
  {
    bThreadCreated = RT_FALSE;
    if ((!RtThread_Free(&zzThread)) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
