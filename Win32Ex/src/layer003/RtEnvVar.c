#include "layer003/RtEnvVar.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"

RT_B RT_API RtEnvVar_Get(RT_CHAR* lpEnvVarName, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten)
{
#ifdef RT_DEFINE_WINDOWS
  DWORD nReturnedValue;
#else /* NOT RT_DEFINE_WINDOWS */
  char* lpReturnedValue;
  size_t nLength;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* Ensure that the 32 or 64 bits signed integer will fit into a DWORD. */
  if (unBufferSize < 0 || unBufferSize > RT_TYPE_MAX_N32)
  {
    RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  nReturnedValue = GetEnvironmentVariable(lpEnvVarName, lpBuffer, (DWORD)unBufferSize);
  if (!nReturnedValue)
  {
    goto handle_error;
  }
  if (nReturnedValue > RT_TYPE_MAX_N)
  {
    RtError_SetLast(RT_ERROR_ARITHMETIC_OVERFLOW);
    goto handle_error;
  }

  /* If buffer is too small GetEnvironmentVariable return the required buffer size and lpBuffer is unknown. */
  if (nReturnedValue > unBufferSize)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
  *lpWritten += nReturnedValue;

#else /* NOT RT_DEFINE_WINDOWS */
  lpReturnedValue = getenv(lpEnvVarName);
  if (!lpReturnedValue)
  {
   /* The getenv function might not set errno. The only failure reason might be that the variable is not found. */
    RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }

  /* Check buffer size. */
  nLength = strlen(lpReturnedValue);
  if (nLength + 1 > unBufferSize)
  {
    RtError_SetLast(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  /* Copy result to buffer. */
  strcpy(lpBuffer, lpReturnedValue);
  *lpWritten += nLength;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  if (unBufferSize > 0)
  {
    lpBuffer[0] = 0;
  }
  bResult = RT_FAILURE;
  goto free_resources;
}


RT_B RT_API RtEnvVar_Set(RT_CHAR* lpEnvVarName, RT_CHAR* lpValue)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  /* Returns FALSE and set last error in case of failure. */
  if (!SetEnvironmentVariable(lpEnvVarName, lpValue)) goto handle_error;

#else

  /* setenv make a copy of the name and the value while putenv make the process environment point to its argument. */
  /* Returns -1 and set errno in case of error. */
  if (setenv(lpEnvVarName, lpValue, 1) == -1) goto handle_error;

#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtEnvVar_Delete(RT_CHAR* lpEnvVarName)
{
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS

  /* Passing RT_NULL as value remove the variable from the process environment. */
  /* Returns FALSE and set last error in case of failure. */
  if (!SetEnvironmentVariable(lpEnvVarName, RT_NULL)) goto handle_error;

#else

  /* Returns -1 and set errno in case of error. */
  if (unsetenv(lpEnvVarName) == -1) goto handle_error;

#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
