#include "layer005/RtErrorMessage.h"

#include "layer001/RtWin32ExOsDefines.h"
#include "layer002/RtErrorCode.h"
#include "layer004/RtChar.h"

RT_B RT_API RtGetLastErrorMessage(RT_CHAR* lpBuffer, RT_N nBufferSize, RT_N *lpWritten)
{
  RT_N nWritten;
  RT_B bResult;
#ifdef RT_DEFINE_WINDOWS

#else
  RT_CHAR* lpMessage;
#endif

#ifdef RT_DEFINE_WINDOWS
  /* Ensure that the 32 or 64 bits signed integer will fit into a DWORD. */
  if (nBufferSize < 0 || nBufferSize > RT_TYPE_MAX_N32)
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  nWritten = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, GetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           lpBuffer, (DWORD)nBufferSize, NULL);

  if (!nWritten) goto handle_error;
  /* Remove trailing end of lines. */
  if (!RtRightTrimStringWithSize(lpBuffer, nWritten, &nWritten)) goto handle_error;
#else
  /* strerror_r is the thread safe version of strerror. */
  lpMessage = strerror_r(errno, lpBuffer, nBufferSize);
  if (!lpMessage)
  {
    /* Some standards says that lpMessage cannot be RT_NULL, some others not. */
    goto handle_error;
  }
  /* In the GNU strerror_r, the buffer might not be really used. */
  nWritten = 0;
  if (!RtCopyString(lpMessage, lpBuffer, nBufferSize, &nWritten)) goto handle_error;
#endif

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  *lpWritten += nWritten;
  return bResult;
}
