#include <RtWin32Ex.h>

RT_UN16 RT_CALL TtTestAtomic()
{
  RT_N nVar;
  RT_N32 nVar32;
  RT_UN16 unResult;

  unResult = 1;

  nVar = 12;
  if (RT_ATOMIC_LOAD(nVar) != 12) goto the_end;
  RT_ATOMIC_STORE(nVar, 13);
  if (nVar != 13) goto the_end;

  nVar32 = 12;
  if (RT_ATOMIC_LOAD32(nVar32) != 12) goto the_end;
  RT_ATOMIC_STORE32(nVar32, 13);
  if (nVar32 != 13) goto the_end;

  nVar = 0;
  if (!RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto the_end;
  if (RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto the_end;
  if (RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto the_end;
  RT_ATOMIC_RELEASE(nVar);
  if (!RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto the_end;
  if (RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto the_end;
  if (RT_ATOMIC_TRY_TO_ACQUIRE(nVar)) goto the_end;
  RT_ATOMIC_RELEASE(nVar);

  nVar32 = 0;
  if (!RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto the_end;
  if (RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto the_end;
  if (RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto the_end;
  RT_ATOMIC_RELEASE(nVar32);
  if (!RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto the_end;
  if (RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto the_end;
  if (RT_ATOMIC_TRY_TO_ACQUIRE32(nVar32)) goto the_end;
  RT_ATOMIC_RELEASE32(nVar32);

  unResult = 0;
the_end:
  return unResult;
}
