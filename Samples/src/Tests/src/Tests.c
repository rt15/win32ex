#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

typedef struct _MyContext
{
  RT_CHAR* _lpSource;
  RT_N _nSourceSize;
  RT_CHAR* _lpDestination;
  RT_B _bErrorHandled;
}
MyContext;

RT_UN16 DisplayHelp(RT_UN16 unResult)
{
  RtWriteStringToConsole(_R("Copy a source without svn files.\nUsage:\ncopy_source source destination\n"));
  return unResult;
}

RT_UN16 RT_CALL WriteLastErrorMessage(RT_CHAR* lpLabel)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_N nWritten;

  nWritten = 0;
  RtCopyString(lpLabel, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &nWritten);
  RtGetLastErrorMessage(&lpBuffer[nWritten], RT_CHAR_BIG_STRING_SIZE - nWritten, &nWritten);
  RtCopyStringWithSize(_R("\n"), 1, &lpBuffer[nWritten], RT_CHAR_BIG_STRING_SIZE - nWritten, &nWritten);

  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  return 1;
}

RT_B RT_CALL BrowsingCallback(RT_CHAR* lpPath , RT_N nType, void* lpContext)
{
  MyContext* lpMyContext;
  RT_CHAR lpMessage[RT_CHAR_THIRD_BIG_STRING_SIZE + 200];
  RT_CHAR lpDestination[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_N nWritten;
  RT_B bResult;

  lpMyContext = (MyContext*)lpContext;

  nWritten = 0;
  RtCopyString(lpMyContext->_lpDestination,        &lpDestination[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - nWritten, &nWritten);
  RtCopyString(&lpPath[lpMyContext->_nSourceSize], &lpDestination[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - nWritten, &nWritten);

  if (RtSearchString(lpPath, _R(".svn")) == -1)
  {
    if (nType == RT_FILE_SYSTEM_TYPE_DIRECTORY)
    {
      if (!RtCheckPath(lpDestination, RT_FILE_SYSTEM_TYPE_DIRECTORY))
      {
        if (!RtCreateDirectory(lpDestination))
        {
          nWritten = 0;
          RtCopyString(_R("Failed to create directory "), &lpMessage[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE + 200 - nWritten, &nWritten);
          RtCopyString(lpDestination,                     &lpMessage[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE + 200 - nWritten, &nWritten);
          RtCopyString(_R(": "),                          &lpMessage[nWritten], RT_CHAR_THIRD_BIG_STRING_SIZE + 200 - nWritten, &nWritten);
          WriteLastErrorMessage(lpMessage);
          lpMyContext->_bErrorHandled = RT_TRUE;
          bResult = RT_FALSE;
          goto the_end;
        }
      }
    }
    else
    {
      if (!RtCopyFile(lpPath, lpDestination))
      {
        RtWriteStringToConsole(_R("Failed to copy file:\n"));
        RtWriteStringToConsole(lpPath);
        RtWriteStringToConsole(_R("\nto:\n"));
        RtWriteStringToConsole(lpDestination);
        RtWriteStringToConsole(_R("\n"));
        WriteLastErrorMessage(_R("Error message: "));
        lpMyContext->_bErrorHandled = RT_TRUE;
        bResult = RT_FALSE;
        goto the_end;
      }
    }
  }

  bResult = RT_TRUE;
the_end:
  return bResult;
}

RT_UN16 CheckDirectory(RT_CHAR* lpLabel, RT_CHAR* lpPath)
{
  RT_CHAR lpMessage[RT_FILE_SYSTEM_MAX_FILE_PATH + 200];
  RT_N nWritten;
  RT_UN16 unResult;

  nWritten = 0;
  RtCopyString(lpLabel,            &lpMessage[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - nWritten, &nWritten);
  RtCopyString(_R(" directory: "), &lpMessage[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - nWritten, &nWritten);
  RtCopyString(lpPath,             &lpMessage[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - nWritten, &nWritten);
  RtCopyString(_R("\n"),           &lpMessage[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpMessage, nWritten);

  if (!RtCheckPath(lpPath, RT_FILE_SYSTEM_TYPE_DIRECTORY))
  {
    nWritten = 0;
    RtCopyString(_R("Issue with "),  &lpMessage[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - nWritten, &nWritten);
    RtCopyString(lpLabel,            &lpMessage[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - nWritten, &nWritten);
    RtCopyString(_R(" directory: "), &lpMessage[nWritten], RT_FILE_SYSTEM_MAX_FILE_PATH + 200 - nWritten, &nWritten);
    unResult = WriteLastErrorMessage(lpMessage);
  }
  else
  {
    unResult = 0;
  }

  return unResult;
}

RT_UN16 CopySource(RT_CHAR* lpSource, RT_CHAR* lpDestination)
{
  MyContext myContext;
  RT_UN16 unResult;

  unResult = CheckDirectory(_R("Source"), lpSource);
  if (unResult)
    goto the_end;
  unResult = CheckDirectory(_R("Destination"), lpDestination);
  if (unResult)
    goto the_end;

  myContext._lpSource = lpSource;
  myContext._nSourceSize = RtGetStringSize(lpSource);
  myContext._lpDestination = lpDestination;
  myContext._bErrorHandled = RT_FALSE;

  if (!RtBrowsePath(lpSource, &BrowsingCallback, RT_TRUE, &myContext))
  {
    if (!myContext._bErrorHandled)
    {
      WriteLastErrorMessage(_R("Failed to browse source files: "));
    }
    unResult = 1;
    goto the_end;
  }

  unResult = 0;
the_end:
  return unResult;
}

RT_B RT_CALL Compare(void* lpItem1, void* lpItem2, void* lpContext, RT_N* lpComparisonResult)
{
  RT_UN32 unItem1;
  RT_UN32 unItem2;

  unItem1 = *(RT_UN32*)lpItem1;
  unItem2 = *(RT_UN32*)lpItem2;

  *lpComparisonResult = unItem1 - unItem2;
  return RT_TRUE;
}

typedef struct _MY_ITEM
{
  RT_LINKED_LIST_ITEM_HEADER header;
}
MY_ITEM;

void RT_CALL WriteToFile(RT_CHAR* lpPath, RT_CHAR* lpString, RT_N nEncoding, RT_HEAP** lpHeap)
{
  RT_CHAR8* lpFileContent;
  RT_N nFileContentSize;

  nFileContentSize = RtEncodeWithHeap(lpString, -1, nEncoding, &lpFileContent, lpHeap);
  if (nFileContentSize == -1)
  {
    WriteLastErrorMessage(_R("Encoding failed: "));
  }

  if (!RtWriteToSmallFile(lpFileContent, nFileContentSize, lpPath, RT_SMALL_FILE_MODE_TRUNCATE))
  {
    WriteLastErrorMessage(_R("Data writing failed: "));
  }

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
  {
    WriteLastErrorMessage(_R("Failed free file content: "));
  }
}

void RT_CALL DiplayFileContent(RT_CHAR* lpPath, RT_N nEncoding, RT_N nBomSize, RT_HEAP** lpHeap)
{
  RT_CHAR* lpFileContentAsString;
  RT_CHAR8* lpFileContent;
  RT_CHAR8* lpData;
  RT_N nDataSize;
  RT_N nFileSize;
  RT_CHAR lpBuffer[500];
  RT_N nWritten;

  nFileSize = RtReadFromSmallFile(lpPath, &lpFileContent, lpHeap);
  RtConvertNumberToString(nFileSize, lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(4, lpPath, _R(" file size: "), lpBuffer, _R("\n"));

  lpData = &lpFileContent[nBomSize];
  nDataSize = nFileSize - nBomSize;

  if (RtDecodeWithHeap(lpData, nDataSize, nEncoding, &lpFileContentAsString, lpHeap) == -1)
  {
    WriteLastErrorMessage(_R("Decoding failed: "));
  }
  RtWriteStringsToConsole(3, _R("OOOO"), lpFileContentAsString, _R("OOOO\n"));

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContentAsString))
  {
    WriteLastErrorMessage(_R("Failed free string file content: "));
  }

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
  {
    WriteLastErrorMessage(_R("Failed free file content: "));
  }
}

RT_UN16 RT_CALL TtTestTypes();
RT_UN16 RT_CALL TtTestMemory();
RT_B RT_CALL ZzTestHeap(RT_HEAP** lpHeap);
RT_UN16 RT_CALL TtTestFileSystem();
RT_UN16 RT_CALL TtTestChar();
RT_UN16 RT_CALL TtTestEncoding();
RT_UN16 RT_CALL TtTestProperties(RT_HEAP** lpHeap);
RT_UN16 RT_CALL TtTestAtomic();
RT_UN16 RT_CALL TtTestThread();
RT_UN16 RT_CALL TtTestInitialization();
RT_B RT_CALL ZzTestEvent();
RT_UN16 RT_CALL TtTestSocket();
RT_UN16 RT_CALL TtTestList(RT_HEAP** lpHeap);

/**
 * The tests must be executed in the right directory.
 */
RT_B RT_CALL ZzAdjustDirectory()
{
  RT_CHAR lpPath[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_N nWritten;
  RT_N nPathSize;
  RT_B bResult;

  nWritten = 0;
  if (!RtGetExecutableFilePath(lpPath, RT_CHAR_HALF_BIG_STRING_SIZE, &nWritten)) goto handle_error;

  nPathSize = nWritten;
  nWritten = 0;
  if (!RtExtractParentPath(lpPath, nPathSize, lpPath, RT_CHAR_HALF_BIG_STRING_SIZE, &nWritten)) goto handle_error;

  nPathSize = nWritten;
  nWritten = 0;
  if (!RtExtractParentPath(lpPath, nPathSize, lpPath, RT_CHAR_HALF_BIG_STRING_SIZE, &nWritten)) goto handle_error;

  if (!RtBuildPath(lpPath, nWritten, _R("src"), RT_CHAR_HALF_BIG_STRING_SIZE - nWritten, &nWritten)) goto handle_error;

  if (!RtBuildPath(lpPath, nWritten, _R("Tests"), RT_CHAR_HALF_BIG_STRING_SIZE - nWritten, &nWritten)) goto handle_error;

  if (!RtSetCurrentDirectory(lpPath)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_UN16 unResult;

  RtRuntimeHeap runtimeHeap;
  void* lpArea;
  RT_UN32* lpArray;
  MY_ITEM* lpLinkedList;
  RT_UN32 unToSearch;
  RT_CHAR lpBuffer[500];
  RT_CHAR* lpTestingString;
  RT_N nIndex;
  RT_N nI;
  RT_N nWritten;

  if (!RtRuntimeHeapCreate(&runtimeHeap))
  {
    unResult = WriteLastErrorMessage(_R("Runtime heap creation failed: "));
    goto the_end;
  }

  if (!ZzAdjustDirectory())
  {
    unResult = WriteLastErrorMessage(_R("Directory adjustement failed: "));
    goto the_end;
  }

  if (TtTestTypes()) goto tests_failed;
  if (TtTestMemory()) goto tests_failed;
  if (!ZzTestHeap(&runtimeHeap.lpHeap)) goto tests_failed;
  if (TtTestFileSystem()) goto tests_failed;
  if (TtTestChar()) goto tests_failed;
  if (TtTestEncoding()) goto tests_failed;
  if (TtTestProperties(&runtimeHeap.lpHeap)) goto tests_failed;
  if (TtTestAtomic()) goto tests_failed;
  if (TtTestThread()) goto tests_failed;
  if (TtTestInitialization()) goto tests_failed;
  if (!ZzTestEvent()) goto tests_failed;
  if (TtTestSocket()) goto tests_failed;
  if (TtTestList(&runtimeHeap.lpHeap)) goto tests_failed;

  RtWriteStringToConsole(_R("Tests successful!!\n\n"));
  goto end_of_tests;

tests_failed:
  RtWriteStringToConsole(_R("Tests failed!!\n\n"));
  unResult = 1;
  goto close_heap;
end_of_tests:

  RtGetExecutableFilePath(lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(2, lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("data/file.txt"), RT_FILE_SYSTEM_TYPE_DIRECTORY), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("dir file.txt: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("data/file.txt"), RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("file file.txt: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("data/directory"), RT_FILE_SYSTEM_TYPE_DIRECTORY), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("dir directory: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("data/directory"), RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("file directory: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("data/directory"), RT_FILE_SYSTEM_TYPE_DIRECTORY | RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("both directory: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("data/file.txt"), RT_FILE_SYSTEM_TYPE_DIRECTORY | RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("both file.txt: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("fake"), RT_FILE_SYSTEM_TYPE_DIRECTORY), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("dir fake: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("fake"), RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("file fake: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtCheckPath(_R("fake"), RT_FILE_SYSTEM_TYPE_DIRECTORY | RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(3, _R("both fake: "), lpBuffer, _R("\n"));

  RtConvertNumberToString(RtGetFileSize(_R("data/file.txt")), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(2, lpBuffer, _R("\n"));

  RtConvertNumberToString(getPageSize(), lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(2, lpBuffer, _R("\n"));

  RtCopyString(_R("FooBarPad"), lpBuffer, 500, &nWritten);

  RtLeftPadString(lpBuffer, _R('0'), 11, lpBuffer, 500, &nWritten);
  RtWriteStringsToConsole(2, lpBuffer, _R("\n"));

  nWritten = 0;
  RtConcatStrings(lpBuffer, 500, &nWritten, 4, _R("foo"), _R("bar"), _R("team"), _R("\n"));
  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  DiplayFileContent(_R("data/file.txt"), 0, 0, &runtimeHeap.lpHeap);
  DiplayFileContent(_R("data/latin1.txt"), RT_ENCODING_ISO_8859_15, 0, &runtimeHeap.lpHeap);
  DiplayFileContent(_R("data/utf8.txt"), RT_ENCODING_UTF_8, 3, &runtimeHeap.lpHeap);

  lpTestingString = _R("OOOOéOOOO");

  RtWriteStringsToConsole(2, lpTestingString, _R("\n"));

  WriteToFile(_R("temp/latin1_test.txt"), lpTestingString, RT_ENCODING_ISO_8859_15, &runtimeHeap.lpHeap);
  WriteToFile(_R("temp/utf8_test.txt"), lpTestingString, RT_ENCODING_UTF_8, &runtimeHeap.lpHeap);

  if (!runtimeHeap.lpHeap->lpAlloc(&runtimeHeap, &lpArea, 256, _R("Some bytes")))
  {
    unResult = WriteLastErrorMessage(_R("Failed to allocate some bytes: "));
    goto close_heap;
  }

  RtCreateLinkedList((void**)&lpLinkedList, &runtimeHeap.lpHeap, 20, sizeof(MY_ITEM));
  for (nI = 0; nI < 5; nI++)
  {
    RtNewLinkedListItemIndex((void**)&lpLinkedList, &nIndex);
    nWritten = 0;
    RtConvertNumberToString(nIndex, &lpBuffer[nWritten], 200 - nWritten, &nWritten);
    RtCopyString(_R("\n"),     &lpBuffer[nWritten], 200 - nWritten, &nWritten);
    RtWriteStringToConsoleWithSize(lpBuffer, nWritten);
  }
  RtFreeArray((void**)&lpLinkedList);

  RtCreateSortableArray((void**)&lpArray, &runtimeHeap.lpHeap, 200, sizeof(RT_UN32), &Compare, RT_NULL);
  for (nI = 0; nI < 200; nI++)
  {
    lpArray[nI] = nI + 1000;
  }
  lpArray[12] = 5;
  lpArray[28] = 6;
  lpArray[34] = 7;
  lpArray[50] = 8;

  RtSetArraySize((void**)&lpArray, 100);
  RtSortSortableArray(lpArray);

  for (nI = 0; nI < 100; nI++)
  {
    nWritten = 0;
    RtConvertNumberToString(lpArray[nI], &lpBuffer[nWritten], 200 - nWritten, &nWritten);
    RtCopyString(_R("\n"), &lpBuffer[nWritten], 200 - nWritten, &nWritten);
    RtWriteStringToConsoleWithSize(lpBuffer, nWritten);
  }

  unToSearch = 1025;
  RtSearchSortableArrayItemIndex(lpArray, &unToSearch, &nIndex);
  nWritten = 0;
  RtCopyString(_R("Item found at index: "), &lpBuffer[nWritten], 200 - nWritten, &nWritten);
  RtConvertNumberToString(nIndex,                &lpBuffer[nWritten], 200 - nWritten, &nWritten);
  RtCopyString(_R("\n"),                    &lpBuffer[nWritten], 200 - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  RtFreeArray((void**)&lpArray);

  unResult = 0;

  if (!runtimeHeap.lpHeap->lpFree(&runtimeHeap, &lpArea))
  {
    WriteLastErrorMessage(_R("Failed to free some bytes: "));
    unResult = 1;
  }

close_heap:
  if (!runtimeHeap.lpHeap->lpClose(&runtimeHeap))
  {
    WriteLastErrorMessage(_R("Failed to close runtime heap: "));
    unResult = 1;
  }

the_end:
  return unResult;

/*
  if (nArgC == 2)
  {
    if (!RtCompareStrings(lpArgV[1], _R("--help")) ||
        !RtCompareStrings(lpArgV[1], _R("-h")) ||
        !RtCompareStrings(lpArgV[1], _R("/?")))
    {
      unResult = DisplayHelp(0);
    }
    else
    {
      unResult = DisplayHelp(1);
    }
  }
  else if (nArgC != 3)
  {
    unResult = DisplayHelp(1);
  }
  else
  {
    unResult = CopySource(lpArgV[1], lpArgV[2]);
  }
  return unResult;
  */
}
