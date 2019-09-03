#ifndef RT_LINKED_LIST_H
#define RT_LINKED_LIST_H

#include "layer000/RtWin32ExTypes.h"
#include "layer004/RtArray.h"

/**
 * @file
 * A simple linked list backed by an array.
 *
 * <p>
 * Use case?
 * </p>
 *
 * <p>
 * The used items linked list is chained in both ways, while free items linked list is chained in only one way.
 * </p>
 */

/**
 * Structures in the linked list must start with a RT_LINKED_LIST_ITEM_HEADER field.
 */
typedef struct _RT_LINKED_LIST_ITEM_HEADER
{
  RT_UN unNextItemIndex;
  RT_UN unPreviousItemIndex;
}
RT_LINKED_LIST_ITEM_HEADER;

typedef struct _RT_LINKED_LIST_HEADER
{
  RT_UN unFirstUsedItemIndex;
  RT_UN unFirstFreeItemIndex;
  RT_ARRAY_HEADER rtArrayHeader;
}
RT_LINKED_LIST_HEADER;

/**
 * @param unSize The initial capacity of the linked list.
 */
void* RT_API RtLinkedList_Create(void** lpLinkedList, RT_HEAP** lpHeap, RT_UN unSize, RT_UN unItemSize);

/**
 * @return The index of the newly created item, RT_TYPE_MAX_UN in case of error.
 */
RT_UN RT_API RtLinkedList_NewItemIndex(void** lpLinkedList, RT_UN* lpItemIndex);

#endif /* RT_LINKED_LIST_H */
