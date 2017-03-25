#ifndef RT_CHAR8_H
#define RT_CHAR8_H

#include "layer000/RtWin32ExTypes.h"

RT_UN RT_API RtGetString8Size(RT_CHAR8* lpInput);

RT_N RT_API RtCompareString8s(RT_CHAR8* lpString1, RT_CHAR8* lpString2);

RT_B RT_API RtCopyString8(RT_CHAR8* lpSource, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_B RT_API RtCopyString8WithSize(RT_CHAR8* lpSource, RT_UN unSize, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_UN RT_API RtSearchString8(RT_CHAR8* lpString, RT_CHAR8* lpSearched);

RT_B RT_API RtConvertString8ToInteger(RT_CHAR8* lpInput, RT_N* lpResult);
RT_B RT_API RtConvertString8ToIntegerWithSize(RT_CHAR8* lpInput, RT_UN unInputSize, RT_N* lpResult);

RT_B RT_API RtConvertString8ToUInteger(RT_CHAR8* lpInput, RT_UN* lpResult);
RT_B RT_API RtConvertString8ToUIntegerWithSize(RT_CHAR8* lpInput, RT_UN unInputSize, RT_UN* lpResult);

/**
 *
 * @param lpString Can be RT_NULL.
 * @return index of <tt>nSearched</tt> in <tt>lpString</tt> or RT_TYPE_MAX_UN if not found.
 */
RT_UN RT_API RtSearchChar8(RT_CHAR8* lpString, RT_CHAR8 nSearched);

#endif /* RT_CHAR8_H */
