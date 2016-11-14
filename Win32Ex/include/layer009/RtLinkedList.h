#ifndef RT_LINKED_LIST_H
#define RT_LINKED_LIST_H

#include "layer000/RtWin32ExTypes.h"
#include "layer008/RtArray.h"

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
  RT_N nNextItemIndex;
  RT_N nPreviousItemIndex;
}
RT_LINKED_LIST_ITEM_HEADER;

typedef struct _RT_LINKED_LIST_HEADER
{
  RT_N nFirstUsedItemIndex;
  RT_N nFirstFreeItemIndex;
  RT_ARRAY_HEADER rtArrayHeader;
}
RT_LINKED_LIST_HEADER;

/**
 * @param nSize The initial capacity of the linked list.
 */
void* RT_API RtCreateLinkedList(void** lpLinkedList, RT_HEAP** lpHeap, RT_UN32 unSize, RT_UN32 unItemSize);

/**
 * @return The index of the newly created item, -1 in case of error.
 */
RT_N RT_API RtNewLinkedListItemIndex(void** lpLinkedList, RT_N* lpItemIndex);

void* RT_API RtDeleteLinkedListItem(void** lpLinkedList, RT_N lpItemIndex);

#endif /* RT_LINKED_LIST_H */
