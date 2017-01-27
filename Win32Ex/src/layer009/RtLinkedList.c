#include "layer009/RtLinkedList.h"

void* RT_API RtCreateLinkedList(void** lpLinkedList, RT_HEAP** lpHeap, RT_UN unSize, RT_UN unItemSize)
{
  RT_LINKED_LIST_HEADER* lpHeader;
  RT_CHAR8* lpItems;
  RT_LINKED_LIST_ITEM_HEADER* lpItem;
  RT_UN unI;

  if (RtCreateArray(lpLinkedList, sizeof(RT_LINKED_LIST_HEADER), lpHeap, unSize, unItemSize))
  {
    lpHeader = *lpLinkedList;
    lpHeader--;
    lpHeader->unFirstUsedItemIndex = RT_TYPE_MAX_UN;
    if (unSize)
    {
      lpHeader->unFirstFreeItemIndex = 0;

      /* Initialize free linked list. */
      lpItems = *lpLinkedList;
      for (unI = 0; unI < unSize - 1; unI++)
      {
        lpItem = (RT_LINKED_LIST_ITEM_HEADER*)(lpItems + unI * unItemSize);
        lpItem->unNextItemIndex = unI + 1;
      }
      lpItem = (RT_LINKED_LIST_ITEM_HEADER*)(lpItems + (unSize - 1) * unItemSize);
      lpItem->unNextItemIndex = RT_TYPE_MAX_UN;
    }
    else
    {
      lpHeader->unFirstFreeItemIndex = RT_TYPE_MAX_UN;
    }
  }
  return *lpLinkedList;
}

RT_UN RT_API RtNewLinkedListItemIndex(void** lpLinkedList, RT_UN* lpItemIndex)
{
  RT_LINKED_LIST_HEADER* lpHeader;
  RT_LINKED_LIST_ITEM_HEADER* lpItem;
  RT_LINKED_LIST_ITEM_HEADER* lpNextItem;
  RT_UN unNextItem;

  lpHeader = *lpLinkedList;
  lpHeader--;

  *lpItemIndex = lpHeader->unFirstFreeItemIndex;
  if (*lpItemIndex != RT_TYPE_MAX_UN)
  {
    lpItem = (RT_LINKED_LIST_ITEM_HEADER*)(((RT_CHAR8*)*lpLinkedList) + *lpItemIndex * lpHeader->rtArrayHeader.unItemSize);

    /* Manage free items list. */
    lpHeader->unFirstFreeItemIndex = lpItem->unNextItemIndex;
  }
  else
  {
    if (RtNewArrayItemIndex(lpLinkedList, lpItemIndex) == RT_TYPE_MAX_UN)
    {
      goto the_end;
    }
    else
    {
      lpItem = (RT_LINKED_LIST_ITEM_HEADER*)(((RT_CHAR8*)*lpLinkedList) + *lpItemIndex * lpHeader->rtArrayHeader.unItemSize);
    }
  }

  /* Add result to used items list. */
  unNextItem = lpHeader->unFirstUsedItemIndex;
  lpHeader->unFirstUsedItemIndex = *lpItemIndex;
  lpItem->unPreviousItemIndex = RT_TYPE_MAX_UN;
  lpItem->unNextItemIndex = unNextItem;
  if (unNextItem != RT_TYPE_MAX_UN)
  {
    lpNextItem = (RT_LINKED_LIST_ITEM_HEADER*)(((RT_CHAR8*)*lpLinkedList) + unNextItem * lpHeader->rtArrayHeader.unItemSize);
    lpNextItem->unPreviousItemIndex = *lpItemIndex;
  }

the_end:
  return *lpItemIndex;
}
