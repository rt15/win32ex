#ifndef CB_TOOLS_H
#define CB_TOOLS_H

#include <RtWin32Ex.h>

RT_UN16 RT_CALL CbWriteLastErrorMessage(RT_CHAR* lpLabel);

void RT_CALL CbWriteMenuTitle(RT_CHAR* lpLabel);

void RT_CALL CbWriteSeparator();

#endif /* CB_TOOLS_H */
