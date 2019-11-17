#include "ZzTests.h"

RT_B RT_CALL ZzTestPipeWithClose()
{
  RT_CHAR8 lpBuffer[8];
  RT_PIPE zzPipe;
  RT_IO_DEVICE* lpInputIoDevice;
  RT_IO_DEVICE* lpOutputIoDevice;
  RT_B bInputCreated;
  RT_B bOutputCreated;
  RT_INPUT_STREAM* lpInputStream;
  RT_OUTPUT_STREAM* lpOutputStream;
  RT_UN unBytesRead;
  RT_B bResult;

  bInputCreated = RT_FALSE;
  bOutputCreated = RT_FALSE;

  lpInputIoDevice = RT_NULL;
  lpOutputIoDevice = RT_NULL;

  if (!RtPipe_Create(&zzPipe)) goto handle_error;
  lpInputIoDevice = RtPipe_GetInput(&zzPipe);
  lpOutputIoDevice = RtPipe_GetOutput(&zzPipe);
  bInputCreated = RT_TRUE;
  bOutputCreated = RT_TRUE;

  lpInputStream = RtIoDevice_GetInputStream(lpInputIoDevice);
  lpOutputStream = RtIoDevice_GetOutputStream(lpOutputIoDevice);

  if (!lpOutputStream->lpWrite(lpOutputStream, "foo", 3)) goto handle_error;

  /* Close the pipe so that lpInputStream->lpRead will read 3 bytes then encounter EOF. */
  bOutputCreated = RT_FALSE;
  if (!RtIoDevice_Free(lpOutputIoDevice)) goto handle_error;

  RT_MEMORY_ZERO(lpBuffer, 8);
  if (!lpInputStream->lpRead(lpInputStream, lpBuffer, 8, &unBytesRead)) goto handle_error;

  if (unBytesRead != 3) goto handle_error;

  if (RT_MEMORY_COMPARE(lpBuffer, "foo\0\0\0\0\0", 8)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bOutputCreated)
  {
    bOutputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpOutputIoDevice) && bResult) goto handle_error;
  }
  if (bInputCreated)
  {
    bInputCreated = RT_FALSE;
    if (!RtIoDevice_Free(lpInputIoDevice) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

}

RT_B RT_CALL ZzTestPipeWithFixedSize(RT_PIPE* lpPipe)
{
  RT_INPUT_STREAM* lpInputStream;
  RT_OUTPUT_STREAM* lpOutputStream;
  RT_CHAR8 lpBuffer[4];
  RT_UN unBytesRead;
  RT_B bResult;

  lpInputStream = RtIoDevice_GetInputStream(&lpPipe->rtInputIoDevice);
  lpOutputStream = RtIoDevice_GetOutputStream(&lpPipe->rtOutputIoDevice);

  if (!lpOutputStream->lpWrite(lpOutputStream, "Foo", 3)) goto handle_error;

  RT_MEMORY_ZERO(lpBuffer, 4);
  if (!lpInputStream->lpRead(lpInputStream, lpBuffer, 3, &unBytesRead)) goto handle_error;

  if (RtChar8_CompareStrings(lpBuffer, "Foo")) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestPipe()
{
  RT_PIPE zzPipe;
  RT_B bPipeCreated;
  RT_B bResult;

  bPipeCreated = RT_FALSE;

  if (!RtPipe_Create(&zzPipe)) goto handle_error;
  bPipeCreated = RT_TRUE;

  if (!ZzTestPipeWithFixedSize(&zzPipe)) goto handle_error;

  if (!ZzTestPipeWithClose()) goto handle_error;

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

