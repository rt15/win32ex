#include "cbtools.h"

void RT_CALL CbWriteSeparator()
{
	rt_console_WriteCString(_R("============\n"));
}

void RT_CALL CbWriteMenuTitle(rt_char *lpLabel)
{
		rt_console_WriteStringsOrErrors(RT_TRUE, _R("\n"), lpLabel, _R(" menu\n\n"), (rt_char*)RT_NULL);
}
