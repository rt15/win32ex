#include "layer009/RtLinkedList.h"

void* RT_API RtCreateLinkedList(void** lpLinkedList, RT_HEAP** lpHeap, RT_UN32 unSize, RT_UN32 unItemSize)
{
  RT_LINKED_LIST_HEADER* lpHeader;
  RT_CHAR8* lpItems;
  RT_LINKED_LIST_ITEM_HEADER* lpItem;
  RT_UN32 unI;

  if (RtCreateArray(lpLinkedList, sizeof(RT_LINKED_LIST_HEADER), lpHeap, unSize, unItemSize))
  {
    lpHeader = *lpLinkedList;
    lpHeader--;
    lpHeader->nFirstUsedItemIndex = -1;
    if (unSize)
    {
      lpHeader->nFirstFreeItemIndex = 0;

      /* Initialize free linked list. */
      lpItems = *lpLinkedList;
      for (unI = 0; unI < unSize - 1; unI++)
      {
        lpItem = (RT_LINKED_LIST_ITEM_HEADER*)(lpItems + unI * unItemSize);
        lpItem->nNextItemIndex = unI + 1;
      }
      lpItem = (RT_LINKED_LIST_ITEM_HEADER*)(lpItems + (unSize - 1) * unItemSize);
      lpItem->nNextItemIndex = -1;
    }
    else
    {
      lpHeader->nFirstFreeItemIndex = -1;
    }
  }
  return *lpLinkedList;
}

RT_N RT_API RtNewLinkedListItemIndex(void** lpLinkedList, RT_N* lpItemIndex)
{
  RT_LINKED_LIST_HEADER* lpHeader;
  RT_LINKED_LIST_ITEM_HEADER* lpItem;
  RT_LINKED_LIST_ITEM_HEADER* lpNextItem;
  RT_N nNextItem;

  lpHeader = *lpLinkedList;
  lpHeader--;

  *lpItemIndex = lpHeader->nFirstFreeItemIndex;
  if (*lpItemIndex != -1)
  {
    lpItem = (RT_LINKED_LIST_ITEM_HEADER*)(((RT_CHAR8*)*lpLinkedList) + *lpItemIndex * lpHeader->rtArrayHeader.unItemSize);

    /* Manage free items list. */
    lpHeader->nFirstFreeItemIndex = lpItem->nNextItemIndex;
  }
  else
  {
    if (RtNewArrayItemIndex(lpLinkedList, lpItemIndex) == -1)
    {
      goto the_end;
    }
    else
    {
      lpItem = (RT_LINKED_LIST_ITEM_HEADER*)(((RT_CHAR8*)*lpLinkedList) + *lpItemIndex * lpHeader->rtArrayHeader.unItemSize);
    }
  }

  /* Add result to used items list. */
  nNextItem = lpHeader->nFirstUsedItemIndex;
  lpHeader->nFirstUsedItemIndex = *lpItemIndex;
  lpItem->nPreviousItemIndex = -1;
  lpItem->nNextItemIndex = nNextItem;
  if (nNextItem != -1)
  {
    lpNextItem = (RT_LINKED_LIST_ITEM_HEADER*)(((RT_CHAR8*)*lpLinkedList) + nNextItem * lpHeader->rtArrayHeader.unItemSize);
    lpNextItem->nPreviousItemIndex = *lpItemIndex;
  }

the_end:
  return *lpItemIndex;
}
