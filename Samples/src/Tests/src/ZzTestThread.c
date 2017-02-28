#include <RtWin32Ex.h>

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

RT_UN16 RT_CALL ZzTestThread()
{
  RT_THREAD thread;
  RT_UN32 unExitCode;
  RT_UN16 unResult;

  unResult = 1;

  if (!RtCreateThread(&thread, &ZzTestSimpleThreadCallback, _R("Hello from thread\n"))) goto the_end;
  if (!RtJoinThread(&thread)) goto free_thread;
  RtWriteStringToConsole(_R("Joined!\n"));
  if (!RtGetThreadExitCode(&thread, &unExitCode)) goto free_thread;
  if (unExitCode != 12) goto free_thread;

  unResult = 0;
free_thread:
  if (!RtFreeThread(&thread)) unResult = 1;
the_end:
  return unResult;
}
