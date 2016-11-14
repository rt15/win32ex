#include "layer002/RtErrorCode.h"

#include "layer001/RtWin32ExOsDefines.h"

/* The count of errors types defined in RtErrorCode.h. */
#define RT_ERROR_ERRORS_COUNT 7

/**
 * Used to translate from win32ex error code to OS error code.
 */
#ifdef RT_DEFINE_WINDOWS
RT_UN32 lpOSErrorByRtError[RT_ERROR_ERRORS_COUNT] = { ERROR_SUCCESS, ERROR_INSUFFICIENT_BUFFER, ERROR_BAD_ARGUMENTS, ERROR_NOT_ENOUGH_MEMORY, ERROR_ALREADY_EXISTS, ERROR_INVALID_FUNCTION, ERROR_ARITHMETIC_OVERFLOW };
#else
RT_N32 lpOSErrorByRtError[RT_ERROR_ERRORS_COUNT] = { 0, ERANGE, EINVAL, ENOMEM, EEXIST, EPERM, EOVERFLOW };
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
