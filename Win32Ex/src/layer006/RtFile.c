#include "layer006/RtFile.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer005/RtFileSystem.h"

#ifndef RT_DEFINE_WINDOWS

/* rwx to user, rx to group and others. */
#define RT_FILE_RIGHTS S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH

#endif

RT_B RT_API RtCreateFile(RT_FILE* lpFile, RT_CHAR* lpFilePath, RT_UN unMode)
{
#ifdef RT_DEFINE_WINDOWS
  RT_UN unFlags;   /* Flags d'ouverture du fichier                              */
  RT_UN unCreationDistribution;
#else
  RT_N nFlags;    /* Flags d'ouverture du fichier                              */
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
      nFlags = O_RDONLY;
#endif
      break;
    case RT_FILE_MODE_READ_WRITE:
#ifdef RT_DEFINE_WINDOWS
      unFlags = GENERIC_READ | GENERIC_WRITE;
      unCreationDistribution = OPEN_ALWAYS;
#else
      nFlags = O_CREAT | O_RDWR;
#endif
      break;
    case RT_FILE_MODE_TRUNCATE:
#ifdef RT_DEFINE_WINDOWS
      unFlags = GENERIC_READ | GENERIC_WRITE;
      unCreationDistribution = CREATE_ALWAYS;
#else
      nFlags = O_CREAT | O_RDWR | O_TRUNC;
#endif
      break;
    case RT_FILE_MODE_NEW:
#ifdef RT_DEFINE_WINDOWS
      unFlags = GENERIC_READ | GENERIC_WRITE;
      unCreationDistribution = CREATE_NEW;
#else
      nFlags = O_CREAT | O_EXCL | O_RDWR;
#endif
      break;
    default:
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }

#ifdef RT_DEFINE_WINDOWS
  lpFile->hFile = CreateFile(lpFilePath,
                              unFlags,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL,
                              unCreationDistribution,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
  if (lpFile->hFile == INVALID_HANDLE_VALUE) goto handle_error;

#else /* NOT RT_DEFINE_WINDOWS */
  lpFile->nFile = open(lpFilePath, nFlags, RT_FILE_RIGHTS);
  if (lpFile->nFile == -1) goto handle_error;
#endif

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_B RT_API RtReadFromFile(RT_FILE* lpFile, RT_CHAR8* lpBuffer, RT_UN32 unBytesToRead)
{
#ifdef RT_DEFINE_WINDOWS
  RT_UN32 unBytesRead;    /* Récupération du nombre d'octets lus                 */
#endif
  RT_B bResult;

  bResult = RT_FALSE;

#ifdef RT_DEFINE_WINDOWS
  if (ReadFile(lpFile->hFile,
               lpBuffer,
               unBytesToRead,
               &unBytesRead,
               NULL))
    if (unBytesRead == unBytesToRead)
      bResult = RT_TRUE;
#else
  if (read(lpFile->nFile, lpBuffer, unBytesToRead) != -1)
    bResult = RT_TRUE;
#endif

  return bResult;
}

RT_B RT_API RtWriteToFile(RT_FILE* lpFile, RT_CHAR8* lpData, RT_UN32 unBytesToWrite)
{
#ifdef RT_DEFINE_WINDOWS
  RT_UN32 unBytesWritten;     /* Récupération du nombre d'octets écris           */
#endif
  RT_B bResult;

  bResult = RT_FALSE;

#ifdef RT_DEFINE_WINDOWS
  if (WriteFile(lpFile->hFile,
                lpData,
                unBytesToWrite,
                &unBytesWritten,
                NULL))
    if (unBytesWritten == unBytesToWrite)
      bResult = RT_TRUE;
#else
  if (write(lpFile->nFile, lpData, unBytesToWrite) != -1)
    bResult = RT_TRUE;
#endif

  return bResult;
}

RT_N RT_API RtComputeFileSize(RT_FILE* lpFile)
{
  RT_UN unOldPos;
  RT_N nResult;

  /* Backup the current position */
  unOldPos = RtGetFilePointer(lpFile);

  /* Go to end of file. */
  if (!RtSetFilePointer(lpFile, 0, RT_FILE_POS_END)) goto handle_error;

  /* Get the new position which is the file size. */
  nResult = RtGetFilePointer(lpFile);

  /* Go back to original position. */
  if (!RtSetFilePointer(lpFile, unOldPos, RT_FILE_POS_BEGIN)) goto handle_error;

free_resources:
  return nResult;
handle_error:
  nResult = -1;
  goto free_resources;
}

RT_B RT_API RtSetFilePointer(RT_FILE* lpFile, RT_N nOffset, RT_UN unFrom)
{
#ifndef RT_DEFINE_WINDOWS
  RT_N32 nFlag;
#endif
  RT_B bResult;

  bResult = RT_FALSE;

#ifdef RT_DEFINE_WINDOWS
  if (SetFilePointer(lpFile->hFile, nOffset, NULL, unFrom) != INVALID_SET_FILE_POINTER)
  {
    bResult = RT_TRUE;
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
  if (lseek(lpFile->nFile, nOffset, nFlag) != -1)
  {
    bResult = RT_TRUE;
  }
#endif

  return bResult;
}

RT_N RT_API RtGetFilePointer(RT_FILE* lpFile)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unReturnedValue;
#endif
  RT_N nResult;
#ifdef RT_DEFINE_WINDOWS
  unReturnedValue = SetFilePointer(lpFile->hFile, 0, NULL, FILE_CURRENT);
  if (unReturnedValue == INVALID_SET_FILE_POINTER)
  {
    nResult = -1;
  }
  else if (unReturnedValue > RT_TYPE_MAX_N)
  {
    RtSetLastError(RT_ERROR_ARITHMETIC_OVERFLOW);
    nResult = -1;
  }
  else
  {
    nResult = unReturnedValue;
  }
#else
  nResult = lseek(lpFile->nFile, 0, SEEK_CUR);
#endif
  return nResult;
}

RT_B RT_API RtFreeFile(RT_FILE* lpFile)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  bResult = CloseHandle(lpFile->hFile);
#else
  bResult = (! close(lpFile->nFile));
#endif
  return bResult;
}
