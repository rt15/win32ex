#ifndef RT_CONSOLE_H
#define RT_CONSOLE_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Console functions.
 */

/**
 * @param unBufferSize Must be at least 3.
 * @return Read characters count and RT_TYPE_MAX_UN in case of error.
 */
RT_UN RT_API RtReadLineFromConsole(RT_CHAR* lpBuffer, RT_UN unBufferSize);

RT_B RT_API RtWriteStringToConsole(RT_CHAR* lpString);

RT_B RT_API RtWriteStringToConsoleWithSize(RT_CHAR* lpString, RT_UN unSize);

RT_B RT_API RtWriteErrorToConsole(RT_CHAR* lpString);

RT_B RT_API RtWriteErrorToConsoleWithSize(RT_CHAR* lpString, RT_UN unSize);

RT_B RT_CDECL_API RtWriteStringsToConsole(RT_UN unStringsCount, ...);

void RT_API RtPauseConsole();

/**
 * Wait for the user to press a key.
 *
 * <p>
 * Beware that if the user press a key like 'é' and the console is in UTF-8, then two characters are actually entered: 0xC3 and 0xA9.<br>
 * The RtReadCharFromConsole will only return 0xC3 (195) and the other character will be discared.<br>
 * As a result this function is interresting to pause the execution and to retrieve ASCII characters only.
 * </p>
 *
 * @return The "first" entered character.
 */
RT_CHAR RT_API RtReadCharFromConsole();

#endif /* RT_CONSOLE_H */
