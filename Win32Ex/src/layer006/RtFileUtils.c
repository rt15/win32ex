#include "layer006/RtFileUtils.h"

#include "layer005/RtFile.h"

RT_B RT_API RtCreateEmptyFile(RT_CHAR* lpPath, RT_B bTruncate)
{
  RT_FILE rtFile;
  RT_B bFileCreated;
  RT_B bResult;

  bFileCreated = RT_FALSE;

  if (!RtCreateFile(&rtFile, lpPath, bTruncate ? RT_FILE_MODE_TRUNCATE : RT_FILE_MODE_NEW)) goto handle_error;
  bFileCreated = RT_TRUE;

  bResult = RT_SUCCESS;
free_resources:
  if (bFileCreated)
  {
    bFileCreated = RT_FALSE;
    if (!RtFreeFile(&rtFile) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
