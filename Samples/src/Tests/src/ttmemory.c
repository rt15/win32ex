#include <RtWin32Ex.h>

/**
 * <tt>lpArea1</tt> should be the same as <tt>lpArea2</tt>.
 */
RT_B RT_CALL ZzTestCompareSameMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_B bResult;

  if (RtCompareMemory(lpArea1, lpArea2, unSize)) goto handle_error;
  if (RtCompareMemory(lpArea2, lpArea1, unSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

/**
 * <tt>lpArea1</tt> must be greater than <tt>lpArea2</tt>.
 */
RT_B RT_CALL ZzTestCompareDifferentMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_B bResult;

  if (RtCompareMemory(lpArea1, lpArea2, unSize) <= 0) goto handle_error;
  if (RtCompareMemory(lpArea2, lpArea1, unSize) >= 0) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCompareMemory()
{
  RT_B bResult;

  if (!ZzTestCompareSameMemory("bar", "bar", 3)) goto handle_error;

  if (!ZzTestCompareDifferentMemory("foo", "bar", 3)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestCopyMemory(void* lpSource, void* lpDestination, RT_UN unSize)
{
  void* lpReturnedValue;
  RT_B bResult;

  lpReturnedValue = RtCopyMemory(lpSource, lpDestination, unSize);
  if (lpReturnedValue != lpDestination) goto handle_error;
  if (RtCompareMemory(lpSource, lpDestination, unSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCopyMemory()
{
  RT_UCHAR8 lpBuffer[512];
  RT_B bResult;

  if (!ZzDoTestCopyMemory("foo", lpBuffer, 3)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestComputeChunksCount()
{
  RT_B bResult;

  if (RtComputeChunksCount(5, 5) != 1)              goto handle_error;
  if (RtComputeChunksCount(6, 5) != 2)              goto handle_error;
  if (RtComputeChunksCount(1, 1) != 1)              goto handle_error;
  if (RtComputeChunksCount(5, 6) != 1)              goto handle_error;
  if (RtComputeChunksCount(0, 1) != 0)              goto handle_error;
  if (RtComputeChunksCount(1, 0) != RT_TYPE_MAX_UN) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestMemory()
{
  RT_B bResult;

  if (!ZzTestCompareMemory()) goto handle_error;
  if (!ZzTestComputeChunksCount()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
