#include "ZzTools.h"

RT_UN16 ZzWriteLastErrorMessage(RT_CHAR* lpLabel)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_N nWritten;

  nWritten = 0;
  RtCopyString(lpLabel, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &nWritten);
  RtGetLastErrorMessage(&lpBuffer[nWritten], RT_CHAR_BIG_STRING_SIZE - nWritten, &nWritten);
  RtCopyStringWithSize(_R("\n"), 1, &lpBuffer[nWritten], RT_CHAR_BIG_STRING_SIZE - nWritten, &nWritten);

  RtWriteStringToConsoleWithSize(lpBuffer, nWritten);

  return 1;
}
