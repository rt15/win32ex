#ifndef RT_PAGE_HEAP_H
#define RT_PAGE_HEAP_H

#include "layer000/RtWin32ExTypes.h"
#include "layer005/RtHeap.h"

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

void* RT_API RtPageHeapCreate(RtPageHeap* lpThis);

/* Static functions. */
void* RT_API RtAllocPage(void** lpArea, RT_UN unSize);
void* RT_API RtReAllocPage(void** lpArea, void* lpCurrentArea, RT_UN unSize);
RT_B RT_API RtFreePage(void** lpArea);

#endif /* RT_PAGE_HEAP_H */
