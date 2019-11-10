#include "layer004/RtFileSystem.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"
#include "layer003/RtChar.h"
#include "layer003/RtEnvVar.h"
#include "layer003/RtFile.h"

#ifndef RT_DEFINE_WINDOWS

/* rwx to user, rx to group and others. */
#define RT_FILE_SYSTEM_RIGHTS S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH

#endif

RT_B RT_API RtFileSystem_CopyFile(RT_CHAR* lpSourceFilePath, RT_CHAR* lpDestinationFilePath)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongSourceFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpLongDestinationFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unOutputSize;
#else
  RT_CHAR8 lpBuffer[BUFSIZ];
  size_t nRead;
  int nSource;
  int nDestination;
  RT_B bSourceOpen;
  RT_B bDestinationOpen;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!RtFileSystem_GetLongPath(lpSourceFilePath, lpLongSourceFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_GetLongPath(lpDestinationFilePath, lpLongDestinationFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;

  bResult = CopyFile(lpLongSourceFilePath, lpLongDestinationFilePath, TRUE);

free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

#else /* NOT RT_DEFINE_WINDOWS */

  bSourceOpen = RT_FALSE;
  bDestinationOpen = RT_FALSE;

  /* Open source. */
  nSource = open(lpSourceFilePath, O_RDONLY);
  if (nSource == -1) goto handle_error;
  bSourceOpen = RT_TRUE;

  /* Open destination. */
  nDestination = open(lpDestinationFilePath, O_WRONLY | O_CREAT | O_EXCL, RT_FILE_SYSTEM_RIGHTS);
  if (nDestination == -1) goto handle_error;
  bDestinationOpen = RT_TRUE;

  while (1)
  {
    nRead = read(nSource, lpBuffer, BUFSIZ);
    if (!nRead)
    {
      break;
    }
    else if (nRead == -1)
    {
      goto handle_error;
    }
    else
    {
      if (write(nDestination, lpBuffer, nRead) == -1)
      {
        goto handle_error;
      }
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  if (bDestinationOpen)
  {
    bDestinationOpen = RT_FALSE;
    if (close(nDestination) && bResult) goto handle_error;
  }
  if (bSourceOpen)
  {
    bSourceOpen = RT_FALSE;
    if (close(nSource) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
#endif
}

RT_B RT_API RtFileSystem_CreateDirectory(RT_CHAR* lpPath)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unOutputSize;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!RtFileSystem_GetLongPath(lpPath, lpLongPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;

  /* If CreateDirectory fails, the return value is zero and SetLastError is called. */
  if (!CreateDirectory(lpLongPath, NULL)) goto handle_error;
#else
 /* mkdir returns zero on success, or -1 if an error occurred (in which case, errno is set appropriately). */
  if (mkdir(lpPath, RT_FILE_SYSTEM_RIGHTS)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_CreateDirectories(RT_CHAR* lpPath)
{
  RT_B bResult;

  /* TODO: Implement for all levels!!! */

  if (!RtFileSystem_CheckDirectory(lpPath))
  {
    if (!RtFileSystem_CreateDirectory(lpPath)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_DeleteDirectory(RT_CHAR* lpPath)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unOutputSize;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  if (!RtFileSystem_GetLongPath(lpPath, lpLongPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;

  /* RemoveDirectory returns 0 and call SetLastError in case of error. */
  if (!RemoveDirectory(lpLongPath)) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */
  /* On success, rmdir returns 0. On error, -1 is returned, and errno is set appropriately.  */
  if (rmdir(lpPath)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_DeleteDirectoryIfExists(RT_CHAR* lpPath)
{
  RT_B bResult;

  /* Attempt to delete the file then check the last error. */
  if (!RtFileSystem_DeleteDirectory(lpPath))
  {
#ifdef RT_DEFINE_WINDOWS
    if (GetLastError() != ERROR_FILE_NOT_FOUND) goto handle_error;
#else
    if (errno != ENOENT) goto handle_error;
#endif
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL RtFileSystem_DeleteDirectoryRecursivelyCallback(RT_CHAR* lpPath, RT_UN unType, void* lpContext)
{
  RT_B bResult;

  if (unType == RT_FILE_SYSTEM_TYPE_DIRECTORY)
  {
    if (!RtFileSystem_DeleteDirectory(lpPath)) goto handle_error;
  }
  else
  {
    if (!RtFileSystem_DeleteFile(lpPath)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_DeleteDirectoryRecursively(RT_CHAR* lpPath)
{
  RT_B bResult;


  /* If the directory does not exist or is empty, then RtFileSystem_DeleteDirectoryIfExists should do the job. */
  if (!RtFileSystem_DeleteDirectoryIfExists(lpPath))
  {
    /* The directory should exist and we failed to delete it, probably because it is not empty. */
    if (!RtFileSystem_BrowsePath(lpPath, RtFileSystem_DeleteDirectoryRecursivelyCallback, RT_TRUE, RT_TRUE, RT_NULL)) goto handle_error;

    /* Finally, delete the directory that should be now empty. */
    if (!RtFileSystem_DeleteDirectory(lpPath)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_GetExecutableFilePath(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unWritten;
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
#else
  RT_UN unTextBufferSize;
#endif

#ifdef RT_DEFINE_WINDOWS
  unWritten = GetModuleFileName(NULL, lpBuffer, (DWORD)unBufferSize);
  if (!unWritten) goto handle_error;
  /* GetModuleFileName doesn't fails if the buffer is too small. */
  if (unWritten == unBufferSize - 1)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
#else
  /* Keep place for NULL character. */
  unTextBufferSize = unBufferSize - 1;

  unWritten = readlink("/proc/self/exe", lpBuffer, unTextBufferSize);
  if (unWritten == -1)
  {
    unWritten = 0;
    goto handle_error;
  }
  /* Add null trailing character as readlink doesn't. */
  lpBuffer[unWritten] = 0;

  if (unWritten == unTextBufferSize)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
#endif

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  *lpOutputSize = unWritten;
  return bResult;
}

RT_B RT_API RtFileSystem_GetTempDirectory(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* Returns the characters copied to lpBuffer, not including the zero terminating character. */
  *lpOutputSize = GetTempPath((DWORD)unBufferSize, lpBuffer);
  if (!*lpOutputSize)
  {
    goto handle_error;
  }
#else /* NOT RT_DEFINE_WINDOWS */
  if (!RtEnvVar_Get(_R("TMPDIR"), lpBuffer, unBufferSize, lpOutputSize))
  {
    /* P_tmpdir macro appears to be "/tmp". */
    if (!RtChar_CopyString(P_tmpdir, lpBuffer, unBufferSize, lpOutputSize)) goto handle_error;
  }
#endif

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  if (unBufferSize > 0)
  {
    lpBuffer[0] = 0;
  }
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_B RT_API RtFileSystem_GetApplicationDataDirectory(RT_CHAR* lpApplicationName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
#ifdef RT_DEFINE_WINDOWS
  HRESULT hResult;
#else /* NOT RT_DEFINE_WINDOWS */
  uid_t nUid;
  long int nPasswdBufferSize;
  char* lpPasswdBuffer;
  int nReturnedValue;
  RT_UN unFirstCharIndex;
  struct passwd passwordInfo;
  struct passwd* lpPasswordInfo;
#endif
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  hResult = SHGetFolderPath(RT_NULL, CSIDL_APPDATA, RT_NULL, 0, lpBuffer);
  if (FAILED(hResult))
  {
    SetLastError(HRESULT_CODE(hResult));
    goto handle_error;
  }
  unWritten = RtChar_GetStringSize(lpBuffer);
  if (!RtChar_CopyString(_R("\\"),          &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(lpApplicationName, &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
#else /* NOT RT_DEFINE_WINDOWS */

  unWritten = 0;
  if (!RtEnvVar_Get(_R("HOME"), lpBuffer, unBufferSize, &unOutputSize))
  {
    /* The getuid function is always successful. */
    nUid = getuid();

    /* Compute size for getpwuid_r buffer. */
    nPasswdBufferSize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (nPasswdBufferSize == -1)
    {
       /* Should be more than enough */
      nPasswdBufferSize = 16384;
    }

    /* Allocate buffer. */
    lpPasswdBuffer = malloc(nPasswdBufferSize);
    if (!lpPasswdBuffer) goto handle_error;

    /* The getpwuid_r function must be used in place of getpwuid in a multithreaded environment. */
    nReturnedValue = getpwuid_r(nUid, &passwordInfo, lpPasswdBuffer, nPasswdBufferSize, &lpPasswordInfo);
    /* In case of issue (Error or no matchin password), lpPasswordInfo is set to NULL. */
    if (!lpPasswordInfo)
    {
      if (nReturnedValue)
      {
        /* In case of error getpwuid_r return an error code. */
        errno = nReturnedValue;
      }
      else
      {
        /* No matchin password found. */
        RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      }
      free(lpPasswdBuffer);
      goto handle_error;
    }

    if (!RtChar_CopyString(passwordInfo.pw_dir, lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize))
    {
      free(lpPasswdBuffer);
      goto handle_error;
    }
    free(lpPasswdBuffer);
  }
  unWritten = unOutputSize;

  /* Add separator if necessary. */
  if (lpBuffer[unWritten - 1] != _R('/'))
  {
    if (!RtChar_CopyString(_R("/"), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  }

  /* Add dot for hidden directory. */
  if (!RtChar_CopyString(_R("."), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  /* Remember index of the first application name character. */
  unFirstCharIndex = unWritten;

  /* Copy application name. */
  if (!RtChar_CopyString(lpApplicationName, &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  /* Lower first character, "ApplicationName" -> "applicationName". */
  lpBuffer[unFirstCharIndex] = RtChar_FastLower(lpBuffer[unFirstCharIndex]);
#endif

  *lpOutputSize = unWritten;
  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  if (unBufferSize > 0)
  {
    lpBuffer[0] = 0;
  }
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_UN RT_API RtFileSystem_GetLastSeparator(RT_CHAR* lpPath, RT_UN unPathSize)
{
  RT_UN unI;

  /* Skip trailing separators. */
  unI = unPathSize - 1;
  while ((unI < RT_TYPE_MAX_UN) && RT_FILE_SYSTEM_IS_SEPARATOR(lpPath[unI]))
  {
    unI--;
  }

  /* Search next separator. */
  while ((unI < RT_TYPE_MAX_UN) && !RT_FILE_SYSTEM_IS_SEPARATOR(lpPath[unI]))
  {
    unI--;
  }

  return unI;
}

void RT_API RtFileSystem_RemoveTrailingSeparators(RT_CHAR* lpPath, RT_UN unPathSize, RT_UN* lpOutputSize)
{
  RT_UN unI;

  /* Skip trailing separators. */
  unI = unPathSize - 1;
  while ((unI < RT_TYPE_MAX_UN) && RT_FILE_SYSTEM_IS_SEPARATOR(lpPath[unI]))
  {
    lpPath[unI] = 0;
    unI--;
  }

  /* Separators only. */
  if (unI == RT_TYPE_MAX_UN && unPathSize > 0)
  {
#ifdef RT_DEFINE_WINDOWS
    lpPath[0] = _R('.');
#else /* NOT RT_DEFINE_WINDOWS */

    lpPath[0] = _R('/');
#endif
    unI++;
  }

  *lpOutputSize = (unI + 1);
}

RT_B RT_API RtFileSystem_GetParentPath(RT_CHAR* lpPath, RT_UN unPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unLastSeparator;
  RT_UN unOutputSize;
  RT_B bResult;

  unLastSeparator = RtFileSystem_GetLastSeparator(lpPath, unPathSize);

  if ((unLastSeparator == RT_TYPE_MAX_UN) || (unLastSeparator == 0))
  {
#ifdef RT_DEFINE_WINDOWS
    bResult = RtChar_CopyString(_R("."), lpBuffer, unBufferSize, lpOutputSize);
#else /* NOT RT_DEFINE_WINDOWS */
    if (unLastSeparator == RT_TYPE_MAX_UN)
    {
      if ((unPathSize > 0) && (lpPath[0] == _R('/')))
      {
        /* Separator only, RtFileSystem_GetLastSeparator returns RT_TYPE_MAX_UN because it is a trailing separator. */
        bResult = RtChar_CopyString(_R("/"), lpBuffer, unBufferSize, lpOutputSize);
      }
      else
      {
        /* Name without separator, "data.txt" -> ".". */
        bResult = RtChar_CopyString(_R("."), lpBuffer, unBufferSize, lpOutputSize);
      }
    }
    if (unLastSeparator == 0)
    {
      /* Last separator is root, "/var" -> "/". */
      bResult = RtChar_CopyString(_R("/"), lpBuffer, unBufferSize, lpOutputSize);
    }
#endif
  }
  else
  {
    if (lpPath == lpBuffer)
    {
      /* We only "cut" the path at the right place. */
      lpPath[unLastSeparator] = 0;
    }
    else
    {
      /* We have to copy the parent path into the buffer. */
      if (!RtChar_CopyStringWithSize(lpPath, unLastSeparator, lpBuffer, unBufferSize, &unOutputSize)) goto handle_error;
    }
    RtFileSystem_RemoveTrailingSeparators(lpBuffer, unLastSeparator, lpOutputSize);
    bResult = RT_SUCCESS;
  }

  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_B RT_API RtFileSystem_GetCurrentDirectory(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unWritten;
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  unWritten = GetCurrentDirectory((DWORD)unBufferSize, lpBuffer);
  if (!unWritten) goto handle_error;
  /* GetCurrentDirectory returns the required buffer size if the buffer is too small. */
  if (unWritten > unBufferSize)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
#else /* NOT RT_DEFINE_WINDOWS */

  if (!getcwd(lpBuffer, unBufferSize)) goto handle_error;
  unWritten = strlen(lpBuffer);
#endif

  *lpOutputSize = unWritten;
  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_B RT_API RtFileSystem_SetCurrentDirectory(RT_CHAR* lpPath)
{
  RT_B bResult;
#ifdef RT_DEFINE_WINDOWS
  /* Returns zero if failed, set last error. */
  /* TODO: Needs RtFileSystem_GetLongPath? */
  bResult = SetCurrentDirectory(lpPath);
#else
  /* Returns zero in case of success, set errno. */
  bResult = !chdir(lpPath);
#endif
  return bResult;
}

RT_B RT_API RtFileSystem_GetFileSize(RT_CHAR* lpFilePath, RT_UN64* lpFileSize)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  WIN32_FILE_ATTRIBUTE_DATA fileInfo;
  LARGE_INTEGER largeInteger;
  RT_UN unOutputSize;
#else
  struct stat fileInfo;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!RtFileSystem_GetLongPath(lpFilePath, lpLongPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;

  /* GetFileAttributesEx returns 0 and use SetLastError in case of error. */
  if (!GetFileAttributesEx(lpLongPath, GetFileExInfoStandard, &fileInfo)) goto handle_error;

  largeInteger.HighPart = fileInfo.nFileSizeHigh;
  largeInteger.LowPart = fileInfo.nFileSizeLow;
  *lpFileSize = largeInteger.QuadPart;

#else /* RT_DEFINE_WINDOWS */

  /* stat returns zero in case of success, -1 in case of failure and set errno. */
  if (stat(lpFilePath, &fileInfo)) goto handle_error;
  *lpFileSize = fileInfo.st_size;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

/* TODO: WTF. */
RT_B RT_API FileSys_IsFullPath(RT_CHAR* lpPath)
{
  RT_B bResult;
#ifdef RT_DEFINE_WINDOWS
  bResult = ((lstrlen(lpPath) >= 2) && (lpPath[1] == ':'));
#else /* RT_DEFINE_WINDOWS */

  bResult = (lpPath[0] == '/');
#endif
  return bResult;
}

/* TODO: WTF. */
RT_B RT_API FileSys_FullPath(RT_CHAR* lpPath, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  unWritten = 0;
  if (!RtFileSystem_GetCurrentDirectory(                             &lpBuffer[unWritten], unBufferSize - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyStringWithSize(RT_FILE_SYSTEM_SEPARATOR_STRING, 1, &lpBuffer[unWritten], unBufferSize - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(lpPath,                                     &lpBuffer[unWritten], unBufferSize - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  *lpOutputSize = unWritten;
  return bResult;
}

#ifdef RT_DEFINE_WINDOWS
RT_B RT_API RtFileSystem_GetLongPath(RT_CHAR* lpPath, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_UN unI;
  RT_B bResult;

  unWritten = 0;
  /* Add long path prefix. */
  if (!RtChar_CopyString(_R("\\\\?\\"), lpBuffer, unBufferSize, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!FileSys_IsFullPath(lpPath))
  {
    /* Convert the relative path as a full path. */
    if (!FileSys_FullPath(lpPath, &lpBuffer[unWritten], unBufferSize - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  }
  else
  {
    /* Copy the full path after the prefix. */
    if (!RtChar_CopyString(lpPath, &lpBuffer[unWritten], unBufferSize - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  }

  /* Long paths are not supporting '/' as separator. */
  for (unI = 0; unI < unWritten; unI++)
  {
    if (lpBuffer[unI] == '/')
    {
      lpBuffer[unI] = '\\';
    }
  }

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  *lpOutputSize = unWritten;
  return bResult;
}
#endif

RT_B RT_API RtFileSystem_BrowsePath(RT_CHAR* lpPath, RT_FILE_SYSTEM_BROWSE_CALLBACK lpCallBack, RT_B bRecursively, RT_B bChildrenFirst, void* lpContext)
{
#ifdef RT_DEFINE_WINDOWS
  WIN32_FIND_DATA findFileData;       /* Information sur les fichiers trouvés */
  HANDLE hFind;                       /* Handle sur la recherche              */
  RT_UN unOutputSize;
#else
  DIR* lpDir;
  struct dirent *lpDirEntry;
#endif
  RT_CHAR lpChild[RT_FILE_SYSTEM_MAX_FILE_PATH]; /* Chemin de travail                    */
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  hFind = INVALID_HANDLE_VALUE;

  if (!RtFileSystem_GetLongPath(lpPath, lpChild, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  /* TODO: Buffer overflow!? */
  lstrcat(lpChild, _R("\\*"));

  hFind = FindFirstFile(lpChild, &findFileData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    if (GetLastError() == ERROR_FILE_NOT_FOUND)
    {
      bResult = RT_SUCCESS;
      goto free_resources;
    }
    else
    {
      goto handle_error;
    }
  }

  do
  {
    if (lstrcmp(_R("."),  findFileData.cFileName) &&
        lstrcmp(_R(".."), findFileData.cFileName))
    {
      lstrcpy(lpChild, lpPath);
      lstrcat(lpChild, _R("\\"));
      lstrcat(lpChild, findFileData.cFileName);

      if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if (!bChildrenFirst)
        {
          if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_DIRECTORY, lpContext)) goto handle_error;
        }

        if (bRecursively)
        {
          if (!RtFileSystem_BrowsePath(lpChild, lpCallBack, bRecursively, bChildrenFirst, lpContext)) goto handle_error;
        }

        if (bChildrenFirst)
        {
          if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_DIRECTORY, lpContext)) goto handle_error;
        }
      }
      else
      {
        if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_FILE, lpContext)) goto handle_error;
      }
    }
  }
  while (FindNextFile(hFind, &findFileData));

  /* Check that we have exited loop because there are no more files. */
  if (GetLastError() != ERROR_NO_MORE_FILES)
  {
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  if (hFind != INVALID_HANDLE_VALUE)
  {
    if (!FindClose(hFind) && bResult)
    {
      hFind = INVALID_HANDLE_VALUE;
      goto handle_error;
    }
    hFind = INVALID_HANDLE_VALUE;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;

#else /* NOT RT_DEFINE_WINDOWS */

  lpDir = RT_NULL;

  lpDir = opendir(lpPath);
  if (!lpDir) goto handle_error;

  while (1)
  {
    errno = 0;
    lpDirEntry = readdir(lpDir);
    /* NULL is returned in case of error or if the the directory content has been read. */
    if (!lpDirEntry)
    {
      if (errno != 0) goto handle_error;

      break;
    }
    else
    {
      if (strcmp(_R("."),  lpDirEntry->d_name) &&
          strcmp(_R(".."), lpDirEntry->d_name))
      {
        strcpy(lpChild, lpPath);
        strcat(lpChild, _R("/"));
        strcat(lpChild, lpDirEntry->d_name);
        if (lpDirEntry->d_type == DT_DIR)
        {
          if (!bChildrenFirst)
          {
            if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_DIRECTORY, lpContext)) goto handle_error;
          }

          if (bRecursively)
          {
            if (!RtFileSystem_BrowsePath(lpChild, lpCallBack, bRecursively, bChildrenFirst, lpContext)) goto handle_error;
          }

          if (bChildrenFirst)
          {
            if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_DIRECTORY, lpContext)) goto handle_error;
          }
        }
        else
        {
          if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_FILE, lpContext)) goto handle_error;
        }
      }
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  if (lpDir)
  {
    /* The closedir() function returns 0 on success. */
    if (closedir(lpDir) && bResult)
    {
      lpDir = RT_NULL;
      goto handle_error;
    }
    lpDir = RT_NULL;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
#endif
}

RT_B RT_API RtFileSystem_CheckPath(RT_CHAR* lpPath, RT_UN unType)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  DWORD nAttributes;
  RT_UN unOutputSize;
#else
  struct stat stats;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  if (!RtFileSystem_GetLongPath(lpPath, lpLongPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize))
  {
    bResult = RT_FAILURE;
  }
  else
  {
    nAttributes = GetFileAttributes(lpLongPath);
    if (nAttributes == INVALID_FILE_ATTRIBUTES)
    {
      bResult = RT_FAILURE;
    }
    else if (nAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      /* The path is a directory. */
      if (unType & RT_FILE_SYSTEM_TYPE_DIRECTORY)
      {
        bResult = RT_SUCCESS;
      }
      else
      {
        /* We did not expect a directory. */
        RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
        bResult = RT_FAILURE;
      }
    }
    else
    {
      /* The path is a file. */
      if (unType & RT_FILE_SYSTEM_TYPE_FILE)
      {
        bResult = RT_SUCCESS;
      }
      else
      {
        /* We did not expect a file. */
        RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
        bResult = RT_FAILURE;
      }
    }
  }
#else /* NOT RT_DEFINE_WINDOWS */

  if (stat(lpPath, &stats))
  {
    bResult = RT_FAILURE;
  }
  else
  {
    if (S_ISDIR(stats.st_mode))
    {
      /* The path is a directory. */
      if (unType & RT_FILE_SYSTEM_TYPE_DIRECTORY)
      {
        bResult = RT_SUCCESS;
      }
      else
      {
        /* We did not expect a directory. */
        RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
        bResult = RT_FAILURE;
      }
    }
    else
    {
      /* The path is a file. */
      if (unType & RT_FILE_SYSTEM_TYPE_FILE)
      {
        bResult = RT_SUCCESS;
      }
      else
      {
        /* We did not expect a file. */
        RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
        bResult = RT_FAILURE;
      }
    }
  }

#endif
  return bResult;
}

RT_B RT_API RtFileSystem_CheckFile(RT_CHAR* lpPath)
{
  return RtFileSystem_CheckPath(lpPath, RT_FILE_SYSTEM_TYPE_FILE);
}

RT_B RT_API RtFileSystem_CheckDirectory(RT_CHAR* lpPath)
{
  return RtFileSystem_CheckPath(lpPath, RT_FILE_SYSTEM_TYPE_DIRECTORY);
}

RT_B RT_API RtFileSystem_CheckFileOrDirectory(RT_CHAR* lpPath)
{
  return RtFileSystem_CheckPath(lpPath, RT_FILE_SYSTEM_TYPE_FILE | RT_FILE_SYSTEM_TYPE_DIRECTORY);
}

RT_B RT_API RtFileSystem_GetFileName(RT_CHAR* lpPath, RT_UN unPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unLastSeparator;
  RT_UN unInputSize;
  RT_B bResult;


  unLastSeparator = RtFileSystem_GetLastSeparator(lpPath, unPathSize);
  unInputSize = 0;
  if (!RtChar_CopyString(&lpPath[unLastSeparator + 1], lpBuffer, unBufferSize, &unInputSize)) goto handle_error;
  RtFileSystem_RemoveTrailingSeparators(lpBuffer, unInputSize, lpOutputSize);

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_CreateEmptyFile(RT_CHAR* lpPath, RT_B bTruncate)
{
  RT_FILE rtFile;
  RT_B bFileCreated;
  RT_B bResult;

  bFileCreated = RT_FALSE;

  if (!RtFile_Create(&rtFile, lpPath, bTruncate ? RT_FILE_MODE_TRUNCATE : RT_FILE_MODE_NEW)) goto handle_error;
  bFileCreated = RT_TRUE;

  bResult = RT_SUCCESS;
free_resources:
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtIoDevice_Free(&rtFile.rtIoDevice) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL RtFileSystem_MoveOrRenameFile(RT_CHAR* lpCurrentFilePath, RT_CHAR* lpNewFilePath, RT_B bRename)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongCurrentFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpLongNewFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unOutputSize;
#else /* NOT RT_DEFINE_WINDOWS */

#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!RtFileSystem_GetLongPath(lpCurrentFilePath, lpLongCurrentFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_GetLongPath(lpNewFilePath, lpLongNewFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!MoveFileEx(lpCurrentFilePath, lpNewFilePath, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH)) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */

  /* rename is faster under Linux but does not work across files systems/mount points. */
  if (rename(lpCurrentFilePath, lpNewFilePath))
  {
    if (bRename)
    {
      /* Rename has failed while it is a renaming (So same file system/mount point). */
      goto handle_error;
    }
    /* The rename call has failed, try another approach to move: copy then delete. */
    if (!RtFileSystem_CopyFile(lpCurrentFilePath, lpNewFilePath)) goto handle_error;
    if (!RtFileSystem_DeleteFile(lpCurrentFilePath)) goto handle_error;
  }
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_MoveFile(RT_CHAR* lpCurrentFilePath, RT_CHAR* lpNewFilePath)
{
  return RtFileSystem_MoveOrRenameFile(lpCurrentFilePath, lpNewFilePath, RT_FALSE);
}

RT_B RT_API RtFileSystem_AppendSeparator(RT_CHAR* lpPath, RT_UN unPathSize, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_CHAR nLastChar;
  RT_UN unOutputSize;
  RT_B bResult;

  nLastChar = lpPath[unPathSize - 1];
  if (RT_FILE_SYSTEM_IS_SEPARATOR(nLastChar))
  {
    /* Separator already exists. */
    *lpOutputSize = unPathSize;
  }
  else
  {
    /* Append separator. */
    if (!RtChar_Copy(RT_FILE_SYSTEM_SEPARATOR, &lpPath[unPathSize], unBufferSize - unPathSize, &unOutputSize)) goto handle_error;
    *lpOutputSize = unPathSize + 1;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_BuildPath(RT_CHAR* lpParentPath, RT_UN unParentPathSize, RT_CHAR* lpChildName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  if (lpParentPath == lpBuffer)
  {
    /* lpBuffer already contains the parent path. */
    unWritten = 0;
    if (!RtFileSystem_AppendSeparator(lpBuffer, unParentPathSize, unBufferSize, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
    if (!RtChar_CopyString(lpChildName, &lpBuffer[unWritten], unBufferSize - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  }
  else
  {
    if (!RtChar_CopyStringWithSize(lpParentPath, unParentPathSize, lpBuffer, unBufferSize, &unOutputSize)) goto handle_error;
    if (!RtFileSystem_AppendSeparator(lpBuffer, unOutputSize, unBufferSize, &unOutputSize)) goto handle_error;
    unWritten = unOutputSize;
    if (!RtChar_CopyString(lpChildName, &lpBuffer[unOutputSize], unBufferSize - unOutputSize, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  }

  *lpOutputSize = unWritten;
  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  if (unBufferSize > 0)
  {
    lpParentPath[0] = 0;
  }
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_B RT_API RtFileSystem_RenameFile(RT_CHAR* lpCurrentFilePath, RT_CHAR* lpNewFileName)
{
  RT_CHAR lpNewFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unOutputSize;
  RT_B bResult;

  if (!RtFileSystem_GetParentPath(lpCurrentFilePath, RtChar_GetStringSize(lpCurrentFilePath), lpNewFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_AppendSeparator(lpNewFilePath, unOutputSize, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtChar_CopyString(lpNewFileName, &lpNewFilePath[unOutputSize], RT_FILE_SYSTEM_MAX_FILE_PATH - unOutputSize, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_MoveOrRenameFile(lpCurrentFilePath, lpNewFilePath, RT_TRUE)) goto handle_error;

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  return bResult;
}

RT_B RT_API RtFileSystem_DeleteFileIfExists(RT_CHAR* lpFilePath)
{
  RT_B bResult;

  /* Attempt to delete the file then check the last error. */
  if (!RtFileSystem_DeleteFile(lpFilePath))
  {
#ifdef RT_DEFINE_WINDOWS
    if (GetLastError() != ERROR_FILE_NOT_FOUND) goto handle_error;
#else
    if (errno != ENOENT) goto handle_error;
#endif
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtFileSystem_DeleteFile(RT_CHAR* lpFilePath)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unOutputSize;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  if (!RtFileSystem_GetLongPath(lpFilePath, lpLongFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;

  /* DeleteFile returns 0 and call SetLastError in case of error. */
  if (!DeleteFile(lpLongFilePath)) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */
  /* On success, unlink returns 0. On error, -1 is returned, and errno is set appropriately.  */
  if (unlink(lpFilePath)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
