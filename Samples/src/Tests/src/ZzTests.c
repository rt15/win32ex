#include "ZzTests.h"

#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

#include "ZzManualTests.h"
#include "ZzTestParseArgs.h"
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

  unFileContentSize = RtEncoding_EncodeWithHeap(lpString, RT_TYPE_MAX_UN, unEncoding, &lpFileContent, lpHeap);
  if (unFileContentSize == RT_TYPE_MAX_UN)
  {
    RtErrorMessage_WriteLast(_R("Encoding failed: "));
  }

  if (!RtSmallFile_Write(lpFileContent, unFileContentSize, lpPath, RT_SMALL_FILE_MODE_TRUNCATE))
  {
    RtErrorMessage_WriteLast(_R("Data writing failed: "));
  }

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
  {
    RtErrorMessage_WriteLast(_R("Failed free file content: "));
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

  unFileSize = RtSmallFile_Read(lpPath, &lpFileContent, lpHeap);
  RtChar_ConvertIntegerToString(unFileSize, lpBuffer, 500, &unWritten);
  RtConsole_WriteStringsOrErrors(RT_TRUE, lpPath, _R(" file size: "), lpBuffer, _R("\n"), (RT_CHAR*)RT_NULL);

  lpData = &lpFileContent[unBomSize];
  unDataSize = unFileSize - unBomSize;

  if (RtEncoding_DecodeWithHeap(lpData, unDataSize, unEncoding, &lpFileContentAsString, lpHeap) == RT_TYPE_MAX_UN)
  {
    RtErrorMessage_WriteLast(_R("Decoding failed: "));
  }
  RtConsole_WriteStringsOrErrors(RT_TRUE, _R("OOOO"), lpFileContentAsString, _R("OOOO\n"), (RT_CHAR*)RT_NULL);

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContentAsString))
  {
    RtErrorMessage_WriteLast(_R("Failed free string file content: "));
  }

  if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
  {
    RtErrorMessage_WriteLast(_R("Failed free file content: "));
  }
}

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

  if (!RtRuntimeHeap_Create(&zzRuntimeHeap))
  {
    RtErrorMessage_WriteLast(_R("Runtime heap creation failed: "));
    goto handle_error;
  }
  bHeapCreated = RT_TRUE;

  if (!ZzAdjustDirectory())
  {
    RtErrorMessage_WriteLast(_R("Directory adjustement failed: "));
    goto handle_error;
  }

  if (!ZzTestTypes()) goto tests_failed;
  if (!ZzTestMemory()) goto tests_failed;
  if (!ZzTestRandom()) goto tests_failed;
  if (!ZzTestHeap(&zzRuntimeHeap.lpHeap)) goto tests_failed;
  if (!ZzTestIoDevice()) goto tests_failed;
  if (!ZzTestFile()) goto tests_failed;
  if (!ZzTestFileSystem()) goto tests_failed;
  if (!ZzTestChar()) goto tests_failed;
  if (!ZzTestBase64()) goto tests_failed;
  if (!ZzTestEncoding()) goto tests_failed;
  if (!ZzTestProperties(&zzRuntimeHeap.lpHeap)) goto tests_failed;
  if (!ZzTestAtomic()) goto tests_failed;
  if (!ZzTestThread()) goto tests_failed;
  if (!ZzTestInitialization()) goto tests_failed;
  if (!ZzTestEvent()) goto tests_failed;
  if (!ZzTestUrl()) goto tests_failed;
  if (!ZzTestSocket()) goto tests_failed;
  /* if (!ZzTestNonBlockingSocket()) goto tests_failed; */
  if (!ZzTestList(&zzRuntimeHeap.lpHeap)) goto tests_failed;
  if (!ZzTestEnvVar()) goto tests_failed;
  if (!ZzTestEnvVars()) goto tests_failed;
  if (!ZzTestCommandLineArgs()) goto tests_failed;
  if (!ZzTestProcess(&zzRuntimeHeap.lpHeap)) goto tests_failed;

  RtConsole_WriteString(_R("Tests successful!!\n\n"));
  goto end_of_tests;

tests_failed:
  RtConsole_WriteString(_R("Tests failed!!\n\n"));
  goto handle_error;
end_of_tests:

  RtChar_ConvertIntegerToString(RtVirtualMemory_GetPageSize(), lpBuffer, 500, &unWritten);
  RtConsole_WriteStringsOrErrors(RT_TRUE, lpBuffer, _R("\n"), (RT_CHAR*)RT_NULL);

  RtChar_CopyString(_R("FooBarPad"), lpBuffer, 500, &unWritten);

  RtChar_LeftPadString(lpBuffer, _R('0'), 11, lpBuffer, 500, &unWritten);
  RtConsole_WriteStringsOrErrors(RT_TRUE, lpBuffer, _R("\n"), (RT_CHAR*)RT_NULL);

  unWritten = 0;
  RtChar_ConcatStrings(lpBuffer, 500, &unWritten, _R("foo"), _R("bar"), _R("team"), _R("\n"), (RT_CHAR*)RT_NULL);
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);

  DiplayFileContent(_R("data/file.txt"), 0, 0, &zzRuntimeHeap.lpHeap);
  DiplayFileContent(_R("data/latin1.txt"), RT_ENCODING_ISO_8859_15, 0, &zzRuntimeHeap.lpHeap);
  DiplayFileContent(_R("data/utf8.txt"), RT_ENCODING_UTF_8, 3, &zzRuntimeHeap.lpHeap);

  lpTestingString = _R("OOOOéOOOO");

  RtConsole_WriteStringsOrErrors(RT_TRUE, lpTestingString, _R("\n"), (RT_CHAR*)RT_NULL);

  ZzWriteToFile(_R("data/latin1_test.txt"), lpTestingString, RT_ENCODING_ISO_8859_15, &zzRuntimeHeap.lpHeap);
  ZzWriteToFile(_R("data/utf8_test.txt"), lpTestingString, RT_ENCODING_UTF_8, &zzRuntimeHeap.lpHeap);

  if (!zzRuntimeHeap.lpHeap->lpAlloc(&zzRuntimeHeap, &lpArea, 256, _R("Some bytes")))
  {
    RtErrorMessage_WriteLast(_R("Failed to allocate some bytes: "));
    goto handle_error;
  }

  RtLinkedList_Create((void**)&lpLinkedList, &zzRuntimeHeap.lpHeap, 20, sizeof(MY_ITEM));
  for (unI = 0; unI < 5; unI++)
  {
    RtLinkedList_NewItemIndex((void**)&lpLinkedList, &unIndex);
    unWritten = 0;
    RtChar_ConvertIntegerToString(unIndex, &lpBuffer[unWritten], 200 - unWritten, &unWritten);
    RtChar_CopyString(_R("\n"),     &lpBuffer[unWritten], 200 - unWritten, &unWritten);
    RtConsole_WriteStringWithSize(lpBuffer, unWritten);
  }
  RtArray_Free((void**)&lpLinkedList);

  RtSortableArray_Create((void**)&lpArray, &zzRuntimeHeap.lpHeap, 200, sizeof(RT_UN32), &ZzCompare, RT_NULL);
  for (unI = 0; unI < 200; unI++)
  {
    lpArray[unI] = unI + 1000;
  }
  lpArray[12] = 5;
  lpArray[28] = 6;
  lpArray[34] = 7;
  lpArray[50] = 8;

  RtArray_SetSize((void**)&lpArray, 100);
  RtSortableArray_Sort(lpArray);

  for (unI = 0; unI < 100; unI++)
  {
    unWritten = 0;
    RtChar_ConvertIntegerToString(lpArray[unI], &lpBuffer[unWritten], 200 - unWritten, &unWritten);
    RtChar_CopyString(_R("\n"), &lpBuffer[unWritten], 200 - unWritten, &unWritten);
    RtConsole_WriteStringWithSize(lpBuffer, unWritten);
  }

  unToSearch = 1025;
  RtSortableArray_SearchItemIndex(lpArray, &unToSearch, &unIndex);
  unWritten = 0;
  RtChar_CopyString(_R("Item found at index: "), &lpBuffer[unWritten], 200 - unWritten, &unWritten);
  RtChar_ConvertIntegerToString(unIndex,                &lpBuffer[unWritten], 200 - unWritten, &unWritten);
  RtChar_CopyString(_R("\n"),                    &lpBuffer[unWritten], 200 - unWritten, &unWritten);
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);

  RtArray_Free((void**)&lpArray);

  if (!zzRuntimeHeap.lpHeap->lpFree(&zzRuntimeHeap, &lpArea))
  {
    RtErrorMessage_WriteLast(_R("Failed to free some bytes: "));
    goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  if (bHeapCreated)
  {
    bHeapCreated = RT_FALSE;
    if (!zzRuntimeHeap.lpHeap->lpClose(&zzRuntimeHeap) && bResult)
    {
      RtErrorMessage_WriteLast(_R("Failed to close runtime heap: "));
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
  RtConsole_WriteStringOrError(_R("Test rtlib.\n\n")
                                _R("Tests [-m|--manual|-h|--help|-r|--read-line]\n")
                                _R("Tests [-a|--args [ARGS]]\n")
                                _R("Tests [-p|--parse-args [ARGS]]\n")
                                _R("Tests [-d|--display-env-var [ENV_VAR]]\n")
                                _R("\n")
                                _R("  --read-line        Read and write a line.\n")
                                _R("  --manual           Perform manual tests.\n")
                                _R("  --args             Display arguments.\n")
                                _R("  --parse-args       Parse and display arguments.\n")
                                _R("  --display-env-var  Display all or a single env var.\n")
                                , bResult);
  return bResult;
}

RT_B RT_CALL ZzDisplayArgs(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_N32 nI;
  RT_B bResult;

  for (nI = 0; nI < nArgC; nI++)
  {
    if (!RtConsole_WriteStringsOrErrors(RT_TRUE, lpArgV[nI], _R("\n"), (RT_CHAR*)RT_NULL)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzReadLine()
{
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_B bResult;

  if (!RtConsole_ReadLine(lpBuffer, RT_CHAR_HALF_BIG_STRING_SIZE)) goto handle_error;
  if (!RtConsole_WriteStringsOrErrors(RT_TRUE, _R("\""), lpBuffer, _R("\"\n"), (RT_CHAR*)RT_NULL)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDisplayEnvVars()
{
  RT_ENV_VARS zzEnvVars;
  RT_CHAR** lpEnvVarsArray;
  RT_B bEnvVarsCreated;
  RT_B bResult;

  bEnvVarsCreated = RT_FALSE;

  if (!RtEnvVars_Create(&zzEnvVars)) goto handle_error;
  bEnvVarsCreated = RT_TRUE;

  if (!RtEnvVars_GetArray(&zzEnvVars, &lpEnvVarsArray)) goto handle_error;

  while (*lpEnvVarsArray)
  {
    if (!RtConsole_WriteString(*lpEnvVarsArray)) goto handle_error;
    if (!RtConsole_WriteStringWithSize(_R("\n"), 1)) goto handle_error;
    lpEnvVarsArray++;
  }

  if (!RtConsole_WriteString(_R("\n"))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bEnvVarsCreated)
  {
    bEnvVarsCreated = RT_FALSE;
    if (!RtEnvVars_Free(&zzEnvVars) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDisplayEnvVar(RT_CHAR* lpEnvVar)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (!RtEnvVar_Get(lpEnvVar, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (!RtConsole_WriteStringsOrErrors(RT_TRUE, lpEnvVar, _R("="), lpBuffer, _R("\n"), (RT_CHAR*)RT_NULL)) goto handle_error;

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
  else if (nArgC == 2 && (!RtChar_CompareStrings(lpArgV[1], _R("--manual")) ||
                          !RtChar_CompareStrings(lpArgV[1], _R("-m"))))
  {
    bResult = ZzManualTests();
  }

  else if (!RtChar_CompareStrings(lpArgV[1], _R("--parse-args")) ||
           !RtChar_CompareStrings(lpArgV[1], _R("-p")))
  {
    bResult = ZzTestParseArgs(nArgC - 1, &lpArgV[1]);
  }

  else if (nArgC == 2 && (!RtChar_CompareStrings(lpArgV[1], _R("--read-line")) ||
                          !RtChar_CompareStrings(lpArgV[1], _R("-r"))))
  {
    bResult = ZzReadLine();
  }
  else if (nArgC == 2 && (!RtChar_CompareStrings(lpArgV[1], _R("--help")) ||
                          !RtChar_CompareStrings(lpArgV[1], _R("-h")) ||
                          !RtChar_CompareStrings(lpArgV[1], _R("/?"))))
  {
    bResult = ZzDisplayHelp(RT_SUCCESS);
  }
  else if (nArgC == 2 && (!RtChar_CompareStrings(lpArgV[1], _R("--display-env-var")) ||
                          !RtChar_CompareStrings(lpArgV[1], _R("-d"))))
  {
    bResult = ZzDisplayEnvVars();
  }
  else if (nArgC == 3 && (!RtChar_CompareStrings(lpArgV[1], _R("--display-env-var")) ||
                          !RtChar_CompareStrings(lpArgV[1], _R("-d"))))
  {
    bResult = ZzDisplayEnvVar(lpArgV[2]);
  }
  else if (!RtChar_CompareStrings(lpArgV[1], _R("--args")) ||
           !RtChar_CompareStrings(lpArgV[1], _R("-a")))
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
