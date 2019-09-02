#ifndef RT_VIRTUAL_MEMORY_H
#define RT_VIRTUAL_MEMORY_H

#include "layer000/RtWin32ExTypes.h"

RT_UN RT_API RtVirtualMemory_GetPageSize();

void* RT_API RtVirtualMemory_Alloc(void** lpArea, RT_UN unSize);
RT_B RT_API RtVirtualMemory_Free(void** lpArea, RT_UN unSize);

#endif /* RT_VIRTUAL_MEMORY_H */
