#define RT_DEFINE_USE_CRT

#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>

RT_B RT_CALL ZzTestCompareMemory();
RT_B RT_CALL ZzTestCopyMemory();
RT_B RT_CALL ZzTestMoveMemory();
RT_B RT_CALL ZzTestSetMemory();
RT_B RT_CALL ZzTestZeroMemory();

RT_B RT_CALL ZzMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_B bResult;

  if (!ZzTestCompareMemory()) goto handle_error;
  RtWriteStringToConsoleWithSize(_R("\n"), 1);

  if (!ZzTestCopyMemory()) goto handle_error;
  RtWriteStringToConsoleWithSize(_R("\n"), 1);

  if (!ZzTestMoveMemory()) goto handle_error;
  RtWriteStringToConsoleWithSize(_R("\n"), 1);

  if (!ZzTestSetMemory()) goto handle_error;
  RtWriteStringToConsoleWithSize(_R("\n"), 1);

  if (!ZzTestZeroMemory()) goto handle_error;
  RtWriteStringToConsoleWithSize(_R("\n"), 1);

  bResult = RT_SUCCESS;
free_resources:
  RtWriteStringToConsole(_R("Pause."));
  RtPauseConsole();
  return bResult;
  
handle_error:
  RtWriteStringToConsole(_R("Error.\n\n"));
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_UN16 unResult;
  if (ZzMain(nArgC, lpArgV))
  {
    unResult = 0;
  }
  else
  {
    unResult = 1;
  }
  return unResult;
}
