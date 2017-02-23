#include "layer006/RtPageHeap.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer003/RtMemory.h"
#include "layer003/RtVirtualMemory.h"

void* RT_API RtPageHeapAlloc(void* lpThis, void** lpArea, RT_UN unSize, RT_CHAR* lpName);
void* RT_API RtPageHeapReAlloc(void* lpThis, void** lpArea, void* lpCurrentArea, RT_UN unSize);
RT_B RT_API RtPageHeapFree(void* lpThis, void** lpArea);
RT_B RT_API RtPageHeapClose(void* lpThis);

RT_HEAP rtPageHeap = {&RtPageHeapAlloc, &RtPageHeapReAlloc, &RtPageHeapFree, &RtPageHeapClose};

void* RT_API RtPageHeapCreate(RtPageHeap* lpThis)
{
  /* Nothing to do. */
  return lpThis;
}

void* RT_API RtPageHeapAlloc(void* lpThis, void** lpArea, RT_UN unSize, RT_CHAR* lpName)
{
  return RtAllocPage(lpArea, unSize);
}

void* RT_API RtPageHeapReAlloc(void* lpThis, void** lpArea, void* lpCurrentArea, RT_UN unSize)
{
  return RtReAllocPage(lpArea, lpCurrentArea, unSize);
}

RT_B RT_API RtPageHeapFree(void* lpThis, void** lpArea)
{
  return RtFreePage(lpArea);
}

void* RT_API RtAllocPage(void** lpArea, RT_UN unSize)
{
  RT_UN* lpAreaInfo;

  if (RtAllocVirtualMemory((void**)&lpAreaInfo, unSize + sizeof(RT_UN)))
  {
    lpAreaInfo[0] = unSize;
    lpAreaInfo++;
  }
  *lpArea = lpAreaInfo;

  return *lpArea;
}

/**
 * TODO: Use getPageSize to alloc/free and copy only if the page count is changing.
 */
void* RT_API RtReAllocPage(void** lpArea, void* lpCurrentArea, RT_UN unSize)
{
  RT_UN* lpAreaInfo;
  RT_UN unContentSize;

  /* Retrieve current size. */
  lpAreaInfo = lpCurrentArea;
  lpAreaInfo--;
  unContentSize = lpAreaInfo[0];

  /* Allocate a new area and copy. */
  if (RtAllocPage(lpArea, unSize))
  {
    /* Copy the smallest between new size and initial size. */
    if (unSize < unContentSize)
    {
       unContentSize = unSize;
    }
    RT_MEMORY_COPY(lpCurrentArea, *lpArea, unContentSize);

    /* Free current area only in case of success to let callee cleanup content if needed. */
    RtFreePage(&lpCurrentArea);
  }

  return *lpArea;
}

RT_B RT_API RtFreePage(void** lpArea)
{
  RT_UN* lpAreaInfo;
  RT_B bResult;

  if (*lpArea)
  {
    lpAreaInfo = *lpArea;
    lpAreaInfo--;
    *lpArea = RT_NULL;
    bResult = RtFreeVirtualMemory((void**)&lpAreaInfo, lpAreaInfo[0] + sizeof(RT_UN));
  }
  else
  {
    bResult = RT_SUCCESS;
  }
  return bResult;
}

RT_B RT_API RtPageHeapClose(void* lpThis)
{
  /* Nothing to do. */
  return RT_TRUE;
}
