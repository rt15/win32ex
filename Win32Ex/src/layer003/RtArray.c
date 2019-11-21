#include "layer003/RtArray.h"

#include "layer001/RtMemory.h"
#include "layer002/RtError.h"

RT_ARRAY* RT_API RtArray_Create(RT_ARRAY* lpArray, RT_GENERIC* lpData, RT_UN32 unGenericTypeSize, RT_UN32 unCapacity)
{
  lpArray->unGenericTypeSize = unGenericTypeSize;
  lpArray->lpData = lpData;
  lpArray->unCapacity = unCapacity;
  lpArray->unSize = 0;
  lpArray->unFlags = 0;
  return lpArray;
}

RT_ARRAY* RT_API RtDynamicArray_Create(RT_DYNAMIC_ARRAY* lpDynamicArray,
                                       RT_GENERIC* lpData, RT_UN32 unGenericTypeSize, RT_UN32 unCapacity,
                                       RT_HEAP** lpHeap)
{
  lpDynamicArray->rtArray.unGenericTypeSize = unGenericTypeSize;
  lpDynamicArray->rtArray.lpData = lpData;
  lpDynamicArray->rtArray.unCapacity = unCapacity;
  lpDynamicArray->rtArray.unSize = 0;
  lpDynamicArray->rtArray.unFlags = RT_ARRAY_FLAG_DYNAMIC;
  lpDynamicArray->lpHeap = lpHeap;
  return &lpDynamicArray->rtArray;
}

RT_B RT_API RtArray_Append(RT_ARRAY* lpArray, RT_ARRAY* lpOtherArray)
{
  RT_UN32 unInitialSize;
  RT_CHAR8* lpInArray;
  RT_B bResult;

  unInitialSize = lpArray->unSize;

  if (!RtArray_SetSize(lpArray, lpArray->unSize + lpOtherArray->unSize)) goto handle_error;

  lpInArray = lpArray->lpData;

  RT_MEMORY_COPY(lpOtherArray->lpData, &lpInArray[lpArray->unGenericTypeSize * unInitialSize], lpArray->unGenericTypeSize * lpOtherArray->unSize);

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtArray_SetSize(RT_ARRAY* lpArray, RT_UN32 unSize)
{
  RT_DYNAMIC_ARRAY* lpDynamicArray;
  RT_HEAP** lpHeap;
  void* lpNewArea;
  RT_B bResult;

  if (unSize > lpArray->unCapacity)
  {
    if (lpArray->unFlags & RT_ARRAY_FLAG_DYNAMIC)
    {
      /* Dynamic array, we will allocate some space. */
      lpDynamicArray = (RT_DYNAMIC_ARRAY*)lpArray;
      lpHeap = lpDynamicArray->lpHeap;

      if (lpArray->unFlags & RT_ARRAY_FLAG_DYNAMIC_BUFFER)
      {
        /* The current buffer has been already allocated in the heap. Perform a reallocation. */
        if (!(*lpHeap)->lpReAlloc(lpHeap, &lpArray->lpData, unSize * lpArray->unGenericTypeSize)) goto handle_error;
      }
      else
      {
        /* The current buffer has been provided by the user (from stack or anything). */
        if (!(*lpHeap)->lpAlloc(lpHeap, &lpNewArea, unSize * lpArray->unGenericTypeSize, _R("array"))) goto handle_error;

        /* Copy from the existing provided buffer to the new one. */
        RT_MEMORY_COPY(lpArray->lpData, lpNewArea, lpArray->unSize * lpArray->unGenericTypeSize);

        /* The allocated buffer is now the new array buffer. */
        lpArray->lpData = lpNewArea;

        /* The buffer is now allocated from the heap. */
        lpArray->unFlags = lpArray->unFlags | RT_ARRAY_FLAG_DYNAMIC_BUFFER;
      }
      lpArray->unCapacity = unSize;
    }
    else
    {
      /* Static array, no way to allocate more resources. */
      RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
      goto handle_error;
    }
  }

  lpArray->unSize = unSize;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtDynamicArray_Free(RT_DYNAMIC_ARRAY* lpDynamicArray)
{
  RT_HEAP** lpHeap;
  RT_B bResult;

  if (lpDynamicArray->rtArray.unFlags & RT_ARRAY_FLAG_DYNAMIC_BUFFER)
  {
    lpHeap = lpDynamicArray->lpHeap;

    /* Free the heap allocated buffer. */
    if (!(*lpHeap)->lpFree(lpHeap, &lpDynamicArray->rtArray.lpData)) goto handle_error;
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
