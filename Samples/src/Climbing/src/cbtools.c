#include "cbtools.h"

void RT_CALL CbWriteSeparator()
{
  RtWriteStringToConsole(_R("============\n"));
}

void RT_CALL CbWriteMenuTitle(RT_CHAR* lpLabel)
{
    RtWriteStringsOrErrorsToConsole(RT_TRUE, _R("\n"), lpLabel, _R(" menu\n\n"), (RT_CHAR*)RT_NULL);
}
