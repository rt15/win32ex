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
RT_B RT_API RtGetEnvVar(RT_CHAR* lpEnvVarName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

/**
 *
 * @param lpValue Must not be RT_NULL.
 */
RT_B RT_API RtSetEnvVar(RT_CHAR* lpEnvVarName, RT_CHAR* lpValue);

/**
 *
 */
RT_B RT_API RtDeleteEnvVar(RT_CHAR* lpEnvVarName);

#endif /* RT_ENVIRONMENT_VARIABLE_H */
