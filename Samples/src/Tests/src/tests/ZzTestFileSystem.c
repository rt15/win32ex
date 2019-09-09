#include "ZzTests.h"

RT_B RT_CALL ZzTestDoGetLastSeparator(RT_CHAR* lpPath, RT_UN unExpected)
{
  RT_CHAR lpBuffer[512];
  RT_UN unWritten;
  RT_UN unIndex;
  RT_B bResult;

  unWritten = 0;
  RtChar_CopyString(_R("Testing RtFileSystem_GetLastSeparator with \""), lpBuffer, 512, &unWritten);
  RtChar_CopyString(lpPath, &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtChar_CopyString(_R("\" expecting "), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtChar_ConvertUIntegerToString(unExpected, &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtChar_CopyString(_R(".\n"), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);

  unIndex = RtFileSystem_GetLastSeparator(lpPath, RtChar_GetStringSize(lpPath));

  if (unIndex != unExpected)
  {
    unWritten = 0;
    RtChar_CopyString(_R("Wrong result: "), lpBuffer, 512, &unWritten);
    RtChar_ConvertUIntegerToString(unIndex, &lpBuffer[unWritten], 512 - unWritten, &unWritten);
    RtChar_CopyString(_R(".\n"), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
    RtConsole_WriteStringWithSize(lpBuffer, unWritten);
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestGetLastSeparator()
{
  RT_B bResult;

  if (!ZzTestDoGetLastSeparator(_R("Foo"), RT_TYPE_MAX_UN)) goto handle_error;
  if (!ZzTestDoGetLastSeparator(_R("Foo/"), RT_TYPE_MAX_UN)) goto handle_error;
  if (!ZzTestDoGetLastSeparator(_R("/Foo/"), 0)) goto handle_error;
  if (!ZzTestDoGetLastSeparator(_R("bar/Foo/"), 3)) goto handle_error;
  if (!ZzTestDoGetLastSeparator(_R("/bar/Foo/"), 4)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestDoRemoveTrailingSeparators(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  RtChar_CopyString(lpPath, lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten);

  RtFileSystem_RemoveTrailingSeparators(lpBuffer, unWritten, &unWritten);
  if (RtChar_CompareStrings(lpBuffer, lpExpected)) goto handle_error;
  if (unWritten != RtChar_GetStringSize(lpExpected)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestRemoveTrailingSeparators()
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  if (!ZzTestDoRemoveTrailingSeparators(_R("/"), _R("."))) goto handle_error;
  if (!ZzTestDoRemoveTrailingSeparators(_R("//"), _R("."))) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */
  if (!ZzTestDoRemoveTrailingSeparators(_R("/"), _R("/"))) goto handle_error;
  if (!ZzTestDoRemoveTrailingSeparators(_R("//"), _R("/"))) goto handle_error;
#endif
  if (!ZzTestDoRemoveTrailingSeparators(_R("Foo"), _R("Foo"))) goto handle_error;
  if (!ZzTestDoRemoveTrailingSeparators(_R("Foo/"), _R("Foo"))) goto handle_error;
  if (!ZzTestDoRemoveTrailingSeparators(_R("Bar/Foo/"), _R("Bar/Foo"))) goto handle_error;
  if (!ZzTestDoRemoveTrailingSeparators(_R("/Bar/Foo//"), _R("/Bar/Foo"))) goto handle_error;
  if (!ZzTestDoRemoveTrailingSeparators(_R(""), _R(""))) goto handle_error;
  bResult = 0;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestDoGetFileName(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (!RtChar_CopyString(_R("Testing RtFileSystem_GetFileName on \""),  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(lpPath,                             &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(_R("\", expecting \""),             &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(lpExpected,                         &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(_R("\", found \""),                 &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtFileSystem_GetFileName(lpPath, RtChar_GetStringSize(lpPath),   &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(_R("\"\n"),                         &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);

  unWritten = 0;
  if (!RtFileSystem_GetFileName(lpPath, RtChar_GetStringSize(lpPath), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpBuffer, lpExpected)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestGetFileName()
{
  RT_B bResult;

  if (!ZzTestDoGetFileName(_R("bar.txt"), _R("bar.txt"))) goto handle_error;
  if (!ZzTestDoGetFileName(_R("foo/bar.txt"), _R("bar.txt"))) goto handle_error;
  if (!ZzTestDoGetFileName(_R("./bar.txt"), _R("bar.txt"))) goto handle_error;
  if (!ZzTestDoGetFileName(_R("/foo/bar.txt"), _R("bar.txt"))) goto handle_error;
  if (!ZzTestDoGetFileName(_R("/foo/data"), _R("data"))) goto handle_error;
  if (!ZzTestDoGetFileName(_R("/foo/data/"), _R("data"))) goto handle_error;
  if (!ZzTestDoGetFileName(_R("../data"), _R("data"))) goto handle_error;
  if (!ZzTestDoGetFileName(_R("/bar.txt"), _R("bar.txt"))) goto handle_error;

#ifdef RT_DEFINE_WINDOWS
  if (!ZzTestDoGetFileName(_R("/"), _R("."))) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */
  if (!ZzTestDoGetFileName(_R("/"), _R("/"))) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestDoGetNewParentPath(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (!RtChar_CopyString(_R("Testing RtFileSystem_GetNewParentPath on \""),  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(lpPath,                                  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(_R("\", expecting \""),                  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(lpExpected,                              &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(_R("\", found \""),                      &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtFileSystem_GetNewParentPath(lpPath, RtChar_GetStringSize(lpPath),   &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyString(_R("\"\n"),                              &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);

  unWritten = 0;
  if (!RtFileSystem_GetNewParentPath(lpPath, RtChar_GetStringSize(lpPath), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpBuffer, lpExpected)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestGetNewParentPath()
{
  RT_B bResult;

  if (!ZzTestDoGetNewParentPath(_R("foo.txt"), _R("."))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("../data.txt"), _R(".."))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("./data.txt"), _R("."))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("data/foo.txt"), _R("data"))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("data//foo.txt"), _R("data"))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("/data/foo.txt"), _R("/data"))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("/data/files/foo.txt"), _R("/data/files"))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("/data/files/foo.txt"), _R("/data/files"))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("/foo/../data.txt"), _R("/foo/.."))) goto handle_error;

#ifdef RT_DEFINE_WINDOWS
  if (!ZzTestDoGetNewParentPath(_R("/"), _R("."))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("/data"), _R("."))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("//data"), _R("."))) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */
  if (!ZzTestDoGetNewParentPath(_R("/"), _R("/"))) goto handle_error;
  if (!ZzTestDoGetNewParentPath(_R("/data"), _R("/"))) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCreateFile(RT_CHAR* lpFilePath)
{
  RT_B bFileCreated;
  RT_FILE zzFile;
  RT_OUTPUT_STREAM* lpOutputStream;
  RT_B bResult;

  bFileCreated = RT_FALSE;
  if (!RtFile_Create(&zzFile, lpFilePath, RT_FILE_MODE_TRUNCATE)) goto handle_error;
  bFileCreated = RT_TRUE;

  lpOutputStream = RtIoDevice_GetOutputStream(&zzFile.rtIoDevice);
  if (!lpOutputStream->lpWrite(lpOutputStream, "Hello, world!", 13)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtIoDevice_Free(&zzFile.rtIoDevice) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

/**
 * <tt>lpFilePath1</tt> should point on an existing non-empty file.<br>
 * <tt>lpFilePath2</tt> should point on a non-existing file.
 */
RT_B RT_CALL ZzTestCreateEmptyFile(RT_CHAR* lpFilePath1, RT_CHAR* lpFilePath2)
{
  RT_UN64 unFileZize;
  RT_B bResult;

  if (!RtFileSystem_CheckFile(lpFilePath1)) goto handle_error;
  if (RtFileSystem_CheckFileOrDirectory(lpFilePath2)) goto handle_error;

  /* Non existing, truncate. */
  if (!RtFileUtils_CreateEmpty(lpFilePath2, RT_TRUE)) goto handle_error;
  if (!RtFileSystem_GetFileSize(lpFilePath2, &unFileZize)) goto handle_error;
  if (unFileZize) goto handle_error;
  if (!RtFileSystem_DeleteFile(lpFilePath2)) goto handle_error;

  /* Non existing, new. */
  if (!RtFileUtils_CreateEmpty(lpFilePath2, RT_FALSE)) goto handle_error;
  if (!RtFileSystem_GetFileSize(lpFilePath2, &unFileZize)) goto handle_error;
  if (unFileZize) goto handle_error;
  if (!RtFileSystem_DeleteFile(lpFilePath2)) goto handle_error;

  /* Existing, new. */
  if (RtFileUtils_CreateEmpty(lpFilePath1, RT_FALSE)) goto handle_error;
  if (!RtFileSystem_GetFileSize(lpFilePath1, &unFileZize)) goto handle_error;
  if (!unFileZize) goto handle_error;

  /* Existing, truncate. */
  if (!RtFileUtils_CreateEmpty(lpFilePath1, RT_TRUE)) goto handle_error;
  if (!RtFileSystem_GetFileSize(lpFilePath1, &unFileZize)) goto handle_error;
  if (unFileZize) goto handle_error;
  if (!RtFileSystem_DeleteFile(lpFilePath1)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestDirectories(RT_CHAR* lpTempDirectory, RT_UN unTempDirectoryPathSize)
{
  RT_CHAR lpDirectoryPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_B bResult;

  if (!RtFileSystem_BuildNewPath(lpTempDirectory, unTempDirectoryPathSize, _R("empty_dir"), lpDirectoryPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  if (!RtFileSystem_CreateDirectories(lpDirectoryPath)) goto handle_error;
  if (!RtFileSystem_CreateDirectories(lpDirectoryPath)) goto handle_error;

  if (RtFileSystem_DeleteFileIfExists(lpDirectoryPath)) goto handle_error;
  if (RtFileSystem_DeleteFile(lpDirectoryPath)) goto handle_error;

  if (!RtFileSystem_DeleteDirectory(lpDirectoryPath)) goto handle_error;
  if (RtFileSystem_DeleteDirectory(lpDirectoryPath)) goto handle_error;
  if (!RtFileSystem_DeleteDirectoryIfExists(lpDirectoryPath)) goto handle_error;
  if (!RtFileSystem_CreateDirectory(lpDirectoryPath)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestMiscWithTemp(RT_CHAR* lpTempDirectory, RT_UN unTempDirectoryPathSize)
{
  RT_CHAR lpFilePath1[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpFilePath2[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_B bResult;

  if (!RtFileSystem_BuildNewPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest1.txt"), lpFilePath1, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  if (!RtFileSystem_BuildNewPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest2.txt"), lpFilePath2, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  /* Truncate the file if it exists. */
  if (!ZzTestCreateFile(lpFilePath1)) goto handle_error;

  /* Ensure that ttest2.txt is not there. */
  if (!RtFileSystem_DeleteFileIfExists(lpFilePath2)) goto handle_error;

  /* RtFileSystem_DeleteFileIfExists should be ok even if the file does not exist. */
  if (!RtFileSystem_DeleteFileIfExists(lpFilePath2)) goto handle_error;

  if (RtFileSystem_DeleteDirectoryIfExists(lpFilePath1)) goto handle_error;
  if (RtFileSystem_DeleteDirectory(lpFilePath1)) goto handle_error;

  if (!RtFileSystem_CheckFile(lpFilePath1)) goto handle_error;
  if (RtFileSystem_CheckFile(lpFilePath2)) goto handle_error;
  if (RtFileSystem_CheckFile(lpTempDirectory)) goto handle_error;

  if (RtFileSystem_CheckDirectory(lpFilePath1)) goto handle_error;
  if (RtFileSystem_CheckDirectory(lpFilePath2)) goto handle_error;
  if (!RtFileSystem_CheckDirectory(lpTempDirectory)) goto handle_error;

  if (!RtFileSystem_CheckFileOrDirectory(lpFilePath1)) goto handle_error;
  if (RtFileSystem_CheckFileOrDirectory(lpFilePath2)) goto handle_error;
  if (!RtFileSystem_CheckFileOrDirectory(lpTempDirectory)) goto handle_error;

  if (!RtFileSystem_RenameFile(lpFilePath1, _R("ttest2.txt"))) goto handle_error;

  if (!RtFileSystem_CopyFile(lpFilePath2, lpFilePath1)) goto handle_error;

  if (!RtFileSystem_DeleteFile(lpFilePath2)) goto handle_error;

  if (!ZzTestCreateEmptyFile(lpFilePath1, lpFilePath2)) goto handle_error;

  if (!ZzTestDirectories(lpTempDirectory, unTempDirectoryPathSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestMisc()
{
  RT_CHAR lpTempDirectory[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unTempDirectoryPathSize;
  RT_B bResult;

  unTempDirectoryPathSize = 0;
  if (!RtFileSystem_GetTempDirectory(lpTempDirectory, RT_FILE_SYSTEM_MAX_FILE_PATH, &unTempDirectoryPathSize)) goto handle_error;
  if (!RtFileSystem_BuildPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest"), RT_FILE_SYSTEM_MAX_FILE_PATH, &unTempDirectoryPathSize)) goto handle_error;

  if (!RtFileSystem_CheckDirectory(lpTempDirectory))
  {
    if (!RtFileSystem_CreateDirectory(lpTempDirectory)) goto handle_error;
  }

  bResult = ZzTestMiscWithTemp(lpTempDirectory, unTempDirectoryPathSize);

free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestFileSystem()
{
  RT_B bResult;

  if (!ZzTestGetLastSeparator()) goto handle_error;
  if (!ZzTestRemoveTrailingSeparators()) goto handle_error;
  if (!ZzTestGetFileName()) goto handle_error;
  if (!ZzTestGetNewParentPath()) goto handle_error;
  if (!ZzTestMisc()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
