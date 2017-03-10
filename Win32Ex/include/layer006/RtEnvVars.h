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
 *   <li>The Windows structure with RtGetEnvVarsBlock (Concatenated null terminated string with a trailing null at the end).</li
 *   <li>The Linux structure with RtGetEnvVarsArray (Null terminated array of pointers to null terminated strings).</li>
 * </ul>
 *
 * <p>
 * Correspond to <tt>environ</tt> Linux variable and <tt>GetEnvironmentStrings</tt> in Windows.<br>
 * The main purpose of this structure is to be used as argument in RtCreateProcess.
 * </p>
 *
 * <p>
 * Manage a copy of the environment so variables are added/removed only in given RT_ENV_VARS.
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

RT_B RT_API RtCreateEnvVars(RT_ENV_VARS* lpEnvVars);

/**
 * Add or update <tt>lpEnvVarName</tt> within <tt>lpEnvVars</tt>.
TODO
RT_B RT_API RtSetEnvVarInEnvVars(RT_ENV_VARS* lpEnvVars, RT_CHAR* lpEnvVarName, RT_CHAR* lpValue);
*/

RT_B RT_API RtGetEnvVarsBlock(RT_ENV_VARS* lpEnvVars, RT_CHAR** lpEnvVarsBlock);

RT_B RT_API RtGetEnvVarsArray(RT_ENV_VARS* lpEnvVars, RT_CHAR*** lpEnvVarsArray);

RT_B RT_API RtFreeEnvVars(RT_ENV_VARS* lpEnvVars);

#endif /* RT_ENV_VARS_H */