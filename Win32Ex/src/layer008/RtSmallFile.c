#include "layer008/RtSmallFile.h"

#include "layer002/RtErrorCode.h"
#include "layer006/RtFile.h"

RT_UN RT_API RtReadFromSmallFile(RT_CHAR* lpFilePath, RT_CHAR8** lpOutput, RT_HEAP** lpHeap)
{
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;

  lpHeapBuffer = RT_NULL;
  unHeapBufferSize = 0;
  return RtReadFromSmallFileWithBuffer(lpFilePath, RT_NULL, 0, &lpHeapBuffer, &unHeapBufferSize, lpOutput, lpHeap);
}

RT_UN RT_API RtReadFromSmallFileWithBuffer(RT_CHAR* lpFilePath, RT_CHAR8* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap)
{
  RT_FILE rtFile;
  RT_B bFreeFile;
  RT_UN unResult;

  bFreeFile = RT_FALSE;

  if (!RtCreateFile(&rtFile, lpFilePath, RT_FILE_MODE_READ)) goto handle_error;
  bFreeFile = RT_TRUE;

  /* Retrieve the size of the file. */
  unResult = RtComputeFileSize(&rtFile);
  if (unResult == RT_TYPE_MAX_UN) goto handle_error;

  /* Allocate a space of size of file + 1 for trailing zero. */
  if (!RtAllocIfNeededWithHeap(lpBuffer, unBufferSize, lpHeapBuffer, lpHeapBufferSize, (void**)lpOutput, unResult + 1, _R("File buffer"), lpHeap)) goto handle_error;

  /* Write file content into buffer. */
  if (!RtReadFromFile(&rtFile, *lpOutput, unResult)) goto handle_error;

  /* Add trailing zero. */
  (*lpOutput)[unResult] = 0;

  goto free_resources;
handle_error:
  *lpOutput = RT_NULL;
  unResult = RT_TYPE_MAX_UN;
free_resources:
  if (bFreeFile)
  {
    if (!RtFreeFile(&rtFile) && unResult != RT_TYPE_MAX_UN)
    {
      bFreeFile = RT_FALSE;
      goto handle_error;
    }
    bFreeFile = RT_FALSE;
  }
  return unResult;
}

RT_B RT_API RtWriteToSmallFile(RT_CHAR8* lpInput, RT_UN unDataSize, RT_CHAR* lpFilePath, RT_UN unMode)
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
  if (!RtWriteToFile(&rtFile, lpInput, unDataSize)) goto handle_error;

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
