#include "layer002/RtBinarySearch.h"

RT_B RT_API RtBinarySearch_SearchIndex(void* lpArea, void* lpItem, RT_UN unSize, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext, RT_UN* lpItemIndex)
{
  RT_CHAR8* lpAreaChars;
  RT_UN unLowerBound;
  RT_UN unUpperBound;
  RT_UN unMiddle;
  RT_N nComparisonResult;
  RT_B bResult;

  *lpItemIndex = RT_TYPE_MAX_UN;
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

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_UN RT_API RtBinarySearch_SearchInsertionIndex(void* lpArea, void* lpItem, RT_UN unSize, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext)
{
  RT_CHAR8* lpAreaChars;
  RT_UN unLowerBound;
  RT_UN unUpperBound;
  RT_UN unMiddle;
  RT_N nComparisonResult;
  RT_UN unResult;

  if (!unSize)
  {
    /* No item yet, insert at first index. */
    unResult = 0;
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
    unResult = unUpperBound + 1;
  }
  else
  {
    unResult = unUpperBound;
  }

free_resources:
  return unResult;

handle_error:
  unResult = RT_TYPE_MAX_UN;
  goto free_resources;
}
