#include <RtWin32Ex.h>

RT_UN16 RT_CALL TtDoGetLastSeparator(RT_CHAR* lpPath, RT_N nExpected)
{
  RT_CHAR lpBuffer[512];
  RT_N nWritten;
  RT_N nIndex;
  RT_UN16 unResult;

  unResult = 1;

  nWritten = 0;
  RtCopyString(_R("Testing RtGetLastSeparator with \""), lpBuffer, 512, &nWritten);
  RtCopyString(lpPath, &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtCopyString(_R("\" expecting "), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtConvertNumberToString(nExpected, &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtCopyString(_R(".\n"), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  nIndex = RtGetLastSeparator(lpPath, RtGetStringSize(lpPath));

  if (nIndex != nExpected)
  {
    nWritten = 0;
    RtCopyString(_R("Wrong result: "), lpBuffer, 512, &nWritten);
    RtConvertNumberToString(nIndex, &lpBuffer[nWritten], 512 - nWritten, &nWritten);
    RtCopyString(_R(".\n"), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
    RtWriteStringToConsoleWithSize(lpBuffer, nWritten);
    goto the_end;
  }
  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtGetLastSeparator()
{
  RT_UN16 unResult;

  unResult = 1;

  if (TtDoGetLastSeparator(_R("Foo"), -1)) goto the_end;
  if (TtDoGetLastSeparator(_R("Foo/"), -1)) goto the_end;
  if (TtDoGetLastSeparator(_R("/Foo/"), 0)) goto the_end;
  if (TtDoGetLastSeparator(_R("bar/Foo/"), 3)) goto the_end;
  if (TtDoGetLastSeparator(_R("/bar/Foo/"), 4)) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}


RT_UN16 RT_CALL TtDoRemoveTrailingSeparators(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nWritten;
  RT_UN16 unResult;

  unResult = 1;

  nWritten = 0;
  RtCopyString(lpPath, lpBuffer, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten);

  RtRemoveTrailingSeparators(lpBuffer, nWritten, &nWritten);
  if (RtCompareStrings(lpBuffer, lpExpected)) goto the_end;
  if (nWritten != RtGetStringSize(lpExpected)) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtRemoveTrailingSeparators()
{
  RT_UN16 unResult;

  unResult = 1;

#ifdef RT_DEFINE_WINDOWS
  if (TtDoRemoveTrailingSeparators(_R("/"), _R("."))) goto the_end;
  if (TtDoRemoveTrailingSeparators(_R("//"), _R("."))) goto the_end;
#else /* NOT RT_DEFINE_WINDOWS */
  if (TtDoRemoveTrailingSeparators(_R("/"), _R("/"))) goto the_end;
  if (TtDoRemoveTrailingSeparators(_R("//"), _R("/"))) goto the_end;
#endif
  if (TtDoRemoveTrailingSeparators(_R("Foo"), _R("Foo"))) goto the_end;
  if (TtDoRemoveTrailingSeparators(_R("Foo/"), _R("Foo"))) goto the_end;
  if (TtDoRemoveTrailingSeparators(_R("Bar/Foo/"), _R("Bar/Foo"))) goto the_end;
  if (TtDoRemoveTrailingSeparators(_R("/Bar/Foo//"), _R("/Bar/Foo"))) goto the_end;
  if (TtDoRemoveTrailingSeparators(_R(""), _R(""))) goto the_end;
  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtDoExtractFileName(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nWritten;
  RT_UN16 unResult;

  unResult = 1;

  nWritten = 0;
  if (!RtCopyString(_R("Testing RtExtractFileName on \""), &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(lpPath,                                &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(_R("\", expecting \""),                &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(lpExpected,                            &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(_R("\", found \""),                    &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtExtractFileName(lpPath, RtGetStringSize(lpPath),  &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(_R("\"\n"),                            &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  nWritten = 0;
  if (!RtExtractFileName(lpPath, RtGetStringSize(lpPath), &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (RtCompareStrings(lpBuffer, lpExpected)) goto the_end;

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtExtractFileName()
{
  RT_UN16 unResult;

  unResult = 1;

  if (TtDoExtractFileName(_R("bar.txt"), _R("bar.txt"))) goto the_end;
  if (TtDoExtractFileName(_R("foo/bar.txt"), _R("bar.txt"))) goto the_end;
  if (TtDoExtractFileName(_R("./bar.txt"), _R("bar.txt"))) goto the_end;
  if (TtDoExtractFileName(_R("/foo/bar.txt"), _R("bar.txt"))) goto the_end;
  if (TtDoExtractFileName(_R("/foo/data"), _R("data"))) goto the_end;
  if (TtDoExtractFileName(_R("/foo/data/"), _R("data"))) goto the_end;
  if (TtDoExtractFileName(_R("../data"), _R("data"))) goto the_end;
  if (TtDoExtractFileName(_R("/bar.txt"), _R("bar.txt"))) goto the_end;

#ifdef RT_DEFINE_WINDOWS
  if (TtDoExtractFileName(_R("/"), _R("."))) goto the_end;
#else /* NOT RT_DEFINE_WINDOWS */
  if (TtDoExtractFileName(_R("/"), _R("/"))) goto the_end;
#endif

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtDoExtractParentPath(RT_CHAR* lpPath, RT_CHAR* lpExpected)
{
  RT_CHAR lpBuffer[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nWritten;
  RT_UN16 unResult;

  unResult = 1;

  nWritten = 0;
  if (!RtCopyString(_R("Testing RtExtractParentPath on \""), &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(lpPath,                                  &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(_R("\", expecting \""),                  &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(lpExpected,                              &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(_R("\", found \""),                      &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtExtractParentPath(lpPath, RtGetStringSize(lpPath),  &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (!RtCopyString(_R("\"\n"),                              &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  nWritten = 0;
  if (!RtExtractParentPath(lpPath, RtGetStringSize(lpPath), &lpBuffer[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH - nWritten, &nWritten)) goto the_end;
  if (RtCompareStrings(lpBuffer, lpExpected)) goto the_end;

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtExtractParentPath()
{
  RT_UN16 unResult;

  unResult = 1;

  if (TtDoExtractParentPath(_R("foo.txt"), _R("."))) goto the_end;
  if (TtDoExtractParentPath(_R("../data.txt"), _R(".."))) goto the_end;
  if (TtDoExtractParentPath(_R("./data.txt"), _R("."))) goto the_end;
  if (TtDoExtractParentPath(_R("data/foo.txt"), _R("data"))) goto the_end;
  if (TtDoExtractParentPath(_R("data//foo.txt"), _R("data"))) goto the_end;
  if (TtDoExtractParentPath(_R("/data/foo.txt"), _R("/data"))) goto the_end;
  if (TtDoExtractParentPath(_R("/data/files/foo.txt"), _R("/data/files"))) goto the_end;
  if (TtDoExtractParentPath(_R("/data/files/foo.txt"), _R("/data/files"))) goto the_end;
  if (TtDoExtractParentPath(_R("/foo/../data.txt"), _R("/foo/.."))) goto the_end;

#ifdef RT_DEFINE_WINDOWS
  if (TtDoExtractParentPath(_R("/"), _R("."))) goto the_end;
  if (TtDoExtractParentPath(_R("/data"), _R("."))) goto the_end;
  if (TtDoExtractParentPath(_R("//data"), _R("."))) goto the_end;
#else /* NOT RT_DEFINE_WINDOWS */
  if (TtDoExtractParentPath(_R("/"), _R("/"))) goto the_end;
  if (TtDoExtractParentPath(_R("/data"), _R("/"))) goto the_end;
#endif

  unResult = 0;

the_end:
  return unResult;
}

RT_B RT_CALL TtCreateFile(RT_CHAR* lpFilePath)
{
  RT_B bFileCreated;
  RT_FILE rtFile;
  RT_B bResult;

  bFileCreated = RT_FALSE;
  if (!RtCreateFile(&rtFile, lpFilePath, RT_FILE_MODE_TRUNCATE)) goto handle_error;
  bFileCreated = RT_TRUE;
  if (!RtWriteToFile(&rtFile, "Hello, world!", 13)) goto handle_error;

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
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

RT_UN16 RT_CALL TtMiscWithTemp(RT_CHAR* lpTempDirectory, RT_N nTempDirectoryPathSize)
{
  RT_CHAR lpFilePath1[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpFilePath2[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nWritten;
  RT_UN16 unResult;

  unResult = 1;

  if (!RtBuildNewPath(lpTempDirectory, nTempDirectoryPathSize, _R("ttest1.txt"), lpFilePath1, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto the_end;
  if (!RtBuildNewPath(lpTempDirectory, nTempDirectoryPathSize, _R("ttest2.txt"), lpFilePath2, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten)) goto the_end;

  if (!TtCreateFile(lpFilePath1)) goto the_end;

  if (!RtDeleteFile(lpFilePath2)) goto the_end;
  if (!RtRenameFile(lpFilePath1, _R("ttest2.txt"))) goto the_end;

  if (!RtCopyFile(lpFilePath2, lpFilePath1)) goto the_end;

  unResult = 0;

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtMisc()
{
  RT_CHAR lpTempDirectory[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_N nTempDirectoryPathSize;
  RT_UN16 unResult;

  unResult = 1;

  nTempDirectoryPathSize = 0;
  if (!RtGetTempDirectory(lpTempDirectory, RT_FILE_SYSTEM_MAX_FILE_PATH, &nTempDirectoryPathSize)) goto the_end;
  if (!RtBuildPath(lpTempDirectory, nTempDirectoryPathSize, _R("ttest"), RT_FILE_SYSTEM_MAX_FILE_PATH, &nTempDirectoryPathSize)) goto the_end;

  if (!RtCheckPath(lpTempDirectory, RT_FILE_SYSTEM_TYPE_DIRECTORY))
  {
    if (!RtCreateDirectory(lpTempDirectory)) goto the_end;
  }

  unResult = TtMiscWithTemp(lpTempDirectory, nTempDirectoryPathSize);

the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestFileSystem()
{
  RT_UN16 unResult;

  unResult = 1;

  if (TtGetLastSeparator()) goto the_end;
  if (TtRemoveTrailingSeparators()) goto the_end;
  if (TtExtractFileName()) goto the_end;
  if (TtExtractParentPath()) goto the_end;
  if (TtMisc()) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}
