#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestGetRandomIntegerWithBoundaries(RT_N nLowerBound, RT_N nUpperBound, RT_B bCheckBoundaries)
{
  RT_N nInteger;
  RT_UN unI;
  RT_B bResult;

  if (bCheckBoundaries)
  {
    /* Check that lower bound is hit. */
    while (RT_TRUE)
    {
      if (!RtRandom_GetIntegerWithBoundaries(nLowerBound, nUpperBound, &nInteger)) goto handle_error;
      if (nInteger == nLowerBound) break;
    }
    /* Check that upper bound is hit. */
    while (RT_TRUE)
    {
      if (!RtRandom_GetIntegerWithBoundaries(nLowerBound, nUpperBound, &nInteger)) goto handle_error;
      if (nInteger == nUpperBound) break;
    }
  }

  for (unI = 0; unI < 2000; unI++)
  {
    if (!RtRandom_GetIntegerWithBoundaries(nLowerBound, nUpperBound, &nInteger)) goto handle_error;
    if (nInteger < nLowerBound || nInteger > nUpperBound)
    {
      RtError_SetLast(RT_ERROR_FUNCTION_FAILED);
      goto handle_error;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestRandom()
{
  RT_B bResult;

  if (!ZzTestGetRandomIntegerWithBoundaries(-1, 0, RT_TRUE)) goto handle_error;
  if (!ZzTestGetRandomIntegerWithBoundaries(-1, -1, RT_TRUE)) goto handle_error;
  if (!ZzTestGetRandomIntegerWithBoundaries(-1, 0, RT_TRUE)) goto handle_error;
  if (!ZzTestGetRandomIntegerWithBoundaries(-3, 3, RT_TRUE)) goto handle_error;
  if (!ZzTestGetRandomIntegerWithBoundaries(12, 15, RT_TRUE)) goto handle_error;
  if (!ZzTestGetRandomIntegerWithBoundaries(-1000000000, 1000000000, RT_FALSE)) goto handle_error;
  if (!ZzTestGetRandomIntegerWithBoundaries(-2000000000, 2000000000, RT_FALSE)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
