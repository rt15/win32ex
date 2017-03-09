#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestPipe()
{
  RT_CHAR8 lpBuffer[8];
  RT_FILE zzReadPipe;
  RT_FILE zzWritePipe;
  RT_B bReadPipeCreated;
  RT_B bWritePipeCreated;
  RT_UN unBytesRead;
  RT_B bResult;

  bReadPipeCreated = RT_FALSE;
  bWritePipeCreated = RT_FALSE;

  if (!RtCreatePipe(&zzReadPipe, &zzWritePipe)) goto handle_error;
  bReadPipeCreated = RT_TRUE;
  bWritePipeCreated = RT_TRUE;

  if (!RtWriteToFile(&zzWritePipe, "foo", 3)) goto handle_error;
  
  /* Close the pipe so that RtReadFromFile will read 3 bytes then encounter EOF. */
  bWritePipeCreated = RT_FALSE;
  if (!RtFreeFile(&zzWritePipe)) goto handle_error;

  RT_MEMORY_ZERO(lpBuffer, 8);
  if (!RtReadFromFile(&zzReadPipe, lpBuffer, 8, &unBytesRead)) goto handle_error;

  if (unBytesRead != 3) goto handle_error;

  if (RT_MEMORY_COMPARE(lpBuffer, "foo\0\0\0\0\0", 8)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bWritePipeCreated)
  {
    bWritePipeCreated = RT_FALSE;
    if (!RtFreeFile(&zzWritePipe) && bResult) goto handle_error;
  }
  if (bReadPipeCreated)
  {
    bReadPipeCreated = RT_FALSE;
    if (!RtFreeFile(&zzReadPipe) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestFile()
{
  RT_B bResult;

  if (!ZzTestPipe()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}