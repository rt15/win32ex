#include "layer006/RtTempFile.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer003/RtChar.h"
#include "layer005/RtFileSystem.h"

RT_B RT_API RtTempFile_Create(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
  RT_ARRAY zzTempDirectory;
  RT_CHAR lpTempDirectory[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_UN unOutputSize;
  RT_B bResult;

  RtArray_Create(&zzTempDirectory, lpTempDirectory, sizeof(RT_CHAR), RT_FILE_SYSTEM_MAX_FILE_PATH);

  if (!RtFileSystem_GetTempDirectory(&zzTempDirectory)) goto handle_error;
  unOutputSize = zzTempDirectory.unSize;
  if (!RtChar_Append(&zzTempDirectory, 0)) goto handle_error;
  if (!RtTempFile_CreateWithParentPath(lpFile, lpPrefix, lpTempDirectory, unOutputSize, lpBuffer, unBufferSize, lpOutputSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtTempFile_CreateWithParentPath(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpParentPath, RT_UN unParentPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
#ifdef RT_DEFINE_LINUX
  RT_UN unOutputSize;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* GetTempFileName create an empty file. */
  if (!GetTempFileName(lpParentPath, lpPrefix, 0, lpBuffer)) goto handle_error;

  if (!RtFile_Create(lpFile, lpBuffer, RT_FILE_MODE_TRUNCATE)) goto handle_error;

  *lpOutputSize = RtChar_GetCStringSize(lpBuffer);
#else
  *lpOutputSize = 0;
  if (!RtFileSystem_BuildPath(lpParentPath, unParentPathSize, lpPrefix, lpParentPath, unBufferSize, &unOutputSize)) goto handle_error; *lpOutputSize += unOutputSize;

  /* The 6 last characters of mkstemp template must be "XXXXXX" and they will be replaced by mkstemp. */
  if (!RtChar_CopyStringWithSize(_R("XXXXXX"), 6, &lpBuffer[*lpOutputSize], unBufferSize - *lpOutputSize, &unOutputSize)) goto handle_error; *lpOutputSize += unOutputSize;

  /* Returns -1 and set errno in case of error. */
  lpFile->rtIoDevice.nFileDescriptor = mkostemp(lpBuffer, O_CLOEXEC);
  if (lpFile->rtIoDevice.nFileDescriptor == -1) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
