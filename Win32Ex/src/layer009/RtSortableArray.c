#include "layer009/RtSortableArray.h"

#include "layer003/RtMemory.h"
#include "layer004/RtBinarySearch.h"
#include "layer004/RtQuickSort.h"

void* RT_API RtCreateSortableArray(void** lpSortableArray, RT_HEAP** lpHeap, RT_UN unSize, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext)
{
  RT_SORTABLE_ARRAY_HEADER* lpHeader;

  if (RtCreateArray(lpSortableArray, sizeof(RT_SORTABLE_ARRAY_HEADER), lpHeap, unSize, unItemSize))
  {
    lpHeader = *lpSortableArray;
    lpHeader--;
    lpHeader->bSorted = RT_FALSE;
    lpHeader->lpComparisonCallback = lpComparisonCallback;
    lpHeader->lpContext = lpContext;
  }
  return *lpSortableArray;
}

RT_UN RT_API RtAddItemToSortableArray(void** lpSortableArray, void* lpNewItem)
{
  RT_SORTABLE_ARRAY_HEADER* lpHeader;
  RT_UN unSize;
  RT_UN unItemSize;
  RT_UN unLastItemIndex;
  void* lpSource;
  void* lpDestination;
  RT_UN unResult;

  /* Add more space at the end of the array. */
  if (RtNewArrayItemIndex(lpSortableArray, &unLastItemIndex) == RT_TYPE_MAX_UN)
  {
    unResult = RT_TYPE_MAX_UN;
    goto the_end;
  }

  lpHeader = (RT_SORTABLE_ARRAY_HEADER*)*lpSortableArray;
  lpHeader--;
  unSize = lpHeader->rtArrayHeader.unSize;
  unItemSize = lpHeader->rtArrayHeader.unItemSize;

  /* Is there only the new item in the array? */
  if (unSize == 1)
  {
    /* We need to Copy the only item and return 0 as insertion index. */
    lpSource = *lpSortableArray;
    unResult = 0;
  }
  else
  {
    /* Search insertion point. */
    unResult = RtBinarySearchInsertionIndex(*lpSortableArray, lpNewItem, unSize - 1, unItemSize, lpHeader->lpComparisonCallback, lpHeader->lpContext);
    if (unResult == RT_TYPE_MAX_UN)
    {
      /* Drop create empty space. */
      RtSetArraySize(lpSortableArray, unSize - 1);
      goto the_end;
    }

    /* Compute insertion address. */
    lpSource = ((RT_UCHAR8*)*lpSortableArray) + unItemSize * unResult;

    /* If an hole is necessary. */
    if (unResult < unSize - 1)
    {
      /* Create an hole at insertion point. */
      lpDestination = ((RT_UCHAR8*)*lpSortableArray) + unItemSize * (unResult + 1);
      RtMoveMemory(lpSource, lpDestination, (unSize - (unResult + 1)) * unItemSize);
    }
  }
  /* Fill the hole. */
  RtCopyMemory(lpNewItem, lpSource, unItemSize);

the_end:
  return unResult;
}

void* RT_API RtDeleteSortableArrayItemIndex(void** lpSortableArray, RT_UN unItemIndex)
{
  RT_SORTABLE_ARRAY_HEADER* lpHeader;
  RT_UN unSize;
  RT_UN unItemSize;
  void* lpSource;
  void* lpDestination;

  lpHeader = *lpSortableArray;
  lpHeader--;

  unSize = lpHeader->rtArrayHeader.unSize;

  /* If it is not the last item of the array. */
  if (unItemIndex < unSize - 1)
  {
    /* Overwrite the item to delete with the end of the array. */
    unItemSize = lpHeader->rtArrayHeader.unItemSize;
    lpSource = ((RT_UCHAR8*)*lpSortableArray) + unItemSize * (unItemIndex + 1);
    lpDestination = ((RT_UCHAR8*)*lpSortableArray) + unItemSize * unItemIndex;
    RtMoveMemory(lpSource, lpDestination, unItemSize * (unSize - (unItemIndex + 1)));
  }

  /* Truncate the array. */
  RtSetArraySize(lpSortableArray, unSize - 1);

  return *lpSortableArray;
}

RT_B RT_API RtSortSortableArray(void* lpSortableArray)
{
  RT_SORTABLE_ARRAY_HEADER* lpHeader;
  RT_B bResult;

  lpHeader = lpSortableArray;
  lpHeader--;

  if (lpHeader->bSorted)
  {
    bResult = RT_TRUE;
  }
  else
  {
    bResult = RtQuickSort(lpSortableArray, lpHeader->rtArrayHeader.unSize, lpHeader->rtArrayHeader.unItemSize, lpHeader->lpComparisonCallback, lpHeader->lpContext);
    if (bResult)
    {
      lpHeader->bSorted = RT_TRUE;
    }
  }

  return bResult;
}

RT_B RT_API RtUnsortSortableArray(void* lpSortableArray)
{
  RT_SORTABLE_ARRAY_HEADER* lpHeader;

  lpHeader = lpSortableArray;
  lpHeader--;

  lpHeader->bSorted = RT_FALSE;

  return RT_TRUE;
}

RT_B RT_API RtSearchSortableArrayItemIndex(void* lpSortableArray, void* lpItem, RT_UN* lpItemIndex)
{
  RT_SORTABLE_ARRAY_HEADER* lpHeader;
  RT_B bResult;

  lpHeader = lpSortableArray;
  lpHeader--;

  if (!lpHeader->bSorted)
  {
    if (!RtSortSortableArray(lpSortableArray)) goto handle_error;
  }

  if (!RtBinarySearchIndex(lpSortableArray, lpItem, lpHeader->rtArrayHeader.unSize, lpHeader->rtArrayHeader.unItemSize, lpHeader->lpComparisonCallback, lpHeader->lpContext, lpItemIndex)) goto handle_error;

  bResult = RT_TRUE;
  goto free_resource;
handle_error:
  *lpItemIndex = RT_TYPE_MAX_UN;
  bResult = RT_FALSE;
free_resource:
  return bResult;
}
