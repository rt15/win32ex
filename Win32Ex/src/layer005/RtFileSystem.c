#include "layer005/RtFileSystem.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer003/RtEnvironmentVariable.h"
#include "layer004/RtChar.h"

#ifndef RT_DEFINE_WINDOWS

/* rwx to user, rx to group and others. */
#define RT_FILE_SYSTEM_RIGHTS S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH

#endif

RT_B RT_API RtCopyFile(RT_CHAR* lpSourceFilePath, RT_CHAR* lpDestinationFilePath)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongSourceFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpLongDestinationFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nWritten;
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
  if (!RtComputeLongPath(lpSourceFilePath, lpLongSourceFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto handle_error;
  if (!RtComputeLongPath(lpDestinationFilePath, lpLongDestinationFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto handle_error;

  bResult = CopyFile(lpLongSourceFilePath, lpLongDestinationFilePath, TRUE);

free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
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

  bResult = RT_TRUE;
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
  bResult = RT_FALSE;
  goto free_resources;
#endif
}

RT_B RT_API RtCreateDirectory(RT_CHAR* lpPath)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nWritten;
#else

#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (RtComputeLongPath(lpPath, lpLongPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten))
  {
    bResult = CreateDirectory(lpLongPath, NULL);
  }
  else
  {
    bResult = RT_FALSE;
  }
#else
  bResult = !mkdir(lpPath, RT_FILE_SYSTEM_RIGHTS);
#endif
  return bResult;
}

RT_B RT_API RtGetExecutableFilePath(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_N nWritten;
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
#else
  RT_N nTextBufferSize;
#endif

#ifdef RT_DEFINE_WINDOWS
  nWritten = GetModuleFileName(NULL, lpBuffer, nBufferSize);
  if (!nWritten) goto handle_error;
  /* GetModuleFileName doesn't fails if the buffer is too small. */
  if (nWritten == nBufferSize - 1)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
#else
  /* Keep place for NULL character. */
  nTextBufferSize = nBufferSize - 1;

  nWritten = readlink("/proc/self/exe", lpBuffer, nTextBufferSize);
  if (nWritten == -1)
  {
    nWritten = 0;
    goto handle_error;
  }
  /* Add null trailing character as readlink doesn't. */
  lpBuffer[nWritten] = 0;

  if (nWritten == nTextBufferSize)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
#endif

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  (*lpWritten) += nWritten;
  return bResult;
}

RT_B RT_API RtGetTempDirectory(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
#ifdef RT_DEFINE_WINDOWS

#else /* NOT RT_DEFINE_WINDOWS */

#endif
  RT_N nWritten;
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* Returns the characters copied to lpBuffer, not including the zero terminating character. */
  nWritten = GetTempPath(nBufferSize, lpBuffer);
  if (!nWritten)
  {
    goto handle_error;
  }
#else /* NOT RT_DEFINE_WINDOWS */
  nWritten = 0;
  if (!RtGetEnvironmentVariable(_R("TMPDIR"), lpBuffer, nBufferSize, &nWritten))
  {
    /* P_tmpdir macro appears to be "/tmp". */
    if (!RtCopyString(P_tmpdir, lpBuffer, nBufferSize, &nWritten)) goto handle_error;
  }
#endif

  (*lpWritten) += nWritten;
  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  if (nBufferSize > 0)
  {
    lpBuffer[0] = 0;
  }
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_B RT_API RtGetApplicationDataDirectory(RT_CHAR* lpApplicationName, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
#ifdef RT_DEFINE_WINDOWS
  HRESULT hResult;
#else /* NOT RT_DEFINE_WINDOWS */
  uid_t nUid;
  long int nPasswdBufferSize;
  char* lpPasswdBuffer;
  int nReturnedValue;
  RT_N nFirstCharIndex;
  struct passwd passwordInfo;
  struct passwd* lpPasswordInfo;
#endif
  RT_N nWritten;
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  hResult = SHGetFolderPath(RT_NULL, CSIDL_APPDATA, RT_NULL, 0, lpBuffer);
  if (FAILED(hResult))
  {
    SetLastError(HRESULT_CODE(hResult));
    goto handle_error;
  }
  nWritten = RtGetStringSize(lpBuffer);
  if (!RtCopyString(_R("\\"),          &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto handle_error;
  if (!RtCopyString(lpApplicationName, &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */

  nWritten = 0;
  if (!RtGetEnvironmentVariable(_R("HOME"), lpBuffer, nBufferSize, &nWritten))
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
        RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      }
      free(lpPasswdBuffer);
      goto handle_error;
    }

    if (!RtCopyString(passwordInfo.pw_dir, lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten))
    {
      free(lpPasswdBuffer);
      goto handle_error;
    }
    free(lpPasswdBuffer);
  }

  /* Add separator if necessary. */
  if (lpBuffer[nWritten - 1] != _R('/'))
  {
    if (!RtCopyString(_R("/"), &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto handle_error;
  }

  /* Add dot for hidden directory. */
  if (!RtCopyString(_R("."), &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto handle_error;

  /* Remember index of the first application name character. */
  nFirstCharIndex = nWritten;

  /* Copy application name. */
  if (!RtCopyString(lpApplicationName, &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto handle_error;

  /* Lower first character, "ApplicationName" -> "applicationName". */
  lpBuffer[nFirstCharIndex] = RtFastLowerChar(lpBuffer[nFirstCharIndex]);
#endif

  (*lpWritten) += nWritten;
  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  if (nBufferSize > 0)
  {
    lpBuffer[0] = 0;
  }
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_N RT_API RtGetLastSeparator(RT_CHAR* lpPath, RT_N nPathSize)
{
  RT_N nI;

  /* Skip trailing separators. */
  nI = nPathSize - 1;
  while ((nI >= 0) && RT_FILE_SYSTEM_IS_SEPARATOR(lpPath[nI]))
  {
    nI--;
  }

  /* Search next separator. */
  while ((nI >= 0) && !RT_FILE_SYSTEM_IS_SEPARATOR(lpPath[nI]))
  {
    nI--;
  }

  return nI;
}

void RT_API RtRemoveTrailingSeparators(RT_CHAR* lpPath, RT_N nPathSize, RT_N *lpWritten)
{
  RT_N nI;

  /* Skip trailing separators. */
  nI = nPathSize - 1;
  while ((nI >= 0) && RT_FILE_SYSTEM_IS_SEPARATOR(lpPath[nI]))
  {
    lpPath[nI] = 0;
    nI--;
  }

  /* Separators only. */
  if (nI == -1 && nPathSize > 0)
  {
#ifdef RT_DEFINE_WINDOWS
    lpPath[0] = _R('.');
#else /* NOT RT_DEFINE_WINDOWS */

    lpPath[0] = _R('/');
#endif
    nI++;
  }

  *lpWritten = *lpWritten - (nPathSize - (nI + 1));
}

RT_B RT_API RtExtractParentPath(RT_CHAR* lpPath, RT_N nPathSize, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N *lpWritten)
{
  RT_N nLastSeparator;
  RT_B bResult;

  nLastSeparator = RtGetLastSeparator(lpPath, nPathSize);

  if ((nLastSeparator == -1) || (nLastSeparator == 0))
  {
#ifdef RT_DEFINE_WINDOWS
    bResult = RtCopyString(_R("."), lpBuffer, nBufferSize, lpWritten);
#else /* NOT RT_DEFINE_WINDOWS */
    if (nLastSeparator == -1)
    {
      if ((nPathSize > 0) && (lpPath[0] == _R('/')))
      {
        /* Separator only, RtGetLastSeparator returns -1 because it is a trailing separator. */
        bResult = RtCopyString(_R("/"), lpBuffer, nBufferSize, lpWritten);
      }
      else
      {
        /* Name without separator, "data.txt" -> ".". */
        bResult = RtCopyString(_R("."), lpBuffer, nBufferSize, lpWritten);
      }
    }
    if (nLastSeparator == 0)
    {
      /* Last separator is root, "/var" -> "/". */
      bResult = RtCopyString(_R("/"), lpBuffer, nBufferSize, lpWritten);
    }
#endif
  }
  else
  {
    if (!RtCopyStringWithSize(lpPath, nLastSeparator, lpBuffer, nBufferSize, lpWritten)) goto handle_error;
    RtRemoveTrailingSeparators(lpBuffer, nLastSeparator, lpWritten);
    bResult = RT_TRUE;
  }

  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_B RT_API RtGetCurrentDirectory(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N *lpWritten)
{
  RT_N nWritten;
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  nWritten = GetCurrentDirectory(nBufferSize, lpBuffer);
  if (!nWritten) goto handle_error;
  /* GetCurrentDirectory returns the required buffer size if the buffer is too small. */
  if (nWritten > nBufferSize)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
#else /* NOT RT_DEFINE_WINDOWS */

  if (!getcwd(lpBuffer, nBufferSize)) goto handle_error;
  nWritten = strlen(lpBuffer);
#endif

  *lpWritten += nWritten;
  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

/**
 * Return -1 in case of error.
 */
RT_N RT_API RtGetFileSize(RT_CHAR* lpPath)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  WIN32_FILE_ATTRIBUTE_DATA fileInfo;
  LARGE_INTEGER largeInteger;
  RT_N nWritten;
#else
  struct stat fileInfo;
#endif
  RT_N nResult;

#ifdef RT_DEFINE_WINDOWS
  if (RtComputeLongPath(lpPath, lpLongPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten))
  {
    if (GetFileAttributesEx(lpLongPath, GetFileExInfoStandard, &fileInfo))
    {
      largeInteger.HighPart = fileInfo.nFileSizeHigh;
      largeInteger.LowPart = fileInfo.nFileSizeLow;
      nResult = (RT_N)largeInteger.QuadPart;
    }
    else
    {
      nResult = -1;
    }
  }
  else
  {
    nResult = -1;
  }
#else /* RT_DEFINE_WINDOWS */

  if (stat(lpPath, &fileInfo) == 0)
  {
    nResult = fileInfo.st_size;
  }
  else
  {
    nResult = -1;
  }
#endif

  return nResult;
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
RT_B RT_API FileSys_FullPath(RT_CHAR* lpPath, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_N nWritten;
  RT_B bResult;

  nWritten = 0;
  if (!RtGetCurrentDirectory(                                   &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;
  if (!RtCopyStringWithSize(RT_FILE_SYSTEM_SEPARATOR_STRING, 1, &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;
  if (!RtCopyString(lpPath,                                     &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  *lpWritten += nWritten;
  return bResult;
}

#ifdef RT_DEFINE_WINDOWS
RT_B RT_API RtComputeLongPath(RT_CHAR* lpPath, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_N nWritten;
  RT_N nI;
  RT_B bResult;

  nWritten = 0;
  /* Add long path prefix. */
  if (!RtCopyString(_R("\\\\?\\"), lpBuffer, nBufferSize, &nWritten)) goto handle_error;

  if (!FileSys_IsFullPath(lpPath))
  {
    /* Convert the relative path as a full path. */
    if (!FileSys_FullPath(lpPath, &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;
  }
  else
  {
    /* Copy the full path after the prefix. */
    if (!RtCopyString(lpPath, &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;
  }

  /* Long paths are not supporting '/' as separator. */
  for (nI = 0; nI < nWritten; nI++)
  {
    if (lpBuffer[nI] == '/')
    {
      lpBuffer[nI] = '\\';
    }
  }

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  *lpWritten += nWritten;
  return bResult;
}
#endif

RT_B RT_API RtBrowsePath(RT_CHAR* lpPath, RT_FILE_SYSTEM_BROWSE_CALLBACK lpCallBack, RT_B bRecursively, void* lpContext)
{
#ifdef RT_DEFINE_WINDOWS
  WIN32_FIND_DATA findFileData;       /* Information sur les fichiers trouv�s */
  HANDLE hFind;                       /* Handle sur la recherche              */
  RT_N nWritten;
#else
  DIR* lpDir;
  struct dirent *lpDirEntry;
#endif
  RT_CHAR lpChild[RT_FILE_SYSTEM_MAX_FILE_PATH]; /* Chemin de travail                    */
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  hFind = INVALID_HANDLE_VALUE;

  if (!RtComputeLongPath(lpPath, lpChild, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto handle_error;
  /* TODO: Buffer overflow!? */
  lstrcat(lpChild, _R("\\*"));

  hFind = FindFirstFile(lpChild, &findFileData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    if (GetLastError() == ERROR_FILE_NOT_FOUND)
    {
      bResult = RT_TRUE;
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
        if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_DIRECTORY, lpContext)) goto handle_error;

        if (bRecursively)
        {
          if (!RtBrowsePath(lpChild, lpCallBack, bRecursively, lpContext)) goto handle_error;
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

  bResult = RT_TRUE;
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
  bResult = RT_FALSE;
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
          if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_DIRECTORY, lpContext)) goto handle_error;

          if (bRecursively)
          {
            if (!RtBrowsePath(lpChild, lpCallBack, bRecursively, lpContext)) goto handle_error;
          }
        }
        else
        {
          if (!lpCallBack(lpChild, RT_FILE_SYSTEM_TYPE_FILE, lpContext)) goto handle_error;
        }
      }
    }
  }

  bResult = RT_TRUE;
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
  bResult = RT_FALSE;
  goto free_resources;
#endif
}

RT_B RT_API RtCheckPath(RT_CHAR* lpPath, RT_N nType)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  DWORD nAttributes;
  RT_N nWritten;
#else
  struct stat stats;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  if (!RtComputeLongPath(lpPath, lpLongPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten))
  {
    bResult = RT_FALSE;
  }
  else
  {
    nAttributes = GetFileAttributes(lpLongPath);
    if (nAttributes == INVALID_FILE_ATTRIBUTES)
    {
      bResult = RT_FALSE;
    }
    else if (nAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      /* The path is a directory. */
      if (nType & RT_FILE_SYSTEM_TYPE_DIRECTORY)
      {
        bResult = RT_TRUE;
      }
      else
      {
        /* We did not expect a directory. */
        RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
        bResult = RT_FALSE;
      }
    }
    else
    {
      /* The path is a file. */
      if (nType & RT_FILE_SYSTEM_TYPE_FILE)
      {
        bResult = RT_TRUE;
      }
      else
      {
        /* We did not expect a file. */
        RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
        bResult = RT_FALSE;
      }
    }
  }
#else /* NOT RT_DEFINE_WINDOWS */

  if (stat(lpPath, &stats))
  {
    bResult = RT_FALSE;
  }
  else
  {
    if (S_ISDIR(stats.st_mode))
    {
      /* The path is a directory. */
      if (nType & RT_FILE_SYSTEM_TYPE_DIRECTORY)
      {
        bResult = RT_TRUE;
      }
      else
      {
        /* We did not expect a directory. */
        RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
        bResult = RT_FALSE;
      }
    }
    else
    {
      /* The path is a file. */
      if (nType & RT_FILE_SYSTEM_TYPE_FILE)
      {
        bResult = RT_TRUE;
      }
      else
      {
        /* We did not expect a file. */
        RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
        bResult = RT_FALSE;
      }
    }
  }

#endif
  return bResult;
}

RT_B RT_API RtExtractFileName(RT_CHAR* lpPath, RT_N nPathSize, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N *lpWritten)
{
  RT_N nLastSeparator;
  RT_N nWritten;
  RT_B bResult;

  nWritten = 0;

  nLastSeparator = RtGetLastSeparator(lpPath, nPathSize);
  if (!RtCopyString(&lpPath[nLastSeparator + 1], lpBuffer, nBufferSize, &nWritten)) goto handle_error;
  RtRemoveTrailingSeparators(lpBuffer, nWritten, &nWritten);

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  *lpWritten += nWritten;
  return bResult;
}


RT_B RT_CALL RtMoveOrRenameFile(RT_CHAR* lpCurrentFilePath, RT_CHAR* lpNewFilePath, RT_B bRename)
{
#ifdef RT_DEFINE_WINDOWS
  RT_CHAR lpLongCurrentFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpLongNewFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nWritten;
#else /* NOT RT_DEFINE_WINDOWS */

#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!RtComputeLongPath(lpCurrentFilePath, lpLongCurrentFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto handle_error;
  if (!RtComputeLongPath(lpNewFilePath, lpLongNewFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto handle_error;
  if (!MoveFileEx(lpCurrentFilePath, lpNewFilePath, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH)) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */

  /* rename is faster under Linux but does not work accross files systems/mount points. */
  if (rename(lpCurrentFilePath, lpNewFilePath))
  {
    if (bRename)
    {
      /* Rename has failed while it is a renaming (So same file system/mount point). */
      goto handle_error;
    }
    /* The rename call has failed, try another approach to move: copy then delete. */
    if (!RtCopyFile(lpCurrentFilePath, lpNewFilePath)) goto handle_error;
    if (!RtDeleteFile(lpCurrentFilePath)) goto handle_error;
  }
#endif

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_B RT_API RtMoveFile(RT_CHAR* lpCurrentFilePath, RT_CHAR* lpNewFilePath)
{
  return RtMoveOrRenameFile(lpCurrentFilePath, lpNewFilePath, RT_FALSE);
}

RT_B RT_API RtAppendSeparator(RT_CHAR* lpPath, RT_N nPathSize, RT_N nBufferSize, RT_N *lpWritten)
{
  RT_CHAR nLastChar;
  RT_B bResult;

  nLastChar = lpPath[nPathSize - 1];
  if (RT_FILE_SYSTEM_IS_SEPARATOR(nLastChar))
  {
    /* Separator already exists. */
    bResult = RT_TRUE;
  }
  else
  {
    /* Append separator. */
    bResult = RtCopyChar(RT_FILE_SYSTEM_SEPARATOR, &lpPath[nPathSize], nBufferSize - nPathSize, lpWritten);
  }
  return bResult;
}

RT_B RT_API RtBuildPath(RT_CHAR* lpParentPath, RT_N nParentPathSize, RT_CHAR* lpChildName, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_N nWritten;
  RT_B bResult;

  nWritten = nParentPathSize;
  if (!RtAppendSeparator(lpParentPath, nParentPathSize, nBufferSize, &nWritten)) goto handle_error;
  if (!RtCopyString(lpChildName, &lpParentPath[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;

  *lpWritten += nWritten - nParentPathSize;
  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  if (nBufferSize > 0)
  {
    lpParentPath[0] = 0;
  }
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_B RT_API RtBuildNewPath(RT_CHAR* lpParentPath, RT_N nParentPathSize, RT_CHAR* lpChildName, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
{
  RT_N nWritten;
  RT_B bResult;

  nWritten = 0;
  if (!RtCopyStringWithSize(lpParentPath, nParentPathSize, lpBuffer, nBufferSize, &nWritten)) goto handle_error;
  if (!RtAppendSeparator(lpBuffer, nParentPathSize, nBufferSize, &nWritten)) goto handle_error;
  if (!RtCopyString(lpChildName, &lpBuffer[nWritten], nBufferSize - nWritten, &nWritten)) goto handle_error;

  lpWritten += nWritten;
  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  if (nBufferSize > 0)
  {
    lpBuffer[0] = 0;
  }
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_B RT_API RtRenameFile(RT_CHAR* lpCurrentFilePath, RT_CHAR* lpNewFileName)
{
  RT_CHAR lpNewFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nWritten;
  RT_B bResult;

  nWritten = 0;
  if (!RtExtractParentPath(lpCurrentFilePath, RtGetStringSize(lpCurrentFilePath), lpNewFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto handle_error;
  if (!RtAppendSeparator(lpNewFilePath, nWritten, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto handle_error;
  if (!RtCopyString(lpNewFileName, &lpNewFilePath[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto handle_error;
  if (!RtMoveOrRenameFile(lpCurrentFilePath, lpNewFilePath, RT_TRUE)) goto handle_error;

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_B RT_API RtDeleteFile(RT_CHAR* lpFilePath)
{
#ifndef RT_DEFINE_WINDOWS
  struct stat stats;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!DeleteFile(lpFilePath))
  {
    if (GetLastError() != ERROR_FILE_NOT_FOUND)
    {
      goto handle_error;
    }
  }
#else /* NOT RT_DEFINE_WINDOWS */

  /* Check if the file exists. */
  if (stat(lpFilePath, &stats))
  {
    if (errno == ENOENT)
    {
      /* Given file doesn't seem to exist, do nothing. */
      goto file_does_not_exist;
    }
  }

  /* The stat call might have failed but we still try to delete file. */
  /* The remove function returns 0 in case of success. */
  if (remove(lpFilePath)) goto handle_error;

file_does_not_exist:
#endif

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  return bResult;
}