#ifndef RT_CHAR8_H
#define RT_CHAR8_H

#include "layer000/RtWin32ExTypes.h"

RT_UN RT_API RtGetString8Size(RT_CHAR8* lpInput);

RT_N RT_API RtCompareString8s(RT_CHAR8* lpString1, RT_CHAR8* lpString2);

RT_B RT_API RtCopyString8WithSize(RT_CHAR8* lpSource, RT_UN unSize, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_UN RT_API RtSearchString8(RT_CHAR8* lpString, RT_CHAR8* lpSearched);

RT_B RT_API RtConvertString8ToInteger(RT_CHAR8* lpInput, RT_N* lpResult);

RT_B RT_API RtConvertString8ToUInteger(RT_CHAR8* lpInput, RT_UN* lpResult);

#endif /* RT_CHAR8_H */
