#include "ZzReplaceInFile.h"

#include "ZzLuaUtils.h"

#include <lauxlib.h>

RT_B ZzPerformWithHeap(RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpFilePath, RT_HEAP** lpHeap)
{
  RT_CHAR8* lpFileContent8;
  RT_CHAR* lpFileContent;
  RT_N nFileContentSize;
  RT_CHAR* lpNewFileContent;
  RT_CHAR8* lpNewFileContent8;
  RT_N nNewFileContentSize;
  RT_N nDelta;

  RT_UN unWritten;
  RT_UN unFileSize;
  RT_UN unNewFileSize;
  RT_N nOcurrencesCount;
  RT_B bResult;

  lpFileContent8 = RT_NULL;
  lpFileContent = RT_NULL;
  lpNewFileContent = RT_NULL;
  lpNewFileContent8 = RT_NULL;

  /* Check file path. */
  if (!RtCheckPath(lpFilePath, RT_FILE_SYSTEM_TYPE_FILE)) goto handle_error;

  /* Read input file content. */
  unFileSize = RtReadFromSmallFile(lpFilePath, &lpFileContent8, lpHeap);
  if (unFileSize == RT_TYPE_MAX_UN) goto handle_error;

  /* Decode input file content. */
  nFileContentSize = RtDecodeWithHeap(lpFileContent8, unFileSize, 0, &lpFileContent, lpHeap);
  if (nFileContentSize == -1) goto handle_error;

  nOcurrencesCount = RtCountStringOccurrences(lpFileContent, lpSearched);
  if (nOcurrencesCount > 0)
  {
    nDelta = (RtGetStringSize(lpReplacement) - RtGetStringSize(lpSearched)) * nOcurrencesCount;
    nNewFileContentSize = nFileContentSize + nDelta;
    if (!(*lpHeap)->lpAlloc(lpHeap, (void**)&lpNewFileContent, (nNewFileContentSize + 1) * sizeof(RT_CHAR), _R("New file content as string."))) goto handle_error;

    unWritten = 0;
    if (!RtReplaceString(lpFileContent, lpSearched, lpReplacement, lpNewFileContent, nNewFileContentSize + 1, &unWritten)) goto handle_error;

    unNewFileSize = RtEncodeWithHeap(lpNewFileContent, unWritten, 0, &lpNewFileContent8, lpHeap);
    if (unNewFileSize == -1) goto handle_error;

    if (!RtWriteToSmallFile(lpNewFileContent8, unNewFileSize, lpFilePath, RT_SMALL_FILE_MODE_TRUNCATE)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  if (lpNewFileContent8)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpNewFileContent8)) goto handle_error;
  }
  if (lpNewFileContent)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpNewFileContent)) goto handle_error;
  }
  if (lpFileContent)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent)) goto handle_error;
  }
  if (lpFileContent8)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent8)) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B ZzPerform(RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpFilePath)
{
  RtRuntimeHeap runtimeHeap;
  RT_B bCloseRuntimeHeap;
  RT_B bResult;

  bCloseRuntimeHeap = RT_FALSE;

  if (!RtRuntimeHeapCreate(&runtimeHeap)) goto handle_error;
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
      bCloseRuntimeHeap = RT_FALSE;
      goto handle_error;
    }
    bCloseRuntimeHeap = RT_FALSE;
  }
  return bResult;
}

RT_EXPORT RT_N32 RT_CDECL ZzReplaceInFile(lua_State* lpLuaState)
{
  RT_CHAR8* lpSearched8;
  RT_CHAR8* lpReplacement8;
  RT_CHAR8* lpFilePath8;

  RT_CHAR lpSearched[RT_CHAR_QUARTER_BIG_STRING_SIZE];
  RT_CHAR lpReplacement[RT_CHAR_QUARTER_BIG_STRING_SIZE];
  RT_CHAR lpFilePath[RT_FILE_SYSTEM_MAX_FILE_PATH];

  RT_UN unWritten;
  RT_N32 nResult;

  nResult = 0;

  /* lpSearched. */
  lpSearched8 = (RT_CHAR8*)luaL_checkstring(lpLuaState, 1);
  if (!lpSearched8)
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  unWritten = RtDecodeWithBuffer(lpSearched8, -1, 0, lpSearched, RT_CHAR_QUARTER_BIG_STRING_SIZE);
  if (unWritten == -1) goto handle_error;

  /* lpReplacement. */
  lpReplacement8 = (RT_CHAR8*)luaL_checkstring(lpLuaState, 2);
  if (!lpReplacement8)
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  unWritten = RtDecodeWithBuffer(lpReplacement8, -1, 0, lpReplacement, RT_CHAR_QUARTER_BIG_STRING_SIZE);
  if (unWritten == -1) goto handle_error;

  /* lpFilePath. */
  lpFilePath8 = (RT_CHAR8*)luaL_checkstring(lpLuaState, 3);
  if (!lpFilePath8)
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  unWritten = RtDecodeWithBuffer(lpFilePath8, -1, 0, lpFilePath, RT_FILE_SYSTEM_MAX_FILE_PATH);
  if (unWritten == -1) goto handle_error;

  if (!ZzPerform(lpSearched, lpReplacement, lpFilePath)) goto handle_error;

free_resources:
  return nResult;
handle_error:
  ZzPushLastErrorMessage(lpLuaState);
  nResult = 1;
  goto free_resources;
}
