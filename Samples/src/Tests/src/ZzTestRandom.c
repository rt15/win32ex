#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestGetRandomNumberWithBoundaries(RT_N nLowerBound, RT_N nUpperBound)
{
  RT_N nNumber;
  RT_N nI;
  RT_B bResult;

  for (nI = 0; nI < 2000; nI++)
  {
    if (!RtGetRandomNumberWithBoundaries(nLowerBound, nUpperBound, &nNumber)) goto handle_error;
    if (nNumber < nLowerBound || nNumber > nUpperBound)
    {
      RtSetLastError(RT_ERROR_FUNCTION_FAILED);
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

  if (!ZzTestGetRandomNumberWithBoundaries(-1, 0)) goto handle_error;
  if (!ZzTestGetRandomNumberWithBoundaries(-1, -1)) goto handle_error;
  if (!ZzTestGetRandomNumberWithBoundaries(-1, 0)) goto handle_error;
  if (!ZzTestGetRandomNumberWithBoundaries(-3, 3)) goto handle_error;
  if (!ZzTestGetRandomNumberWithBoundaries(12, 15)) goto handle_error;
  if (!ZzTestGetRandomNumberWithBoundaries(-1000000000, 1000000000)) goto handle_error;
  if (!ZzTestGetRandomNumberWithBoundaries(-2000000000, 2000000000)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}