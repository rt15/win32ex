#include "layer005/RtFile.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"
#include "layer003/RtChar.h"
#include "layer004/RtFileSystem.h"

#ifndef RT_DEFINE_WINDOWS

/* rwx to user, rx to group and others. */
#define RT_FILE_RIGHTS S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH

#endif

RT_B RT_API RtCreateFile(RT_FILE* lpFile, RT_CHAR* lpFilePath, RT_UN unMode)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unFlags;   /* Flags d'ouverture du fichier                              */
  DWORD unCreationDistribution;
#else
  int nFlags;      /* Flags d'ouverture du fichier                              */
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

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtCreateTempFile(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten)
{
  RT_UN unWritten;
  RT_B bResult;

  /* Little trick: use the buffer to store the temp directory. */
  unWritten = 0;
  if (!RtFileSystem_GetTempDirectory(lpBuffer, unBufferSize, &unWritten)) goto handle_error;

  if (!RtCreateTempFileWithParentPath(lpFile, lpPrefix, lpBuffer, unWritten, lpBuffer, unBufferSize, lpWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtCreateTempFileWithParentPath(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpParentPath, RT_UN unParentPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten)
{
#ifdef RT_DEFINE_LINUX
  RT_UN unWritten;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* GetTempFileName create an empty file. */
  if (!GetTempFileName(lpParentPath, lpPrefix, 0, lpBuffer)) goto handle_error;

  if (!RtCreateFile(lpFile, lpBuffer, RT_FILE_MODE_TRUNCATE)) goto handle_error;

  *lpWritten += RtChar_GetStringSize(lpBuffer);
#else
  unWritten = unParentPathSize;
  if (!RtFileSystem_BuildPath(lpParentPath, unParentPathSize, lpPrefix, unBufferSize, &unWritten)) goto handle_error;

  /* The 6 last characters of mkstemp template must be "XXXXXX" and they will be replaced by mkstemp. */
  if (!RtChar_CopyStringWithSize(_R("XXXXXX"), 6, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;

  /* Returns -1 and set errno in case of error. */
  lpFile->nFile = mkostemp(lpBuffer, O_CLOEXEC);
  if (lpFile->nFile == -1) goto handle_error;

  *lpWritten += unWritten;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtReadFromFile(RT_FILE* lpFile, RT_CHAR8* lpBuffer, RT_UN unBytesToRead, RT_UN* lpBytesRead)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unBytesRead;
#else
  ssize_t nBytesRead;
#endif
  RT_B bResult;

  bResult = RT_FAILURE;

#ifdef RT_DEFINE_WINDOWS
  /* TODO: Manage more than 4Go? */
  if (!ReadFile(lpFile->hFile, lpBuffer, (DWORD)unBytesToRead, &unBytesRead, NULL)) goto handle_error;
  *lpBytesRead = unBytesRead;
#else
  /* read returns -1 and set errno in case of issue. */
  nBytesRead = read(lpFile->nFile, lpBuffer, unBytesToRead);
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

RT_B RT_API RtWriteToFile(RT_FILE* lpFile, RT_CHAR8* lpData, RT_UN unBytesToWrite)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unBytesWritten;
#endif
  RT_B bResult;

  bResult = RT_FAILURE;

#ifdef RT_DEFINE_WINDOWS
  if (WriteFile(lpFile->hFile,
                lpData,
                (DWORD)unBytesToWrite,
                &unBytesWritten,
                NULL))
    if (unBytesWritten == unBytesToWrite)
      bResult = RT_SUCCESS;
#else
  if (write(lpFile->nFile, lpData, unBytesToWrite) != -1)
    bResult = RT_SUCCESS;
#endif

  return bResult;
}

RT_UN RT_API RtGetFileSize(RT_FILE* lpFile)
{
  RT_UN unOldPos;
  RT_UN unResult;

  /* Backup the current position */
  unOldPos = RtGetFilePointer(lpFile);

  /* Go to end of file. */
  if (!RtSetFilePointer(lpFile, 0, RT_FILE_POS_END)) goto handle_error;

  /* Get the new position which is the file size. */
  unResult = RtGetFilePointer(lpFile);

  /* Go back to original position. */
  if (!RtSetFilePointer(lpFile, unOldPos, RT_FILE_POS_BEGIN)) goto handle_error;

free_resources:
  return unResult;
handle_error:
  unResult = RT_TYPE_MAX_UN;
  goto free_resources;
}

RT_B RT_API RtSetFilePointer(RT_FILE* lpFile, RT_N nOffset, RT_UN unFrom)
{
#ifndef RT_DEFINE_WINDOWS
  RT_N32 nFlag;
#endif
  RT_B bResult;

  bResult = RT_FAILURE;

#ifdef RT_DEFINE_WINDOWS
  if (SetFilePointer(lpFile->hFile, (LONG)nOffset, NULL, (DWORD)unFrom) != INVALID_SET_FILE_POINTER)
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
  if (lseek(lpFile->nFile, nOffset, nFlag) != -1)
  {
    bResult = RT_SUCCESS;
  }
#endif

  return bResult;
}

RT_UN RT_API RtGetFilePointer(RT_FILE* lpFile)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unReturnedValue;
#else
  off_t nReturnedValue;
#endif
  RT_UN unResult;
#ifdef RT_DEFINE_WINDOWS
  unReturnedValue = SetFilePointer(lpFile->hFile, 0, NULL, FILE_CURRENT);
  if (unReturnedValue == INVALID_SET_FILE_POINTER)
  {
    unResult = RT_TYPE_MAX_UN;
  }
  else
  {
    unResult = unReturnedValue;
  }
#else
  nReturnedValue = lseek(lpFile->nFile, 0, SEEK_CUR);
  if (nReturnedValue < 0)
  {
    unResult = RT_TYPE_MAX_UN;
  }
  else
  {
    unResult = nReturnedValue;
  }
#endif
  return unResult;
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
