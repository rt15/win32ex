#include <RtWin32Ex.h>

typedef struct _ZZ_LIST_ITEM
{
  RT_UN32 unValue;
  RT_CHAR lpValue[32];
}
ZZ_LIST_ITEM;

RT_B RT_CALL ZzTestDisplayList(void* lpList)
{
  RT_ARRAY rtMessage;
  RT_CHAR lpMessage[512];
  RT_ARRAY rtConversionBuffer;
  RT_CHAR lpConversionBuffer[64];
  RT_ARRAY rtString;
  RT_UN unSize;
  ZZ_LIST_ITEM* lpItem;
  RT_B bFirst;
  RT_UN unI;
  RT_B bResult;

  bFirst = RT_TRUE;
  unSize = RtList_GetSize(lpList);

  RtArray_Create(&rtMessage, lpMessage, sizeof(RT_CHAR), 512);
  RtChar_CreateString(&rtString, _R(", "));
  RtArray_Create(&rtConversionBuffer, lpConversionBuffer, sizeof(RT_CHAR), 64);

  for (unI = 0; unI < unSize; unI++)
  {
    if (bFirst)
    {
      bFirst = RT_FALSE;
    }
    else
    {
      if (!RtArray_Append(&rtMessage, &rtString)) goto handle_error;
    }

    RtList_GetItem(lpList, unI, (void**)&lpItem);
    if (!RtChar_ConvertUnsignedIntegerToString(lpItem->unValue, &rtConversionBuffer)) goto handle_error;
    if (!RtArray_Append(&rtMessage, &rtConversionBuffer)) goto handle_error;
  }

  if (!RtChar_AppendCString(&rtMessage, _R("\n"))) goto handle_error;

  if (!RtConsole_WriteString(&rtMessage)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestCheckList(void* lpList, RT_UN unExpectedSize, RT_UN unExpectedItemSize, RT_UN unExpectedChunkSize, RT_UN unExpectedChunksCount)
{
  RT_ARRAY rtMessage;
  RT_CHAR lpMessage[512];
  RT_ARRAY rtConversionBuffer;
  RT_CHAR lpConversionBuffer[64];
  RT_UN unSize;
  RT_UN unItemSize;
  RT_UN unChunkSize;
  RT_UN unChunksCount;
  RT_LIST_HEADER* lpListHeader;
  RT_B bResult;

  lpListHeader = lpList;
  lpListHeader--;

  unSize = lpListHeader->unSize;
  unItemSize = lpListHeader->unItemSize;
  unChunkSize = lpListHeader->unChunkSize;
  unChunksCount = lpListHeader->rtArrayHeader.unSize;

  RtArray_Create(&rtMessage, lpMessage, sizeof(RT_CHAR), 512);
  RtArray_Create(&rtConversionBuffer, lpConversionBuffer, sizeof(RT_CHAR), 64);

  /* Size. */
  if (!RtChar_AppendCString(&rtMessage, _R("List size = "))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unSize, &rtConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&rtMessage, &rtConversionBuffer)) goto handle_error;

  /* Item size. */
  if (!RtChar_AppendCString(&rtMessage, _R(", itemSize = "))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unItemSize, &rtConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&rtMessage, &rtConversionBuffer)) goto handle_error;

  /* Chunk size. */
  if (!RtChar_AppendCString(&rtMessage, _R(", chunkSize = "))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unChunkSize, &rtConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&rtMessage, &rtConversionBuffer)) goto handle_error;

  /* Chunks count. */
  if (!RtChar_AppendCString(&rtMessage, _R(", chunksCount = "))) goto handle_error;
  if (!RtChar_ConvertUnsignedIntegerToString(unChunksCount, &rtConversionBuffer)) goto handle_error;
  if (!RtArray_Append(&rtMessage, &rtConversionBuffer)) goto handle_error;

  if (!RtChar_AppendCString(&rtMessage, _R("\n"))) goto handle_error;
  if (!RtConsole_WriteString(&rtMessage)) goto handle_error;

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
  RT_UN unOutputSize;
  RT_UN unItemSize;
  RT_B bResult;

  lpList = RT_NULL;

  unItemSize = sizeof(RT_UN32) + 32 * sizeof(RT_CHAR);

  if (!RtList_Create(&lpList, lpHeap, 35, sizeof(ZZ_LIST_ITEM), 16)) goto handle_error;

  if (!ZzTestCheckList(lpList, 35, unItemSize, 16, 3)) goto handle_error;

  for (unI = 0; unI < 35; unI++)
  {
    RtList_GetItem(lpList, unI, (void**)&lpItem);
    lpItem->unValue = unI;
    RtChar_CopyString(_R("This is item characters."), lpItem->lpValue, 32, &unOutputSize);
  }

  if (!ZzTestDisplayList(lpList)) goto handle_error;

  /* Reduce the size of the list. */
  if (!RtList_SetSize(&lpList, 29)) goto handle_error;


  if (!ZzTestCheckList(lpList, 29, unItemSize, 16, 2)) goto handle_error;
  if (!ZzTestDisplayList(lpList)) goto handle_error;

  /* Increase the size of the list. */
  if (!RtList_SetSize(&lpList, 49)) goto handle_error;

  if (!ZzTestCheckList(lpList, 49, unItemSize, 16, 4)) goto handle_error;

  for (unI = 0; unI < 49; unI++)
  {
    RtList_GetItem(lpList, unI, (void**)&lpItem);
    lpItem->unValue = unI;
  }

  if (!ZzTestDisplayList(lpList)) goto handle_error;

  if (!RtList_DeleteItemIndex(&lpList, 12)) goto handle_error;

  if (!ZzTestDisplayList(lpList)) goto handle_error;

  if (!RtList_NewItem(&lpList, (void**)&lpItem)) goto handle_error;
  lpItem->unValue = 42;

  if (!ZzTestDisplayList(lpList)) goto handle_error;

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
