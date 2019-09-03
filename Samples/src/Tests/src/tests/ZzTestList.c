#include "ZzTests.h"

typedef struct _ZZ_LIST_ITEM
{
  RT_UN32 unValue;
  RT_CHAR lpValue[32];
}
ZZ_LIST_ITEM;

void RT_CALL ZzTestDisplayList(void* lpList)
{
  RT_CHAR lpBuffer[512];
  RT_UN unSize;
  ZZ_LIST_ITEM* lpItem;
  RT_UN unWritten;
  RT_B bFirst;
  RT_UN unI;

  bFirst = RT_TRUE;
  unSize = RtList_GetSize(lpList);
  unWritten = 0;
  for (unI = 0; unI < unSize; unI++)
  {
    if (bFirst)
    {
      bFirst = RT_FALSE;
    }
    else
    {
      RtChar_CopyString(_R(", "), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
    }

    RtList_GetItem(lpList, unI, (void**)&lpItem);
    RtChar_ConvertIntegerToString(lpItem->unValue, &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  }

  RtChar_CopyString(_R("\n"), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);
}

RT_B RT_CALL ZzTestCheckList(void* lpList, RT_UN unExpectedSize, RT_UN unExpectedItemSize, RT_UN unExpectedChunkSize, RT_UN unExpectedChunksCount)
{
  RT_UN unSize;
  RT_UN unItemSize;
  RT_UN unChunkSize;
  RT_UN unChunksCount;
  RT_LIST_HEADER* lpListHeader;
  RT_CHAR lpBuffer[512];
  RT_UN unWritten;
  RT_B bResult;

  lpListHeader = lpList;
  lpListHeader--;

  unSize = lpListHeader->unSize;
  unItemSize = lpListHeader->unItemSize;
  unChunkSize = lpListHeader->unChunkSize;
  unChunksCount = lpListHeader->rtArrayHeader.unSize;

  unWritten = 0;
  RtChar_CopyString(_R("List size = "), lpBuffer, 512, &unWritten);
  RtChar_ConvertIntegerToString(unSize, &lpBuffer[unWritten], 512 - unWritten, &unWritten);

  RtChar_CopyString(_R(", itemSize = "), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtChar_ConvertIntegerToString(unItemSize, &lpBuffer[unWritten], 512 - unWritten, &unWritten);

  RtChar_CopyString(_R(", chunkSize = "), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtChar_ConvertIntegerToString(unChunkSize, &lpBuffer[unWritten], 512 - unWritten, &unWritten);

  RtChar_CopyString(_R(", chunksCount = "), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtChar_ConvertIntegerToString(unChunksCount, &lpBuffer[unWritten], 512 - unWritten, &unWritten);

  RtChar_CopyString(_R("\n"), &lpBuffer[unWritten], 512 - unWritten, &unWritten);
  RtConsole_WriteStringWithSize(lpBuffer, unWritten);

  if (unExpectedSize != unSize) goto handle_error;
  if (unExpectedItemSize != unItemSize) goto handle_error;
  if (unExpectedChunkSize != unChunkSize) goto handle_error;
  if (unExpectedChunksCount != unChunksCount) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestList(RT_HEAP** lpHeap)
{
  void* lpList;
  RT_UN32 unI;
  ZZ_LIST_ITEM* lpItem;
  RT_UN unItemIndex;
  RT_UN unWritten;
  RT_UN unItemSize;
  RT_B bResult;

  lpList = RT_NULL;

  unItemSize = sizeof(RT_UN32) + 32 * sizeof(RT_CHAR);

  if (!RtList_Create(&lpList, lpHeap, 23, sizeof(ZZ_LIST_ITEM), 10)) goto handle_error;

  if (!ZzTestCheckList(lpList, 23, unItemSize, 10, 3)) goto handle_error;

  for (unI = 0; unI < 23; unI++)
  {
    RtList_GetItem(lpList, unI, (void**)&lpItem);
    lpItem->unValue = unI;
    RtChar_CopyString(_R("This is item characters."), lpItem->lpValue, 32, &unWritten);
  }

  ZzTestDisplayList(lpList);

  /* Reduce the size of the list. */
  if (!RtList_SetSize(&lpList, 17)) goto handle_error;


  if (!ZzTestCheckList(lpList, 17, unItemSize, 10, 2)) goto handle_error;
  ZzTestDisplayList(lpList);

  /* Increase the size of the list. */
  if (!RtList_SetSize(&lpList, 31)) goto handle_error;

  if (!ZzTestCheckList(lpList, 31, unItemSize, 10, 4)) goto handle_error;

  for (unI = 0; unI < 31; unI++)
  {
    RtList_GetItem(lpList, unI, (void**)&lpItem);
    lpItem->unValue = unI;
  }

  ZzTestDisplayList(lpList);

  if (!RtList_DeleteItemIndex(&lpList, 12)) goto handle_error;

  ZzTestDisplayList(lpList);

  if (!RtList_NewItem(&lpList, (void**)&lpItem)) goto handle_error;
  lpItem->unValue = 42;

  ZzTestDisplayList(lpList);

  if (RtList_NewItemIndex(&lpList, &unItemIndex) == RT_TYPE_MAX_UN) goto handle_error;

  if (!RtList_GetItem(lpList, unItemIndex, (void**)&lpItem)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (lpList)
  {
    if (!RtList_Free(&lpList) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
