#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestWriteSmallFile(RT_CHAR* lpFilePath)
{
  RT_CHAR8* lpText;
  RT_B bResult;

  lpText = "First line.\r\nSecond line.\r\n";

  if (!RtSmallFile_Write(lpText, RtChar8_GetStringSize(lpText), lpFilePath, RT_SMALL_FILE_MODE_NEW)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestFileSize(RT_CHAR* lpFilePath)
{
  RT_FILE zzFile;
  RT_B bFileCreated;
  RT_UN unFileSize;
  RT_UN64 unFileSize64;
  RT_B bResult;

  bFileCreated = RT_FALSE;

  if (!RtFileSystem_GetFileSize(lpFilePath, &unFileSize64)) goto handle_error;
  if (unFileSize64 != 27) goto handle_error;

  if (!RtFile_Create(&zzFile, lpFilePath, RT_FILE_MODE_READ)) goto handle_error;
  bFileCreated = RT_TRUE;

  if (!RtFile_GetSize(&zzFile, &unFileSize)) goto handle_error;
  if (unFileSize != 27) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtIoDevice_Free(&zzFile.rtIoDevice) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestReadSmallFile(RT_HEAP** lpHeap, RT_CHAR* lpFilePath)
{
  RT_CHAR8* lpExpectedText;
  RT_CHAR8* lpOutput;
  RT_B bResult;

  lpOutput = RT_NULL;

  lpExpectedText = "First line.\r\nSecond line.\r\n";

  if (!RtSmallFile_Read(lpFilePath, &lpOutput, lpHeap)) goto handle_error;

  if (RT_MEMORY_COMPARE(lpOutput, lpExpectedText, RtChar8_GetStringSize(lpExpectedText))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (lpOutput)
  {
    if (!(*lpHeap)->lpFree(lpHeap, (void**)&lpOutput)) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSmallFile(RT_HEAP** lpHeap)
{
  RT_CHAR* lpFilePath;
  RT_B bResult;

  lpFilePath = _R("data/small_file.txt");
  if (!RtFileSystem_DeleteFileIfExists(lpFilePath)) goto handle_error;

  if (!ZzTestWriteSmallFile(lpFilePath)) goto handle_error;
  if (!ZzTestFileSize(lpFilePath)) goto handle_error;
  if (!ZzTestReadSmallFile(lpHeap, lpFilePath)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
