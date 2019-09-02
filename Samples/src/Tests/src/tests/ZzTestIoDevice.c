#include "ZzTests.h"

RT_B RT_CALL ZzTestIoDevice()
{
  RT_IO_DEVICE rtIoDevice;
  RT_OUTPUT_STREAM* lpOutputStream;
  RT_B bResult;

  if (!RtIoDevice_CreateFromStdOutput(&rtIoDevice)) goto handle_error;

  lpOutputStream = &rtIoDevice.rtOutputStream;
  if (!lpOutputStream->lpWrite(lpOutputStream, "Foo\n", 4)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
