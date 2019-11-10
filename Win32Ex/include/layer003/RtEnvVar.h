#ifndef RT_ENV_VAR_H
#define RT_ENV_VAR_H

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
 * In case of error, lpBuffer starts with a zero character and <tt>lpOutputSize</tt> is left untouched.
 * </p>
 */
RT_B RT_API RtEnvVar_Get(RT_CHAR* lpEnvVarName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

/**
 *
 * @param lpValue Must not be RT_NULL.
 */
RT_B RT_API RtEnvVar_Set(RT_CHAR* lpEnvVarName, RT_CHAR* lpValue);

/**
 *
 */
RT_B RT_API RtEnvVar_Delete(RT_CHAR* lpEnvVarName);

#endif /* RT_ENV_VAR_H */
