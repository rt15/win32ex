#include "layer006/RtTempFile.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer003/RtChar.h"
#include "layer004/RtFileSystem.h"

RT_B RT_API RtTempFile_Create(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten)
{
  RT_UN unWritten;
  RT_B bResult;

  /* Little trick: use the buffer to store the temp directory. */
  unWritten = 0;
  if (!RtFileSystem_GetTempDirectory(lpBuffer, unBufferSize, &unWritten)) goto handle_error;

  if (!RtTempFile_CreateWithParentPath(lpFile, lpPrefix, lpBuffer, unWritten, lpBuffer, unBufferSize, lpWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtTempFile_CreateWithParentPath(RT_FILE* lpFile, RT_CHAR* lpPrefix, RT_CHAR* lpParentPath, RT_UN unParentPathSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten)
{
#ifdef RT_DEFINE_LINUX
  RT_UN unWritten;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* GetTempFileName create an empty file. */
  if (!GetTempFileName(lpParentPath, lpPrefix, 0, lpBuffer)) goto handle_error;

  if (!RtFile_Create(lpFile, lpBuffer, RT_FILE_MODE_TRUNCATE)) goto handle_error;

  *lpWritten += RtChar_GetStringSize(lpBuffer);
#else
  unWritten = unParentPathSize;
  if (!RtFileSystem_BuildPath(lpParentPath, unParentPathSize, lpPrefix, unBufferSize, &unWritten)) goto handle_error;

  /* The 6 last characters of mkstemp template must be "XXXXXX" and they will be replaced by mkstemp. */
  if (!RtChar_CopyStringWithSize(_R("XXXXXX"), 6, &lpBuffer[unWritten], unBufferSize - unWritten, &unWritten)) goto handle_error;

  /* Returns -1 and set errno in case of error. */
  lpFile->rtIoDevice.nFileDescriptor = mkostemp(lpBuffer, O_CLOEXEC);
  if (lpFile->rtIoDevice.nFileDescriptor == -1) goto handle_error;

  *lpWritten += unWritten;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
