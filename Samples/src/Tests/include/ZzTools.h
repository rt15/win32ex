#ifndef ZZ_TOOLS_H
#define ZZ_TOOLS_H

#include <RtWin32Ex.h>

RT_B RT_CALL ZzAdjustDirectory();

RT_B RT_CALL ZzCheckTextFile(RT_CHAR* lpFilePath, RT_HEAP** lpHeap, ...);

RT_B RT_CALL ZzWriteLinesToFile(RT_FILE* lpFile, RT_HEAP** lpHeap, ...);

#endif /* ZZ_TOOLS_H */
