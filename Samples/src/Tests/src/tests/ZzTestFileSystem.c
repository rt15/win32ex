#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestDoGetLastSeparator(RT_CHAR* lpPath, RT_UN unExpected)
{
  RT_UN unIndex;
  RT_B bResult;

  unIndex = RtFileSystem_GetLastSeparator(lpPath, RtChar_GetCStringSize(lpPath));
  if (unIndex != unExpected) goto handle_error;

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
  RT_UN unInputSize;
  RT_UN unOutputSize;
  RT_B bResult;

  RtChar_CopyString(lpPath, lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &unInputSize);

  RtFileSystem_RemoveTrailingSeparators(lpBuffer, unInputSize, &unOutputSize);
  if (RtChar1337_CompareStrings(lpBuffer, lpExpected)) goto handle_error;
  if (unOutputSize != RtChar_GetCStringSize(lpExpected)) goto handle_error;

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
  RT_UN unPathSize;
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  unPathSize = RtChar_GetCStringSize(lpPath);

  unWritten = 0;
  if (!RtChar_CopyString(_R("Testing RtFileSystem_GetFileName on \""), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(lpPath,                                       &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\", expecting \""),                       &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(lpExpected,                                   &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\", found \""),                           &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtFileSystem_GetFileName(lpPath, unPathSize,                    &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\"\n"),                                   &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);

  if (!RtFileSystem_GetFileName(lpPath, unPathSize, lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (unOutputSize != RtChar_GetCStringSize(lpBuffer)) goto handle_error;
  if (RtChar1337_CompareStrings(lpBuffer, lpExpected)) goto handle_error;

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

RT_B RT_CALL ZzTestDoGetParentPath(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  unWritten = 0;
  if (!RtChar_CopyString(_R("Testing RtFileSystem_GetParentPath on \""), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(lpPath,                                         &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\", expecting \""),                         &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(lpExpected,                                     &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\", found \""),                             &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtFileSystem_GetParentPath(lpPath, RtChar_GetCStringSize(lpPath),  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R("\"\n"),                                     &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);

  if (!RtFileSystem_GetParentPath(lpPath, RtChar_GetCStringSize(lpPath), lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (unOutputSize != RtChar_GetCStringSize(lpBuffer)) goto handle_error;
  if (RtChar1337_CompareStrings(lpBuffer, lpExpected)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestGetParentPath()
{
  RT_B bResult;

  if (!ZzTestDoGetParentPath(_R("foo.txt"), _R("."))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("../data.txt"), _R(".."))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("./data.txt"), _R("."))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("data/foo.txt"), _R("data"))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("data//foo.txt"), _R("data"))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("/data/foo.txt"), _R("/data"))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("/data/files/foo.txt"), _R("/data/files"))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("/data/files/foo.txt"), _R("/data/files"))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("/foo/../data.txt"), _R("/foo/.."))) goto handle_error;

#ifdef RT_DEFINE_WINDOWS
  if (!ZzTestDoGetParentPath(_R("/"), _R("."))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("/data"), _R("."))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("//data"), _R("."))) goto handle_error;
#else /* NOT RT_DEFINE_WINDOWS */
  if (!ZzTestDoGetParentPath(_R("/"), _R("/"))) goto handle_error;
  if (!ZzTestDoGetParentPath(_R("/data"), _R("/"))) goto handle_error;
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
  if (!RtFileSystem_CreateEmptyFile(lpFilePath2, RT_TRUE)) goto handle_error;
  if (!RtFileSystem_GetFileSize(lpFilePath2, &unFileZize)) goto handle_error;
  if (unFileZize) goto handle_error;
  if (!RtFileSystem_DeleteFile(lpFilePath2)) goto handle_error;

  /* Non existing, new. */
  if (!RtFileSystem_CreateEmptyFile(lpFilePath2, RT_FALSE)) goto handle_error;
  if (!RtFileSystem_GetFileSize(lpFilePath2, &unFileZize)) goto handle_error;
  if (unFileZize) goto handle_error;
  if (!RtFileSystem_DeleteFile(lpFilePath2)) goto handle_error;

  /* Existing, new. */
  if (RtFileSystem_CreateEmptyFile(lpFilePath1, RT_FALSE)) goto handle_error;
  if (!RtFileSystem_GetFileSize(lpFilePath1, &unFileZize)) goto handle_error;
  if (!unFileZize) goto handle_error;

  /* Existing, truncate. */
  if (!RtFileSystem_CreateEmptyFile(lpFilePath1, RT_TRUE)) goto handle_error;
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
  RT_UN unOutputSize;
  RT_B bResult;

  if (!RtFileSystem_BuildPath(lpTempDirectory, unTempDirectoryPathSize, _R("empty_dir"), lpDirectoryPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (unOutputSize != RtChar_GetCStringSize(lpDirectoryPath)) goto handle_error;

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
  RT_UN unOutputSize;
  RT_B bResult;

  if (!RtFileSystem_BuildPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest1.txt"), lpFilePath1, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_BuildPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest2.txt"), lpFilePath2, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;

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
  RT_ARRAY zzTempDirectory;


  RT_CHAR lpTempDirectory[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unOutputSize;
  RT_B bResult;

  RtArray_Create(&zzTempDirectory, lpTempDirectory, sizeof(RT_CHAR), RT_FILE_SYSTEM_MAX_FILE_PATH);

  if (!RtFileSystem_GetTempDirectory(&zzTempDirectory)) goto handle_error;
  unOutputSize = zzTempDirectory.unSize;
  if (!RtChar_Append(&zzTempDirectory, 0)) goto handle_error;
  if (!RtFileSystem_BuildPath(lpTempDirectory, unOutputSize, _R("ttest"), lpTempDirectory, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (unOutputSize != RtChar_GetCStringSize(lpTempDirectory)) goto handle_error;

  if (!RtFileSystem_CheckDirectory(lpTempDirectory))
  {
    if (!RtFileSystem_CreateDirectory(lpTempDirectory)) goto handle_error;
  }

  bResult = ZzTestMiscWithTemp(lpTempDirectory, unOutputSize);

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
  if (!ZzTestGetParentPath()) goto handle_error;
  if (!ZzTestMisc()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
