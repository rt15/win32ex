#ifndef RT_ERROR_MESSAGE_H
#define RT_ERROR_MESSAGE_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Manage operating system error messages in an unified way for Windows and Linux.
 */

/**
 * Get the last error message.
 *
 * <p>
 * Both Windows and Linux provide a function to get last error code and a function to convert it to a string.
 * </p>
 *
 * @return RT_FALSE in case of error.
 */
RT_B RT_API RtGetLastErrorMessage(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten);

/**
 * Write last error message into console standard error stream.
 *
 * <p>
 * The optional <tt>lpPrefix</tt> is directly concatenated before the operating system error.
 * </p>
 */
RT_B RT_API RtWriteLastErrorMessage(RT_CHAR* lpPrefix);

/**
 * Write last error message into console standard error stream.
 *
 * <p>
 * The optional parameters are directly concatenated before the operating system error.
 * </p>
 *
 * @param lpNull Not used, should be RT_NULL.
 * @param ... Do not forget trailing (RT_CHAR*)RT_NULL.
 */
RT_B RT_CDECL_API RtWriteLastErrorMessageVariadic(void* lpNull, ...);

RT_B RT_API RtVWriteLastErrorMessage(va_list lpVaList);

#endif /* RT_ERROR_MESSAGE_H */
