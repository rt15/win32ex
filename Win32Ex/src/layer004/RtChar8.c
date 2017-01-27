#include "layer004/RtChar8.h"

RT_UN RT_API RtGetString8Size(RT_CHAR8* lpInput)
{
  RT_CHAR8* lpInInput;
  RT_UN unResult;

  lpInInput = lpInput;

  while (*lpInInput++);

  unResult = (RT_UN)(lpInInput - lpInput - 1);

  return unResult;
}

RT_N RT_API RtCompareString8s(RT_CHAR8* lpString1, RT_CHAR8* lpString2)
{
  RT_N nResult;

  nResult = 0;

  while(!(nResult = *(RT_UCHAR8*)lpString1 - *(RT_UCHAR8*)lpString2) && *lpString2)
  {
    lpString1++;
    lpString2++;
  }

  return nResult;
}
