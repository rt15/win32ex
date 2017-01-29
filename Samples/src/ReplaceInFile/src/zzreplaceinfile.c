#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>

RT_UN16 ZzWriteLastErrorMessage(RT_CHAR* lpLabel)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;

  unWritten = 0;
  RtCopyString(lpLabel, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &unWritten);
  RtGetLastErrorMessage(&lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);
  RtCopyStringWithSize(_R("\n"), 1, &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);

  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  return 1;
}

RT_UN16 ZzDisplayHelp(RT_UN32 unResult)
{
  RtWriteStringToConsole(_R("Replace all occurrences of a String by another.\nUsage:\nReplaceInFile searched replacement file\n"));
  return unResult;
}

RT_B ZzPerformWithHeap(RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpFilePath, RT_HEAP** lpHeap)
{
  RT_CHAR8* lpFileContent;
  RT_CHAR* lpFileContentAsString;
  RT_N nFileContentAsStringSize;
  RT_CHAR* lpNewFileContentAsString;
  RT_CHAR8* lpNewFileContent;
  RT_N nNewFileContentAsStringSize;
  RT_N nDelta;

  RT_UN unWritten;
  RT_UN unFileSize;
  RT_UN unNewFileSize;
  RT_N nOcurrencesCount;
  RT_B bResult;

  lpFileContent = RT_NULL;
  lpFileContentAsString = RT_NULL;
  lpNewFileContentAsString = RT_NULL;
  lpNewFileContent = RT_NULL;

  /* Check file path. */
  if (!RtCheckPath(lpFilePath, RT_FILE_SYSTEM_TYPE_FILE))
  {
    ZzWriteLastErrorMessage(_R("Issue with input file: "));
    goto handle_error;
  }

  /* Read input file content. */
  unFileSize = RtReadFromSmallFile(lpFilePath, &lpFileContent, lpHeap);
  if (unFileSize == RT_TYPE_MAX_UN)
  {
    ZzWriteLastErrorMessage(_R("Failed to read input file: "));
    goto handle_error;
  }

  /* Decode input file content. */
  nFileContentAsStringSize = RtDecodeWithHeap(lpFileContent, unFileSize, 0, &lpFileContentAsString, lpHeap);
  if (nFileContentAsStringSize == -1)
  {
    ZzWriteLastErrorMessage(_R("Failed to decode input file: "));
    goto handle_error;
  }

  nOcurrencesCount = RtCountStringOccurrences(lpFileContentAsString, lpSearched);
  if (nOcurrencesCount > 0)
  {
    nDelta = (RtGetStringSize(lpReplacement) - RtGetStringSize(lpSearched)) * nOcurrencesCount;
    nNewFileContentAsStringSize = nFileContentAsStringSize + nDelta;
    if (!(*lpHeap)->lpAlloc(lpHeap, (void**)&lpNewFileContentAsString, (nNewFileContentAsStringSize + 1) * sizeof(RT_CHAR), _R("New file content as string.")))
    {
      ZzWriteLastErrorMessage(_R("Failed to allocate result buffer: "));
      goto handle_error;
    }
    unWritten = 0;
    if (!RtReplaceString(lpFileContentAsString, lpSearched, lpReplacement, lpNewFileContentAsString, nNewFileContentAsStringSize + 1, &unWritten))
    {
      ZzWriteLastErrorMessage(_R("Replacement failed: "));
      goto handle_error;
    }

    unNewFileSize = RtEncodeWithHeap(lpNewFileContentAsString, unWritten, 0, &lpNewFileContent, lpHeap);
    if (unNewFileSize == -1)
    {
      ZzWriteLastErrorMessage(_R("Failed to encode output file: "));
      goto handle_error;
    }

    RtWriteToSmallFile(lpNewFileContent, unNewFileSize, lpFilePath, RT_SMALL_FILE_MODE_TRUNCATE);
  }

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  if (lpNewFileContent)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpNewFileContent))
    {
      ZzWriteLastErrorMessage(_R("Failed free new file content: "));
      goto handle_error;
    }
  }
  if (lpNewFileContentAsString)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpNewFileContentAsString))
    {
      ZzWriteLastErrorMessage(_R("Failed free new file content as string: "));
      goto handle_error;
    }
  }
  if (lpFileContentAsString)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContentAsString))
    {
      ZzWriteLastErrorMessage(_R("Failed free file content as string: "));
      goto handle_error;
    }
  }
  if (lpFileContent)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
    {
      ZzWriteLastErrorMessage(_R("Failed free file content: "));
      goto handle_error;
    }
  }
  return bResult;
}

RT_B ZzPerform(RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpFilePath)
{
  RtRuntimeHeap runtimeHeap;
  RT_B bCloseRuntimeHeap;
  RT_B bResult;

  bCloseRuntimeHeap = RT_FALSE;

  if (!RtRuntimeHeapCreate(&runtimeHeap))
  {
    ZzWriteLastErrorMessage(_R("Runtime heap creation failed: "));
    goto handle_error;
  }
  bCloseRuntimeHeap = RT_TRUE;

  bResult = ZzPerformWithHeap(lpSearched, lpReplacement, lpFilePath, &runtimeHeap.lpHeap);

  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  if (bCloseRuntimeHeap)
  {
    if (!runtimeHeap.lpHeap->lpClose(&runtimeHeap))
    {
      ZzWriteLastErrorMessage(_R("Failed to close runtime heap: "));
      bCloseRuntimeHeap = RT_FALSE;
      goto handle_error;
    }
    bCloseRuntimeHeap = RT_FALSE;
  }
  return bResult;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_UN32 unResult;

  if (nArgC == 4)
  {
    unResult = !ZzPerform(lpArgV[1], lpArgV[2], lpArgV[3]);
  }
  else if (nArgC == 2)
  {
    if ((!RtCompareStrings(_R("/?"), lpArgV[1])) ||
        (!RtCompareStrings(_R("-h"), lpArgV[1])) ||
        (!RtCompareStrings(_R("--help"), lpArgV[1])))
    {
      unResult = 0;
    }
    else
    {
      unResult = 1;
    }
    unResult = ZzDisplayHelp(unResult);
  }
  else
  {
    unResult = ZzDisplayHelp(1);
  }

  return unResult;
}
