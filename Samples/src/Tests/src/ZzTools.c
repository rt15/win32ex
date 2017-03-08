#include "ZzTools.h"

RT_B RT_CALL ZzCheckTextFile(RT_CHAR* lpFilePath, RT_HEAP** lpHeap, ...)
{
  va_list lpVaList;
  va_list lpVaList2;

  RT_UN unWritten;
  RT_CHAR* lpLine;
  RT_CHAR* lpReference;
  RT_UN unReferenceSize;
  RT_UN unReferenceBufferSize;
  RT_UN unFileSize;
  RT_CHAR8* lpRawFileContent;
  RT_CHAR* lpFileContent;
  RT_UN unContentSize;
  RT_B bResult;

  lpReference = RT_NULL;
  lpRawFileContent = RT_NULL;
  lpFileContent = RT_NULL;

  va_start(lpVaList, lpHeap);
  RT_VA_COPY(lpVaList2, lpVaList);

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

  if (!(*lpHeap)->lpAlloc(lpHeap, (void**)&lpReference, unReferenceBufferSize * sizeof(RT_CHAR), _R("Reference buffer"))) goto handle_error;

  unWritten = 0;
  while (RT_TRUE)
  {
    lpLine = va_arg(lpVaList2, RT_CHAR*);
    if (lpLine)
    {
      if (!RtCopyString(lpLine,               &lpReference[unWritten], unReferenceBufferSize - unWritten, &unWritten)) goto handle_error;
      if (!RtCopyStringWithSize(_R("\n"), 1,  &lpReference[unWritten], unReferenceBufferSize - unWritten, &unWritten)) goto handle_error;
    }
    else
    {
      break;
    }
  }

  unFileSize = RtReadFromSmallFile(lpFilePath, &lpRawFileContent, lpHeap);
  if (unFileSize == -1) goto handle_error;

  unContentSize = RtDecodeWithHeap(lpRawFileContent, unFileSize, RT_ENCODING_US_ASCII, &lpFileContent, lpHeap);
  if (unContentSize == -1) goto handle_error;

  if (unReferenceSize != unContentSize) goto handle_error;

  if (RT_MEMORY_COMPARE(lpFileContent, lpReference, unReferenceSize)) goto handle_error;

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
  va_end(lpVaList2);
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
