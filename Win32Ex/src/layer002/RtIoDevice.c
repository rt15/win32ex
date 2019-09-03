#include "layer002/RtIoDevice.h"

#include "layer001/RtMemory.h"
#include "layer001/RtWin32ExOsHeaders.h"

#ifdef RT_DEFINE_WINDOWS
void RT_API RtIoDevice_CreateFromHandle(RT_IO_DEVICE* lpIoDevice, RT_H hHandle)
{
  lpIoDevice->hHandle = hHandle;
  lpIoDevice->rtInputStream.lpRead = &RtIoDevice_Read;
  lpIoDevice->rtOutputStream.lpWrite = &RtIoDevice_Write;
}
#else /* NOT RT_DEFINE_WINDOWS */
void RT_API RtIoDevice_CreateFromFileDescriptor(RT_IO_DEVICE* lpIoDevice, RT_N32 nFileDescriptor)
{
  lpIoDevice->nFileDescriptor = nFileDescriptor;
  lpIoDevice->rtInputStream.lpRead = &RtIoDevice_Read;
  lpIoDevice->rtOutputStream.lpWrite = &RtIoDevice_Write;
}
#endif

RT_B RT_API RtIoDevice_CreateFromStdInput(RT_IO_DEVICE* lpIoDevice)
{
#ifdef RT_DEFINE_WINDOWS
  RT_H hHandle;
  RT_B bResult;
#endif

#ifdef RT_DEFINE_WINDOWS
  hHandle = GetStdHandle(STD_INPUT_HANDLE);
  /* GetStdHandle can return NULL but we do not consider it an error. */
  if (hHandle == INVALID_HANDLE_VALUE) goto handle_error;

  RtIoDevice_CreateFromHandle(lpIoDevice, hHandle);

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
#else
  RtIoDevice_CreateFromFileDescriptor(lpIoDevice, 0);
  return RT_SUCCESS;
#endif
}

RT_B RT_API RtIoDevice_CreateFromStdOutput(RT_IO_DEVICE* lpIoDevice)
{
#ifdef RT_DEFINE_WINDOWS
  RT_H hHandle;
  RT_B bResult;
#endif

#ifdef RT_DEFINE_WINDOWS
  hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  /* GetStdHandle can return NULL but we do not consider it an error. */
  if (hHandle == INVALID_HANDLE_VALUE) goto handle_error;

  RtIoDevice_CreateFromHandle(lpIoDevice, hHandle);

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
#else
  RtIoDevice_CreateFromFileDescriptor(lpIoDevice, 1);
  return RT_SUCCESS;
#endif
}

RT_B RT_API RtIoDevice_CreateFromStdError(RT_IO_DEVICE* lpIoDevice)
{
#ifdef RT_DEFINE_WINDOWS
  RT_H hHandle;
  RT_B bResult;
#endif

#ifdef RT_DEFINE_WINDOWS
  hHandle = GetStdHandle(STD_ERROR_HANDLE);
  /* GetStdHandle can return NULL but we do not consider it an error. */
  if (hHandle == INVALID_HANDLE_VALUE) goto handle_error;

  RtIoDevice_CreateFromHandle(lpIoDevice, hHandle);

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
#else
  RtIoDevice_CreateFromFileDescriptor(lpIoDevice, 2);
  return RT_SUCCESS;
#endif
}

RT_INPUT_STREAM* RT_API RtIoDevice_GetInputStream(RT_IO_DEVICE* lpIoDevice)
{
  return &lpIoDevice->rtInputStream;
}

RT_OUTPUT_STREAM* RT_API RtIoDevice_GetOutputStream(RT_IO_DEVICE* lpIoDevice)
{
  return &lpIoDevice->rtOutputStream;
}

RT_B RT_API RtIoDevice_Read(RT_INPUT_STREAM* lpInputStream, RT_CHAR8* lpBuffer, RT_UN unBytesToRead, RT_UN* lpBytesRead)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unBytesRead;
#else
  ssize_t nBytesRead;
#endif
  RT_IO_DEVICE* lpIoDevice;
  RT_B bResult;

  bResult = RT_FAILURE;

  lpIoDevice = RT_MEMORY_CONTAINER_OF(lpInputStream, RT_IO_DEVICE, rtInputStream);

#ifdef RT_DEFINE_WINDOWS
  /* TODO: Manage more than 4Go? */
  if (!ReadFile(lpIoDevice->hHandle, lpBuffer, (DWORD)unBytesToRead, &unBytesRead, NULL)) goto handle_error;
  *lpBytesRead = unBytesRead;
#else
  /* read returns -1 and set errno in case of issue. */
  nBytesRead = read(lpIoDevice->nFileDescriptor, lpBuffer, unBytesToRead);
  if (nBytesRead == -1) goto handle_error;
  *lpBytesRead = nBytesRead;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtIoDevice_Write(RT_OUTPUT_STREAM* lpOutputStream, RT_CHAR8* lpData, RT_UN unBytesToWrite)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unBytesWritten;
#endif
  RT_IO_DEVICE* lpIoDevice;
  RT_B bResult;

  bResult = RT_FAILURE;

  lpIoDevice = RT_MEMORY_CONTAINER_OF(lpOutputStream, RT_IO_DEVICE, rtOutputStream);

#ifdef RT_DEFINE_WINDOWS
  if (WriteFile(lpIoDevice->hHandle,
                lpData,
                (DWORD)unBytesToWrite,
                &unBytesWritten,
                NULL))
    if (unBytesWritten == unBytesToWrite)
      bResult = RT_SUCCESS;
#else
  if (write(lpIoDevice->nFileDescriptor, lpData, unBytesToWrite) != -1)
    bResult = RT_SUCCESS;
#endif

  return bResult;
}

RT_B RT_API RtIoDevice_IsInheritable(RT_IO_DEVICE* lpIoDevice, RT_B* lpInheritable)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unFlags;
#else
  int nFlags;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  if (!GetHandleInformation(lpIoDevice->hHandle, &unFlags)) goto handle_error;
  if (unFlags & HANDLE_FLAG_INHERIT)
  {
    *lpInheritable = RT_TRUE;
  }
  else
  {
    *lpInheritable = RT_FALSE;
  }

#else /* NOT RT_DEFINE_WINDOWS */

  nFlags = fcntl(lpIoDevice->nFileDescriptor, F_GETFD, 0);
  if (nFlags == -1) goto handle_error;

  if (nFlags & FD_CLOEXEC)
  {
    *lpInheritable = RT_FALSE;
  }
  else
  {
    *lpInheritable = RT_TRUE;
  }

#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtIoDevice_SetInheritable(RT_IO_DEVICE* lpIoDevice, RT_B bInheritable)
{
  RT_B bCurrentValue;
#ifdef RT_DEFINE_LINUX
  int nFlags;
#endif
  RT_B bResult;

  if (!RtIoDevice_IsInheritable(lpIoDevice, &bCurrentValue)) goto handle_error;

#ifdef RT_DEFINE_LINUX
  nFlags = fcntl(lpIoDevice->nFileDescriptor, F_GETFD, 0);
  if (nFlags == -1) goto handle_error;
#endif

  if (bInheritable && !bCurrentValue)
  {
#ifdef RT_DEFINE_WINDOWS
    /* Add inheritance. */
    if (!SetHandleInformation(lpIoDevice->hHandle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) goto handle_error;
#else
    /* Remove FD_CLOEXEC flag so that the descriptor will be valid in children. */
    if (fcntl(lpIoDevice->nFileDescriptor, F_SETFD, nFlags & ~FD_CLOEXEC) == -1) goto handle_error;
#endif
  }
  else if (!bInheritable && bCurrentValue)
  {
#ifdef RT_DEFINE_WINDOWS
    /* Remove inheritance. */
    if (!SetHandleInformation(lpIoDevice->hHandle, HANDLE_FLAG_INHERIT, 0)) goto handle_error;
#else
    /* Add FD_CLOEXEC flag so that the descriptor will not be available in children. */
    if (fcntl(lpIoDevice->nFileDescriptor, F_SETFD, nFlags | FD_CLOEXEC) == -1) goto handle_error;
#endif
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtIoDevice_Free(RT_IO_DEVICE* lpIoDevice)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* If the function succeeds, the return value is nonzero. Otherwise, zero is returned and last error is set. */
  if (!CloseHandle(lpIoDevice->hHandle)) goto handle_error;
#else
  /* Return zero on success. On error, -1 is returned, and errno is set appropriately. */
  if (close(lpIoDevice->nFileDescriptor)) goto handle_error;
#endif
  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
