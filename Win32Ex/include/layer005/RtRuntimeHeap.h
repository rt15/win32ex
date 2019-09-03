#ifndef RT_RUNTIME_HEAP_H
#define RT_RUNTIME_HEAP_H

#include "layer000/RtWin32ExTypes.h"
#include "layer003/RtHeap.h"

/**
 * Multithreaded heap based on the process heap on windows and runtime heap on linux.
 */
typedef struct _RtRuntimeHeap
{
  RT_HEAP* lpHeap;
}
RtRuntimeHeap;

void* RT_API RtRuntimeHeap_Create(RtRuntimeHeap* lpThis);

#endif /* RT_RUNTIME_HEAP_H */
