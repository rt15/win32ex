#include <RtWin32Ex.h>

#include "ZzResources.h"
#include "ZzUtils.h"


RT_N RT_CALL ZzCompareMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  return memcmp(lpArea1, lpArea2, unSize);
}

RT_B RT_CALL ZzTestCompareMemory()
{
  RT_CHRONO rtChrono;
  RT_UCHAR8 lpArea1[ZZ_BUFFER_SIZE];
  RT_UCHAR8 lpArea2[ZZ_BUFFER_SIZE];
  RT_UN unI;
  RT_B bResult;

  /* Initialize source buffer. */
  for (unI = 0; unI < ZZ_BUFFER_SIZE - 1; unI++)
  {
    lpArea1[unI] = (RT_UCHAR8)(unI % 200);
    lpArea2[unI] = lpArea1[unI];
  }
  lpArea1[ZZ_BUFFER_SIZE - 1] = 'b';
  lpArea2[ZZ_BUFFER_SIZE - 1] = 'a';

  /* Test memcmp. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    if (memcmp(lpArea1, lpArea2, ZZ_BUFFER_SIZE) <= 0) goto handle_error;
  }
  if (!ZzStopChrono(_R("memcmp"), &rtChrono)) goto handle_error;

  /* Test memcmp in function. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    if (ZzCompareMemory(lpArea1, lpArea2, ZZ_BUFFER_SIZE) <= 0) goto handle_error;
  }
  if (!ZzStopChrono(_R("memcmp in function"), &rtChrono)) goto handle_error;

  /* Test RtCompareMemory. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    if (RtCompareMemory(lpArea1, lpArea2, ZZ_BUFFER_SIZE) <= 0) goto handle_error;
  }
  if (!ZzStopChrono(_R("RtCompareMemory"), &rtChrono)) goto handle_error;

  /* Test RT_MEMORY_COMPARE. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    if (RT_MEMORY_COMPARE(lpArea1, lpArea2, ZZ_BUFFER_SIZE) <= 0) goto handle_error;
  }
  if (!ZzStopChrono(_R("RT_MEMORY_COMPARE"), &rtChrono)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
