#include <RtWin32Ex.h>

typedef struct _TT_LIST_ITEM
{
  RT_UN32 unValue;
  RT_CHAR lpValue[32];
}
TT_LIST_ITEM;

void RT_CALL TtDisplayList(void* lpList)
{
  RT_CHAR lpBuffer[512];
  RT_UN32 unSize;
  TT_LIST_ITEM* lpItem;
  RT_N nWritten;
  RT_B bFirst;
  RT_UN32 unI;

  bFirst = RT_TRUE;
  unSize = RtGetListSize(lpList);
  nWritten = 0;
  for (unI = 0; unI < unSize; unI++)
  {
    if (bFirst)
    {
      bFirst = RT_FALSE;
    }
    else
    {
      RtCopyString(_R(", "), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
    }

    RtGetListItem(lpList, unI, (void**)&lpItem);
    RtConvertNumberToString(lpItem->unValue, &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  }

  RtCopyString(_R("\n"), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);
}

RT_UN16 RT_CALL TtCheckList(void* lpList, RT_N nExpectedSize, RT_N nExpectedItemSize, RT_N nExpectedChunkSize, RT_N nExpectedChunksCount)
{
  RT_N nSize;
  RT_N nItemSize;
  RT_N nChunkSize;
  RT_N nChunksCount;
  RT_LIST_HEADER* lpListHeader;
  RT_CHAR lpBuffer[512];
  RT_N nWritten;
  RT_UN16 unResult;

  unResult = 1;

  lpListHeader = lpList;
  lpListHeader--;

  nSize = lpListHeader->unSize;
  nItemSize = lpListHeader->unItemSize;
  nChunkSize = lpListHeader->unChunkSize;
  nChunksCount = lpListHeader->rtArrayHeader.unSize;

  nWritten = 0;
  RtCopyString(_R("List size = "), lpBuffer, 512, &nWritten);
  RtConvertNumberToString(nSize, &lpBuffer[nWritten], 512 - nWritten, &nWritten);

  RtCopyString(_R(", itemSize = "), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtConvertNumberToString(nItemSize, &lpBuffer[nWritten], 512 - nWritten, &nWritten);

  RtCopyString(_R(", chunkSize = "), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtConvertNumberToString(nChunkSize, &lpBuffer[nWritten], 512 - nWritten, &nWritten);

  RtCopyString(_R(", chunksCount = "), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtConvertNumberToString(nChunksCount, &lpBuffer[nWritten], 512 - nWritten, &nWritten);

  RtCopyString(_R("\n"), &lpBuffer[nWritten], 512 - nWritten, &nWritten);
  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  if (nExpectedSize != nSize) goto the_end;
  if (nExpectedItemSize != nItemSize) goto the_end;
  if (nExpectedChunkSize != nChunkSize) goto the_end;
  if (nExpectedChunksCount != nChunksCount) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestList(RT_HEAP** lpHeap)
{
  void* lpList;
  RT_N nI;
  TT_LIST_ITEM* lpItem;
  RT_N nItemIndex;
  RT_UN16 unResult;
  RT_N nWritten;
  RT_N nItemSize;

  unResult = 1;

  nItemSize = sizeof(RT_UN32) + 32 * sizeof(RT_CHAR);

  if (!RtCreateList(&lpList, lpHeap, 23, sizeof(TT_LIST_ITEM), 10)) goto the_end;

  if (TtCheckList(lpList, 23, nItemSize, 10, 3)) goto the_end;

  for (nI = 0; nI < 23; nI++)
  {
    RtGetListItem(lpList, nI, (void**)&lpItem);
    lpItem->unValue = nI;
    RtCopyString(_R("This is item characters."), lpItem->lpValue, 32, &nWritten);
  }

  TtDisplayList(lpList);

  /* Reduce the size of the list. */
  if (!RtSetListSize(&lpList, 17)) goto the_end;


  if (TtCheckList(lpList, 17, nItemSize, 10, 2)) goto the_end;
  TtDisplayList(lpList);

  /* Increase the size of the list. */
  if (!RtSetListSize(&lpList, 31)) goto the_end;

  if (TtCheckList(lpList, 31, nItemSize, 10, 4)) goto the_end;

  for (nI = 0; nI < 31; nI++)
  {
    RtGetListItem(lpList, nI, (void**)&lpItem);
    lpItem->unValue = nI;
  }

  TtDisplayList(lpList);

  if (!RtDeleteListItemIndex(&lpList, 12)) goto the_end;

  TtDisplayList(lpList);

  if (!RtNewListItem(&lpList, (void**)&lpItem)) goto the_end;
  lpItem->unValue = 42;

  TtDisplayList(lpList);

  if (RtNewListItemIndex(&lpList, &nItemIndex) == -1) goto the_end;

  if (!RtGetListItem(lpList, nItemIndex, (void**)&lpItem)) goto the_end;

  if (!RtFreeList(&lpList)) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}
