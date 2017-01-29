#include "layer003/RtMemory.h"

#include "layer002/RtErrorCode.h"

RT_N RT_API RtCompareMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_CHAR8* lpCharArea1;   /* Facilite l'acc�s aux octets de lpArea1              */
  RT_CHAR8* lpCharArea2;   /* Idem pour lpArea2                                   */
  RT_UN unI;

  lpCharArea1 = (RT_CHAR8*)lpArea1;
  lpCharArea2 = (RT_CHAR8*)lpArea2;
  unI = 0;
  while ((unI < unSize) && (lpCharArea1[unI] == lpCharArea2[unI]))
    unI++;
  if (unI == unSize) unI--;
  return lpCharArea1[unI] - lpCharArea2[unI];
}

void* RT_API RtCopyMemory(void* lpSource, void* lpDestination, RT_UN unSize)
{
  RT_CHAR8* lpCharDest;    /* Facilite l'acc�s � la zone de destination           */
  RT_CHAR8* lpCharSrc;     /* Facilite l'acc�s � la zone source                   */
  RT_UN unI;

  lpCharDest = lpDestination;
  lpCharSrc = lpSource;
  for (unI = 0; unI < unSize; unI++)
    lpCharDest[unI] = lpCharSrc[unI];

  return lpDestination;
}

void* RT_API RtMoveMemory(void* lpSource, void* lpDestination, RT_UN unSize)
{
  RT_CHAR8* lpCharDest;    /* Facilite l'acc�s � la zone de destination           */
  RT_CHAR8* lpCharSrc;     /* Facilite l'acc�s � la zone source                   */
  RT_UN unI;

  lpCharDest = lpDestination;
  lpCharSrc = lpSource;

  if (lpDestination > lpSource)
  {
    for (unI = unSize - 1; unI != 0; unI--)
      lpCharDest[unI] = lpCharSrc[unI];
    lpCharDest[0] = lpCharSrc[0];
  }
  else
    for (unI = 0; unI < unSize; unI++)
      lpCharDest[unI] = lpCharSrc[unI];

  return lpDestination;
}

/**
 * Met une zone m�moire de la taille sp�cifi�e � la valeur sp�cifi�e
 *
 * Renvoie lpArea.
 */
void* RT_API RtSetMemory(void* lpArea, RT_UN unValue, RT_UN unSize)
{
  RT_UCHAR8* lpByteArea;    /* Facilite l'acc�s � la zone                        */
  RT_UN unI;

  lpByteArea = (RT_UCHAR8*)lpArea;
  /* TODO: Do not work per byte when possible. */
  for (unI = 0; unI < unSize; unI++)
  {
    lpByteArea[unI] = (RT_UCHAR8)unValue;
  }
  return lpArea;
}

/**
 * Affecte � z�ro un objet
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
