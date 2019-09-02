#include "layer003/RtPipe.h"

#include "layer001/RtWin32ExOsHeaders.h"

RT_B RT_API RtPipe_Create(RT_PIPE* lpPipe)
{
#ifdef RT_DEFINE_WINDOWS
  RT_H hInput;
  RT_H hOutput;
#else
  RT_N32 lpPipes[2];
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* In case of failure, returns zero and set last error. */
  if (!CreatePipe(&hInput, &hOutput, RT_NULL, 0)) goto handle_error;
  RtIoDevice_CreateFromHandle(&lpPipe->rtInputIoDevice, hInput);
  RtIoDevice_CreateFromHandle(&lpPipe->rtOutputIoDevice, hOutput);
#else
  /* In case of failure, returns -1 and set errno. */
  if (pipe2(lpPipes, O_CLOEXEC) == -1) goto handle_error;
  RtIoDevice_CreateFromFileDescriptor(&lpPipe->rtInputIoDevice, lpPipes[0]);
  RtIoDevice_CreateFromFileDescriptor(&lpPipe->rtOutputIoDevice, lpPipes[1]);
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_IO_DEVICE* RtPipe_GetInput(RT_PIPE* lpPipe)
{
  return &lpPipe->rtInputIoDevice;
}

RT_IO_DEVICE* RtPipe_GetOutput(RT_PIPE* lpPipe)
{
  return &lpPipe->rtOutputIoDevice;
}
