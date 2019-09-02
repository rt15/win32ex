#include "ZzTests.h"

/**
 * <tt>lpArea1</tt> should be the same as <tt>lpArea2</tt>.
 */
RT_B RT_CALL ZzTestCompareSameMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_B bResult;

  if (RtMemory_Compare(lpArea1, lpArea2, unSize)) goto handle_error;
  if (RtMemory_Compare(lpArea2, lpArea1, unSize)) goto handle_error;

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

  if (RtMemory_Compare(lpArea1, lpArea2, unSize) <= 0) goto handle_error;
  if (RtMemory_Compare(lpArea2, lpArea1, unSize) >= 0) goto handle_error;

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

  lpReturnedValue = RtMemory_Copy(lpSource, lpBuffer1, unSize);
  if (lpReturnedValue != lpBuffer1) goto handle_error;
  if (RT_MEMORY_COMPARE(lpSource, lpBuffer1, unSize)) goto handle_error;

  lpReturnedValue = RT_MEMORY_COPY(lpSource, lpBuffer2, unSize);
  if (lpReturnedValue != lpBuffer2) goto handle_error;
  if (RT_MEMORY_COMPARE(lpSource, lpBuffer2, unSize)) goto handle_error;

  lpReturnedValue = RtMemory_Move(lpSource, lpBuffer3, unSize);
  if (lpReturnedValue != lpBuffer3) goto handle_error;
  if (RT_MEMORY_COMPARE(lpSource, lpBuffer3, unSize)) goto handle_error;

  lpReturnedValue = RT_MEMORY_MOVE(lpSource, lpBuffer4, unSize);
  if (lpReturnedValue != lpBuffer4) goto handle_error;
  if (RT_MEMORY_COMPARE(lpSource, lpBuffer4, unSize)) goto handle_error;

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
      RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
      goto handle_error;
    }
    RT_MEMORY_COPY(lpInput, lpBuffer, unSize);

    if (unTest)
    {
      lpResult = RtMemory_Move(lpBuffer, &lpBuffer[unDestinationIndex], unSourceSize);
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

RT_B RT_CALL ZzCheckSetMemory(RT_UCHAR8* lpBuffer, RT_UN unSize, RT_UCHAR8 unExpected, RT_UCHAR8 unTrailing)
{
  RT_UN unI;
  RT_B bResult;

  for (unI = 0; unI < unSize; unI++)
  {
    if (lpBuffer[unI] != unExpected) goto handle_error;
  }
  if (lpBuffer[unSize] != unTrailing) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestSetMemory(RT_UN unSize)
{
  RT_UCHAR8 lpBuffer[32];
  void* lpResult;
  RT_UN unI;
  RT_B bResult;

  for (unI = 0; unI <= unSize; unI++) lpBuffer[unI] = 85;
  lpResult = RtMemory_Set(lpBuffer, 170, unSize);
  if (lpResult != lpBuffer) goto handle_error;
  if (!ZzCheckSetMemory(lpBuffer, unSize, 170, 85)) goto handle_error;

  for (unI = 0; unI <= unSize; unI++) lpBuffer[unI] = 85;
  lpResult = RT_MEMORY_SET(lpBuffer, 170, unSize);
  if (lpResult != lpBuffer) goto handle_error;
  if (!ZzCheckSetMemory(lpBuffer, unSize, 170, 85)) goto handle_error;

  for (unI = 0; unI <= unSize; unI++) lpBuffer[unI] = 85;
  lpResult = RtMemory_Zero(lpBuffer, unSize);
  if (lpResult != lpBuffer) goto handle_error;
  if (!ZzCheckSetMemory(lpBuffer, unSize, 0, 85)) goto handle_error;

  for (unI = 0; unI <= unSize; unI++) lpBuffer[unI] = 85;
  lpResult = RT_MEMORY_ZERO(lpBuffer, unSize);
  if (lpResult != lpBuffer) goto handle_error;
  if (!ZzCheckSetMemory(lpBuffer, unSize, 0, 85)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSetMemory()
{
  RT_UN unI;
  RT_B bResult;

  for (unI = 1; unI < 28; unI++)
  {
    if (!ZzDoTestSetMemory(unI)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzDoTestSwapMemory(RT_UN unSize)
{
  RT_UCHAR8 lpArea1[32];
  RT_UCHAR8 lpArea2[32];
  RT_UCHAR8 unI;
  RT_B bResult;

  for (unI = 0; unI < unSize; unI++)
  {
    lpArea1[unI] = 'a' + unI;
    lpArea2[unI] = 'A' + unI;
  }
  lpArea1[unSize] = 1;
  lpArea2[unSize] = 2;

  RtMemory_Swap(lpArea1, lpArea2, unSize);
  for (unI = 0; unI < unSize; unI++)
  {
    if (lpArea1[unI] != 'A' + unI) goto handle_error;
    if (lpArea2[unI] != 'a' + unI) goto handle_error;
  }
  if (lpArea1[unSize] != 1) goto handle_error;
  if (lpArea2[unSize] != 2) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSwapMemory()
{
  RT_UN unI;
  RT_B bResult;

  for (unI = 0; unI < 27; unI++)
  {
    if (!ZzDoTestSwapMemory(unI)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestGetChunksCount()
{
  RT_B bResult;

  if (RtMemory_GetChunksCount(5, 5) != 1)              goto handle_error;
  if (RtMemory_GetChunksCount(6, 5) != 2)              goto handle_error;
  if (RtMemory_GetChunksCount(1, 1) != 1)              goto handle_error;
  if (RtMemory_GetChunksCount(5, 6) != 1)              goto handle_error;
  if (RtMemory_GetChunksCount(0, 1) != 0)              goto handle_error;
  if (RtMemory_GetChunksCount(1, 0) != RT_TYPE_MAX_UN) goto handle_error;

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
  if (!ZzTestSetMemory()) goto handle_error;
  if (!ZzTestSwapMemory()) goto handle_error;
  if (!ZzTestGetChunksCount()) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
