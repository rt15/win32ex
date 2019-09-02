#ifndef ZZ_TOOLS_H
#define ZZ_TOOLS_H

#include <RtWin32Ex.h>

RT_B RT_CALL ZzAdjustDirectory();

RT_B RT_CDECL ZzCheckTextFile(RT_CHAR* lpFilePath, RT_HEAP** lpHeap, ...);
RT_B RT_CALL ZzVCheckTextFile(va_list lpValist, RT_CHAR* lpFilePath, RT_HEAP** lpHeap);

RT_B RT_CDECL ZzWriteLinesToDevice(RT_IO_DEVICE* lpIoDevice, RT_HEAP** lpHeap, ...);
RT_B RT_CALL ZzVWriteLinesToDevice(va_list lpValist, RT_IO_DEVICE* lpIoDevice, RT_HEAP** lpHeap);

RT_N RT_CALL ZzCompareStringsArrays(RT_CHAR** lpStrings1, RT_CHAR** lpStrings2);

#endif /* ZZ_TOOLS_H */
