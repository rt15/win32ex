#ifndef RT_ENV_VARS_H
#define RT_ENV_VARS_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Set of environement variables.
 *
 * <p>
 * The variables can be retrieved in 2 structures:
 * </p>
 * <ul>
 *   <li>The Windows structure with RtEnvVars_GetBlock (Concatenated null terminated string with a trailing null at the end).</li
 *   <li>The Linux structure with RtEnvVars_GetArray (Null terminated array of pointers to null terminated strings).</li>
 * </ul>
 *
 * <p>
 * Correspond to <tt>environ</tt> Linux variable and <tt>GetEnvironmentStrings</tt> in Windows.<br>
 * The main purpose of this structure is to be used as argument in RtProcess_Create.
 * </p>
 *
 * <p>
 * Manage a copy of the environment so variables are added/removed only in given RT_ENV_VARS.
 * </p>
 *
 * <p>
 * Environment variables are case sensitive under Linux but not under Windows.
 * </p>
 *
 * <p>
 * Maximum environment variable names size is <tt>RT_CHAR_HALF_BIG_STRING_SIZE</tt>.
 * </p>
 *
 * <p>
 * While parsing the variables, beware that Windows defines some variables starting with equals like <tt>=ExitCode=00000000</tt>.<br>
 * These variables are not visible using <tt>set</tt> command line and might be there for DOS compatibility reasons.
 * </p>
 */

typedef struct _RT_ENV_VARS
{
  /* Windows like concatenated null terminated strings with 2 nulls after the last string. */
  RT_CHAR* lpEnvVarsBlock;
  /* Linux like null terminated array of null terminated strings. */
  RT_CHAR** lpEnvVarsArray;
#ifdef RT_DEFINE_WINDOWS
  RT_B bWindowsBlock;     /* Whether lpEnvVarsBlock has been allocated by Windows or the library. */
#endif
}
RT_ENV_VARS;

RT_B RT_API RtEnvVars_Create(RT_ENV_VARS* lpEnvVars);

/**
 * Add or update <tt>lpEnvVarName</tt> within <tt>lpEnvVars</tt>.
TODO
RT_B RT_API RtSetEnvVarInEnvVars(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR* lpValue);
*/

RT_B RT_API RtEnvVars_GetBlock(RT_ENV_VARS* lpEnvVars, RT_CHAR** lpEnvVarsBlock);

RT_B RT_API RtEnvVars_GetArray(RT_ENV_VARS* lpEnvVars, RT_CHAR*** lpEnvVarsArray);

/**
 * Find whether a variable exists in <tt>lpEnvVars</tt>.
 */
RT_B RT_API RtEnvVars_Contains(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_B* lpContains);

/**
 * Retrieve value of <tt>lpEnvVarName</tt> in <tt>lpEnvVars</tt>.<br>
 * Fails if the variable does not exist.
 */
RT_B RT_API RtEnvVars_GetEnvVar(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

/**
 * Be sure that the variable doesn't exist yet.<br>
 * You can use <tt>RtEnvVars_RemoveEnvVar</tt> to ensure that.
 *
 * <p>
 * The <tt>RT_ENV_VARS</tt> should be considered corrupted in case of failure.
 * </p>
 */
RT_B RT_API RtEnvVars_AddEnvVar(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR* lpValue);

/**
 * Remove an environment variable of given <tt>RT_ENV_VARS</tt> if it exists.<br>
 * Can be used to ensure that an environment variable doesn't exist before adding it.
 */
RT_B RT_API RtEnvVars_RemoveEnvVar(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName);

/**
 * If the variable already exists, replace the value.<br>
 * If the variable does not exist, add the variable.
 */
RT_B RT_API RtEnvVars_MergeEnvVar(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR* lpValue);

RT_B RT_API RtEnvVars_Free(RT_ENV_VARS* lpEnvVars);

#endif /* RT_ENV_VARS_H */
