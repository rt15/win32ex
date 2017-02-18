#include "layer003/RtMemory.h"

#include "layer002/RtErrorCode.h"

RT_N RT_API RtCompareMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
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

void* RT_API RtCopyMemory(void* lpSource, void* lpDestination, RT_UN unSize)
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

void* RT_API RtMoveMemory(void* lpSource, void* lpDestination, RT_UN unSize)
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

void* RT_API RtSetMemory(void* lpArea, RT_UN unValue, RT_UN unSize)
{
  /* TODO: Optimize. */

  RT_UCHAR8* lpByteArea;    /* Facilite l'accès à la zone                        */
  RT_UN unI;

  lpByteArea = (RT_UCHAR8*)lpArea;
  for (unI = 0; unI < unSize; unI++)
  {
    lpByteArea[unI] = (RT_UCHAR8)unValue;
  }
  return lpArea;
}

/**
 * Affecte à zéro un objet
 */
void* RT_API RtZeroMemory(void* lpArea, RT_UN unSize)
{
  return RtSetMemory(lpArea, 0, unSize);
}

void RT_API RtSwapMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_CHAR8* lpChars1;
  RT_CHAR8* lpChars2;
  RT_CHAR8 nTemp;

  /* TODO: Optimize. */

  lpChars1 = lpArea1;
  lpChars2 = lpArea2;

  while (unSize--)
  {
    nTemp = *lpChars1;
    *lpChars1++ = *lpChars2;
    *lpChars2++ = nTemp;
  }
}

RT_UN16* RT_API RtSwapBytes16(RT_UN16* lpSource, RT_UN16* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = RT_MEMORY_SWAP_BYTES16(lpSource[unI]);
  }

  return lpDestination;
}

RT_UN32* RT_API RtSwapBytes32(RT_UN32* lpSource, RT_UN32* lpDestination, RT_UN unSize)
{
  RT_UN unI;

  for (unI = 0; unI < unSize; unI++)
  {
    lpDestination[unI] = RT_MEMORY_SWAP_BYTES32(lpSource[unI]);
  }

  return lpDestination;
}

RT_UN RT_API RtComputeChunksCount(RT_UN unSize, RT_UN unChunkSize)
{
  RT_UN unResult;

  if (!unChunkSize)
  {
    unResult = RT_TYPE_MAX_UN;
  }
  else
  {
    unResult = unSize / unChunkSize;
    if (unSize % unChunkSize)
    {
      unResult++;
    }
  }
  return unResult;
}
