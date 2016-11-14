#ifndef RT_ERROR_CODE_H
#define RT_ERROR_CODE_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Manage operating system error codes in an unified way for Windows and Linux.
 *
 * <p>
 * The purpose is to be able to set last error code whatever the underlying OS is.<br>
 * As a result this header could be used by any C file.
 * </p>
 */

/* The operation completed successfully. */
#define RT_ERROR_SUCCESS 0
/* The data area passed to a system call is too small. */
#define RT_ERROR_INSUFFICIENT_BUFFER 1
/* One or more arguments are not correct. */
#define RT_ERROR_BAD_ARGUMENTS 2
/* Not enough memory or memory issue, but not buffer related. */
#define RT_ERROR_NOT_ENOUGH_MEMORY 3
/* The "thing" already exists. */
#define RT_ERROR_ALREADY_EXISTS 4
/* A function has failed. Avoid this generic error code if possible. */
#define RT_ERROR_FUNCTION_FAILED 5
/* A numerical type cannot contain given value. */
#define RT_ERROR_ARITHMETIC_OVERFLOW 6

/**
 * Can be used to set the operating system last error code.
 *
 * The given <tt>nError</tt> will be mapped to an underlying operating system error code.
 *
 * @param nError The Win32Ex error code (RT_ERROR_XXXXX). If not known, RT_ERROR_BAD_ARGUMENTS is used.
 */
void RT_API RtSetLastError(RT_UN unError);

#endif /* RT_ERROR_CODE_H */
