#include "ZzTools.h"

RT_B RT_CALL ZzVConcatLines(va_list lpVaList, RT_HEAP** lpHeap, void** lpBuffer)
{
  va_list lpVaList2;
  RT_UN unWritten;
  RT_CHAR* lpLine;
  RT_CHAR* lpBufferChars;
  RT_UN unReferenceSize;
  RT_UN unReferenceBufferSize;
  RT_B bResult;

  RT_VA_COPY(lpVaList, lpVaList2);

  /* Compute reference size. */
  unReferenceSize = 0;
  while (RT_TRUE)
  {
    lpLine = va_arg(lpVaList, RT_CHAR*);
    if (lpLine)
    {
      unReferenceSize += RtGetStringSize(lpLine);
      unReferenceSize++; /* End of line. */
    }
    else
    {
      break;
    }
  }
  /* Trailing zero. */
  unReferenceBufferSize = unReferenceSize + 1;

  if (!(*lpHeap)->lpAlloc(lpHeap, lpBuffer, unReferenceBufferSize * sizeof(RT_CHAR), _R("Reference buffer"))) goto handle_error;

  lpBufferChars = *lpBuffer;

  unWritten = 0;
  while (RT_TRUE)
  {
    lpLine = va_arg(lpVaList2, RT_CHAR*);
    if (lpLine)
    {
      if (!RtCopyString(lpLine,               &lpBufferChars[unWritten], unReferenceBufferSize - unWritten, &unWritten)) goto handle_error;
      if (!RtCopyStringWithSize(_R("\n"), 1,  &lpBufferChars[unWritten], unReferenceBufferSize - unWritten, &unWritten)) goto handle_error;
    }
    else
    {
      break;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  va_end(lpVaList2);
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzCheckTextFile(RT_CHAR* lpFilePath, RT_HEAP** lpHeap, ...)
{
  va_list lpVaList;
  RT_CHAR* lpReference;
  RT_UN unFileSize;
  RT_CHAR8* lpRawFileContent;
  RT_CHAR* lpFileContent;
  RT_UN unContentSize;
  RT_B bResult;

  lpReference = RT_NULL;
  lpRawFileContent = RT_NULL;
  lpFileContent = RT_NULL;

  va_start(lpVaList, lpHeap);

  if (!ZzVConcatLines(lpVaList, lpHeap, (void**)&lpReference)) goto handle_error;

  unFileSize = RtReadFromSmallFile(lpFilePath, &lpRawFileContent, lpHeap);
  if (unFileSize == -1) goto handle_error;

  unContentSize = RtDecodeWithHeap(lpRawFileContent, unFileSize, RT_ENCODING_US_ASCII, &lpFileContent, lpHeap);
  if (unContentSize == -1) goto handle_error;

  if (RtGetStringSize(lpReference) != unContentSize) goto handle_error;

  if (RT_MEMORY_COMPARE(lpFileContent, lpReference, unContentSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (lpFileContent)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpFileContent) && bResult) goto handle_error;
  }
  if (lpRawFileContent)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpRawFileContent) && bResult) goto handle_error;
  }
  if (lpReference)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpReference) && bResult) goto handle_error;
  }
  va_end(lpVaList);
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzWriteLinesToFile(RT_FILE* lpFile, RT_HEAP** lpHeap, ...)
{
  va_list lpVaList;
  RT_CHAR* lpLines;
  RT_CHAR8* lpRawFileContent;
  RT_UN unRawFileContentSize;
  RT_B bResult;

  lpLines = RT_NULL;
  lpRawFileContent = RT_NULL;

  va_start(lpVaList, lpHeap);

  if (!ZzVConcatLines(lpVaList, lpHeap, (void**)&lpLines)) goto handle_error;

  unRawFileContentSize = RtEncodeWithHeap(lpLines, RT_TYPE_MAX_UN, RT_ENCODING_US_ASCII, &lpRawFileContent, lpHeap);
  if (unRawFileContentSize == -1) goto handle_error;

  if (!RtWriteToFile(lpFile, lpRawFileContent, unRawFileContentSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (lpRawFileContent)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpRawFileContent) && bResult) goto handle_error;
  }
  if (lpLines)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpLines) && bResult) goto handle_error;
  }
  va_end(lpVaList);
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

/**
 * The tests must be executed in the right directory.
 */
RT_B RT_CALL ZzAdjustDirectory()
{
  RT_CHAR lpPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unWritten;
  RT_UN unPathSize;
  RT_B bResult;

  unWritten = 0;
  if (!RtGetExecutableFilePath(lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  unPathSize = unWritten;
  unWritten = 0;
  if (!RtExtractParentPath(lpPath, unPathSize, lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  unPathSize = unWritten;
  unWritten = 0;
  if (!RtExtractParentPath(lpPath, unPathSize, lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unWritten)) goto handle_error;

  if (!RtBuildPath(lpPath, unWritten, _R("src"), RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;

  if (!RtBuildPath(lpPath, unWritten, _R("Tests"), RT_FILE_SYSTEM_MAX_FILE_PATH - unWritten, &unWritten)) goto handle_error;

  if (!RtSetCurrentDirectory(lpPath)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
