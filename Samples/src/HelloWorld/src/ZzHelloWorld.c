#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RtWriteStringToConsole(_R("Hello, world!\n"));
  return 0;
}
