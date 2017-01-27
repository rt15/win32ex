#include "layer003/RtMemory.h"

#include "layer002/RtErrorCode.h"

/**
 * Compare deux zones mémoire
 *
 * Renvoie 0 si les deux zones sont égales. Renvoie un nombre positif si le
 * premier octet qui ne concorde pas est dans lpArea1. Sinon renvoie un
 * nombre négatif.
 */
RT_N RT_API RtCompareMemory(void* lpArea1, void* lpArea2, RT_UN unSize)
{
  RT_CHAR8* lpCharArea1;   /* Facilite l'accès aux octets de lpArea1              */
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
  RT_CHAR8* lpCharDest;    /* Facilite l'accès à la zone de destination           */
  RT_CHAR8* lpCharSrc;     /* Facilite l'accès à la zone source                   */
  RT_UN unI;

  lpCharDest = lpDestination;
  lpCharSrc = lpSource;
  for (unI = 0; unI < unSize; unI++)
    lpCharDest[unI] = lpCharSrc[unI];

  return lpDestination;
}

/**
 * Copie unSize octets depuis source vers destination
 *
 * Fait comme si un tampon intermédiaire était utilisé pour éviter les
 * problèmes de recouvrement. Renvoie la destination.
 */
void* RT_API RtMoveMemory(void* lpSource, void* lpDestination, RT_UN unSize)
{
  RT_CHAR8* lpCharDest;    /* Facilite l'accès à la zone de destination           */
  RT_CHAR8* lpCharSrc;     /* Facilite l'accès à la zone source                   */
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
 * Met une zone mémoire de la taille spécifiée à la valeur spécifiée
 *
 * Renvoie lpArea.
 */
void* RT_API RtSetMemory(void* lpArea, RT_N nValue, RT_UN unSize)
{
  RT_UCHAR8* lpByteArea;    /* Facilite l'accès à la zone                        */
  RT_UN unI;

  lpByteArea = (RT_UCHAR8*)lpArea;
  /* TODO: Do not work per byte. */
  for (unI = 0; unI < unSize; unI++)
  {
    lpByteArea[unI] = (RT_UCHAR8)nValue;
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

RT_UN RT_API RtComputeChunksCount(RT_UN32 unSize, RT_UN32 unChunkSize)
{
  RT_UN unResult;

  unResult = unSize / unChunkSize;
  if (unSize % unChunkSize)
  {
    unResult++;
  }

  goto free_resources;
handle_error:
  unResult = RT_TYPE_MAX_UN;
free_resources:
  return unResult;
}
