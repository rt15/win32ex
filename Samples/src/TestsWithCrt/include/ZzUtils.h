#ifndef ZZ_UTILS_H
#define ZZ_UTILS_H

#include <RtWin32Ex.h>

#include <windows.h>

RT_B RT_CALL ZzStartChrono(LARGE_INTEGER* lpStartCounter);

RT_B RT_CALL ZzStopChrono(RT_CHAR* lpIdentifier, LARGE_INTEGER* lpStartCounter);

#endif /* ZZ_UTILS_H */