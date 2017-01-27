#include "layer003/RtEnvironmentVariable.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"

RT_B RT_API RtGetEnvironmentVariable(RT_CHAR* lpEnvironmentVariableName, RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N* lpWritten)
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
  if (nBufferSize < 0 || nBufferSize > RT_TYPE_MAX_N32)
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  nReturnedValue = GetEnvironmentVariable(lpEnvironmentVariableName, lpBuffer, (DWORD)nBufferSize);
  if (!nReturnedValue)
  {
    goto handle_error;
  }
  if (nReturnedValue > RT_TYPE_MAX_N)
  {
    RtSetLastError(RT_ERROR_ARITHMETIC_OVERFLOW);
    goto handle_error;
  }

  /* If buffer is too small GetEnvironmentVariable return the required buffer size and lpBuffer is unknown. */
  if ((RT_N)nReturnedValue > nBufferSize)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }
  *lpWritten += nReturnedValue;

#else /* NOT RT_DEFINE_WINDOWS */
  lpReturnedValue = getenv(lpEnvironmentVariableName);
  if (!lpReturnedValue)
  {
   /* The getenv function might not set errno. The only failure reason might be that the variable is not found. */
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }

  /* Check buffer size. */
  nLength = strlen(lpReturnedValue);
  if (nLength + 1 > nBufferSize)
  {
    RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
    goto handle_error;
  }

  /* Copy result to buffer. */
  strcpy(lpBuffer, lpReturnedValue);
  *lpWritten += nLength;
#endif

  bResult = RT_TRUE;
free_resources:
  return bResult;

handle_error:
  if (nBufferSize > 0)
  {
    lpBuffer[0] = 0;
  }
  bResult = RT_FALSE;
  goto free_resources;
}
