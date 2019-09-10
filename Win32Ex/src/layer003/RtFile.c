#include "layer003/RtFile.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"

#ifndef RT_DEFINE_WINDOWS

/* rwx to user, rx to group and others. */
#define RT_FILE_RIGHTS S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH

#endif

RT_B RT_API RtFile_Create(RT_FILE* lpFile, RT_CHAR* lpFilePath, RT_UN unMode)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unFlags;   /* Flags d'ouverture du fichier                              */
  DWORD unCreationDistribution;
  RT_H hFile;
#else
  int nFlags;      /* Flags d'ouverture du fichier                              */
  RT_N32 nFile;
#endif
  RT_B bResult;

  /* Flags determination. */
  switch (unMode)
  {
    case RT_FILE_MODE_READ:
#ifdef RT_DEFINE_WINDOWS
      unFlags = GENERIC_READ;
      unCreationDistribution = OPEN_EXISTING;
#else
      nFlags = O_CLOEXEC | O_RDONLY;
#endif
      break;
    case RT_FILE_MODE_READ_WRITE:
#ifdef RT_DEFINE_WINDOWS
      unFlags = GENERIC_READ | GENERIC_WRITE;
      unCreationDistribution = OPEN_ALWAYS;
#else
      nFlags = O_CLOEXEC | O_CREAT | O_RDWR;
#endif
      break;
    case RT_FILE_MODE_TRUNCATE:
#ifdef RT_DEFINE_WINDOWS
      unFlags = GENERIC_READ | GENERIC_WRITE;
      unCreationDistribution = CREATE_ALWAYS;
#else
      nFlags = O_CLOEXEC | O_CREAT | O_RDWR | O_TRUNC;
#endif
      break;
    case RT_FILE_MODE_NEW:
#ifdef RT_DEFINE_WINDOWS
      unFlags = GENERIC_READ | GENERIC_WRITE;
      unCreationDistribution = CREATE_NEW;
#else
      nFlags = O_CLOEXEC | O_CREAT | O_EXCL | O_RDWR;
#endif
      break;
    default:
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }

#ifdef RT_DEFINE_WINDOWS

  hFile = CreateFile(lpFilePath,
                     unFlags,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL,
                     unCreationDistribution,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);
  if (hFile == INVALID_HANDLE_VALUE) goto handle_error;

#else /* NOT RT_DEFINE_WINDOWS */
  nFile = open(lpFilePath, nFlags, RT_FILE_RIGHTS);
  if (nFile == -1) goto handle_error;
#endif

#ifdef RT_DEFINE_WINDOWS
  RtIoDevice_CreateFromHandle(&lpFile->rtIoDevice, hFile);
#else
  RtIoDevice_CreateFromFileDescriptor(&lpFile->rtIoDevice, nFile);
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFile_GetSize(RT_FILE* lpFile, RT_UN* lpFileSize)
{
  RT_UN unOldPos;
  RT_B bResult;

  /* Backup the current position */
  if (!RtFile_GetPointer(lpFile, &unOldPos)) goto handle_error;

  /* Go to end of file. */
  if (!RtFile_SetPointer(lpFile, 0, RT_FILE_POS_END)) goto handle_error;

  /* Get the new position which is the file size. */
  if (!RtFile_GetPointer(lpFile, lpFileSize)) goto handle_error;

  /* Go back to original position. */
  if (!RtFile_SetPointer(lpFile, unOldPos, RT_FILE_POS_BEGIN)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFile_SetPointer(RT_FILE* lpFile, RT_N nOffset, RT_UN unFrom)
{
#ifndef RT_DEFINE_WINDOWS
  RT_N32 nFlag;
#endif
  RT_B bResult;

  bResult = RT_FAILURE;

#ifdef RT_DEFINE_WINDOWS
  if (SetFilePointer(lpFile->rtIoDevice.hHandle, (LONG)nOffset, NULL, (DWORD)unFrom) != INVALID_SET_FILE_POINTER)
  {
    bResult = RT_SUCCESS;
  }
#else
  switch (unFrom)
  {
    case RT_FILE_POS_END:
      nFlag = SEEK_END;
      break;
    case RT_FILE_POS_CURRENT:
      nFlag = SEEK_CUR;
      break;
    default:
      nFlag = SEEK_SET;
      break;
  }
  if (lseek(lpFile->rtIoDevice.nFileDescriptor, nOffset, nFlag) != -1)
  {
    bResult = RT_SUCCESS;
  }
#endif

  return bResult;
}

RT_B RT_API RtFile_GetPointer(RT_FILE* lpFile, RT_UN* lpOffset)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unReturnedValue;
#else
  off_t nReturnedValue;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  unReturnedValue = SetFilePointer(lpFile->rtIoDevice.hHandle, 0, NULL, FILE_CURRENT);
  if (unReturnedValue == INVALID_SET_FILE_POINTER) goto handle_error;
  *lpOffset = unReturnedValue;
#else
  nReturnedValue = lseek(lpFile->rtIoDevice.nFileDescriptor, 0, SEEK_CUR);
  if (nReturnedValue < 0) goto handle_error;
  *lpOffset = nReturnedValue;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
