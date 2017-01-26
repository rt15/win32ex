#include "layer006/RtRuntimeHeap.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer005/RtStaticHeap.h"

void* RT_API RtRuntimeHeapAlloc(void* lpThis, void** lpArea, RT_UN unSize, RT_CHAR* lpName);
void* RT_API RtRuntimeHeapReAlloc(void* lpThis, void** lpArea, void* lpCurrentArea, RT_UN unSize);
RT_B RT_API RtRuntimeHeapFree(void* lpThis, void** lpArea);
RT_B RT_API RtRuntimeHeapClose(void* lpThis);

RT_HEAP rtRuntimeHeap = {&RtRuntimeHeapAlloc, &RtRuntimeHeapReAlloc, &RtRuntimeHeapFree, &RtRuntimeHeapClose};

void* RT_API RtRuntimeHeapCreate(RtRuntimeHeap* lpThis)
{
  lpThis->lpHeap = &rtRuntimeHeap;
  return lpThis;
}

void* RT_API RtRuntimeHeapAlloc(void* lpThis, void** lpArea, RT_UN unSize, RT_CHAR* lpName)
{
  return RtAlloc(lpArea, unSize);
}

void* RT_API RtRuntimeHeapReAlloc(void* lpThis, void** lpArea, void* lpCurrentArea, RT_UN unSize)
{
  return RtReAlloc(lpArea, lpCurrentArea, unSize);
}

RT_B RT_API RtRuntimeHeapFree(void* lpThis, void** lpArea)
{
  return RtFree(lpArea);
}

RT_B RT_API RtRuntimeHeapClose(void* lpThis)
{
  return RT_TRUE;
}
