#include "layer004/RtBinarySearch.h"

RT_B RT_API RtBinarySearchIndex(void* lpArea, void* lpItem, RT_UN32 unSize, RT_UN32 unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext, RT_N* lpItemIndex)
{
  RT_CHAR8* lpAreaChars;
  RT_UN unLowerBound;
  RT_UN unUpperBound;
  RT_UN unMiddle;
  RT_N nComparisonResult;
  RT_B bResult;

  *lpItemIndex = -1;
  if (unSize)
  {
    lpAreaChars = lpArea;

    unLowerBound = 0;
    unUpperBound = unSize - 1;

    while (unLowerBound < unUpperBound)
    {
      /* Code must guarantee the interval is reduced at each iteration. */
      unMiddle = unLowerBound + (unUpperBound - unLowerBound) / 2;

      if (!lpComparisonCallback(&lpAreaChars[unMiddle * unItemSize], lpItem, lpContext, &nComparisonResult)) goto handle_error;
      if (nComparisonResult < 0)
      {
        unLowerBound = unMiddle + 1;
      }
      else
      {
        unUpperBound = unMiddle;
      }
    }

    /* At this point unLowerBound should be equal to unUpperBound. */

    /* Deferred test for equality. */
    if (!lpComparisonCallback(&lpAreaChars[unLowerBound * unItemSize], lpItem, lpContext, &nComparisonResult)) goto handle_error;
    if (!nComparisonResult)
    {
      *lpItemIndex = unLowerBound;
    }
  }

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FALSE;
  goto free_resources;
}

RT_N RT_API RtBinarySearchInsertionIndex(void* lpArea, void* lpItem, RT_UN32 unSize, RT_UN32 unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext)
{
  RT_CHAR8* lpAreaChars;
  RT_UN unLowerBound;
  RT_UN unUpperBound;
  RT_UN unMiddle;
  RT_N nComparisonResult;
  RT_N nResult;

  if (!unSize)
  {
    /* No item yet, insert at first index. */
    nResult = 0;
    goto free_resources;
  }

  lpAreaChars = lpArea;

  unLowerBound = 0;
  unUpperBound = unSize - 1;

  while (unLowerBound < unUpperBound)
  {
    /* Code must guarantee the interval is reduced at each iteration. */
    unMiddle = unLowerBound + (unUpperBound - unLowerBound) / 2;

    if (!lpComparisonCallback(&lpAreaChars[unMiddle * unItemSize], lpItem, lpContext, &nComparisonResult)) goto handle_error;
    if (nComparisonResult < 0)
    {
      unLowerBound = unMiddle + 1;
    }
    else
    {
      unUpperBound = unMiddle;
    }
  }
  if (!lpComparisonCallback(&lpAreaChars[unUpperBound * unItemSize], lpItem, lpContext, &nComparisonResult)) goto handle_error;
  if (nComparisonResult < 0)
  {
    nResult = unUpperBound + 1;
  }
  else
  {
    nResult = unUpperBound;
  }

free_resources:
  return nResult;

handle_error:
  nResult = -1;
  goto free_resources;
}
