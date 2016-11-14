#ifndef RT_WIN32_EX_MEM_H
#define RT_WIN32_EX_MEM_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Sometimes compiler generates calls to memcmp/memcpy/memmove/memset without using intrinsic functions.<br>
 * This header can define corresponding symbols when CRT is not used.
 *
 * <p>
 * Partial copy from RtMemory.c, changing:
 * </p>
 * <ul>
 * <li>function names to memcmp, memcpy, memmove and memset.</li>
 * <li>Calling convention from RT_API to RT_CDECL.</li>
 * <li>RT_UN into RT_SIZE_T.</li>
 * <li>RT_N into int.</li>
 * </ul>
 */

/* There is no point to use this header if we use CRT. */
#ifndef RT_DEFINE_USE_CRT

/* Allow to define memcmp, memcpy and memset while they are intrinsic and intrinsics are activated. */
#ifdef RT_DEFINE_VC
#pragma function(memcmp, memcpy, memset)
#endif

#ifdef RT_DEFINE_VC

typedef size_t RT_SIZE_T;

#else /* NOT RT_DEFINE_VC */

typedef RT_UN RT_SIZE_T;

#endif

int RT_CDECL memcmp(void* lpArea1, void* lpArea2, RT_SIZE_T nSize)
{
  RT_CHAR8* lpCharArea1;   /* Facilite l'acc�s aux octets de lpArea1              */
  RT_CHAR8* lpCharArea2;   /* Idem pour lpArea2                                   */
  RT_SIZE_T nI;

  lpCharArea1 = (RT_CHAR8*)lpArea1;
  lpCharArea2 = (RT_CHAR8*)lpArea2;
  nI = 0;
  while ((nI < nSize) && (lpCharArea1[nI] == lpCharArea2[nI]))
    nI++;
  if (nI == nSize) nI--;
  return lpCharArea1[nI] - lpCharArea2[nI];
}

void* RT_CDECL memcpy(void* lpSource, void* lpDestination, RT_SIZE_T nSize)
{
  RT_CHAR8* lpCharDest;    /* Facilite l'acc�s � la zone de destination           */
  RT_CHAR8* lpCharSrc;     /* Facilite l'acc�s � la zone source                   */
  RT_SIZE_T nI;

  lpCharDest = lpDestination;
  lpCharSrc = lpSource;
  for (nI = 0; nI < nSize; nI++)
    lpCharDest[nI] = lpCharSrc[nI];

  return lpDestination;
}

void* RT_CDECL memmove(void* lpSource, void* lpDestination, RT_SIZE_T nSize)
{
  RT_CHAR8* lpCharDest;    /* Facilite l'acc�s � la zone de destination           */
  RT_CHAR8* lpCharSrc;     /* Facilite l'acc�s � la zone source                   */
  RT_SIZE_T nI;

  lpCharDest = lpDestination;
  lpCharSrc = lpSource;

  if (lpDestination > lpSource)
  {
    for (nI = nSize - 1; nI != 0; nI--)
      lpCharDest[nI] = lpCharSrc[nI];
    lpCharDest[0] = lpCharSrc[0];
  }
  else
    for (nI = 0; nI < nSize; nI++)
      lpCharDest[nI] = lpCharSrc[nI];

  return lpDestination;
}

void* RT_CDECL memset(void* lpArea, int nValue, RT_SIZE_T nSize)
{
  RT_UCHAR8* lpByteArea;    /* Facilite l'acc�s � la zone                        */
  RT_SIZE_T nI;

  lpByteArea = (RT_UCHAR8*)lpArea;
  for (nI = 0; nI < nSize; nI++)
    lpByteArea[nI] = nValue;
  return lpArea;
}

#endif

#endif /* RT_WIN32_EX_MEM_H */