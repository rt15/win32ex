#include "ZzTools.h"

void RT_CALL ZzWriteLastErrorMessage(RT_CHAR* lpLabel)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;

  unWritten = 0;
  RtCopyString(lpLabel, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &unWritten);
  RtGetLastErrorMessage(&lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);
  RtCopyStringWithSize(_R("\n"), 1, &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);

  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);
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
