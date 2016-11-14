#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>

/*
select XDBM_LOG_SCRIPT_NAME, XDBM_LOG_MODEL, XDBM_LOG_SCRIPT_VERSION from ITEM_MAIN21.XDBM_LOG
select XDBM_LOG_SCRIPT_NAME, XDBM_LOG_MODEL, XDBM_LOG_SCRIPT_VERSION from ITEM_CACHE21.XDBM_LOG
select XDBM_LOG_SCRIPT_NAME, XDBM_LOG_MODEL, XDBM_LOG_SCRIPT_VERSION from ITEM_VIEW21.XDBM_LOG
*/

#define XS_SCHEMAS_COUNT 3
RT_CHAR* xs_lpSchemas[XS_SCHEMAS_COUNT] = { _R("Main"), _R("Cache"), _R("View") };

#define XS_TAGS_COUNT 2
RT_CHAR* xs_lpTagsNames[XS_TAGS_COUNT] = { _R("@model"), _R("@version") };

#define XS_BUFFER_SIZE 200

typedef struct _XS_CONTEXT
{
  RT_HEAP** lpHeap;
}
XS_CONTEXT;

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

RT_B RT_CALL XsManageTag(RT_CHAR* lpFileContent, RT_CHAR* lpTagName)
{
  RT_CHAR lpValue[XS_BUFFER_SIZE];
  RT_N nBegin;
  RT_N nEnd;
  RT_N nI;
  RT_N nJ;
  RT_B bResult;

  nBegin = RtSearchString(lpFileContent, lpTagName);
  if (nBegin == -1)
  {
    RtWriteStringToConsole(_R("Tag not found"));
    bResult = RT_FALSE;
    goto the_end;
  }
  nBegin += RtGetStringSize(lpTagName);
  while (lpFileContent[nBegin] != _R(':'))
  {
    nBegin++;
  }
  nBegin++;
  while (lpFileContent[nBegin] <= _R(' '))
  {
    nBegin++;
  }
  nEnd = nBegin;
  while (lpFileContent[nEnd] > _R(' '))
  {
    nEnd++;
  }

  nJ = 0;
  for (nI = nBegin; nI < nEnd; nI++)
  {
    lpValue[nJ] = lpFileContent[nI];
    nJ++;
  }
  lpValue[nJ] = 0;

  RtWriteStringsToConsole(2, _R("\t"), lpValue);

  bResult = RT_TRUE;

the_end:
  return bResult;
}

RT_B RT_CALL XsBrowseCallback(RT_CHAR* lpPath, RT_N nType, void* lpContext)
{
  XS_CONTEXT* lpXsContext;
  RT_HEAP** lpHeap;
  RT_CHAR lpLowerCasePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpFileName[RT_FILE_SYSTEM_MAX_FILE_NAME];
  RT_CHAR8* lpData;
  RT_CHAR* lpFileContent;
  RT_N nFileContentSize;
  RT_N nDataSize;
  RT_N nWritten;
  RT_N nI;
  RT_B bResult;

  bResult = RT_TRUE;

  if (nType == RT_FILE_SYSTEM_TYPE_FILE)
  {
    nWritten = 0;
    RtCopyString(lpPath, lpLowerCasePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &nWritten);
    RtFastLowerString(lpLowerCasePath);
    if (RtStringEndsWithWithSize(lpLowerCasePath, nWritten, _R(".sql"), 4))
    {
      nWritten = 0;
      RtExtractFileName(lpPath, RtGetStringSize(lpPath), lpFileName, RT_FILE_SYSTEM_MAX_FILE_NAME, &nWritten);
      /* Write file name without extension. */
      RtWriteStringToConsoleWithSize(lpFileName, nWritten - 4);

      lpXsContext = (XS_CONTEXT*)lpContext;
      lpHeap = lpXsContext->lpHeap;
      nDataSize = RtReadFromSmallFile(lpPath, &lpData, lpHeap);
      if (nDataSize == -1)
      {
        WriteLastErrorMessage(_R("Failed to read file: "));
        bResult = RT_FALSE;
      }

      nFileContentSize = RtDecodeWithHeap(lpData, nDataSize, RT_ENCODING_UTF_8, &lpFileContent, lpHeap);
      if (nFileContentSize == -1)
      {
        WriteLastErrorMessage(_R("Failed to decode file content: "));
        bResult = RT_FALSE;
      }
      else
      {
        for (nI = 0; nI < XS_TAGS_COUNT; nI++)
        {
          if (!XsManageTag(lpFileContent, xs_lpTagsNames[nI]))
          {
            bResult = RT_FALSE;
            break;
          }
        }

        if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
        {
          WriteLastErrorMessage(_R("Failed to free decoded file content: "));
          bResult = RT_FALSE;
        }
      }

      if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpData))
      {
        WriteLastErrorMessage(_R("Failed to free file content: "));
        bResult = RT_FALSE;
      }

      RtWriteStringToConsoleWithSize(_R("\n"), 1);
    }
  }

  return bResult;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RtRuntimeHeap runtimeHeap;
  RT_N nWritten;
  RT_CHAR* lpSchema;
  RT_CHAR lpPath[XS_BUFFER_SIZE];
  XS_CONTEXT context;
  RT_N nI;
  RT_UN32 unResult;

  if (!RtRuntimeHeapCreate(&runtimeHeap))
  {
    unResult = WriteLastErrorMessage(_R("Runtime heap creation failed: "));
    goto the_end;
  }

  context.lpHeap = &runtimeHeap.lpHeap;

  for (nI = 0; nI < XS_SCHEMAS_COUNT; nI++)
  {
    lpSchema = xs_lpSchemas[nI];

    nWritten = 0;
    RtCopyString(_R("database/"),                &lpPath[nWritten], XS_BUFFER_SIZE - nWritten, &nWritten);
    RtCopyString(lpSchema,                       &lpPath[nWritten], XS_BUFFER_SIZE - nWritten, &nWritten);
    RtCopyString(_R("/database/oracle/scripts"), &lpPath[nWritten], XS_BUFFER_SIZE - nWritten, &nWritten);

    if (!RtBrowsePath(lpPath, &XsBrowseCallback, RT_TRUE, &context))
    {
      unResult = 1;
      goto close_heap;
    }
    RtWriteStringToConsoleWithSize(_R("\n"), 1);
  }

  unResult = 0;
close_heap:
  if (!runtimeHeap.lpHeap->lpClose(&runtimeHeap))
  {
    unResult = WriteLastErrorMessage(_R("Failed to close runtime heap: "));
  }
the_end:
  return unResult;
}
