#include "layer006/RtSmallFile.h"

#include "layer002/RtError.h"
#include "layer003/RtFile.h"
#include "layer003/RtHeapHelper.h"

RT_UN RT_API RtSmallFile_Read(RT_CHAR* lpFilePath, RT_CHAR8** lpOutput, RT_HEAP** lpHeap)
{
  void* lpHeapBuffer;
  RT_UN unHeapBufferSize;

  lpHeapBuffer = RT_NULL;
  unHeapBufferSize = 0;
  return RtSmallFile_ReadWithBuffer(lpFilePath, RT_NULL, 0, &lpHeapBuffer, &unHeapBufferSize, lpOutput, lpHeap);
}

RT_UN RT_API RtSmallFile_ReadWithBuffer(RT_CHAR* lpFilePath, RT_CHAR8* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, RT_CHAR8** lpOutput, RT_HEAP** lpHeap)
{
  RT_FILE rtFile;
  RT_B bFreeFile;
  RT_INPUT_STREAM* lpInputStream;
  RT_UN unBytesRead;
  RT_UN unResult;

  bFreeFile = RT_FALSE;

  if (!RtFile_Create(&rtFile, lpFilePath, RT_FILE_MODE_READ)) goto handle_error;
  bFreeFile = RT_TRUE;

  /* Retrieve the size of the file. */
  if (!RtFile_GetSize(&rtFile, &unResult)) goto handle_error;

  /* Allocate a space of size of file + 1 for trailing zero. */
  if (!RtHeapHelper_AllocIfNeeded(lpBuffer, unBufferSize, lpHeapBuffer, lpHeapBufferSize, (void**)lpOutput, unResult + 1, _R("File buffer"), lpHeap)) goto handle_error;

  /* Write file content into buffer. */
  lpInputStream = RtIoDevice_GetInputStream(&rtFile.rtIoDevice);
  if (!lpInputStream->lpRead(lpInputStream, *lpOutput, unResult, &unBytesRead)) goto handle_error;

  /* We know the size of the file so we should be able to read it completely. */
  if (unResult != unBytesRead)
  {
    RtError_SetLast(RT_ERROR_FUNCTION_FAILED);
    goto handle_error;
  }

  /* Add trailing zero. */
  (*lpOutput)[unResult] = 0;

  goto free_resources;
handle_error:
  *lpOutput = RT_NULL;
  unResult = RT_TYPE_MAX_UN;
free_resources:
  if (bFreeFile)
  {
    bFreeFile = RT_FALSE;
    if (!RtIoDevice_Free(&rtFile.rtIoDevice) && unResult != RT_TYPE_MAX_UN) goto handle_error;
    bFreeFile = RT_FALSE;
  }
  return unResult;
}

RT_B RT_API RtSmallFile_Write(RT_CHAR8* lpInput, RT_UN unDataSize, RT_CHAR* lpFilePath, RT_UN unMode)
{
  RT_FILE rtFile;
  RT_B bFileOpen;
  RT_UN unFileMode;
  RT_OUTPUT_STREAM* lpOutputStream;
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
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
  }

  if (!RtFile_Create(&rtFile, lpFilePath, unFileMode)) goto handle_error;
  bFileOpen = RT_TRUE;

  if (unFileMode == RT_SMALL_FILE_MODE_APPEND)
  {
    if (!RtFile_SetPointer(&rtFile, 0, RT_FILE_POS_END)) goto handle_error;
  }

  lpOutputStream = RtIoDevice_GetOutputStream(&rtFile.rtIoDevice);
  if (!lpOutputStream->lpWrite(lpOutputStream, lpInput, unDataSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bFileOpen)
  {
    bFileOpen = RT_FALSE;
    if (!RtIoDevice_Free(&rtFile.rtIoDevice) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
