#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>
#include <RTWin32ExMem.h>

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

RT_B RT_CALL XsManageTag(RT_CHAR* lpFileContent, RT_CHAR* lpTagName)
{
  RT_CHAR lpValue[XS_BUFFER_SIZE];
  RT_N nBegin;
  RT_N nEnd;
  RT_N nI;
  RT_N unJ;
  RT_B bResult;

  nBegin = RtChar_SearchString(lpFileContent, lpTagName);
  if (nBegin == -1)
  {
    RtWriteStringToConsole(_R("Tag not found"));
    bResult = RT_FAILURE;
    goto the_end;
  }
  nBegin += RtChar_GetStringSize(lpTagName);
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

  unJ = 0;
  for (nI = nBegin; nI < nEnd; nI++)
  {
    lpValue[unJ] = lpFileContent[nI];
    unJ++;
  }
  lpValue[unJ] = 0;

  RtWriteStringsOrErrorsToConsole(RT_TRUE, _R("\t"), lpValue, (RT_CHAR*)RT_NULL);

  bResult = RT_SUCCESS;

the_end:
  return bResult;
}

RT_B RT_CALL XsBrowseCallback(RT_CHAR* lpPath, RT_UN unType, void* lpContext)
{
  XS_CONTEXT* lpXsContext;
  RT_HEAP** lpHeap;
  RT_CHAR lpLowerCasePath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR lpFileName[RT_FILE_SYSTEM_MAX_FILE_NAME];
  RT_CHAR8* lpData;
  RT_CHAR* lpFileContent;
  RT_N nFileContentSize;
  RT_N nDataSize;
  RT_UN unWritten;
  RT_N nI;
  RT_B bResult;

  bResult = RT_SUCCESS;

  if (unType == RT_FILE_SYSTEM_TYPE_FILE)
  {
    unWritten = 0;
    RtChar_CopyString(lpPath, lpLowerCasePath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten);
    RtChar_FastLowerString(lpLowerCasePath);
    if (RtChar_StringEndsWithWithSize(lpLowerCasePath, unWritten, _R(".sql"), 4))
    {
      unWritten = 0;
      RtFileSystem_GetFileName(lpPath, RtChar_GetStringSize(lpPath), lpFileName, RT_FILE_SYSTEM_MAX_FILE_NAME, &unWritten);
      /* Write file name without extension. */
      RtWriteStringToConsoleWithSize(lpFileName, unWritten - 4);

      lpXsContext = (XS_CONTEXT*)lpContext;
      lpHeap = lpXsContext->lpHeap;
      nDataSize = RtReadFromSmallFile(lpPath, &lpData, lpHeap);
      if (nDataSize == -1)
      {
        RtWriteLastErrorMessage(_R("Failed to read file: "));
        bResult = RT_FAILURE;
      }

      nFileContentSize = RtEncoding_DecodeWithHeap(lpData, nDataSize, RT_ENCODING_UTF_8, &lpFileContent, lpHeap);
      if (nFileContentSize == -1)
      {
        RtWriteLastErrorMessage(_R("Failed to decode file content: "));
        bResult = RT_FAILURE;
      }
      else
      {
        for (nI = 0; nI < XS_TAGS_COUNT; nI++)
        {
          if (!XsManageTag(lpFileContent, xs_lpTagsNames[nI]))
          {
            bResult = RT_FAILURE;
            break;
          }
        }

        if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent))
        {
          RtWriteLastErrorMessage(_R("Failed to free decoded file content: "));
          bResult = RT_FAILURE;
        }
      }

      if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpData))
      {
        RtWriteLastErrorMessage(_R("Failed to free file content: "));
        bResult = RT_FAILURE;
      }

      RtWriteStringToConsoleWithSize(_R("\n"), 1);
    }
  }

  return bResult;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RtRuntimeHeap runtimeHeap;
  RT_UN unWritten;
  RT_CHAR* lpSchema;
  RT_CHAR lpPath[XS_BUFFER_SIZE];
  XS_CONTEXT context;
  RT_N nI;
  RT_UN32 unResult;

  if (!RtRuntimeHeapCreate(&runtimeHeap))
  {
    unResult = RtWriteLastErrorMessage(_R("Runtime heap creation failed: "));
    goto the_end;
  }

  context.lpHeap = &runtimeHeap.lpHeap;

  for (nI = 0; nI < XS_SCHEMAS_COUNT; nI++)
  {
    lpSchema = xs_lpSchemas[nI];

    unWritten = 0;
    RtChar_CopyString(_R("database/"),                &lpPath[unWritten], XS_BUFFER_SIZE - unWritten, &unWritten);
    RtChar_CopyString(lpSchema,                       &lpPath[unWritten], XS_BUFFER_SIZE - unWritten, &unWritten);
    RtChar_CopyString(_R("/database/oracle/scripts"), &lpPath[unWritten], XS_BUFFER_SIZE - unWritten, &unWritten);

    if (!RtFileSystem_BrowsePath(lpPath, &XsBrowseCallback, RT_TRUE, RT_FALSE, &context))
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
    unResult = RtWriteLastErrorMessage(_R("Failed to close runtime heap: "));
  }
the_end:
  return unResult;
}
