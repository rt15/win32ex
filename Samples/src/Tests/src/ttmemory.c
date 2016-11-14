#include <RtWin32Ex.h>

RT_UN16 RT_CALL TtTestComputeChunksCount()
{
  RT_UN16 unResult;

  unResult = 1;

  if (RtComputeChunksCount(5, 5) != 1) goto the_end;
  if (RtComputeChunksCount(6, 5) != 2) goto the_end;
  if (RtComputeChunksCount(1, 1) != 1) goto the_end;
  if (RtComputeChunksCount(5, 6) != 1) goto the_end;
  if (RtComputeChunksCount(0, 1) != 0) goto the_end;
  if (RtComputeChunksCount(1, 0) != -1) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}

RT_UN16 RT_CALL TtTestMemory()
{
  RT_UN16 unResult;

  unResult = 1;

  if (TtTestComputeChunksCount()) goto the_end;

  unResult = 0;
the_end:
  return unResult;
}
