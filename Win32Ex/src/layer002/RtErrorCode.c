#include "layer002/RtErrorCode.h"

#include "layer001/RtWin32ExOsDefines.h"

/* The count of errors types defined in RtErrorCode.h. */
#define RT_ERROR_ERRORS_COUNT 9

/**
 * Used to translate from win32ex error code to OS error code.
 */
#ifdef RT_DEFINE_WINDOWS
RT_UN32 lpOSErrorByRtError[RT_ERROR_ERRORS_COUNT] = { ERROR_SUCCESS, ERROR_INSUFFICIENT_BUFFER, ERROR_BAD_ARGUMENTS, ERROR_NOT_ENOUGH_MEMORY, ERROR_ALREADY_EXISTS, ERROR_INVALID_FUNCTION, ERROR_ARITHMETIC_OVERFLOW, ERROR_IO_PENDING, WSAEWOULDBLOCK };
#else
RT_N32 lpOSErrorByRtError[RT_ERROR_ERRORS_COUNT] = { 0, ERANGE, EINVAL, ENOMEM, EEXIST, EPERM, EOVERFLOW, EWOULDBLOCK, EWOULDBLOCK };
#endif

void RT_API RtSetLastError(RT_UN unError)
{
  if (unError >= RT_ERROR_ERRORS_COUNT)
  {
    unError = RT_ERROR_BAD_ARGUMENTS;
  }
#ifdef RT_DEFINE_WINDOWS
  SetLastError(lpOSErrorByRtError[unError]);
#else
  errno = lpOSErrorByRtError[unError];
#endif
}

RT_B RT_API RtWouldBlockError()
{
#ifdef RT_DEFINE_WINDOWS
  DWORD unError;
#else
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  unError = GetLastError();
  bResult = (unError == WSAEWOULDBLOCK || unError == ERROR_IO_PENDING);
#else
  bResult = (errno == EWOULDBLOCK);
#endif
  return bResult;
}
