#include "layer002/RtVirtualMemory.h"

#include "layer001/RtWin32ExOsHeaders.h"

RT_UN rt_nPageSize;

RT_UN RT_API RtVirtualMemory_GetPageSize()
{
#ifdef RT_DEFINE_WINDOWS
  SYSTEM_INFO systemInfo;
#endif
  if (rt_nPageSize == 0)
  {
#ifdef RT_DEFINE_WINDOWS
    GetSystemInfo(&systemInfo);
    rt_nPageSize = systemInfo.dwPageSize;
#else
    rt_nPageSize = getpagesize();
#endif
  }
  return rt_nPageSize;
}

void* RT_API RtVirtualMemory_Alloc(void** lpArea, RT_UN unSize)
{
#ifdef RT_DEFINE_WINDOWS
  *lpArea = VirtualAlloc(NULL, unSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
  *lpArea = (RT_UN*)mmap(0, unSize, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (*lpArea == MAP_FAILED)
  {
    *lpArea = RT_NULL;
  }
#endif
  return *lpArea;
}

RT_B RT_API RtVirtualMemory_Free(void** lpArea, RT_UN unSize)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  bResult = VirtualFree(*lpArea, 0, MEM_RELEASE);
#else
  bResult = (munmap(*lpArea, unSize) == 0);
#endif
  *lpArea = RT_NULL;
  return bResult;
}
