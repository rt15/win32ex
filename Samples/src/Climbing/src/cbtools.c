#include "cbtools.h"

void RT_CALL CbWriteSeparator()
{
  RtConsole_WriteString(_R("============\n"));
}

void RT_CALL CbWriteMenuTitle(RT_CHAR* lpLabel)
{
    RtConsole_WriteStringsOrErrors(RT_TRUE, _R("\n"), lpLabel, _R(" menu\n\n"), (RT_CHAR*)RT_NULL);
}
