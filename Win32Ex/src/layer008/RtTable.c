#include "layer008/RtTable.h"

#include "layer003/RtMemory.h"
#include "layer008/RtArray.h"
#include "layer009/RtSortableArray.h"

RT_B RT_API RtCreateTable(RT_TABLE* lpTable, RT_TABLE_METADATA* lpTableMetadata, RT_HEAP** lpHeap)
{
  RT_N nTableIndexesCount;
  RT_N nI;
  RT_B bResult;

  bResult = RT_FALSE;

  lpTable->lpTableMetadata = lpTableMetadata;

  if (!RtCreateArray(&lpTable->lpTableData, 0, lpHeap, 0, lpTableMetadata->nItemSize))
  {
    goto the_end;
  }
  nTableIndexesCount = lpTableMetadata->nTableIndexesCount;
  if (nTableIndexesCount <= 0)
  {
    lpTable->lpTableIndexes = RT_NULL;
  }
  else
  {
    if (!RtCreateArray((void**)&lpTable->lpTableIndexes, 0, lpHeap, nTableIndexesCount, sizeof(RT_TABLE_INDEX)))
    {
      goto free_table_data;
    }
    /* Init table indexes to RT_NULL if we need to cleanup. */
    for (nI = 0; nI < nTableIndexesCount; nI++)
    {
      lpTable->lpTableIndexes[nI].lpIndexes = RT_NULL;
    }

    /* Create table indexes. */
    for (nI = 0; nI < nTableIndexesCount; nI++)
    {
      if (!RtCreateSortableArray((void**)&lpTable->lpTableIndexes[nI].lpIndexes, lpHeap, 0, sizeof(RT_N), lpTableMetadata->lpComparisonCallbacks[nI], lpTable))
      {
        goto cleanup_table_indexes;
      }
      lpTable->lpTableIndexes[nI].bEnabled = RT_TRUE;
    }
  }

  bResult = RT_TRUE;
  goto the_end;
cleanup_table_indexes:
  for (nI = 0; nI < nTableIndexesCount; nI++)
  {
    RtFreeArray((void**)&lpTable->lpTableIndexes[nI].lpIndexes);
  }
  RtFreeArray((void**)&lpTable->lpTableIndexes);
free_table_data:
  RtFreeArray(&lpTable->lpTableData);
the_end:
  return bResult;
}

RT_B RT_API RtIndexNewTableItem(RT_TABLE* lpTable)
{
  RT_N nTableIndexesCount;
  RT_TABLE_INDEX* lpTableIndex;
  RT_N nItemSize;
  RT_N nItemUserDataSize;
  RT_N nSize;
  RT_N nNewItemIndex;
  void* lpNewTableItem;
  RT_N nIndex;
  RT_N* lpIndexReference;
  RT_N nInsertionIndex;
  RT_N nI, nJ;
  RT_B bResult;

  nSize = RtGetArraySize(lpTable->lpTableData);
  nItemSize = lpTable->lpTableMetadata->nItemSize;
  nItemUserDataSize = lpTable->lpTableMetadata->nItemUserDataSize;

  /* Pointer on the added item. */
  nNewItemIndex = nSize - 1;
  lpNewTableItem = ((RT_UCHAR8*)lpTable->lpTableData) + nItemSize * nNewItemIndex;

  nTableIndexesCount = lpTable->lpTableMetadata->nTableIndexesCount;
  for (nI = 0; nI < nTableIndexesCount; nI++)
  {
    lpTableIndex = &lpTable->lpTableIndexes[nI];
    if (lpTableIndex->bEnabled)
    {
      /* Add the new item index to the table index and retrieve insertion point. */
      nInsertionIndex = RtAddItemToSortableArray((void**)&lpTableIndex->lpIndexes, &nNewItemIndex);
      if (nInsertionIndex == -1)
      {
        bResult = RT_FALSE;
        goto the_end;
      }

      /* Increment the references of the indexes after the insertion point. */
      for (nJ = nInsertionIndex + 1; nJ < nSize; nJ++)
      {
        nIndex = lpTableIndex->lpIndexes[nJ];

        lpIndexReference = (RT_N*)(((RT_UCHAR8*)lpTable->lpTableData) + nItemSize * nIndex + nItemUserDataSize + nI * sizeof (RT_N));
        (*lpIndexReference)++;
      }

      /* Update the reference of the added item. */
      lpIndexReference = (RT_N*)(((RT_UCHAR8*)lpNewTableItem) + nItemUserDataSize + nI * sizeof (RT_N));
      *lpIndexReference = nInsertionIndex;
    }
  }

  bResult = RT_TRUE;
the_end:
  return bResult;
}

RT_B RT_API RtDeleteTableItem(RT_TABLE* lpTable, RT_UN32 unItemIndex)
{
  RT_N nSize;
  RT_N nItemSize;
  RT_N nItemUserDataSize;
  RT_N nTableIndexesCount;
  RT_N* lpIndexReference;
  RT_N nIndexInTableIndex;
  RT_TABLE_INDEX* lpTableIndex;
  RT_N nI, nJ;
  RT_B bResult;

  nSize = RtGetArraySize(lpTable->lpTableData);
  nItemSize = lpTable->lpTableMetadata->nItemSize;
  nItemUserDataSize = lpTable->lpTableMetadata->nItemUserDataSize;

  nTableIndexesCount = lpTable->lpTableMetadata->nTableIndexesCount;
  for (nI = 0; nI < nTableIndexesCount; nI++)
  {
    lpTableIndex = &lpTable->lpTableIndexes[nI];
    if (lpTableIndex->bEnabled)
    {
      /* Find the index in the table index. */
      lpIndexReference = (RT_N*)(((RT_UCHAR8*)lpTable->lpTableData) + unItemIndex * nItemSize + nItemUserDataSize + nI * sizeof (RT_N));
      nIndexInTableIndex = *lpIndexReference;

      if (!RtDeleteSortableArrayItemIndex((void**)&lpTableIndex->lpIndexes, nIndexInTableIndex))
      {
        bResult = RT_FALSE;
        goto the_end;
      }

      /* Update index references. As an index has been removed, all following indexes references must be decremented. */
      for (nJ = nIndexInTableIndex; nJ < nSize - 1; nJ++)
      {
        lpIndexReference = (RT_N*)(((RT_UCHAR8*)lpTable->lpTableData) + lpTableIndex->lpIndexes[nJ] * nItemSize + nItemUserDataSize + nI * sizeof (RT_N));
        (*lpIndexReference)--;
      }

      /* Delete the data item by copying the last item to the item to delete. */
      if (!RtDeleteArrayItemIndex((void**)&lpTable->lpTableData, unItemIndex))
      {
        bResult = RT_FALSE;
        goto the_end;
      }

      /* If the deleted item was not the last one. */
      if (unItemIndex != nSize - 1)
      {
        /* Update the index because the last data item has been moved. */
        lpIndexReference = (RT_N*)(((RT_UCHAR8*)lpTable->lpTableData) + unItemIndex * nItemSize + nItemUserDataSize + nI * sizeof (RT_N));
        nIndexInTableIndex = *lpIndexReference;
        lpTableIndex->lpIndexes[nIndexInTableIndex] = unItemIndex;
      }
    }
  }

  bResult = RT_TRUE;
the_end:
  return bResult;
}

RT_B RT_API RtFreeTable(RT_TABLE* lpTable)
{
  RT_N nTableIndexesCount;
  RT_N nI;
  RT_B bResult;

  bResult = RT_TRUE;
  if (lpTable)
  {
    nTableIndexesCount = lpTable->lpTableMetadata->nTableIndexesCount;

    for (nI = 0; nI < nTableIndexesCount; nI++)
    {
      bResult = bResult && RtFreeArray((void**)&lpTable->lpTableIndexes[nI].lpIndexes);
    }
    bResult = bResult && RtFreeArray((void**)&lpTable->lpTableIndexes);
    bResult = bResult && RtFreeArray(&lpTable->lpTableData);
  }

  return bResult;
}
