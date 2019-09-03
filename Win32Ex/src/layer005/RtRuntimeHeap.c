#include "layer005/RtRuntimeHeap.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"
#include "layer004/RtStaticHeap.h"

void* RT_API RtRuntimeHeap_Alloc(void* lpThis, void** lpArea, RT_UN unSize, RT_CHAR* lpName);
void* RT_API RtRuntimeHeap_ReAlloc(void* lpThis, void** lpArea, RT_UN unSize);
RT_B RT_API RtRuntimeHeap_Free(void* lpThis, void** lpArea);
RT_B RT_API RtRuntimeHeap_Close(void* lpThis);

RT_HEAP rtRuntimeHeap = {&RtRuntimeHeap_Alloc, &RtRuntimeHeap_ReAlloc, &RtRuntimeHeap_Free, &RtRuntimeHeap_Close};

void* RT_API RtRuntimeHeap_Create(RtRuntimeHeap* lpThis)
{
  lpThis->lpHeap = &rtRuntimeHeap;
  return lpThis;
}

void* RT_API RtRuntimeHeap_Alloc(void* lpThis, void** lpArea, RT_UN unSize, RT_CHAR* lpName)
{
  return RtStaticHeap_Alloc(lpArea, unSize);
}

void* RT_API RtRuntimeHeap_ReAlloc(void* lpThis, void** lpArea, RT_UN unSize)
{
  return RtStaticHeap_ReAlloc(lpArea, unSize);
}

RT_B RT_API RtRuntimeHeap_Free(void* lpThis, void** lpArea)
{
  return RtStaticHeap_Free(lpArea);
}

RT_B RT_API RtRuntimeHeap_Close(void* lpThis)
{
  return RT_TRUE;
}
