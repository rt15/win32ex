#include "layer001/RtMemory.h"

RT_N32 RT_API RtMemory_Compare(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_UN unWordsCount;
  RT_UN* lpWordArea1;
  RT_UN* lpWordArea2;
  RT_UN unRemainder;
  RT_UCHAR8* lpUCharArea1;
  RT_UCHAR8* lpUCharArea2;
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
    unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
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

void* RT_API RtMemory_Copy(void* lpSource, void* lpDestination, RT_UN unSize)
{
  RT_UN unWordsCount;
  RT_UN* lpWordSource;
  RT_UN* lpWordDestination;
  RT_UN unRemainder;
  RT_CHAR8* lpCharSource;
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
    unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
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

void* RT_API RtMemory_Move(void* lpSource, void* lpDestination, RT_UN unSize)
{
  RT_UN unWordsCount;
  RT_UN* lpWordSource;
  RT_UN* lpWordDestination;
  RT_UN unRemainder;
  RT_CHAR8* lpCharSource;
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
      unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
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
    /* No overwriting possible, same code as RtMemory_Copy. */
    if (unWordsCount)
    {
      lpWordSource = lpSource;
      lpWordDestination = lpDestination;
      for (unI = 0; unI < unWordsCount; unI++)
      {
        lpWordDestination[unI] = lpWordSource[unI];
      }
      unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
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

void* RT_API RtMemory_Set(void* lpArea, RT_N32 nValue, RT_UN unSize)
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
    unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
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

void* RT_API RtMemory_Zero(void* lpArea, RT_UN unSize)
{
  RT_UN unWordsCount;
  RT_N* lpWordArea;
  RT_UN unRemainder;
  RT_CHAR8* lpCharArea;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_N);
  if (unWordsCount)
  {
    lpWordArea = lpArea;
    for (unI = 0; unI < unWordsCount; unI++)
    {
      lpWordArea[unI] = 0;
    }
    unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
    if (unRemainder)
    {
      lpCharArea = (RT_CHAR8*)&lpWordArea[unWordsCount];
      for (unI = 0; unI < unRemainder; unI++)
      {
        lpCharArea[unI] = 0;
      }
    }
  }
  else
  {
    lpCharArea = lpArea;
    for (unI = 0; unI < unSize; unI++)
    {
      lpCharArea[unI] = 0;
    }
  }

  return lpArea;
}

void RT_API RtMemory_Swap(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_UN unWordsCount;
  RT_UN* lpWordArea1;
  RT_UN* lpWordArea2;
  RT_UN unTempWord;
  RT_UN unRemainder;
  RT_UCHAR8* lpCharArea1;
  RT_UCHAR8* lpCharArea2;
  RT_UCHAR8 unTempChar;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_UN);
  if (unWordsCount)
  {
    lpWordArea1 = lpArea1;
    lpWordArea2 = lpArea2;
    for (unI = 0; unI < unWordsCount; unI++)
    {
      unTempWord = lpWordArea1[unI];
      lpWordArea1[unI] = lpWordArea2[unI];
      lpWordArea2[unI] = unTempWord;
    }
    unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
    if (unRemainder)
    {
      lpCharArea1 = (RT_UCHAR8*)&lpWordArea1[unWordsCount];
      lpCharArea2 = (RT_UCHAR8*)&lpWordArea2[unWordsCount];
      for (unI = 0; unI < unRemainder; unI++)
      {
        unTempChar = lpCharArea1[unI];
        lpCharArea1[unI] = lpCharArea2[unI];
        lpCharArea2[unI] = unTempChar;
      }
    }
  }
  else
  {
    lpCharArea1 = lpArea1;
    lpCharArea2 = lpArea2;
    for (unI = 0; unI < unSize; unI++)
    {
        unTempChar = lpCharArea1[unI];
        lpCharArea1[unI] = lpCharArea2[unI];
        lpCharArea2[unI] = unTempChar;
    }
  }
}

RT_UN16* RT_API RtMemory_SwapBytes16(RT_UN16* lpSource, RT_UN16* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = RT_MEMORY_SWAP_BYTES16(lpSource[unI]);
  }

  return lpDestination;
}

RT_UN32* RT_API RtMemory_SwapBytes32(RT_UN32* lpSource, RT_UN32* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = RT_MEMORY_SWAP_BYTES32(lpSource[unI]);
  }

  return lpDestination;
}
