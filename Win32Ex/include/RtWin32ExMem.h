#ifndef RT_WIN32_EX_MEM_H
#define RT_WIN32_EX_MEM_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Sometimes (when?) compiler generates calls to memcmp/memcpy/memmove/memset without using intrinsic functions.<br>
 * This header can define corresponding symbols when CRT is not used.
 *
 * <p>
 * Partial copy from RtMemory.c, mostly changing:
 * </p>
 * <ul>
 * <li>function names to memcmp, memcpy, memmove and memset.</li>
 * <li>Calling convention from RT_API to RT_CDECL.</li>
 * <li>Orders of sources and destinations in prototypes.</li>
 * <li>RT_N into int.</li>
 * </ul>
 */

/* There is no point to use this header if we use CRT. */
#ifndef RT_DEFINE_USE_CRT

/* Allow to define memcmp, memcpy and memset while they are intrinsic and intrinsics are activated. */
#ifdef RT_DEFINE_VC

/* memmove is not available as intrinsic. */
#pragma function(memcmp, memcpy, memset)

#endif

int RT_CDECL memcmp(const void* lpArea1, const void* lpArea2, size_t unSize)
{
  RT_UN unWordsCount;
  const RT_UN* lpWordArea1;
  const RT_UN* lpWordArea2;
  RT_UN unRemainder;
  const RT_UCHAR8* lpUCharArea1;
  const RT_UCHAR8* lpUCharArea2;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_UN);
  if (unWordsCount)
  {
    lpWordArea1 = lpArea1;
    lpWordArea2 = lpArea2;
    for (unI = 0; unI < unWordsCount; unI++)
    {
      if (lpWordArea1[unI] != lpWordArea2[unI])
      {
        unRemainder = unSize - unI * sizeof(RT_UN);
        lpUCharArea1 = (RT_UCHAR8*)&lpWordArea1[unI];
        lpUCharArea2 = (RT_UCHAR8*)&lpWordArea2[unI];
        goto handleRemainder;
      }
    }
    unRemainder = unSize % sizeof(RT_UN);
    lpUCharArea1 = (RT_UCHAR8*)&lpWordArea1[unWordsCount];
    lpUCharArea2 = (RT_UCHAR8*)&lpWordArea2[unWordsCount];
  }
  else
  {
    unRemainder = unSize;
    lpUCharArea1 = lpArea1;
    lpUCharArea2 = lpArea2;
  }

handleRemainder:
  unI = 0;
  while ((unI < unRemainder) && (lpUCharArea1[unI] == lpUCharArea2[unI]))
    unI++;
  if (unI == unRemainder) unI--;
  return lpUCharArea1[unI] - lpUCharArea2[unI];
}

void* RT_CDECL memcpy(void* lpDestination, const void* lpSource, size_t unSize)
{
  RT_UN unWordsCount;
  const RT_UN* lpWordSource;
  RT_UN* lpWordDestination;
  RT_UN unRemainder;
  const RT_CHAR8* lpCharSource;
  RT_CHAR8* lpCharDestination;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_UN);
  if (unWordsCount)
  {
    lpWordSource = lpSource;
    lpWordDestination = lpDestination;
    for (unI = 0; unI < unWordsCount; unI++)
    {
      lpWordDestination[unI] = lpWordSource[unI];
    }
    unRemainder = unSize % sizeof(RT_UN);
    if (unRemainder)
    {
      lpCharSource = (RT_CHAR8*)&lpWordSource[unWordsCount];
      lpCharDestination = (RT_CHAR8*)&lpWordDestination[unWordsCount];
      for (unI = 0; unI < unRemainder; unI++)
      {
        lpCharDestination[unI] = lpCharSource[unI];
      }
    }
  }
  else
  {
    lpCharSource = lpSource;
    lpCharDestination = lpDestination;
    for (unI = 0; unI < unSize; unI++)
    {
      lpCharDestination[unI] = lpCharSource[unI];
    }
  }

  return lpDestination;
}

void* RT_CDECL memmove(void* lpDestination, const void* lpSource, size_t unSize)
{
  RT_UN unWordsCount;
  const RT_UN* lpWordSource;
  RT_UN* lpWordDestination;
  RT_UN unRemainder;
  const RT_CHAR8* lpCharSource;
  RT_CHAR8* lpCharDestination;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_UN);
  if (lpDestination > lpSource)
  {
    if (unWordsCount)
    {
      lpWordSource = (RT_UN*)&((RT_CHAR8*)lpSource)[unSize];
      lpWordDestination = (RT_UN*)&((RT_CHAR8*)lpDestination)[unSize];
      for (unI = 1; unI <= unWordsCount; unI++)
      {
        lpWordDestination[-(RT_N)unI] = lpWordSource[-(RT_N)unI];
      }
      unRemainder = unSize % sizeof(RT_UN);
      lpCharSource = (RT_CHAR8*)&lpWordSource[-(RT_N)unWordsCount];
      lpCharDestination = (RT_CHAR8*)&lpWordDestination[-(RT_N)unWordsCount];;
    }
    else
    {
      unRemainder = unSize;
      lpCharSource = &((RT_CHAR8*)lpSource)[unSize];
      lpCharDestination = &((RT_CHAR8*)lpDestination)[unSize];
    }
    for (unI = 1; unI <= unRemainder; unI++)
    {
      lpCharDestination[-(RT_N)unI] = lpCharSource[-(RT_N)unI];
    }
  }
  else
  {
    /* No overwriting possible, same code as RtCopyMemory. */
    if (unWordsCount)
    {
      lpWordSource = lpSource;
      lpWordDestination = lpDestination;
      for (unI = 0; unI < unWordsCount; unI++)
      {
        lpWordDestination[unI] = lpWordSource[unI];
      }
      unRemainder = unSize % sizeof(RT_UN);
      if (unRemainder)
      {
        lpCharSource = (RT_CHAR8*)&lpWordSource[unWordsCount];
        lpCharDestination = (RT_CHAR8*)&lpWordDestination[unWordsCount];
        for (unI = 0; unI < unRemainder; unI++)
        {
          lpCharDestination[unI] = lpCharSource[unI];
        }
      }
    }
    else
    {
      lpCharSource = lpSource;
      lpCharDestination = lpDestination;
      for (unI = 0; unI < unSize; unI++)
      {
        lpCharDestination[unI] = lpCharSource[unI];
      }
    }
  }

  return lpDestination;
}

void* RT_CDECL memset(void* lpArea, int nValue, size_t unSize)
{
  RT_UN unWordsCount;
  RT_N* lpWordArea;
  RT_N nWord;
  RT_UN unRemainder;
  RT_CHAR8* lpCharArea;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_N);
  if (unWordsCount)
  {
    if (nValue)
    {
#ifdef RT_DEFINE_64
      nWord = 0x0101010101010101 * (RT_UCHAR8)nValue;
#else
      nWord = 0x01010101 * (RT_UCHAR8)nValue;
#endif
    }
    else
    {
      nWord = 0;
    }

    lpWordArea = lpArea;
    for (unI = 0; unI < unWordsCount; unI++)
    {
      lpWordArea[unI] = nWord;
    }
    unRemainder = unSize % sizeof(RT_N);
    if (unRemainder)
    {
      lpCharArea = (RT_CHAR8*)&lpWordArea[unWordsCount];
      for (unI = 0; unI < unRemainder; unI++)
      {
        lpCharArea[unI] = (RT_CHAR8)nValue;
      }
    }
  }
  else
  {
    lpCharArea = lpArea;
    for (unI = 0; unI < unSize; unI++)
    {
      lpCharArea[unI] = (RT_CHAR8)nValue;
    }
  }

  return lpArea;
}

#endif

#endif /* RT_WIN32_EX_MEM_H */
