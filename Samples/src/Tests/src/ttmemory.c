#include <RtWin32Ex.h>

/**
 * <tt>lpArea1</tt> should be the same as <tt>lpArea2</tt>.
 */
RT_B RT_CALL ZzTestCompareSameMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_B bResult;

  if (RtCompareMemory(lpArea1, lpArea2, unSize)) goto handle_error;
  if (RtCompareMemory(lpArea2, lpArea1, unSize)) goto handle_error;

  if (RT_MEMORY_COMPARE(lpArea1, lpArea2, unSize)) goto handle_error;
  if (RT_MEMORY_COMPARE(lpArea2, lpArea1, unSize)) goto handle_error;

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

  if (RT_MEMORY_COMPARE(lpArea1, lpArea2, unSize) <= 0) goto handle_error;
  if (RT_MEMORY_COMPARE(lpArea2, lpArea1, unSize) >= 0) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCompareMemory()
{
  RT_CHAR8 lpArea1[32];
  RT_CHAR8 lpArea2[32];
  RT_UN unI;
  RT_UN unJ;
  RT_B bResult;

  if (!ZzTestCompareSameMemory("bar", "bar", 3)) goto handle_error;
  if (!ZzTestCompareSameMemory("bar!", "bar!", 4)) goto handle_error;
  if (!ZzTestCompareSameMemory("foobar", "foobar", 6)) goto handle_error;

  if (!ZzTestCompareDifferentMemory("foo", "bar", 3)) goto handle_error;
  if (!ZzTestCompareDifferentMemory("fooo", "foob", 4)) goto handle_error;
  if (!ZzTestCompareDifferentMemory("foobar", "fooba!", 6)) goto handle_error;

  for (unI = 1; unI < 28; unI++)
  {
    for (unJ = 0; unJ < unI - 1; unJ++)
    {
      lpArea1[unJ] = (RT_CHAR8)('a' + unJ);
      lpArea2[unJ] = lpArea1[unJ];
    }
    lpArea1[unI - 1] = 'B';
    lpArea2[unI - 1] = 'A';
    lpArea1[unI] = 0;
    lpArea2[unI] = 0;
    if (!ZzTestCompareDifferentMemory(lpArea1, lpArea2, unI)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestCopyMemory(void* lpSource, RT_UN unSize)
{
  RT_UCHAR8 lpBuffer1[128];
  RT_UCHAR8 lpBuffer2[128];
  RT_UCHAR8 lpBuffer3[128];
  RT_UCHAR8 lpBuffer4[128];
  void* lpReturnedValue;
  RT_B bResult;

  lpReturnedValue = RtCopyMemory(lpSource, lpBuffer1, unSize);
  if (lpReturnedValue != lpBuffer1) goto handle_error;
  if (RtCompareMemory(lpSource, lpBuffer1, unSize)) goto handle_error;

  lpReturnedValue = RT_MEMORY_COPY(lpSource, lpBuffer2, unSize);
  if (lpReturnedValue != lpBuffer2) goto handle_error;
  if (RtCompareMemory(lpSource, lpBuffer2, unSize)) goto handle_error;

  lpReturnedValue = RtMoveMemory(lpSource, lpBuffer3, unSize);
  if (lpReturnedValue != lpBuffer3) goto handle_error;
  if (RtCompareMemory(lpSource, lpBuffer3, unSize)) goto handle_error;

  lpReturnedValue = RT_MEMORY_MOVE(lpSource, lpBuffer4, unSize);
  if (lpReturnedValue != lpBuffer4) goto handle_error;
  if (RtCompareMemory(lpSource, lpBuffer4, unSize)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCopyMemory()
{
  RT_B bResult;

  if (!ZzDoTestCopyMemory("foo", 3)) goto handle_error;
  if (!ZzDoTestCopyMemory("foob", 4)) goto handle_error;
  if (!ZzDoTestCopyMemory("foobar", 6)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestMoveMemory(RT_CHAR8* lpInput, RT_UN unSourceSize, RT_UN unDestinationIndex, RT_CHAR8* lpExpected, RT_UN unSize)
{
  RT_CHAR8 lpBuffer[RT_CHAR_THIRD_BIG_STRING_SIZE];
  void* lpResult;
  RT_UN unTest;
  RT_B bResult;

  for (unTest = 0; unTest < 2; unTest++)
  {
    /* Copy input into editable memory. */
    if (unSize > RT_CHAR_THIRD_BIG_STRING_SIZE)
    {
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
    RtCopyMemory(lpInput, lpBuffer, unSize);

    if (unTest)
    {
      lpResult = RtMoveMemory(lpBuffer, &lpBuffer[unDestinationIndex], unSourceSize);
    }
    else
    {
      lpResult = RT_MEMORY_MOVE(lpBuffer, &lpBuffer[unDestinationIndex], unSourceSize);
    }
    if (lpResult != &lpBuffer[unDestinationIndex]) goto handle_error;

    /* Check result against expected. */  
    if (RT_MEMORY_COMPARE(lpBuffer, lpExpected, unSize)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestMoveMemory()
{
  RT_B bResult;

  if (!ZzDoTestMoveMemory("123", 2, 1, "112", 3)) goto handle_error;
  if (!ZzDoTestMoveMemory("1234567890abcde", 10, 5, "123451234567890", 15)) goto handle_error;

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
  if (!ZzTestCopyMemory()) goto handle_error;
  if (!ZzTestMoveMemory()) goto handle_error;
  if (!ZzTestComputeChunksCount()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
