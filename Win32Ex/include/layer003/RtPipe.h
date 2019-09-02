#ifndef RT_PIPE_H
#define RT_PIPE_H

#include "layer000/RtWin32ExTypes.h"
#include "layer002/RtIoDevice.h"

typedef struct _RT_PIPE
{
  RT_IO_DEVICE rtInputIoDevice;
  RT_IO_DEVICE rtOutputIoDevice;
}
RT_PIPE;

/**
 * Do not forget to close input and output I/O devices when they are no more needed.
 */
RT_B RT_API RtPipe_Create(RT_PIPE* lpPipe);

RT_IO_DEVICE* RtPipe_GetInput(RT_PIPE* lpPipe);
RT_IO_DEVICE* RtPipe_GetOutput(RT_PIPE* lpPipe);

#endif /* RT_PIPE_H */
