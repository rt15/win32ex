#include <RtWin32Ex.h>

#include "ZzResources.h"
#include "ZzUtils.h"

void* RT_CALL ZzSetMemory(void* lpArea, RT_N32 nValue, RT_UN unSize)
{
  return memset(lpArea, nValue, unSize);
}

RT_B RT_CALL ZzTestSetMemory()
{
  RT_CHRONO rtChrono;
  RT_UCHAR8 lpBuffer[ZZ_BUFFER_SIZE];
  RT_N unI;
  RT_B bResult;

  /* Test memset. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* VC is inlining code based on ZZ_BUFFER_SIZE. */
    memset(lpBuffer, 1, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("memset"), &rtChrono)) goto handle_error;

  /* Test memset in function. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* Avoid inlining and size prediction, should force CRT usage. */
    ZzSetMemory(lpBuffer, 2, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("memset in function"), &rtChrono)) goto handle_error;

  /* Test RtSetMemory. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    RtSetMemory(lpBuffer, 3, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("RtSetMemory"), &rtChrono)) goto handle_error;

  /* Test RT_MEMORY_SET. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* Intrinsic should be used, CRT otherwise. */
    RT_MEMORY_SET(lpBuffer, 4, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("RT_MEMORY_SET"), &rtChrono)) goto handle_error;

  /* Ensure that lpDestination is used. */
  for (unI = 0; unI < ZZ_BUFFER_SIZE; unI++)
  {
    if (!lpBuffer[unI] == 4) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
