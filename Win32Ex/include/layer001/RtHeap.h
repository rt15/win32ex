#ifndef RT_HEAP_H
#define RT_HEAP_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Declares the method of an heap.
 */

typedef void* (RT_CALL *RT_HEAP_ALLOC)(void* lpThis, void** lpArea, RT_UN unSize, RT_CHAR* lpName);

/**
 * <p>
 * If ReAlloc fails, lpArea is not deallocated and its content remains unchanged.
 * </p>
 *
 * @return Address of new area (*lpArea) or RT_NULL in case of failure.
 */
typedef void* (RT_CALL *RT_HEAP_RE_ALLOC)(void* lpThis, void** lpArea, RT_UN unSize);
/**
 *
 * <p>
 * lpArea is set to RT_NULL.
 * </p>
 *
 */
typedef RT_B (RT_CALL *RT_HEAP_FREE)(void* lpThis, void** lpArea);

typedef RT_B (RT_CALL *RT_HEAP_CLOSE)(void* lpThis);

typedef struct _RT_HEAP
{
  RT_HEAP_ALLOC lpAlloc;
  RT_HEAP_RE_ALLOC lpReAlloc;
  RT_HEAP_FREE lpFree;
  RT_HEAP_CLOSE lpClose;
}
RT_HEAP;

#endif /* RT_HEAP_H */
