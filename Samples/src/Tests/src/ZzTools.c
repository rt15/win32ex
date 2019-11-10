#include "ZzTools.h"

RT_B RT_CALL ZzVConcatLines(va_list lpVaList, RT_HEAP** lpHeap, void** lpBuffer)
{
  va_list lpVaList2;
  RT_UN unWritten;
  RT_UN unOutputSize;
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
      unReferenceSize += RtChar_GetStringSize(lpLine);
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
      if (!RtChar_CopyString(lpLine,               &lpBufferChars[unWritten], unReferenceBufferSize - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
      if (!RtChar_CopyStringWithSize(_R("\n"), 1,  &lpBufferChars[unWritten], unReferenceBufferSize - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
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

RT_B RT_CDECL ZzCheckTextFile(RT_CHAR* lpFilePath, RT_HEAP** lpHeap, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpHeap);
  bResult = ZzVCheckTextFile(lpVaList, lpFilePath, lpHeap);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_CALL ZzVCheckTextFile(va_list lpVaList, RT_CHAR* lpFilePath, RT_HEAP** lpHeap)
{
  RT_CHAR* lpReference;
  RT_UN unFileSize;
  RT_CHAR8* lpRawFileContent;
  RT_CHAR* lpFileContent;
  RT_UN unContentSize;
  RT_B bResult;

  lpReference = RT_NULL;
  lpRawFileContent = RT_NULL;
  lpFileContent = RT_NULL;

  if (!ZzVConcatLines(lpVaList, lpHeap, (void**)&lpReference)) goto handle_error;

  unFileSize = RtSmallFile_Read(lpFilePath, &lpRawFileContent, lpHeap);
  if (unFileSize == -1) goto handle_error;

  unContentSize = RtEncoding_DecodeWithHeap(lpRawFileContent, unFileSize, RT_ENCODING_US_ASCII, &lpFileContent, lpHeap);
  if (unContentSize == -1) goto handle_error;

  if (RtChar_GetStringSize(lpReference) != unContentSize) goto handle_error;

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
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CDECL ZzWriteLinesToDevice(RT_IO_DEVICE* lpIoDevice, RT_HEAP** lpHeap, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpHeap);
  bResult = ZzVWriteLinesToDevice(lpVaList, lpIoDevice, lpHeap);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_CALL ZzVWriteLinesToDevice(va_list lpVaList, RT_IO_DEVICE* lpIoDevice, RT_HEAP** lpHeap)
{
  RT_CHAR* lpLines;
  RT_CHAR8* lpRawFileContent;
  RT_UN unRawFileContentSize;
  RT_OUTPUT_STREAM* lpOutputStream;
  RT_B bResult;

  lpLines = RT_NULL;
  lpRawFileContent = RT_NULL;

  if (!ZzVConcatLines(lpVaList, lpHeap, (void**)&lpLines)) goto handle_error;

  unRawFileContentSize = RtEncoding_EncodeWithHeap(lpLines, RT_TYPE_MAX_UN, RT_ENCODING_US_ASCII, &lpRawFileContent, lpHeap);
  if (unRawFileContentSize == -1) goto handle_error;

  lpOutputStream = RtIoDevice_GetOutputStream(lpIoDevice);
  if (!lpOutputStream->lpWrite(lpOutputStream, lpRawFileContent, unRawFileContentSize)) goto handle_error;

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
  RT_UN unOutputSize;
  RT_B bResult;

  if (!RtFileSystem_GetExecutableFilePath(                       lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_GetParentPath(lpPath, unOutputSize,          lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_GetParentPath(lpPath, unOutputSize,          lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_BuildPath(lpPath, unOutputSize, _R("src"),   lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;
  if (!RtFileSystem_BuildPath(lpPath, unOutputSize, _R("Tests"), lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH, &unOutputSize)) goto handle_error;

  if (!RtFileSystem_SetCurrentDirectory(lpPath)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_N RT_CALL ZzCompareStringsArrays(RT_CHAR** lpStrings1, RT_CHAR** lpStrings2)
{
  RT_CHAR** lpInStrings1;
  RT_CHAR** lpInStrings2;
  RT_N nResult;

  lpInStrings1 = lpStrings1;
  lpInStrings2 = lpStrings2;

  nResult = 0;
  while (*lpInStrings1 && *lpInStrings2)
  {
    nResult = RtChar_CompareStrings(*lpInStrings1, *lpInStrings2);
    if (nResult)
    {
      break;
    }
    (*lpInStrings1)++;
    (*lpInStrings2)++;
  }
  if (!nResult)
  {
    if (!*lpInStrings1)
    {
      if (!*lpInStrings2)
      {
        nResult = 0;
      }
      else
      {
        nResult = 1;
      }
    }
    else
    {
      /* *lpInStrings1 is not RT_NULL so *lpInStrings2 is. */
      nResult = -1;
    }
  }
  return nResult;
}
