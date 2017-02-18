#include <RtWin32Ex.h>

#include "ZzResources.h"
#include "ZzUtils.h"

void* RT_CALL ZzMoveMemory(void* lpSource, void* lpDestination, RT_UN unSize)
{
  return memmove(lpDestination, lpSource, unSize);
}

/**
 * Initialize source buffer (abcc).
 */
void RT_CALL ZzInitBuffer(RT_UCHAR8* lpBuffer)
{
  RT_UN unI;

  for (unI = 0; unI < ZZ_BUFFER_SIZE / 4; unI++)
    lpBuffer[unI] = 'a';
  for (unI = ZZ_BUFFER_SIZE / 4; unI < ZZ_BUFFER_SIZE / 2; unI++)
    lpBuffer[unI] = 'b';
  for (unI = ZZ_BUFFER_SIZE / 2; unI < ZZ_BUFFER_SIZE; unI++)
    lpBuffer[unI] = 'c';
}

/**
 * Check result buffer (abcc -> aabc, then aabc -> aaac as we call move in a loop).
 */
RT_B RT_CALL ZzCheckBuffer(RT_UCHAR8* lpBuffer)
{
  RT_UN unI;
  RT_B bResult;

  for (unI = 0; unI < ZZ_BUFFER_SIZE * 3 / 4; unI++)
    if (lpBuffer[unI] != 'a') goto handle_error;
  for (unI = ZZ_BUFFER_SIZE * 3 / 4; unI < ZZ_BUFFER_SIZE; unI++)
    if (lpBuffer[unI] != 'c') goto handle_error;
    
  bResult = RT_SUCCESS;
free_resources:
  return bResult;
  
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestMoveMemory()
{
  LARGE_INTEGER rtStartCounter;
  RT_UCHAR8 lpSource[ZZ_BUFFER_SIZE];
  RT_UCHAR8* lpDestination;
  RT_N unI;
  RT_B bResult;

  /* We will copy ab on bc. */
  lpDestination = &lpSource[ZZ_BUFFER_SIZE / 4];

  /* Test memmove. */
  ZzInitBuffer(lpSource);
  if (!ZzStartChrono(&rtStartCounter)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* VC is inlining code based on ZZ_BUFFER_SIZE. */
    memmove(lpDestination, lpSource, ZZ_BUFFER_SIZE / 2);
  }
  if (!ZzStopChrono(_R("memmove"), &rtStartCounter)) goto handle_error;
  if (!ZzCheckBuffer(lpSource)) goto handle_error;

  /* Test memmove in function. */
  ZzInitBuffer(lpSource);
  if (!ZzStartChrono(&rtStartCounter)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* Avoid inlining and size prediction, should force CRT usage. */
    ZzMoveMemory(lpSource, lpDestination, ZZ_BUFFER_SIZE / 2);
  }
  if (!ZzStopChrono(_R("memmove in function"), &rtStartCounter)) goto handle_error;
  if (!ZzCheckBuffer(lpSource)) goto handle_error;

  /* Test RtMoveMemory. */
  ZzInitBuffer(lpSource);
  if (!ZzStartChrono(&rtStartCounter)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    RtMoveMemory(lpSource, lpDestination, ZZ_BUFFER_SIZE / 2);
  }
  if (!ZzStopChrono(_R("RtMoveMemory"), &rtStartCounter)) goto handle_error;
  if (!ZzCheckBuffer(lpSource)) goto handle_error;

  /* Test RT_MEMORY_MOVE. */
  ZzInitBuffer(lpSource);
  if (!ZzStartChrono(&rtStartCounter)) goto handle_error;
  for (unI = 0; unI < ZZ_TESTS_COUNT; unI++)
  {
    /* Intrinsic should be used, CRT otherwise. */
    RT_MEMORY_MOVE(lpSource, lpDestination, ZZ_BUFFER_SIZE / 2);
  }
  if (!ZzStopChrono(_R("RT_MEMORY_MOVE"), &rtStartCounter)) goto handle_error;
  if (!ZzCheckBuffer(lpSource)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
