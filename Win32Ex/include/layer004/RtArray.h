#ifndef RT_ARRAY_H
#define RT_ARRAY_H

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtHeap.h"

/**
 * @file
 * Arrays.
 *
 * <p>
 * When using a custom header, it must ends with RT_ARRAY_HEADER, and nHeaderSize must be given to the sizeof the new structure.
 * </p>
 */

typedef struct _RT_ARRAY_HEADER
{
  RT_UN unHeaderSize;
  RT_HEAP** lpHeap;
  RT_UN unSize;
  RT_UN unItemSize;
}
RT_ARRAY_HEADER;

/**
 * @file
 * Arrays management.
 *
 * <p>
 * <tt>nHeaderSize</tt> can be 0 to use default header.
 * </p>
 *
 * @param nHeaderSize The total size of the custom header ending with RT_ARRAY_HEADER.
 */
void* RT_API RtArray_Create(void** lpArray, RT_UN unHeaderSize, RT_HEAP** lpHeap, RT_UN unSize, RT_UN unItemSize);
void* RT_API RtArray_SetSize(void** lpArray, RT_UN unSize);
RT_UN RT_API RtArray_GetSize(void* lpArray);
RT_UN RT_API RtArray_GetItemSize(void* lpArray);

void* RT_API RtArray_NewItem(void** lpArray, void** lpItem);


/**
 * Adds a new item and returns its index.
 *
 * Return RT_TYPE_MAX_UN in case of error.
 */
RT_UN RT_API RtArray_NewItemIndex(void** lpArray, RT_UN* lpItemIndex);

/**
 * This function moves the last item if it is not the item to delete.
 * As a result, indexes on the array items might become invalid.
 */
void* RT_API RtArray_DeleteItemIndex(void** lpArray, RT_UN unItemIndex);

/**
 *
 * <p>
 * Can be use before RtArray_GetLastItem to simulate a pop from a stack.
 * </p>
 *
 * @return A pointer on the last item of the array.
 */
void* RT_API RtArray_GetLastItem(void* lpArray);

void* RT_API RtArray_GetItem(void* lpArray, RT_UN unItemIndex, void** lpItem);

/**
 * Delete the last item of the array.
 *
 * <p>
 * Can be use after RtArray_GetLastItem to simulate a pop from a stack.
 * </p>
 *
 * @return Address of the pointer on the array.
 */
void* RT_API RtArray_DeleteLastItem(void** lpArray);

RT_B RT_API RtArray_Free(void** lpArray);

#endif /* RT_ARRAY_H */
