#include <RtWin32Ex.h>

#include "ZzResources.h"
#include "ZzUtils.h"

void* RT_CALL ZzZeroMemory(void* lpArea, RT_UN unSize)
{
  return memset(lpArea, 0, unSize);
}

RT_B RT_CALL ZzTestZeroMemory()
{
  RT_CHRONO rtChrono;
  RT_UCHAR8 lpBuffer[ZZ_BUFFER_SIZE];
  RT_N unI;
  RT_B bResult;

  /* Test memset 0. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* VC is inlining code based on ZZ_BUFFER_SIZE. */
    memset(lpBuffer, 0, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("memset 0"), &rtChrono)) goto handle_error;

  /* Test memset 0 in function. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* Avoid inlining and size prediction, should force CRT usage. */
    ZzZeroMemory(lpBuffer, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("memset 0 in function"), &rtChrono)) goto handle_error;

  /* Test RtMemory_Zero. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    RtMemory_Zero(lpBuffer, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("RtMemory_Zero"), &rtChrono)) goto handle_error;

  /* Test RT_MEMORY_ZERO. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* Intrinsic should be used, CRT otherwise. */
    RT_MEMORY_ZERO(lpBuffer, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("RT_MEMORY_ZERO"), &rtChrono)) goto handle_error;

  /* Ensure that lpDestination is used. */
  for (unI = 0; unI < ZZ_BUFFER_SIZE; unI++)
  {
    if (lpBuffer[unI]) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
