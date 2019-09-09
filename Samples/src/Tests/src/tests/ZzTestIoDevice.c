#include "ZzTests.h"

RT_B RT_CALL ZzTestInheritance(RT_IO_DEVICE* lpIoDevice)
{
  RT_B bInheritable;
  RT_B bResult;

  /* All files must not be inheritable by default. */
  if (!RtIoDevice_IsInheritable(lpIoDevice, &bInheritable)) goto handle_error;
  if (bInheritable) goto handle_error;

  /* Switch to inheritable. */
  if (!RtIoDevice_SetInheritable(lpIoDevice, RT_TRUE)) goto handle_error;
  if (!RtIoDevice_IsInheritable(lpIoDevice, &bInheritable)) goto handle_error;
  if (!bInheritable) goto handle_error;

  /* Try again. */
  if (!RtIoDevice_SetInheritable(lpIoDevice, RT_TRUE)) goto handle_error;
  if (!RtIoDevice_IsInheritable(lpIoDevice, &bInheritable)) goto handle_error;
  if (!bInheritable) goto handle_error;

  /* Switch back to non-inheritable. */
  if (!RtIoDevice_SetInheritable(lpIoDevice, RT_FALSE)) goto handle_error;
  if (!RtIoDevice_IsInheritable(lpIoDevice, &bInheritable)) goto handle_error;
  if (bInheritable) goto handle_error;

  /* Try again. */
  if (!RtIoDevice_SetInheritable(lpIoDevice, RT_FALSE)) goto handle_error;
  if (!RtIoDevice_IsInheritable(lpIoDevice, &bInheritable)) goto handle_error;
  if (bInheritable) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestIoDevice()
{
  RT_PIPE zzPipe;
  RT_B bPipeCreated;
  RT_B bResult;

  bPipeCreated = RT_FALSE;

  if (!RtPipe_Create(&zzPipe)) goto handle_error;
  bPipeCreated = RT_TRUE;

  if (!ZzTestInheritance(&zzPipe.rtInputIoDevice)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bPipeCreated)
  {
    bPipeCreated = RT_FALSE;
    if (!RtPipe_Free(&zzPipe) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
