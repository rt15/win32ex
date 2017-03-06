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

#endif /* RT_ERROR_MESSAGE_H */
