#include "ZzTests.h"

RT_B RT_CALL ZzTestAtomic()
{
  RT_N nVar;
  RT_N32 nVar32;
  RT_B bResult;

  nVar = 12;
  if (RT_ATOMIC_LOAD(nVar) != 12) goto handle_error;
  RT_ATOMIC_STORE(nVar, 13);
  if (nVar != 13) goto handle_error;

  nVar32 = 12;
  if (RT_ATOMIC_LOAD32(nVar32) != 12) goto handle_error;
  RT_ATOMIC_STORE32(nVar32, 13);
  if (nVar32 != 13) goto handle_error;

  nVar = 0;
  if (!RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto handle_error;
  if (RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto handle_error;
  if (RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto handle_error;
  RT_ATOMIC_RELEASE(nVar);
  if (!RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto handle_error;
  if (RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto handle_error;
  if (RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto handle_error;
  RT_ATOMIC_RELEASE(nVar);

  nVar32 = 0;
  if (!RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto handle_error;
  if (RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto handle_error;
  if (RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto handle_error;
  RT_ATOMIC_RELEASE(nVar32);
  if (!RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto handle_error;
  if (RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto handle_error;
  if (RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto handle_error;
  RT_ATOMIC_RELEASE32(nVar32);

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
