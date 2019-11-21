#ifndef RT_CHAR8_H
#define RT_CHAR8_H

#include "layer000/RtWin32ExTypes.h"

RT_UN RT_API RtChar8_GetStringSize(RT_CHAR8* lpInput);

RT_N RT_API RtChar8_CompareStrings(RT_CHAR8* lpString1, RT_CHAR8* lpString2);

RT_B RT_API RtChar8_CopyString(RT_CHAR8* lpSource, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

RT_B RT_API RtChar8_CopyStringWithSize(RT_CHAR8* lpSource, RT_UN unSize, RT_CHAR8* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

RT_UN RT_API RtChar8_SearchString(RT_CHAR8* lpString, RT_CHAR8* lpSearched);

RT_B RT_API RtChar8_ConvertStringToInteger(RT_CHAR8* lpInput, RT_N* lpResult);
RT_B RT_API RtChar8_ConvertStringToIntegerWithSize(RT_CHAR8* lpInput, RT_UN unInputSize, RT_N* lpResult);

RT_B RT_API RtChar8_ConvertStringToUnsignedInteger(RT_CHAR8* lpInput, RT_UN* lpResult);
RT_B RT_API RtChar8_ConvertStringToUnsignedIntegerWithSize(RT_CHAR8* lpInput, RT_UN unInputSize, RT_UN* lpResult);

/**
 *
 * @param lpString Can be RT_NULL.
 * @return index of <tt>nSearched</tt> in <tt>lpString</tt> or RT_TYPE_MAX_UN if not found.
 */
RT_UN RT_API RtChar8_Search(RT_CHAR8* lpString, RT_CHAR8 nSearched);

#endif /* RT_CHAR8_H */
