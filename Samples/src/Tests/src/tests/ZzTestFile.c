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

RT_B RT_CALL ZzTestPipe()
{
  RT_CHAR8 lpBuffer[8];
  RT_PIPE zzPipe;
  RT_IO_DEVICE* lpInput;
  RT_IO_DEVICE* lpOutput;
  RT_B bInputCreated;
  RT_B bOutputCreated;
  RT_UN unBytesRead;
  RT_B bResult;

  bInputCreated = RT_FALSE;
  bOutputCreated = RT_FALSE;

  if (!RtPipe_Create(&zzPipe)) goto handle_error;
  lpInput = RtPipe_GetInput(&zzPipe);
  lpOutput = RtPipe_GetOutput(&zzPipe);
  bInputCreated = RT_TRUE;
  bOutputCreated = RT_TRUE;

  if (!RtIoDevice_Write(RtIoDevice_GetOutputStream(lpOutput), "foo", 3)) goto handle_error;

  /* Close the pipe so that RtReadFromFile will read 3 bytes then encounter EOF. */
  bOutputCreated = RT_FALSE;
  if (!RtIoDevice_Free(lpOutput)) goto handle_error;

  RT_MEMORY_ZERO(lpBuffer, 8);
  if (!RtIoDevice_Read(RtIoDevice_GetInputStream(lpInput), lpBuffer, 8, &unBytesRead)) goto handle_error;

  if (unBytesRead != 3) goto handle_error;

  if (RT_MEMORY_COMPARE(lpBuffer, "foo\0\0\0\0\0", 8)) goto handle_error;

  if (!ZzTestInheritance(lpInput)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bOutputCreated)
  {
    bOutputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpOutput) && bResult) goto handle_error;
  }
  if (bInputCreated)
  {
    bInputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpInput) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestFile()
{
  RT_B bResult;

  if (!ZzTestPipe()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
