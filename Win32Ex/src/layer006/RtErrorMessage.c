#include "layer006/RtErrorMessage.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"
#include "layer003/RtChar.h"
#include "layer005/RtConsole.h"

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
    RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  unWritten = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, GetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           lpBuffer, (DWORD)unBufferSize, NULL);

  if (!unWritten) goto handle_error;
  /* Remove trailing end of lines. */
  if (!RtChar_RightTrimStringWithSize(lpBuffer, unWritten, &unWritten)) goto handle_error;
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
  if (!RtChar_CopyString(lpMessage, lpBuffer, unBufferSize, &unWritten)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  *lpWritten += unWritten;
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtWriteLastErrorMessage(RT_CHAR* lpPrefix)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (lpPrefix)
  {
    if (!RtChar_CopyString(lpPrefix, &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  }
  if (!RtGetLastErrorMessage(             &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtChar_CopyStringWithSize(_R("\n"), 1,  &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;

  if (!RtWriteErrorToConsoleWithSize(lpBuffer, unWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CDECL_API RtWriteLastErrorMessageVariadic(void* lpNull, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpNull);
  bResult = RtVWriteLastErrorMessage(lpVaList);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_API RtVWriteLastErrorMessage(va_list lpVaList)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  unWritten = 0;
  if (!RtChar_VConcatStrings(lpVaList, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (!RtWriteLastErrorMessage(lpBuffer)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
