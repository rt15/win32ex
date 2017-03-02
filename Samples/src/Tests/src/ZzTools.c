#include "ZzTools.h"

void RT_CALL ZzWriteLastErrorMessage(RT_CHAR* lpLabel)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;

  unWritten = 0;
  RtCopyString(lpLabel, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &unWritten);
  RtGetLastErrorMessage(&lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);
  RtCopyStringWithSize(_R("\n"), 1, &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);

  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);
}
