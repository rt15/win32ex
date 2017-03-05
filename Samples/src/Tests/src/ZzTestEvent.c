#include <RtWin32Ex.h>

#define ZZ_TEST_EVENT_THREAD_STATUS_INIT      0
#define ZZ_TEST_EVENT_THREAD_STATUS_SIGNAL_1  1
#define ZZ_TEST_EVENT_THREAD_STATUS_SIGNAL_2  2

RT_UN zz_unTestEventThreadStatus;

RT_UN32 RT_CALL ZzTestEventThreadCallback(void* lpParameter)
{
  RT_EVENT* lpEvent;
  RT_UN32 unResult;

  lpEvent = (RT_EVENT*)lpParameter;

  if (!RtWaitForEvent(lpEvent)) goto handle_error;

  zz_unTestEventThreadStatus = ZZ_TEST_EVENT_THREAD_STATUS_SIGNAL_1;

  if (!RtWaitForEvent(lpEvent)) goto handle_error;

  zz_unTestEventThreadStatus = ZZ_TEST_EVENT_THREAD_STATUS_SIGNAL_2;

  unResult = RT_SUCCESS;
free_resources:
  return unResult;

handle_error:
  unResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestEvent()
{
  RT_B bEventCreated;
  RT_THREAD zzThread;
  RT_B bThreadCreated;
  RT_EVENT zzEvent;
  RT_B bResult;

  bEventCreated = RT_FALSE;

  bThreadCreated = RT_FALSE;
  zz_unTestEventThreadStatus = ZZ_TEST_EVENT_THREAD_STATUS_INIT;

  if (!RtCreateEvent(&zzEvent)) goto handle_error;
  bEventCreated = RT_TRUE;

  if (!RtCreateThread(&zzThread, &ZzTestEventThreadCallback, &zzEvent)) goto handle_error;
  bThreadCreated = RT_TRUE;

  /* Let other thread wait for the event. */
  RtSleep(10);

  if (zz_unTestEventThreadStatus != ZZ_TEST_EVENT_THREAD_STATUS_INIT) goto handle_error;

  if (!RtSignalEvent(&zzEvent)) goto handle_error;

  /* Wait for thread to set status flag. */
  RtSleep(10);
  if (zz_unTestEventThreadStatus != ZZ_TEST_EVENT_THREAD_STATUS_SIGNAL_1) goto handle_error;

  if (!RtSignalEvent(&zzEvent)) goto handle_error;

  /* Wait for thread to set status flag. */
  RtSleep(10);
  if (zz_unTestEventThreadStatus != ZZ_TEST_EVENT_THREAD_STATUS_SIGNAL_2) goto handle_error;

  if (!RtJoinAndCheckThread(&zzThread)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bThreadCreated)
  {
    bThreadCreated = RT_FALSE;
    if (!RtFreeThread(&zzThread) && bResult) goto handle_error;
  }
  if (bEventCreated)
  {
    bEventCreated = RT_FALSE;
    if (!RtFreeEvent(&zzEvent) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
