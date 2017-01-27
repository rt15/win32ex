#ifndef RT_ENVIRONMENT_VARIABLE_H
#define RT_ENVIRONMENT_VARIABLE_H

#include "layer000/RtWin32ExTypes.h"

/**
 * <p>
 * In case of error, lpBuffer start with a zero character and <tt>lpWritten</tt> is left untouched.
 * </p>
 */
RT_B RT_API RtGetEnvironmentVariable(RT_CHAR* lpEnvironmentVariableName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

#endif /* RT_ENVIRONMENT_VARIABLE_H */
