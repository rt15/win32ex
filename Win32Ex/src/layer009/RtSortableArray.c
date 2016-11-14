#include "layer009/RtSortableArray.h"

#include "layer003/RtMemory.h"
#include "layer004/RtBinarySearch.h"
#include "layer004/RtQuickSort.h"

void* RT_API RtCreateSortableArray(void** lpSortableArray, RT_HEAP** lpHeap, RT_UN32 unSize, RT_UN32 unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext)
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

RT_N RT_API RtAddItemToSortableArray(void** lpSortableArray, void* lpNewItem)
{
  RT_SORTABLE_ARRAY_HEADER* lpHeader;
  RT_N nSize;
  RT_N nItemSize;
  RT_N nLastItemIndex;
  void* lpSource;
  void* lpDestination;
  RT_N nResult;

  /* Add more space at the end of the array. */
  if (RtNewArrayItemIndex(lpSortableArray, &nLastItemIndex) == -1)
  {
    nResult = -1;
    goto the_end;
  }

  lpHeader = (RT_SORTABLE_ARRAY_HEADER*)*lpSortableArray;
  lpHeader--;
  nSize = lpHeader->rtArrayHeader.unSize;
  nItemSize = lpHeader->rtArrayHeader.unItemSize;

  /* Is there only the new item in the array? */
  if (nSize == 1)
  {
    /* We need to Copy the only item and return 0 as insertion index. */
    lpSource = *lpSortableArray;
    nResult = 0;
  }
  else
  {
    /* Search insertion point. */
    nResult = RtBinarySearchInsertionIndex(*lpSortableArray, lpNewItem, nSize - 1, nItemSize, lpHeader->lpComparisonCallback, lpHeader->lpContext);
    if (nResult == -1)
    {
      /* Drop create empty space. */
      RtSetArraySize(lpSortableArray, nSize - 1);
      goto the_end;
    }

    /* Compute insertion address. */
    lpSource = ((RT_UCHAR8*)*lpSortableArray) + nItemSize * nResult;

    /* If an hole is necessary. */
    if (nResult < nSize - 1)
    {
      /* Create an hole at insertion point. */
      lpDestination = ((RT_UCHAR8*)*lpSortableArray) + nItemSize * (nResult + 1);
      RtMoveMemory(lpSource, lpDestination, (nSize - (nResult + 1)) * nItemSize);
    }
  }
  /* Fill the hole. */
  RtCopyMemory(lpNewItem, lpSource, nItemSize);

the_end:
  return nResult;
}

void* RT_API RtDeleteSortableArrayItemIndex(void** lpSortableArray, RT_UN32 unItemIndex)
{
  RT_SORTABLE_ARRAY_HEADER* lpHeader;
  RT_UN32 unSize;
  RT_UN32 unItemSize;
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

RT_B RT_API RtSearchSortableArrayItemIndex(void* lpSortableArray, void* lpItem, RT_N* lpItemIndex)
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
  *lpItemIndex = -1;
  bResult = RT_FALSE;
free_resource:
  return bResult;
}
