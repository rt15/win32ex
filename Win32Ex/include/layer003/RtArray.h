#ifndef RT_ARRAY_H
#define RT_ARRAY_H

#include "layer000/RtWin32ExTypes.h"
#include "layer001/RtHeap.h"

/**
 * @file
 * Arrays management.
 *
 * <p>
 * If the array is null terminated, the size does not include the null character.
 * </p>
 *
 * <p>
 * If RT_ARRAY_FLAG_DYNAMIC the structure is RT_DYNAMIC_ARRAY. Otherwise it is RT_ARRAY.
 * </p>
 */

/* The array is null terminated. The size does not include the null character. */
#define RT_ARRAY_FLAG_NULL_TERMINATED 1
/* The capacity of the array can be extended (using an heap). */
#define RT_ARRAY_FLAG_DYNAMIC 2
/* The data is in the heap. */
#define RT_ARRAY_FLAG_DYNAMIC_BUFFER 3

typedef struct _RT_ARRAY
{
  RT_GENERIC* lpData;
  RT_UN32 unGenericTypeSize;
  RT_UN32 unCapacity;
  RT_UN32 unSize;
  RT_UN32 unFlags;
}
RT_ARRAY;

typedef struct _RT_DYNAMIC_ARRAY
{
  RT_ARRAY rtArray;
  RT_HEAP** lpHeap;
}
RT_DYNAMIC_ARRAY;

/**
 * Create an array form a buffer, often a stack buffer.
 *
 * <p>
 * The size of the resulting array is zero, not null terminated.
 * </p>
 *
 * @param lpArray Resulting array.
 * @param lpData Buffer for the future array.
 * @param unGenericTypeSize Size of an array item.
 * @param unCapacity Capacity of the array, usually equals to the buffer size.
 */
RT_ARRAY* RT_API RtArray_Create(RT_ARRAY* lpArray, RT_GENERIC* lpData, RT_UN32 unGenericTypeSize, RT_UN32 unCapacity);

/**
 * Create a dynamic array.
 *
 * @param lpDynamicArray Resulting dynamic array.
 * @param lpData Initial buffer for the array, can be RT_NULL if unCapacity is zero.
 * @param unGenericTypeSize Size of an array item.
 * @param unCapacity Capacity of the array, usually equals to the initial buffer size.
 * @param lpHeap Heap to extend the array if needed.
 */
RT_ARRAY* RT_API RtDynamicArray_Create(RT_DYNAMIC_ARRAY* lpDynamicArray,
                                       RT_GENERIC* lpData, RT_UN32 unGenericTypeSize, RT_UN32 unCapacity,
                                       RT_HEAP** lpHeap);

RT_B RT_API RtArray_Append(RT_ARRAY* lpArray, RT_ARRAY* lpOtherArray);

RT_B RT_API RtArray_SetSize(RT_ARRAY* lpArray, RT_UN32 unSize);

RT_B RT_API RtDynamicArray_Free(RT_DYNAMIC_ARRAY* lpDynamicArray);

#endif /* RT_ARRAY_H */
