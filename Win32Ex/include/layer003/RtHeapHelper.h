#ifndef RT_HEAP_HELPER_H
#define RT_HEAP_HELPER_H

#include "layer000/RtWin32ExTypes.h"
#include "layer001/RtHeap.h"

/**
 * Perform an heap allocation only if lpBuffer and lpHeapBuffer are too small.<br>
 * lpHeapBuffer can be reused if allocated.
 *
 * @param lpBuffer Can be null. If so, <tt>lpHeapBuffer</tt> will be used.
 * @param lpHeapBuffer Can be null as <tt>lpBuffer</tt> can be enough. If not null, *lpHeapBuffer can be null or point on an already allocated heap memory.
 * @param lpHeapBufferSize Can be null <tt>lpBuffer</tt> can be enough. Otherwise point on the size of *lpHeapBuffer. Updated with the size of *lpHeapBuffer if it is allocated/reallocated.
 * @param lpArea <tt>lpBuffer</tt> if it is enough, an area allocated using <tt>lpHeap</tt> otherwise (Same as lpHeapBuffer).
 * @param lpHeap Can be <tt>RT_NULL</tt>. In that case we rely only <tt>lpBuffer</tt>
 * @return <tt>*lpArea</tt>.
 */
void* RT_API RtHeapHelper_AllocIfNeeded(void* lpBuffer, RT_UN unBufferSize, void** lpHeapBuffer, RT_UN* lpHeapBufferSize, void** lpArea, RT_UN unSize, RT_CHAR* lpName, RT_HEAP** lpHeap);

#endif /* RT_HEAP_H */
