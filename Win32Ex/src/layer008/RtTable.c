#include "layer008/RtTable.h"

#include "layer003/RtMemory.h"
#include "layer008/RtArray.h"
#include "layer009/RtSortableArray.h"

RT_B RT_API RtCreateTable(RT_TABLE* lpTable, RT_TABLE_METADATA* lpTableMetadata, RT_HEAP** lpHeap)
{
  RT_UN unTableIndexesCount;
  RT_UN unI;
  RT_B bResult;

  bResult = RT_FALSE;

  lpTable->lpTableMetadata = lpTableMetadata;

  if (!RtCreateArray(&lpTable->lpTableData, 0, lpHeap, 0, lpTableMetadata->unItemSize))
  {
    goto the_end;
  }
  unTableIndexesCount = lpTableMetadata->unTableIndexesCount;
  if (unTableIndexesCount <= 0)
  {
    lpTable->lpTableIndexes = RT_NULL;
  }
  else
  {
    if (!RtCreateArray((void**)&lpTable->lpTableIndexes, 0, lpHeap, unTableIndexesCount, sizeof(RT_TABLE_INDEX)))
    {
      goto free_table_data;
    }
    /* Init table indexes to RT_NULL if we need to cleanup. */
    for (unI = 0; unI < unTableIndexesCount; unI++)
    {
      lpTable->lpTableIndexes[unI].lpIndexes = RT_NULL;
    }

    /* Create table indexes. */
    for (unI = 0; unI < unTableIndexesCount; unI++)
    {
      if (!RtCreateSortableArray((void**)&lpTable->lpTableIndexes[unI].lpIndexes, lpHeap, 0, sizeof(RT_UN), lpTableMetadata->lpComparisonCallbacks[unI], lpTable))
      {
        goto cleanup_table_indexes;
      }
      lpTable->lpTableIndexes[unI].bEnabled = RT_TRUE;
    }
  }

  bResult = RT_TRUE;
  goto the_end;
cleanup_table_indexes:
  for (unI = 0; unI < unTableIndexesCount; unI++)
  {
    RtFreeArray((void**)&lpTable->lpTableIndexes[unI].lpIndexes);
  }
  RtFreeArray((void**)&lpTable->lpTableIndexes);
free_table_data:
  RtFreeArray(&lpTable->lpTableData);
the_end:
  return bResult;
}

RT_B RT_API RtIndexNewTableItem(RT_TABLE* lpTable)
{
  RT_UN unTableIndexesCount;
  RT_TABLE_INDEX* lpTableIndex;
  RT_UN unItemSize;
  RT_UN unItemUserDataSize;
  RT_UN unSize;
  RT_UN unNewItemIndex;
  void* lpNewTableItem;
  RT_UN unIndex;
  RT_UN* lpIndexReference;
  RT_UN unInsertionIndex;
  RT_UN unI, unJ;
  RT_B bResult;

  unSize = RtGetArraySize(lpTable->lpTableData);
  unItemSize = lpTable->lpTableMetadata->unItemSize;
  unItemUserDataSize = lpTable->lpTableMetadata->unItemUserDataSize;

  /* Pointer on the added item. */
  unNewItemIndex = unSize - 1;
  lpNewTableItem = ((RT_UCHAR8*)lpTable->lpTableData) + unItemSize * unNewItemIndex;

  unTableIndexesCount = lpTable->lpTableMetadata->unTableIndexesCount;
  for (unI = 0; unI < unTableIndexesCount; unI++)
  {
    lpTableIndex = &lpTable->lpTableIndexes[unI];
    if (lpTableIndex->bEnabled)
    {
      /* Add the new item index to the table index and retrieve insertion point. */
      unInsertionIndex = RtAddItemToSortableArray((void**)&lpTableIndex->lpIndexes, &unNewItemIndex);
      if (unInsertionIndex == RT_TYPE_MAX_UN)
      {
        bResult = RT_FALSE;
        goto the_end;
      }

      /* Increment the references of the indexes after the insertion point. */
      for (unJ = unInsertionIndex + 1; unJ < unSize; unJ++)
      {
        unIndex = lpTableIndex->lpIndexes[unJ];

        lpIndexReference = (RT_UN*)(((RT_UCHAR8*)lpTable->lpTableData) + unItemSize * unIndex + unItemUserDataSize + unI * sizeof (RT_UN));
        (*lpIndexReference)++;
      }

      /* Update the reference of the added item. */
      lpIndexReference = (RT_UN*)(((RT_UCHAR8*)lpNewTableItem) + unItemUserDataSize + unI * sizeof (RT_UN));
      *lpIndexReference = unInsertionIndex;
    }
  }

  bResult = RT_TRUE;
the_end:
  return bResult;
}

RT_B RT_API RtDeleteTableItem(RT_TABLE* lpTable, RT_UN unItemIndex)
{
  RT_UN unSize;
  RT_UN unItemSize;
  RT_UN unItemUserDataSize;
  RT_UN unTableIndexesCount;
  RT_UN* lpIndexReference;
  RT_UN unIndexInTableIndex;
  RT_TABLE_INDEX* lpTableIndex;
  RT_UN unI, unJ;
  RT_B bResult;

  unSize = RtGetArraySize(lpTable->lpTableData);
  unItemSize = lpTable->lpTableMetadata->unItemSize;
  unItemUserDataSize = lpTable->lpTableMetadata->unItemUserDataSize;

  unTableIndexesCount = lpTable->lpTableMetadata->unTableIndexesCount;
  for (unI = 0; unI < unTableIndexesCount; unI++)
  {
    lpTableIndex = &lpTable->lpTableIndexes[unI];
    if (lpTableIndex->bEnabled)
    {
      /* Find the index in the table index. */
      lpIndexReference = (RT_UN*)(((RT_UCHAR8*)lpTable->lpTableData) + unItemIndex * unItemSize + unItemUserDataSize + unI * sizeof (RT_UN));
      unIndexInTableIndex = *lpIndexReference;

      if (!RtDeleteSortableArrayItemIndex((void**)&lpTableIndex->lpIndexes, unIndexInTableIndex))
      {
        bResult = RT_FALSE;
        goto the_end;
      }

      /* Update index references. As an index has been removed, all following indexes references must be decremented. */
      for (unJ = unIndexInTableIndex; unJ < unSize - 1; unJ++)
      {
        lpIndexReference = (RT_UN*)(((RT_UCHAR8*)lpTable->lpTableData) + lpTableIndex->lpIndexes[unJ] * unItemSize + unItemUserDataSize + unI * sizeof (RT_UN));
        (*lpIndexReference)--;
      }

      /* Delete the data item by copying the last item to the item to delete. */
      if (!RtDeleteArrayItemIndex((void**)&lpTable->lpTableData, unItemIndex))
      {
        bResult = RT_FALSE;
        goto the_end;
      }

      /* If the deleted item was not the last one. */
      if (unItemIndex != unSize - 1)
      {
        /* Update the index because the last data item has been moved. */
        lpIndexReference = (RT_UN*)(((RT_UCHAR8*)lpTable->lpTableData) + unItemIndex * unItemSize + unItemUserDataSize + unI * sizeof (RT_UN));
        unIndexInTableIndex = *lpIndexReference;
        lpTableIndex->lpIndexes[unIndexInTableIndex] = unItemIndex;
      }
    }
  }

  bResult = RT_TRUE;
the_end:
  return bResult;
}

RT_B RT_API RtFreeTable(RT_TABLE* lpTable)
{
  RT_UN unTableIndexesCount;
  RT_UN unI;
  RT_B bResult;

  bResult = RT_TRUE;
  if (lpTable)
  {
    unTableIndexesCount = lpTable->lpTableMetadata->unTableIndexesCount;

    for (unI = 0; unI < unTableIndexesCount; unI++)
    {
      bResult = bResult && RtFreeArray((void**)&lpTable->lpTableIndexes[unI].lpIndexes);
    }
    bResult = bResult && RtFreeArray((void**)&lpTable->lpTableIndexes);
    bResult = bResult && RtFreeArray(&lpTable->lpTableData);
  }

  return bResult;
}
