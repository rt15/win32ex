#ifndef RT_CONSOLE_H
#define RT_CONSOLE_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Console functions.
 */

/**
 * @param nBufferSize Must be at least 3.
 * @return Read characters count and -1 in case of error.
 */
RT_N RT_API RtReadLineFromConsole(RT_CHAR* lpBuffer, RT_N nBufferSize);

RT_B RT_API RtWriteStringToConsole(RT_CHAR* lpString);

RT_B RT_API RtWriteStringToConsoleWithSize(RT_CHAR* lpString, RT_N nSize);

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
