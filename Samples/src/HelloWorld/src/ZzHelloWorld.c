#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>
#include <RtWin32ExMem.h>

RT_UN16 RT_CALL RtMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_ARRAY zzMessage;
  RT_UN16 unResult;

  if (RtConsole_WriteString(RtChar_CreateString(&zzMessage, _R("Hello, world!\n"))))
  {
    unResult = 0;
  }
  else
  {
    unResult = 1;
  }
  return unResult;
}
