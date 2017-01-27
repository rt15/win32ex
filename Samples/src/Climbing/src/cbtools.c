#include "cbtools.h"

RT_UN16 RT_CALL CbWriteLastErrorMessage(RT_CHAR* lpLabel)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;

  unWritten = 0;
  RtCopyString(lpLabel, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &unWritten);
  RtGetLastErrorMessage(&lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);
  RtCopyStringWithSize(_R("\n"), 1, &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten);

  RtWriteStringToConsoleWithSize(lpBuffer, unWritten);

  return 1;
}

void RT_CALL CbWriteSeparator()
{
  RtWriteStringToConsole(_R("============\n"));
}

void RT_CALL CbWriteMenuTitle(RT_CHAR* lpLabel)
{
    RtWriteStringsToConsole(3, _R("\n"), lpLabel, _R(" menu\n\n"));
}
