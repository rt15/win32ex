#include "layer003/RtComparisonCallback.h"

RT_B RT_API RtDefaultCompareCallback(void* lpItem1, void* lpItem2, void* lpContext, RT_N* lpComparisonResult)
{
  RT_N* lpValue1;
  RT_N* lpValue2;

  lpValue1 = (RT_N*)lpItem1;
  lpValue2 = (RT_N*)lpItem2;

  *lpComparisonResult = *lpValue1 - *lpValue2;
  return RT_TRUE;
}
