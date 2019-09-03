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
RT_UN RT_API RtConsole_ReadLine(RT_CHAR* lpBuffer, RT_UN unBufferSize);

RT_B RT_API RtConsole_WriteString(RT_CHAR* lpString);

RT_B RT_API RtConsole_WriteStringWithSize(RT_CHAR* lpString, RT_UN unSize);

RT_B RT_API RtConsole_WriteError(RT_CHAR* lpString);

RT_B RT_API RtConsole_WriteErrorWithSize(RT_CHAR* lpString, RT_UN unSize);

RT_B RT_API RtConsole_WriteStringOrError(RT_CHAR* lpString, RT_B bString);

RT_B RT_API RtConsole_WriteStringOrErrorWithSize(RT_CHAR* lpString, RT_UN unSize, RT_B bString);

RT_B RT_CDECL_API RtConsole_WriteStringsOrErrors(RT_B bString, ...);

RT_B RT_API RtConsole_VWriteStringsOrErrors(va_list lpVaList, RT_B bString);

void RT_API RtConsole_Pause();

/**
 * Wait for the user to press a key.
 *
 * <p>
 * Beware that if the user press a key like 'é' and the console is in UTF-8, then two characters are actually entered: 0xC3 and 0xA9.<br>
 * The RtConsole_ReadChar will only return 0xC3 (195) and the other character will be discared.<br>
 * As a result this function is interresting to pause the execution and to retrieve ASCII characters only.
 * </p>
 *
 * @return The "first" entered character.
 */
RT_CHAR RT_API RtConsole_ReadChar();

#endif /* RT_CONSOLE_H */
