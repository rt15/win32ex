#include "layer005/RtErrorMessage.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer004/RtChar.h"

RT_B RT_API RtGetLastErrorMessage(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN *lpWritten)
{
  RT_UN unWritten;
  RT_B bResult;
#ifdef RT_DEFINE_WINDOWS

#else
  RT_CHAR* lpMessage;
#endif

#ifdef RT_DEFINE_WINDOWS
  /* Ensure that the 32 or 64 bits signed integer will fit into a DWORD. */
  if (unBufferSize < 0 || unBufferSize > RT_TYPE_MAX_N32)
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  unWritten = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, GetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           lpBuffer, (DWORD)unBufferSize, NULL);

  if (!unWritten) goto handle_error;
  /* Remove trailing end of lines. */
  if (!RtRightTrimStringWithSize(lpBuffer, unWritten, &unWritten)) goto handle_error;
#else
  /* strerror_r is the thread safe version of strerror. */
  lpMessage = strerror_r(errno, lpBuffer, unBufferSize);
  if (!lpMessage)
  {
    /* Some standards says that lpMessage cannot be RT_NULL, some others not. */
    goto handle_error;
  }
  /* In the GNU strerror_r, the buffer might not be really used. */
  unWritten = 0;
  if (!RtCopyString(lpMessage, lpBuffer, unBufferSize, &unWritten)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  *lpWritten += unWritten;
  return bResult;
}
