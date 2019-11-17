#include "layer005/RtList.h"

#include "layer001/RtMemory.h"
#include "layer002/RtError.h"

void* RT_API RtList_Create(void** lpList, RT_HEAP** lpHeap, RT_UN unSize, RT_UN unItemSize, RT_UN unChunkSize)
{
  RT_LIST_HEADER* lpHeader;
  RT_UN unChunksCount;
  void** lpChunks;
  RT_UN unI;

  lpChunks = RT_NULL;

  if (!RT_MEMORY_IS_POWER_OF_TWO(unChunkSize))
  {
    RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }

  /* Compute chunks count. */
  unChunksCount = RT_MEMORY_GET_CHUNKS_COUNT(unSize, unChunkSize);
  if (unChunksCount == RT_TYPE_MAX_UN)
  {
    goto handle_error;
  }

  /* Create array of pointers on chunks. */
  if (RtArray_Create(lpList, sizeof(RT_LIST_HEADER), lpHeap, unChunksCount, sizeof(void*)))
  {
    lpHeader = *lpList;
    lpHeader--;
    lpHeader->unSize = unSize;
    lpHeader->unChunkSize = unChunkSize;
    lpHeader->unItemSize = unItemSize;
    lpChunks = (void**)*lpList;
  }
  else
  {
    goto handle_error;
  }

  /* Initialize all pointers to be able to clean memory in case of issue. */
  for (unI = 0; unI < unChunksCount; unI++)
  {
    lpChunks[unI] = RT_NULL;
  }

  /* Allocate all chunks. */
  for (unI = 0; unI < unChunksCount; unI++)
  {
    if (!(*lpHeap)->lpAlloc(lpHeap, (void**)&lpChunks[unI], unChunkSize * unItemSize, _R("Chunk")))
    {
      goto handle_error;
    }
  }

  goto free_resources;
handle_error:
  if (lpChunks)
  {
    /* Some chunks might have been allocated. */
    for (unI = 0; unI < unChunksCount; unI++)
    {
      if (lpChunks[unI])
      {
        /* Free chunk. */
        (*lpHeap)->lpFree(lpHeap, &lpChunks[unI]);
      }
      else
      {
        /* All remaining chunks have not been allocated. */
        break;
      }
    }
    /* Free chunks pointers. */
    RtArray_Free((void**)&lpChunks);
  }
  *lpList = RT_NULL;
free_resources:
  return *lpList;
}

RT_UN RT_API RtList_GetSize(void* lpList)
{
  RT_LIST_HEADER* lpListHeader;

  lpListHeader = lpList;
  lpListHeader--;
  return lpListHeader->unSize;
}

void* RT_API RtList_GetItem(void* lpList, RT_UN unItemIndex, void** lpItem)
{
  RT_LIST_HEADER* lpListHeader;
  void** lpChunks;
  RT_UN unChunksCount;
  RT_UN unChunkIndex;
  RT_UN unChunkSize;
  RT_UN unItemSize;
  RT_UN unItemIndexInChunk;

  lpListHeader = lpList;
  lpListHeader--;

  unChunksCount = lpListHeader->rtArrayHeader.unSize;
  unChunkSize = lpListHeader->unChunkSize;
  unItemSize = lpListHeader->unItemSize;

  /* Find the chunk index. */
  unChunkIndex = unItemIndex / unChunkSize;
  if (unChunkIndex >= unChunksCount)
  {
    RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
    *lpItem = RT_NULL;
    goto free_resources;
  }

  /* Find the item. */
  unItemIndexInChunk = (RT_MEMORY_MODULO_POWER_OF_TWO(unItemIndex, unChunkSize));
  lpChunks = (void**)lpList;
  *lpItem = ((RT_UCHAR8*)lpChunks[unChunkIndex]) + unItemIndexInChunk * unItemSize;
free_resources:
  return *lpItem;
}

void* RT_API RtList_SetSize(void** lpList, RT_UN unSize)
{
  RT_UN unCurrentChunksCount;
  RT_LIST_HEADER* lpListHeader;
  RT_UN unChunkSize;
  RT_UN unNewChunksCount;
  RT_UN unItemSize;
  RT_HEAP** lpHeap;
  void** lpChunks;
  RT_UN unI;

  lpListHeader = *lpList;
  lpListHeader--;
  unCurrentChunksCount = lpListHeader->rtArrayHeader.unSize;
  unChunkSize = lpListHeader->unChunkSize;
  lpHeap = lpListHeader->rtArrayHeader.lpHeap;
  unItemSize = lpListHeader->unItemSize;

  /* Compute new chunks count. */
  unNewChunksCount = RT_MEMORY_GET_CHUNKS_COUNT(unSize, unChunkSize);
  if (unNewChunksCount == RT_TYPE_MAX_UN)
  {
    goto handle_error;
  }

  /* If we need to add or remove chunks. */
  if (unCurrentChunksCount != unNewChunksCount)
  {
    /* Size reduced. */
    if (unNewChunksCount < unCurrentChunksCount)
    {
      lpChunks = (void**)*lpList;

      /* Remove existing chunks. */
      for (unI = unNewChunksCount; unI < unCurrentChunksCount; unI++)
      {
        if (!(*lpHeap)->lpFree(lpHeap, &lpChunks[unI]))
        {
          lpChunks[unI] = RT_NULL;
          goto handle_error;
        }
        lpChunks[unI] = RT_NULL;
      }

      /* Reduce chunks array size. TODO: There can be a memory leak of chunks. */
      if (!RtArray_SetSize(lpList, unNewChunksCount)) goto free_resources;
    }
    else
    {
      /* Size increased. We need more chunks. TODO: There can be a memory leak of chunks. */
      if (!RtArray_SetSize(lpList, unNewChunksCount)) goto free_resources;

      lpChunks = (void**)*lpList;

      /* Allocate new chunks. */
      for (unI = unCurrentChunksCount; unI < unNewChunksCount; unI++)
      {
        if (!(*lpHeap)->lpAlloc(lpHeap, (void**)&lpChunks[unI], unChunkSize * unItemSize, _R("Chunk")))
        {
          goto handle_error;
        }
      }
    }
  }

  /* Set the new size. */
  if (*lpList)
  {
    lpListHeader = *lpList;
    lpListHeader--;
    lpListHeader->unSize = unSize;
  }
  goto free_resources;
handle_error:
  if (*lpList)
  {
    RtList_Free(lpList);
  }
free_resources:
  return *lpList;
}

void* RT_API RtList_DeleteItemIndex(void** lpList, RT_UN unItemIndex)
{
  RT_LIST_HEADER* lpListHeader;
  RT_UN unItemSize;
  RT_UN unLastItemIndex;
  void* lpLastItem;
  void* lpItemToDelete;

  lpListHeader = *lpList;
  lpListHeader--;
  unItemSize = lpListHeader->unItemSize;
  unLastItemIndex = lpListHeader->unSize - 1;
  if (unItemIndex != unLastItemIndex)
  {
    /* Copy last item into item to delete. */
    /* TODO: Manage errors in two next lines. */
    RtList_GetItem(*lpList, unLastItemIndex, &lpLastItem);
    RtList_GetItem(*lpList, unItemIndex, &lpItemToDelete);
    RT_MEMORY_COPY(lpLastItem, lpItemToDelete, unItemSize);
  }

  return RtList_SetSize(lpList, unLastItemIndex);
}

void* RT_API RtList_NewItem(void** lpList, void** lpItem)
{
  RT_LIST_HEADER* lpListHeader;
  RT_UN unSize;

  lpListHeader = *lpList;
  lpListHeader--;

  /* Increase the size of the list. */
  unSize = lpListHeader->unSize;
  if (!RtList_SetSize(lpList, unSize + 1))
  {
    *lpItem = RT_NULL;
    goto free_resources;
  }

  RtList_GetItem(*lpList, unSize, lpItem);

free_resources:
  return *lpItem;
}

RT_UN RT_API RtList_NewItemIndex(void** lpList, RT_UN* lpItemIndex)
{
  RT_LIST_HEADER* lpListHeader;
  RT_UN unSize;

  lpListHeader = *lpList;
  lpListHeader--;

  /* Increase the size of the list. */
  unSize = lpListHeader->unSize;
  if (!RtList_SetSize(lpList, unSize + 1))
  {
    *lpItemIndex = RT_TYPE_MAX_UN;
    goto free_resources;
  }

  *lpItemIndex = unSize;

free_resources:
  return *lpItemIndex;
}


RT_B RT_API RtList_Free(void** lpList)
{
  RT_LIST_HEADER* lpListHeader;
  RT_UN unChunksCount;
  void** lpChunks;
  RT_HEAP** lpHeap;
  RT_UN unI;
  RT_B bResult;

  bResult = RT_SUCCESS;

  if (*lpList)
  {
    lpListHeader = *lpList;
    lpListHeader--;

    unChunksCount = lpListHeader->rtArrayHeader.unSize;
    lpHeap = lpListHeader->rtArrayHeader.lpHeap;

    /* Free all chunks. */
    lpChunks = (void**)*lpList;
    for (unI = 0; unI < unChunksCount; unI++)
    {
      if (!(*lpHeap)->lpFree(lpHeap, &lpChunks[unI]))
      {
        bResult = RT_FAILURE;
      }
    }

    /* Free pointers array. */
    if (!RtArray_Free(lpList))
    {
      bResult = RT_FAILURE;
    }
  }

  return bResult;
}
