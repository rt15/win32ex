#ifndef RT_PAGE_HEAP_H
#define RT_PAGE_HEAP_H

#include "layer000/RtWin32ExTypes.h"
#include "layer001/RtHeap.h"

/**
 * @file
 * Provide a page heap and associated static functions.
 */

/**
 * Heap working with pages.
 */
typedef struct _RtPageHeap
{
  RT_HEAP* lpHeap;
}
RtPageHeap;

void* RT_API RtPageHeap_Create(RtPageHeap* lpThis);

/* Static functions. */
void* RT_API RtPageHeap_Alloc(void** lpArea, RT_UN unSize);
void* RT_API RtPageHeap_ReAlloc(void** lpArea, RT_UN unSize);
RT_B RT_API RtPageHeap_Free(void** lpArea);

#endif /* RT_PAGE_HEAP_H */
