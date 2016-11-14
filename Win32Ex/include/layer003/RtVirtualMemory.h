#ifndef RT_VIRTUAL_MEMORY_H
#define RT_VIRTUAL_MEMORY_H

#include "layer000/RtWin32ExTypes.h"

RT_UN RT_API getPageSize();

void* RT_API RtAllocVirtualMemory(void** lpArea, RT_UN unSize);
RT_B RT_API RtFreeVirtualMemory(void** lpArea, RT_UN unSize);

#endif /* RT_VIRTUAL_MEMORY_H */
