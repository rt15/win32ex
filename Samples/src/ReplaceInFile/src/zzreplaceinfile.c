#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>


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
  if (!RtFileSystem_CheckPath(lpFilePath, RT_FILE_SYSTEM_TYPE_FILE))
  {
    RtWriteLastErrorMessage(_R("Issue with input file: "));
    goto handle_error;
  }

  /* Read input file content. */
  unFileSize = RtReadFromSmallFile(lpFilePath, &lpFileContent, lpHeap);
  if (unFileSize == RT_TYPE_MAX_UN)
  {
    RtWriteLastErrorMessage(_R("Failed to read input file: "));
    goto handle_error;
  }

  /* Decode input file content. */
  nFileContentAsStringSize = RtEncoding_DecodeWithHeap(lpFileContent, unFileSize, 0, &lpFileContentAsString, lpHeap);
  if (nFileContentAsStringSize == -1)
  {
    RtWriteLastErrorMessage(_R("Failed to decode input file: "));
    goto handle_error;
  }

  nOcurrencesCount = RtChar_CountStringOccurrences(lpFileContentAsString, lpSearched);
  if (nOcurrencesCount > 0)
  {
    nDelta = (RtChar_GetStringSize(lpReplacement) - RtChar_GetStringSize(lpSearched)) * nOcurrencesCount;
    nNewFileContentAsStringSize = nFileContentAsStringSize + nDelta;
    if (!(*lpHeap)->lpAlloc(lpHeap, (void**)&lpNewFileContentAsString, (nNewFileContentAsStringSize + 1) * sizeof(RT_CHAR), _R("New file content as string.")))
    {
      RtWriteLastErrorMessage(_R("Failed to allocate result buffer: "));
      goto handle_error;
    }
    unWritten = 0;
    if (!RtChar_ReplaceString(lpFileContentAsString, lpSearched, lpReplacement, lpNewFileContentAsString, nNewFileContentAsStringSize + 1, &unWritten))
    {
      RtWriteLastErrorMessage(_R("Replacement failed: "));
      goto handle_error;
    }

    unNewFileSize = RtEncoding_EncodeWithHeap(lpNewFileContentAsString, unWritten, 0, &lpNewFileContent, lpHeap);
    if (unNewFileSize == -1)
    {
      RtWriteLastErrorMessage(_R("Failed to encode output file: "));
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
      RtWriteLastErrorMessage(_R("Failed free new file content: "));
      goto handle_error;
    }
  }
  if (lpNewFileContentAsString)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpNewFileContentAsString))
    {
      RtWriteLastErrorMessage(_R("Failed free new file content as string: "));
      goto handle_error;
    }
  }
  if (lpFileContentAsString)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContentAsString))
    {
      RtWriteLastErrorMessage(_R("Failed free file content as string: "));
      goto handle_error;
    }
  }
  if (lpFileContent)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
    {
      RtWriteLastErrorMessage(_R("Failed free file content: "));
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
    RtWriteLastErrorMessage(_R("Runtime heap creation failed: "));
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
      RtWriteLastErrorMessage(_R("Failed to close runtime heap: "));
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
    if ((!RtChar_CompareStrings(_R("/?"), lpArgV[1])) ||
        (!RtChar_CompareStrings(_R("-h"), lpArgV[1])) ||
        (!RtChar_CompareStrings(_R("--help"), lpArgV[1])))
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
