#include "layer002/RtEvent.h"

#include "layer001/RtWin32ExOsHeaders.h"

RT_B RT_API RtEvent_Create(RT_EVENT* lpEvent)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* A non-manual-reset event object is used to avoid race condition. */
  lpEvent->hEvent = CreateEvent(RT_NULL, RT_FALSE, RT_FALSE, RT_NULL);
  if (!lpEvent->hEvent) goto handle_error;
#else
  /* Initial value is set to zero to ensure that read calls will block. */
  /* File descriptor is not inherited. */
  lpEvent->nFileDescriptor = eventfd(0, EFD_CLOEXEC);
  if (lpEvent->nFileDescriptor == -1) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtEvent_Signal(RT_EVENT* lpEvent)
{
#ifdef RT_DEFINE_LINUX
  uint64_t unValue;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!SetEvent(lpEvent->hEvent)) goto handle_error;
#else
  /* Using write on an eventfd object add given value to current value. */
  /* Beware that on an (old?) Linux write might release a single waiting thread, not all. */
  unValue = 1;
  if (write(lpEvent->nFileDescriptor, &unValue, sizeof(uint64_t)) != sizeof(uint64_t)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtEvent_WaitFor(RT_EVENT* lpEvent)
{
#ifdef RT_DEFINE_LINUX
  uint64_t unValue;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (WaitForSingleObject(lpEvent->hEvent,  INFINITE) != WAIT_OBJECT_0) goto handle_error;
#else
  /* This call to read will block on a zero eventfd object value until a call to write. Then read set the eventfd object to zero. */
  if (read(lpEvent->nFileDescriptor, &unValue, sizeof(uint64_t)) != sizeof(uint64_t)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtEvent_Free(RT_EVENT* lpEvent)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!CloseHandle(lpEvent->hEvent)) goto handle_error;
#else
  /* The close function returns zero on success. */
  if (close(lpEvent->nFileDescriptor)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
