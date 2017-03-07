#include "cbtools.h"

void RT_CALL CbWriteSeparator()
{
  RtWriteStringToConsole(_R("============\n"));
}

void RT_CALL CbWriteMenuTitle(RT_CHAR* lpLabel)
{
    RtWriteStringsToConsole(3, _R("\n"), lpLabel, _R(" menu\n\n"));
}
