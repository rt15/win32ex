#include "layer008/RtSmallFile.h"

#include "layer002/RtErrorCode.h"
#include "layer006/RtFile.h"

RT_N RT_API RtReadFromSmallFile(RT_CHAR* lpFilePath, RT_CHAR8** lpOutput, RT_HEAP** lpHeap)
{
  void* lpHeapBuffer;
  RT_N nHeapBufferSize;

  lpHeapBuffer = RT_NULL;
  nHeapBufferSize = 0;
  return RtReadFromSmallFileWithBuffer(lpFilePath, RT_NULL, 0, &lpHeapBuffer, &nHeapBufferSize, lpOutput, lpHeap);
}

RT_N RT_API RtReadFromSmallFileWithBuffer(RT_CHAR* lpFilePath, RT_CHAR8* lpBuffer, RT_N nBufferSize, void** lpHeapBuffer, RT_N* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap)
{
  RT_FILE rtFile;
  RT_B bFreeFile;
  RT_N nResult;

  bFreeFile = RT_FALSE;

  if (!RtCreateFile(&rtFile, lpFilePath, RT_FILE_MODE_READ)) goto handle_error;
  bFreeFile = RT_TRUE;

  /* Retrieve the size of the file. */
  nResult = RtComputeFileSize(&rtFile);
  if (nResult == -1) goto handle_error;

  /* Allocate a space of size of file + 1 for trailing zero. */
  if (!RtAllocIfNeededWithHeap(lpBuffer, nBufferSize, lpHeapBuffer, lpHeapBufferSize, (void**)lpOutput, nResult + 1, _R("File buffer"), lpHeap)) goto handle_error;

  /* Write file content into buffer. */
  if (!RtReadFromFile(&rtFile, *lpOutput, nResult)) goto handle_error;

  /* Add trailing zero. */
  (*lpOutput)[nResult] = 0;

  goto free_resources;
handle_error:
  *lpOutput = RT_NULL;
  nResult = -1;
free_resources:
  if (bFreeFile)
  {
    if (!RtFreeFile(&rtFile) && nResult != -1)
    {
      bFreeFile = RT_FALSE;
      goto handle_error;
    }
    bFreeFile = RT_FALSE;
  }
  return nResult;
}

RT_B RT_API RtWriteToSmallFile(RT_CHAR8* lpInput, RT_N nDataSize, RT_CHAR* lpFilePath, RT_UN unMode)
{
  RT_FILE rtFile;
  RT_B bFileOpen;
  RT_UN unFileMode;
  RT_B bResult;

  bFileOpen = RT_FALSE;

  switch (unMode)
  {
    case RT_SMALL_FILE_MODE_APPEND:
      unFileMode = RT_FILE_MODE_READ_WRITE;
      break;
    case RT_SMALL_FILE_MODE_TRUNCATE:
      unFileMode = RT_FILE_MODE_TRUNCATE;
      break;
    case RT_SMALL_FILE_MODE_NEW:
      unFileMode = RT_FILE_MODE_NEW;
      break;
    default:
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }

  if (!RtCreateFile(&rtFile, lpFilePath, unFileMode)) goto handle_error;
  bFileOpen = RT_TRUE;

  if (unFileMode == RT_SMALL_FILE_MODE_APPEND)
  {
    if (!RtSetFilePointer(&rtFile, 0, RT_FILE_POS_END)) goto handle_error;
  }
  if (!RtWriteToFile(&rtFile, lpInput, nDataSize)) goto handle_error;

  bResult = RT_TRUE;
free_resources:
  if (bFileOpen)
  {
    bFileOpen = RT_FALSE;
    if (!RtFreeFile(&rtFile) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}
