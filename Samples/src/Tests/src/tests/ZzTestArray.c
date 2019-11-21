#include <RtWin32Ex.h>

RT_B RT_CALL ZzTestSetStaticArraySize()
{
  RT_ARRAY zzArray;
  RT_CHAR lpBuffer[64];
  RT_B bResult;

  RtArray_Create(&zzArray, lpBuffer, sizeof(RT_CHAR), 64);
  if (zzArray.unSize) goto handle_error;

  if (!RtArray_SetSize(&zzArray, 64)) goto handle_error;
  if (zzArray.unSize != 64) goto handle_error;

  if (RtArray_SetSize(&zzArray, 65)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSetDynamicArraySizeWithoutHeap(RT_HEAP** lpHeap)
{
  RT_DYNAMIC_ARRAY zzDynamicArray;
  RT_CHAR lpBuffer[64];
  RT_B bDynamicArrayCreated;
  RT_B bResult;

  bDynamicArrayCreated = RT_FALSE;

  RtDynamicArray_Create(&zzDynamicArray, lpBuffer, sizeof(RT_CHAR), 64, lpHeap);
  /* Hack: remove the heap from the array. */
  zzDynamicArray.lpHeap = RT_NULL;
  if (zzDynamicArray.rtArray.unSize) goto handle_error;

  if (!RtArray_SetSize(&zzDynamicArray.rtArray, 64)) goto handle_error;
  if (zzDynamicArray.rtArray.unSize != 64) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bDynamicArrayCreated)
  {
    bDynamicArrayCreated = RT_FALSE;
    if (!RtDynamicArray_Free(&zzDynamicArray) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestSetDynamicArraySize(RT_HEAP** lpHeap)
{
  RT_DYNAMIC_ARRAY zzDynamicArray;
  RT_UN32 lpBuffer[64];
  RT_UN32* lpData;
  RT_B bDynamicArrayCreated;
  RT_UN unI;
  RT_B bResult;

  bDynamicArrayCreated = RT_FALSE;

  RtDynamicArray_Create(&zzDynamicArray, lpBuffer, sizeof(RT_UN32), 64, lpHeap);
  if (!RtArray_SetSize(&zzDynamicArray.rtArray, 64)) goto handle_error;
  if (zzDynamicArray.rtArray.unFlags & RT_ARRAY_FLAG_DYNAMIC_BUFFER) goto handle_error;

  lpData = (RT_UN32*)zzDynamicArray.rtArray.lpData;
  for (unI = 0; unI < 64; unI++)
  {
    lpData[unI] = unI;
  }

  if (!RtArray_SetSize(&zzDynamicArray.rtArray, 800)) goto handle_error;
  if (zzDynamicArray.rtArray.unSize != 800) goto handle_error;

  if (!(zzDynamicArray.rtArray.unFlags & RT_ARRAY_FLAG_DYNAMIC_BUFFER)) goto handle_error;

  lpData = (RT_UN32*)zzDynamicArray.rtArray.lpData;

  for (unI = 0; unI < 64; unI++)
  {
    if (lpData[unI] != unI) goto handle_error;
  }
  for (unI = 0; unI < 800; unI++)
  {
    lpData[unI] = unI;
  }

  if (!RtArray_SetSize(&zzDynamicArray.rtArray, 8000)) goto handle_error;
  if (zzDynamicArray.rtArray.unSize != 8000) goto handle_error;

  lpData = (RT_UN32*)zzDynamicArray.rtArray.lpData;

  for (unI = 0; unI < 800; unI++)
  {
    if (lpData[unI] != unI) goto handle_error;
  }
  for (unI = 0; unI < 8000; unI++)
  {
    lpData[unI] = unI;
  }

  bResult = RT_SUCCESS;
free_resources:
  if (bDynamicArrayCreated)
  {
    bDynamicArrayCreated = RT_FALSE;
    if (!RtDynamicArray_Free(&zzDynamicArray) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestArray(RT_HEAP** lpHeap)
{
  RT_B bResult;

  if (!ZzTestSetStaticArraySize()) goto handle_error;
  if (!ZzTestSetDynamicArraySizeWithoutHeap(lpHeap)) goto handle_error;
  if (!ZzTestSetDynamicArraySize(lpHeap)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
