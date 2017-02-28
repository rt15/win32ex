#include <RtWin32Ex.h>

RT_UN16 RT_CALL ZzTestDoGetLastSeparator(RT_CHAR* lpPath, RT_UN unExpected)
{
  RT_CHAR lpBuffer[512];
  RT_UN unWritten;
  RT_UN unIndex;
  RT_UN16 unResult;

  unResult = 1;

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
    goto the_end;
  }
  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL ZzTestGetLastSeparator()
{
  RT_UN16 unResult;

  unResult = 1;

  if (ZzTestDoGetLastSeparator(_R("Foo"), RT_TYPE_MAX_UN)) goto the_end;
  if (ZzTestDoGetLastSeparator(_R("Foo/"), RT_TYPE_MAX_UN)) goto the_end;
  if (ZzTestDoGetLastSeparator(_R("/Foo/"), 0)) goto the_end;
  if (ZzTestDoGetLastSeparator(_R("bar/Foo/"), 3)) goto the_end;
  if (ZzTestDoGetLastSeparator(_R("/bar/Foo/"), 4)) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}


RT_UN16 RT_CALL ZzTestDoRemoveTrailingSeparators(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_UN16 unResult;

  unResult = 1;

  unWritten = 0;
  RtCopyString(lpPath, lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten);

  RtRemoveTrailingSeparators(lpBuffer, unWritten, &unWritten);
  if (RtCompareStrings(lpBuffer, lpExpected)) goto the_end;
  if (unWritten != RtGetStringSize(lpExpected)) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL ZzTestRemoveTrailingSeparators()
{
  RT_UN16 unResult;

  unResult = 1;

#ifdef RT_DEFINE_WINDOWS
  if (ZzTestDoRemoveTrailingSeparators(_R("/"), _R("."))) goto the_end;
  if (ZzTestDoRemoveTrailingSeparators(_R("//"), _R("."))) goto the_end;
#else /* NOT RT_DEFINE_WINDOWS */
  if (ZzTestDoRemoveTrailingSeparators(_R("/"), _R("/"))) goto the_end;
  if (ZzTestDoRemoveTrailingSeparators(_R("//"), _R("/"))) goto the_end;
#endif
  if (ZzTestDoRemoveTrailingSeparators(_R("Foo"), _R("Foo"))) goto the_end;
  if (ZzTestDoRemoveTrailingSeparators(_R("Foo/"), _R("Foo"))) goto the_end;
  if (ZzTestDoRemoveTrailingSeparators(_R("Bar/Foo/"), _R("Bar/Foo"))) goto the_end;
  if (ZzTestDoRemoveTrailingSeparators(_R("/Bar/Foo//"), _R("/Bar/Foo"))) goto the_end;
  if (ZzTestDoRemoveTrailingSeparators(_R(""), _R(""))) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL ZzTestDoExtractFileName(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_UN16 unResult;

  unResult = 1;

  unWritten = 0;
  if (!RtCopyString(_R("Testing RtExtractFileName on \""), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(lpPath,                                &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(_R("\", expecting \""),                &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(lpExpected,                            &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(_R("\", found \""),                    &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtExtractFileName(lpPath, RtGetStringSize(lpPath),  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(_R("\"\n"),                            &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  unWritten = 0;
  if (!RtExtractFileName(lpPath, RtGetStringSize(lpPath), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (RtCompareStrings(lpBuffer, lpExpected)) goto the_end;

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL ZzTestExtractFileName()
{
  RT_UN16 unResult;

  unResult = 1;

  if (ZzTestDoExtractFileName(_R("bar.txt"), _R("bar.txt"))) goto the_end;
  if (ZzTestDoExtractFileName(_R("foo/bar.txt"), _R("bar.txt"))) goto the_end;
  if (ZzTestDoExtractFileName(_R("./bar.txt"), _R("bar.txt"))) goto the_end;
  if (ZzTestDoExtractFileName(_R("/foo/bar.txt"), _R("bar.txt"))) goto the_end;
  if (ZzTestDoExtractFileName(_R("/foo/data"), _R("data"))) goto the_end;
  if (ZzTestDoExtractFileName(_R("/foo/data/"), _R("data"))) goto the_end;
  if (ZzTestDoExtractFileName(_R("../data"), _R("data"))) goto the_end;
  if (ZzTestDoExtractFileName(_R("/bar.txt"), _R("bar.txt"))) goto the_end;

#ifdef RT_DEFINE_WINDOWS
  if (ZzTestDoExtractFileName(_R("/"), _R("."))) goto the_end;
#else /* NOT RT_DEFINE_WINDOWS */
  if (ZzTestDoExtractFileName(_R("/"), _R("/"))) goto the_end;
#endif

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL ZzTestDoExtractParentPath(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_UN16 unResult;

  unResult = 1;

  unWritten = 0;
  if (!RtCopyString(_R("Testing RtExtractParentPath on \""), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(lpPath,                                  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(_R("\", expecting \""),                  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(lpExpected,                              &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(_R("\", found \""),                      &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtExtractParentPath(lpPath, RtGetStringSize(lpPath),  &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (!RtCopyString(_R("\"\n"),                              &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  unWritten = 0;
  if (!RtExtractParentPath(lpPath, RtGetStringSize(lpPath), &lpBuffer[unWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto the_end;
  if (RtCompareStrings(lpBuffer, lpExpected)) goto the_end;

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL ZzTestExtractParentPath()
{
  RT_UN16 unResult;

  unResult = 1;

  if (ZzTestDoExtractParentPath(_R("foo.txt"), _R("."))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("../data.txt"), _R(".."))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("./data.txt"), _R("."))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("data/foo.txt"), _R("data"))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("data//foo.txt"), _R("data"))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("/data/foo.txt"), _R("/data"))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("/data/files/foo.txt"), _R("/data/files"))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("/data/files/foo.txt"), _R("/data/files"))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("/foo/../data.txt"), _R("/foo/.."))) goto the_end;

#ifdef RT_DEFINE_WINDOWS
  if (ZzTestDoExtractParentPath(_R("/"), _R("."))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("/data"), _R("."))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("//data"), _R("."))) goto the_end;
#else /* NOT RT_DEFINE_WINDOWS */
  if (ZzTestDoExtractParentPath(_R("/"), _R("/"))) goto the_end;
  if (ZzTestDoExtractParentPath(_R("/data"), _R("/"))) goto the_end;
#endif

  unResult = 0;

the_end:
  return unResult;
}

RT_B RT_CALL ZzTestCreateFile(RT_CHAR* lpFilePath)
{
  RT_B bFileCreated;
  RT_FILE rtFile;
  RT_B bResult;

  bFileCreated = RT_FALSE;
  if (!RtCreateFile(&rtFile, lpFilePath, RT_FILE_MODE_TRUNCATE)) goto handle_error;
  bFileCreated = RT_TRUE;
  if (!RtWriteToFile(&rtFile, "Hello, world!", 13)) goto handle_error;

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  if (bFileCreated)
  {
    if (!RtFreeFile(&rtFile) && bResult)
    {
      bFileCreated = RT_FALSE;
      goto handle_error;
    }
    bFileCreated = RT_FALSE;
  }
  return bResult;
}

RT_UN16 RT_CALL ZzTestMiscWithTemp(RT_CHAR* lpTempDirectory, RT_UN unTempDirectoryPathSize)
{
  RT_CHAR lpFilePath1[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpFilePath2[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_UN16 unResult;

  unResult = 1;

  if (!RtBuildNewPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest1.txt"), lpFilePath1, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto the_end;
  if (!RtBuildNewPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest2.txt"), lpFilePath2, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto the_end;

  if (!ZzTestCreateFile(lpFilePath1)) goto the_end;

  if (!RtDeleteFile(lpFilePath2)) goto the_end;
  if (!RtRenameFile(lpFilePath1, _R("ttest2.txt"))) goto the_end;

  if (!RtCopyFile(lpFilePath2, lpFilePath1)) goto the_end;

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL ZzTestMisc()
{
  RT_CHAR lpTempDirectory[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unTempDirectoryPathSize;
  RT_UN16 unResult;

  unResult = 1;

  unTempDirectoryPathSize = 0;
  if (!RtGetTempDirectory(lpTempDirectory, RT_FILE_SYSTEM_MAX_FILE_PATH, &unTempDirectoryPathSize)) goto the_end;
  if (!RtBuildPath(lpTempDirectory, unTempDirectoryPathSize, _R("ttest"), RT_FILE_SYSTEM_MAX_FILE_PATH, &unTempDirectoryPathSize)) goto the_end;

  if (!RtCheckPath(lpTempDirectory, RT_FILE_SYSTEM_TYPE_DIRECTORY))
  {
    if (!RtCreateDirectory(lpTempDirectory)) goto the_end;
  }

  unResult = ZzTestMiscWithTemp(lpTempDirectory, unTempDirectoryPathSize);

the_end:
  return unResult;
}

RT_UN16 RT_CALL ZzTestFileSystem()
{
  RT_UN16 unResult;

  unResult = 1;

  if (ZzTestGetLastSeparator()) goto the_end;
  if (ZzTestRemoveTrailingSeparators()) goto the_end;
  if (ZzTestExtractFileName()) goto the_end;
  if (ZzTestExtractParentPath()) goto the_end;
  if (ZzTestMisc()) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}
