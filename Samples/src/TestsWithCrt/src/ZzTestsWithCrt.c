#define RT_DEFINE_USE_CRT

#include <RtWin32Ex.h>
#include <RtWin32ExMain.h>

RT_B RT_CALL ZzMain(RT_N32 nArgC, RT_CHAR* lpArgV[])
{
  RT_B bResult;

  bResult = RT_TRUE;
  return bResult;
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
