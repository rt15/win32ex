#include "ZzTests.h"

RT_B RT_CALL ZzTestDoGetLastSeparator(RT_CHAR* lpPath, RT_UN unExpected)
{
  RT_CHAR lpBuffer[512];
  RT_UN unWritten;
  RT_UN unIndex;
  RT_B bResult;

  unWritten = 0;
  RtCopyString(_R("Testing RtGetLastSeparator with \""), lpBuffer, 512, &unWritten);
  RtCopyString(lpPath, &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtCopyString(_R("\" expecting "), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtConvertUIntegerToString(unExpected, &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtCopyString(_R(".\n"), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  unIndex = RtGetLastSeparator(lpPath, RtGetStringSize(lpPath));

  if (unIndex != unExpected)
  {
    unWritten = 0;
    RtCopyString(_R("Wrong result: "), lpBuffer, 512, &unWritten);
    RtConvertUIntegerToString(unIndex, &lpBuffer[unWritten], 512 - unWritten, &unWritten);
    RtCopyString(_R(".\n"), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
    RtWriteStringToConsoleWithSize(lpBuffer, unWritten);
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
  RtCopyString(lpPath, lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten);

  RtRemoveTrailingSeparators(lpBuffer, unWritten, &unWritten);
  if (RtCompareStrings(lpBuffer, lpExpected)) goto handle_error;
  if (unWritten != RtGetStringSize(lpExpected)) goto handle_error;

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
  if (!RtCopyString(_R("Testing RtGetFileName on \""),  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(lpPath,                             &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\", expecting \""),             &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(lpExpected,                         &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\", found \""),                 &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtGetFileName(lpPath, RtGetStringSize(lpPath),   &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\"\n"),                         &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  unWritten = 0;
  if (!RtGetFileName(lpPath, RtGetStringSize(lpPath), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (RtCompareStrings(lpBuffer, lpExpected)) goto handle_error;

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
  if (!RtCopyString(_R("Testing RtGetNewParentPath on \""),  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(lpPath,                                  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\", expecting \""),                  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(lpExpected,                              &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\", found \""),                      &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtGetNewParentPath(lpPath, RtGetStringSize(lpPath),   &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R("\"\n"),                              &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  unWritten = 0;
  if (!RtGetNewParentPath(lpPath, RtGetStringSize(lpPath), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;
  if (RtCompareStrings(lpBuffer, lpExpected)) goto handle_error;

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
  RT_B bResult;

  bFileCreated = RT_FALSE;
  if (!RtCreateFile(&zzFile, lpFilePath, RT_FILE_MODE_TRUNCATE)) goto handle_error;
  bFileCreated = RT_TRUE;
  if (!RtWriteToFile(&zzFile, "Hello, world!", 13)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtFreeFile(&zzFile) && bResult) goto handle_error;
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

  if (!RtCheckPath(lpFilePath1, RT_FILE_SYSTEM_TYPE_FILE)) goto handle_error;
  if (RtCheckPath(lpFilePath2, RT_FILE_SYSTEM_TYPE_FILE | RT_FILE_SYSTEM_TYPE_DIRECTORY)) goto handle_error;

  /* Non existing, truncate. */
  if (!RtCreateEmptyFile(lpFilePath2, RT_TRUE)) goto handle_error;
  if (!RtGetFileSystemFileSize(lpFilePath2, &unFileZize)) goto handle_error;
  if (unFileZize) goto handle_error;
  if (!RtDeleteFile(lpFilePath2)) goto handle_error;

  /* Non existing, new. */
  if (!RtCreateEmptyFile(lpFilePath2, RT_FALSE)) goto handle_error;
  if (!RtGetFileSystemFileSize(lpFilePath2, &unFileZize)) goto handle_error;
  if (unFileZize) goto handle_error;
  if (!RtDeleteFile(lpFilePath2)) goto handle_error;

  /* Existing, new. */
  if (RtCreateEmptyFile(lpFilePath1, RT_FALSE)) goto handle_error;
  if (!RtGetFileSystemFileSize(lpFilePath1, &unFileZize)) goto handle_error;
  if (!unFileZize) goto handle_error;

  /* Existing, truncate. */
  if (!RtCreateEmptyFile(lpFilePath1, RT_TRUE)) goto handle_error;
  if (!RtGetFileSystemFileSize(lpFilePath1, &unFileZize)) goto handle_error;
  if (unFileZize) goto handle_error;
  if (!RtDeleteFile(lpFilePath1)) goto handle_error;

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

  if (!RtBuildNewPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest1.txt"), lpFilePath1, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;
  if (!RtBuildNewPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest2.txt"), lpFilePath2, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  if (!ZzTestCreateFile(lpFilePath1)) goto handle_error;

  if (!RtDeleteFile(lpFilePath2)) goto handle_error;
  if (!RtRenameFile(lpFilePath1, _R("ttest2.txt"))) goto handle_error;

  if (!RtCopyFile(lpFilePath2, lpFilePath1)) goto handle_error;

  if (!RtDeleteFile(lpFilePath2)) goto handle_error;

  if (!ZzTestCreateEmptyFile(lpFilePath1, lpFilePath2)) goto handle_error;

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
  if (!RtGetTempDirectory(lpTempDirectory, RT_FILE_SYSTEM_MAX_FILE_PATH, &unTempDirectoryPathSize)) goto handle_error;
  if (!RtBuildPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest"), RT_FILE_SYSTEM_MAX_FILE_PATH, &unTempDirectoryPathSize)) goto handle_error;

  if (!RtCheckPath(lpTempDirectory, RT_FILE_SYSTEM_TYPE_DIRECTORY))
  {
    if (!RtCreateDirectory(lpTempDirectory)) goto handle_error;
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
