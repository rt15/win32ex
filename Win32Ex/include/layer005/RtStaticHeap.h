#ifndef RT_STATIC_HEAP_H
#define RT_STATIC_HEAP_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Use process heap under Windows and CRT under Linux.
 *
 * <p>
 * Thread safe if process heap/malloc is which should be the case.
 * </p>
 */

void* RT_API RtAlloc(void** lpArea, RT_UN unSize);

/**
 *
 *
 * @param lpCurrentArea Left unchanged in case of failure.
 * @return *lpArea
 */
void* RT_API RtReAlloc(void** lpArea, void* lpCurrentArea, RT_UN unSize);

RT_B RT_API RtFree(void** lpArea);

/**
 * Perform an heap allocation only if lpBuffer is too small.
 *
 * @param lpHeapBufferSize If not RT_NULL, must be initialized with zero or the size of heap buffer.
 */
void* RT_API RtAllocIfNeeded(void* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, void** lpArea, RT_UN unSize);

#endif /* RT_STATIC_HEAP_H */
