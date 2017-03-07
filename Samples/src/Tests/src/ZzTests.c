#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

#include "ZzTools.h"

typedef struct _MyContext
{
  RT_CHAR* _lpSource;
  RT_UN _unSourceSize;
  RT_CHAR* _lpDestination;
  RT_B _bErrorHandled;
}
MyContext;

RT_B RT_CALL ZzCompare(void* lpItem1, void* lpItem2, void* lpContext, RT_N* lpComparisonResult)
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

void RT_CALL ZzWriteToFile(RT_CHAR* lpPath, RT_CHAR* lpString, RT_UN unEncoding, RT_HEAP** lpHeap)
{
  RT_CHAR8* lpFileContent;
  RT_UN unFileContentSize;

  unFileContentSize = RtEncodeWithHeap(lpString, RT_TYPE_MAX_UN, unEncoding, &lpFileContent, lpHeap);
  if (unFileContentSize == RT_TYPE_MAX_UN)
  {
    RtWriteLastErrorMessage(_R("Encoding failed: "));
  }

  if (!RtWriteToSmallFile(lpFileContent, unFileContentSize, lpPath, RT_SMALL_FILE_MODE_TRUNCATE))
  {
    RtWriteLastErrorMessage(_R("Data writing failed: "));
  }

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
  {
    RtWriteLastErrorMessage(_R("Failed free file content: "));
  }
}

void RT_CALL DiplayFileContent(RT_CHAR* lpPath, RT_UN unEncoding, RT_UN unBomSize, RT_HEAP** lpHeap)
{
  RT_CHAR* lpFileContentAsString;
  RT_CHAR8* lpFileContent;
  RT_CHAR8* lpData;
  RT_UN unDataSize;
  RT_UN unFileSize;
  RT_CHAR lpBuffer[500];
  RT_UN unWritten;

  unFileSize = RtReadFromSmallFile(lpPath, &lpFileContent, lpHeap);
  RtConvertIntegerToString(unFileSize, lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(4, lpPath, _R(" file size: "), lpBuffer, _R("\n"));

  lpData = &lpFileContent[unBomSize];
  unDataSize = unFileSize - unBomSize;

  if (RtDecodeWithHeap(lpData, unDataSize, unEncoding, &lpFileContentAsString, lpHeap) == RT_TYPE_MAX_UN)
  {
    RtWriteLastErrorMessage(_R("Decoding failed: "));
  }
  RtWriteStringsToConsole(3, _R("OOOO"), lpFileContentAsString, _R("OOOO\n"));

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContentAsString))
  {
    RtWriteLastErrorMessage(_R("Failed free string file content: "));
  }

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
  {
    RtWriteLastErrorMessage(_R("Failed free file content: "));
  }
}

RT_B RT_CALL ZzTestTypes();
RT_B RT_CALL ZzTestMemory();
RT_B RT_CALL ZzTestRandom();
RT_B RT_CALL ZzTestHeap(RT_HEAP** lpHeap);
RT_B RT_CALL ZzTestFileSystem();
RT_B RT_CALL ZzTestChar();
RT_B RT_CALL ZzTestEncoding();
RT_B RT_CALL ZzTestProperties(RT_HEAP** lpHeap);
RT_B RT_CALL ZzTestAtomic();
RT_B RT_CALL ZzTestThread();
RT_B RT_CALL ZzTestInitialization();
RT_B RT_CALL ZzTestEvent();
RT_B RT_CALL ZzTestSocket();
RT_B RT_CALL ZzTestList(RT_HEAP** lpHeap);
RT_B RT_CALL ZzTestEnvironmentVariable();
RT_B RT_CALL ZzTestProcess();

RT_B RT_CALL ZzManualTests();

/**
 * All automated tests.
 */
RT_B RT_CALL ZzTests()
{
  RT_B bResult;

  RT_B bHeapCreated;

  RtRuntimeHeap zzRuntimeHeap;
  void* lpArea;
  RT_UN32* lpArray;
  MY_ITEM* lpLinkedList;
  RT_UN32 unToSearch;
  RT_CHAR lpBuffer[500];
  RT_CHAR* lpTestingString;
  RT_UN unIndex;
  RT_UN32 unI;
  RT_UN unWritten;

  bHeapCreated = RT_FALSE;

  if (!RtRuntimeHeapCreate(&zzRuntimeHeap))
  {
    RtWriteLastErrorMessage(_R("Runtime heap creation failed: "));
    goto handle_error;
  }
  bHeapCreated = RT_TRUE;

  if (!ZzAdjustDirectory())
  {
    RtWriteLastErrorMessage(_R("Directory adjustement failed: "));
    goto handle_error;
  }

  if (!ZzTestTypes()) goto tests_failed;
  if (!ZzTestMemory()) goto tests_failed;
  if (!ZzTestRandom()) goto tests_failed;
  if (!ZzTestHeap(&zzRuntimeHeap.lpHeap)) goto tests_failed;
  if (!ZzTestFileSystem()) goto tests_failed;
  if (!ZzTestChar()) goto tests_failed;
  if (!ZzTestEncoding()) goto tests_failed;
  if (!ZzTestProperties(&zzRuntimeHeap.lpHeap)) goto tests_failed;
  if (!ZzTestAtomic()) goto tests_failed;
  if (!ZzTestThread()) goto tests_failed;
  if (!ZzTestInitialization()) goto tests_failed;
  if (!ZzTestEvent()) goto tests_failed;
  if (!ZzTestSocket()) goto tests_failed;
  if (!ZzTestList(&zzRuntimeHeap.lpHeap)) goto tests_failed;
  if (!ZzTestEnvironmentVariable()) goto tests_failed;
  if (!ZzTestProcess()) goto tests_failed;

  RtWriteStringToConsole(_R("Tests successful!!\n\n"));
  goto end_of_tests;

tests_failed:
  RtWriteStringToConsole(_R("Tests failed!!\n\n"));
  goto handle_error;
end_of_tests:

  RtConvertIntegerToString(RtCheckPath(_R("data/file.txt"), RT_FILE_SYSTEM_TYPE_DIRECTORY), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("dir file.txt: "), lpBuffer, _R("\n"));

  RtConvertIntegerToString(RtCheckPath(_R("data/file.txt"), RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("file file.txt: "), lpBuffer, _R("\n"));

  RtConvertIntegerToString(RtCheckPath(_R("data/directory"), RT_FILE_SYSTEM_TYPE_DIRECTORY), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("dir directory: "), lpBuffer, _R("\n"));

  RtConvertIntegerToString(RtCheckPath(_R("data/directory"), RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("file directory: "), lpBuffer, _R("\n"));

  RtConvertIntegerToString(RtCheckPath(_R("data/directory"), RT_FILE_SYSTEM_TYPE_DIRECTORY | RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("both directory: "), lpBuffer, _R("\n"));

  RtConvertIntegerToString(RtCheckPath(_R("data/file.txt"), RT_FILE_SYSTEM_TYPE_DIRECTORY | RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("both file.txt: "), lpBuffer, _R("\n"));

  RtConvertIntegerToString(RtCheckPath(_R("fake"), RT_FILE_SYSTEM_TYPE_DIRECTORY), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("dir fake: "), lpBuffer, _R("\n"));

  RtConvertIntegerToString(RtCheckPath(_R("fake"), RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("file fake: "), lpBuffer, _R("\n"));

  RtConvertIntegerToString(RtCheckPath(_R("fake"), RT_FILE_SYSTEM_TYPE_DIRECTORY | RT_FILE_SYSTEM_TYPE_FILE), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(3, _R("both fake: "), lpBuffer, _R("\n"));

  RtConvertUIntegerToString(RtGetFileSize(_R("data/file.txt")), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(2, lpBuffer, _R("\n"));

  RtConvertIntegerToString(getPageSize(), lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(2, lpBuffer, _R("\n"));

  RtCopyString(_R("FooBarPad"), lpBuffer, 500, &unWritten);

  RtLeftPadString(lpBuffer, _R('0'), 11, lpBuffer, 500, &unWritten);
  RtWriteStringsToConsole(2, lpBuffer, _R("\n"));

  unWritten = 0;
  RtConcatStrings(lpBuffer, 500, &unWritten, 4, _R("foo"), _R("bar"), _R("team"), _R("\n"));
  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  DiplayFileContent(_R("data/file.txt"), 0, 0, &zzRuntimeHeap.lpHeap);
  DiplayFileContent(_R("data/latin1.txt"), RT_ENCODING_ISO_8859_15, 0, &zzRuntimeHeap.lpHeap);
  DiplayFileContent(_R("data/utf8.txt"), RT_ENCODING_UTF_8, 3, &zzRuntimeHeap.lpHeap);

  lpTestingString = _R("OOOOéOOOO");

  RtWriteStringsToConsole(2, lpTestingString, _R("\n"));

  ZzWriteToFile(_R("temp/latin1_test.txt"), lpTestingString, RT_ENCODING_ISO_8859_15, &zzRuntimeHeap.lpHeap);
  ZzWriteToFile(_R("temp/utf8_test.txt"), lpTestingString, RT_ENCODING_UTF_8, &zzRuntimeHeap.lpHeap);

  if (!zzRuntimeHeap.lpHeap->lpAlloc(&zzRuntimeHeap, &lpArea, 256, _R("Some bytes")))
  {
    RtWriteLastErrorMessage(_R("Failed to allocate some bytes: "));
    goto handle_error;
  }

  RtCreateLinkedList((void**)&lpLinkedList, &zzRuntimeHeap.lpHeap, 20, sizeof(MY_ITEM));
  for (unI = 0; unI < 5; unI++)
  {
    RtNewLinkedListItemIndex((void**)&lpLinkedList, &unIndex);
    unWritten = 0;
    RtConvertIntegerToString(unIndex, &lpBuffer[unWritten], 200 - unWritten, &unWritten);
    RtCopyString(_R("\n"),     &lpBuffer[unWritten], 200 - unWritten, &unWritten);
    RtWriteStringToConsoleWithSize(lpBuffer, unWritten);
  }
  RtFreeArray((void**)&lpLinkedList);

  RtCreateSortableArray((void**)&lpArray, &zzRuntimeHeap.lpHeap, 200, sizeof(RT_UN32), &ZzCompare, RT_NULL);
  for (unI = 0; unI < 200; unI++)
  {
    lpArray[unI] = unI + 1000;
  }
  lpArray[12] = 5;
  lpArray[28] = 6;
  lpArray[34] = 7;
  lpArray[50] = 8;

  RtSetArraySize((void**)&lpArray, 100);
  RtSortSortableArray(lpArray);

  for (unI = 0; unI < 100; unI++)
  {
    unWritten = 0;
    RtConvertIntegerToString(lpArray[unI], &lpBuffer[unWritten], 200 - unWritten, &unWritten);
    RtCopyString(_R("\n"), &lpBuffer[unWritten], 200 - unWritten, &unWritten);
    RtWriteStringToConsoleWithSize(lpBuffer, unWritten);
  }

  unToSearch = 1025;
  RtSearchSortableArrayItemIndex(lpArray, &unToSearch, &unIndex);
  unWritten = 0;
  RtCopyString(_R("Item found at index: "), &lpBuffer[unWritten], 200 - unWritten, &unWritten);
  RtConvertIntegerToString(unIndex,                &lpBuffer[unWritten], 200 - unWritten, &unWritten);
  RtCopyString(_R("\n"),                    &lpBuffer[unWritten], 200 - unWritten, &unWritten);
  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  RtFreeArray((void**)&lpArray);

  if (!zzRuntimeHeap.lpHeap->lpFree(&zzRuntimeHeap, &lpArea))
  {
    RtWriteLastErrorMessage(_R("Failed to free some bytes: "));
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  if (bHeapCreated)
  {
    bHeapCreated = RT_FALSE;
    if (!zzRuntimeHeap.lpHeap->lpClose(&zzRuntimeHeap) && bResult)
    {
      RtWriteLastErrorMessage(_R("Failed to close runtime heap: "));
      goto handle_error;
    }
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDisplayHelp(RT_B bResult)
{
  RtWriteStringToConsole(_R("Test rtlib.\n\n")
                         _R("Tests [-m|--manual|-h|--help]\n")
                         _R("Tests [-a|--args ARGS]\n\n")
                         _R("  --manual          Perform manual tests.\n")
                         _R("  --args            Display arguments.\n")
                         );
  return bResult;
}

RT_B RT_CALL ZzDisplayArgs(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_UN unI;
  RT_B bResult;

  for (unI = 0; unI < nArgC; unI++)
  {
    if (!RtWriteStringsToConsole(2, lpArgV[unI], _R("\n"))) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_B bResult;

  if (nArgC == 1)
  {
    bResult = ZzTests();
  }
  else if (nArgC == 2)
  {
    if (!RtCompareStrings(lpArgV[1], _R("--manual")) ||
        !RtCompareStrings(lpArgV[1], _R("-m")))
    {
      bResult = ZzManualTests();
    }
    else if (!RtCompareStrings(lpArgV[1], _R("--help")) ||
             !RtCompareStrings(lpArgV[1], _R("-h")) ||
             !RtCompareStrings(lpArgV[1], _R("/?")))
    {
      bResult = ZzDisplayHelp(RT_SUCCESS);
    }
    else
    {
      bResult = ZzDisplayHelp(RT_FAILURE);
    }
  }
  else if (!RtCompareStrings(lpArgV[1], _R("--args")) ||
           !RtCompareStrings(lpArgV[1], _R("-a")))
  {
    bResult = ZzDisplayArgs(nArgC, lpArgV);
  }
  else
  {
    bResult = ZzDisplayHelp(RT_FAILURE);
  }
  return bResult;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_UN16 unResult;
  if (ZzMain(nArgC, lpArgV))
  {
    unResult = 0;
  }
  else
  {
    unResult = 1;
  }
  return unResult;
}
