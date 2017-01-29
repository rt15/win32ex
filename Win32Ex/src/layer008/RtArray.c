#include "layer008/RtArray.h"

#include "layer003/RtMemory.h"

/**
 * Initialise un tableau avec la taille sépcifiée
 */
void* RT_API RtCreateArray(void** lpArray, RT_UN unHeaderSize, RT_HEAP** lpHeap, RT_UN unSize, RT_UN unItemSize)
{
  RT_ARRAY_HEADER* lpArrayHeader;
  void* lpArea;

  if (unHeaderSize == 0)
  {
    unHeaderSize = sizeof(RT_ARRAY_HEADER);
  }

  *lpArray = RT_NULL;
  if ((*lpHeap)->lpAlloc(lpHeap, &lpArea, unHeaderSize + unSize * unItemSize, _R("array")))
  {
    *lpArray = (RT_CHAR8*)lpArea + unHeaderSize;
    lpArrayHeader = *lpArray;
    lpArrayHeader--;
    lpArrayHeader->unHeaderSize = unHeaderSize;
    lpArrayHeader->lpHeap = lpHeap;
    lpArrayHeader->unItemSize = unItemSize;
    lpArrayHeader->unSize = unSize;
  }
  return *lpArray;
}

/**
 * Redimensionne un tableau
 *
 * Si possible, renvoie le même pointeur. Sinon
 * alloue une nouvelle zone et recopie le contenu
 * de l'original. Si la taille de la nouvelle zone
 * est inférieure à la taille de l'ancienne, les
 * données sont tronquées.
 */
void* RT_API RtSetArraySize(void** lpArray, RT_UN unSize)
{
  RT_ARRAY_HEADER* lpArrayHeader;
  RT_UN unHeaderSize;
  void* lpArea;

  lpArrayHeader = *lpArray;
  lpArrayHeader--;
  unHeaderSize = lpArrayHeader->unHeaderSize;

  lpArea = (RT_CHAR8*)*lpArray - unHeaderSize;

  *lpArray = RT_NULL;
  /* TODO: Fix unavoidable memory leak if the array is an array of pointers... */
  if ((*lpArrayHeader->lpHeap)->lpReAlloc(lpArrayHeader->lpHeap, &lpArea, lpArea, unHeaderSize + unSize * lpArrayHeader->unItemSize))
  {
    *lpArray = (RT_CHAR8*)lpArea + unHeaderSize;
    lpArrayHeader = *lpArray;
    lpArrayHeader--;
    lpArrayHeader->unSize = unSize;
  }
  return *lpArray;
}

/**
 * Renvoie le nombre d'éléments du tableau
 */
RT_UN RT_API RtGetArraySize(void* lpArray)
{
  RT_ARRAY_HEADER* lpArrayHeader;

  lpArrayHeader = lpArray;
  lpArrayHeader--;
  return lpArrayHeader->unSize;
}

RT_UN RT_API RtGetArrayItemSize(void* lpArray)
{
  RT_ARRAY_HEADER* lpArrayHeader;

  lpArrayHeader = lpArray;
  lpArrayHeader--;
  return lpArrayHeader->unItemSize;
}

/**
 * Add a new item at the end of the array.
 * Return a pointer on the new item.
 * In case of error, *lpArray is set to NULL.
 */
void* RT_API RtNewArrayItem(void** lpArray, void** lpItem)
{
  RT_ARRAY_HEADER* lpArrayHeader;
  RT_UN unInitialSize;
  RT_UN unItemSize;

  lpArrayHeader = *lpArray;
  lpArrayHeader--;
  unInitialSize = lpArrayHeader->unSize;
  unItemSize = lpArrayHeader->unItemSize;
  if (RtSetArraySize(lpArray, unInitialSize + 1))
  {
    *lpItem = (void*)(((RT_UCHAR8*)*lpArray) + unInitialSize * unItemSize);
  }
  else
  {
    *lpItem = RT_NULL;
  }
  return *lpItem;
}

RT_UN RT_API RtNewArrayItemIndex(void** lpArray, RT_UN* lpItemIndex)
{
  RT_ARRAY_HEADER* lpArrayHeader;
  RT_UN unInitialSize;

  lpArrayHeader = *lpArray;
  lpArrayHeader--;
  unInitialSize = lpArrayHeader->unSize;
  if (RtSetArraySize(lpArray, unInitialSize + 1))
  {
    *lpItemIndex = unInitialSize;
  }
  else
  {
    *lpItemIndex = RT_TYPE_MAX_UN;
  }
  return *lpItemIndex;
}

void* RT_API RtDeleteArrayItemIndex(void** lpArray, RT_UN unItemIndex)
{
  RT_ARRAY_HEADER* lpArrayHeader;
  RT_UN unLastItemIndex;
  RT_UN unItemSize;

  lpArrayHeader = *lpArray;
  lpArrayHeader--;
  unLastItemIndex = lpArrayHeader->unSize - 1;
  unItemSize = lpArrayHeader->unItemSize;

  /* If the item to remove is not the last item */
  if (unItemIndex != unLastItemIndex)
  {
    /* Copy the last item in the deleted item */
    RtCopyMemory(((RT_UCHAR8*)*lpArray) + unLastItemIndex * unItemSize, ((RT_UCHAR8*)*lpArray) + unItemIndex * unItemSize, unItemSize);
  }
  return RtSetArraySize(lpArray, unLastItemIndex);
}

void* RT_API RtGetArrayItem(void* lpArray, RT_UN unItemIndex, void** lpItem)
{
  RT_ARRAY_HEADER* lpArrayHeader;

  lpArrayHeader = lpArray;
  lpArrayHeader--;
  *lpItem = ((RT_UCHAR8*)lpArray) + unItemIndex * lpArrayHeader->unItemSize;
  return *lpItem;
}

void* RT_API RtGetLastArrayItem(void* lpArray)
{
  RT_ARRAY_HEADER* lpArrayHeader;

  lpArrayHeader = lpArray;
  lpArrayHeader--;
  return ((RT_UCHAR8*)lpArray) + (lpArrayHeader->unSize - 1) * lpArrayHeader->unItemSize;
}

void* RT_API RtDeleteLastArrayItem(void** lpArray)
{
  RT_ARRAY_HEADER* lpArrayHeader;

  lpArrayHeader = *lpArray;
  lpArrayHeader--;
  return RtSetArraySize(lpArray, lpArrayHeader->unSize - 1);
}

RT_B RT_API RtFreeArray(void** lpArray)
{
  RT_ARRAY_HEADER* lpArrayHeader;
  RT_UN unHeaderSize;
  void* lpArea;
  RT_B bResult;

  if (*lpArray)
  {
    lpArrayHeader = *lpArray;
    lpArrayHeader--;

    /* Retrieve address of the allocated data (unHeaderSize can be different from sizeof(RT_ARRAY_HEADER)). */
    unHeaderSize = lpArrayHeader->unHeaderSize;
    lpArea = (RT_CHAR8*)*lpArray - unHeaderSize;

    bResult = ((*lpArrayHeader->lpHeap)->lpFree(lpArrayHeader->lpHeap, &lpArea));

    *lpArray = RT_NULL;
  }
  else
  {
    bResult = RT_SUCCESS;
  }

  return bResult;
}
