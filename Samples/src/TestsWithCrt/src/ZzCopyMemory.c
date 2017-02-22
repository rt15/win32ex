#include <RtWin32Ex.h>

#include "ZzResources.h"
#include "ZzUtils.h"

void* RT_CALL ZzCopyMemory(void* lpSource, void* lpDestination, RT_UN unSize)
{
  return memcpy(lpDestination, lpSource, unSize);
}

RT_B RT_CALL ZzTestCopyMemory()
{
  RT_CHRONO rtChrono;
  RT_UCHAR8 lpSource[ZZ_BUFFER_SIZE];
  RT_UCHAR8 lpDestination[ZZ_BUFFER_SIZE];
  RT_N unI;
  RT_B bResult;

  /* Initialize source buffer. */
  for (unI = 0; unI < ZZ_BUFFER_SIZE; unI++)
  {
    lpSource[unI] = (RT_UCHAR8)(unI % 200);
  }

  /* Test memcpy. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* VC is inlining code based on ZZ_BUFFER_SIZE. */
    memcpy(lpDestination, lpSource, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("memcpy"), &rtChrono)) goto handle_error;

  /* Test memcpy in function. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* Avoid inlining and size prediction, should force CRT usage. */
    ZzCopyMemory(lpSource, lpDestination, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("memcpy in function"), &rtChrono)) goto handle_error;

  /* Test RtCopyMemory. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    RtCopyMemory(lpSource, lpDestination, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("RtCopyMemory"), &rtChrono)) goto handle_error;

  /* Test RT_MEMORY_COPY. */
  if (!ZzStartChrono(&rtChrono)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* Intrinsic should be used, CRT otherwise. */
    RT_MEMORY_COPY(lpSource, lpDestination, ZZ_BUFFER_SIZE);
  }
  if (!ZzStopChrono(_R("RT_MEMORY_COPY"), &rtChrono)) goto handle_error;

  /* Ensure that lpDestination is used. */
  if (RT_MEMORY_COMPARE(lpSource, lpDestination, ZZ_BUFFER_SIZE)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
