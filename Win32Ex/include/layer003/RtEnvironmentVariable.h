#ifndef RT_ENVIRONMENT_VARIABLE_H
#define RT_ENVIRONMENT_VARIABLE_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Environment variable manipulation.
 *
 * <p>
 * Beware that you cannot modify environment in a thread safe way under Linux.<br>
 * putenv()/setenv()/clearenv()/unsetenv() are not thread safe.<br>
 * Many functions are calling getenv which can crash if the environment is modified.
 * </p>
 *
 */

/**
 * Fails if the variable does not exist.
 *
 * <p>
 * In case of error, lpBuffer start with a zero character and <tt>lpWritten</tt> is left untouched.
 * </p>
 */
RT_B RT_API RtGetEnvironmentVariable(RT_CHAR* lpEnvironmentVariableName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

#endif /* RT_ENVIRONMENT_VARIABLE_H */
