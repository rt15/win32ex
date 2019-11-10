#ifndef RT_IO_DEVICE_H
#define RT_IO_DEVICE_H

#include "layer000/RtWin32ExTypes.h"
#include "layer001/RtInputStream.h"
#include "layer001/RtOutputStream.h"

/**
 * @file
 * Device used for I/O: a file, a socket, a pipe...
 *
 * <p>
 * A device may support input only, output only or may support both input and output.
 * <p/>
 */

typedef struct _RT_IO_DEVICE
{
  RT_INPUT_STREAM rtInputStream;
  RT_OUTPUT_STREAM rtOutputStream;
#ifdef RT_DEFINE_WINDOWS
  RT_H hHandle;
#else /* NOT RT_DEFINE_WINDOWS */
  RT_N32 nFileDescriptor;
#endif

}
RT_IO_DEVICE;

/**
 * In most cases, you should not need these functions.<br>
 * Create the correct I/O device instead using <tt>RtFile_Create</tt>, <tt>RtPipe_Create</tt> and so on.
 */
#ifdef RT_DEFINE_WINDOWS
void RT_API RtIoDevice_CreateFromHandle(RT_IO_DEVICE* lpIoDevice, RT_H hHandle);
#else /* NOT RT_DEFINE_WINDOWS */
void RT_API RtIoDevice_CreateFromFileDescriptor(RT_IO_DEVICE* lpIoDevice, RT_N32 nFileDescriptor);
#endif

RT_B RT_API RtIoDevice_CreateFromStdInput(RT_IO_DEVICE* lpIoDevice);
RT_B RT_API RtIoDevice_CreateFromStdOutput(RT_IO_DEVICE* lpIoDevice);
RT_B RT_API RtIoDevice_CreateFromStdError(RT_IO_DEVICE* lpIoDevice);

RT_INPUT_STREAM* RT_API RtIoDevice_GetInputStream(RT_IO_DEVICE* lpIoDevice);
RT_OUTPUT_STREAM* RT_API RtIoDevice_GetOutputStream(RT_IO_DEVICE* lpIoDevice);

/**
 * In most cases, you should not need this function.<br>
 * Use <tt>lpInputStream->lpRead</tt> instead.
 */
RT_B RT_API RtIoDevice_Read(RT_INPUT_STREAM* lpInputStream, RT_CHAR8* lpBuffer, RT_UN unBytesToRead, RT_UN* lpBytesRead);

/**
 * In most cases, you should not need this function.<br>
 * Use <tt>lpOutputStream->lpWrite</tt> instead.
 */
RT_B RT_API RtIoDevice_Write(RT_OUTPUT_STREAM* lpOutputStream, RT_CHAR8* lpData, RT_UN unBytesToWrite);

/**
 * <p>
 * Under Windows, an inheritable handle is valid in a child process created with CreateProcess and bInheritHandles TRUE.
 * </p>
 *
 * <p>
 * Under Linux, a non-inheritable file descriptor is closed when execl/execlp/execle/execv/execvp/execvpe is used.<br>
 * It is not closed by a fork.
 * </p>
 */
RT_B RT_API RtIoDevice_IsInheritable(RT_IO_DEVICE* lpIoDevice, RT_B* lpInheritable);

/**
 * Update inherability of given I/O Device if needed.
 *
 * <p>
 * Beware that there can be a race condition with this function.<br>
 * If another thread is performing fork/exec or CreateProcess, then there can be a file descriptor/handle leak.<br>
 * See O_CLOEXEC flag description in open manual.
 * </p>
 *
 * <p>
 * Under Windows, an inheritable handle can be used by a process created with CreateProcess and bInheritHandles TRUE.
 * </p>
 *
 * <p>
 * Under Linux, a non-inheritable file descriptor is closed when execl/execlp/execle/execv/execvp/execvpe is used.<br>
 * It is not closed by a fork.
 * </p>
 */
RT_B RT_API RtIoDevice_SetInheritable(RT_IO_DEVICE* lpIoDevice, RT_B bInheritable);

/**
 * Should be called only with generic devices like files and pipes, not socket.<br>
 * Should not be called with std Input/Output/Error.
 */
RT_B RT_API RtIoDevice_Free(RT_IO_DEVICE* lpIoDevice);

#endif /* RT_IO_DEVICE_H */
