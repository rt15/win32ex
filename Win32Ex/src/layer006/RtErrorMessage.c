#include "layer006/RtErrorMessage.h"

#include "layer001/RtWin32ExOsHeaders.h"
#include "layer002/RtError.h"
#include "layer003/RtChar.h"
#include "layer005/RtConsole.h"

RT_B RT_API RtErrorMessage_GetLast(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize)
{
#ifdef RT_DEFINE_WINDOWS
  RT_UN unInputSize;
#else
  RT_CHAR* lpMessage;
#endif
  RT_B bResult;

#ifdef RT_DEFINE_WINDOWS
  /* Ensure that the 32 or 64 bits signed integer will fit into a DWORD. */
  if (unBufferSize < 0 || unBufferSize > RT_TYPE_MAX_N32)
  {
    RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  unInputSize = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                              NULL, GetLastError(),
                              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                              lpBuffer, (DWORD)unBufferSize, NULL);

  if (!unInputSize) goto handle_error;
  /* Remove trailing end of lines. */
  RtChar_RightTrimString(lpBuffer, unInputSize, lpOutputSize);
#else
  /* strerror_r is the thread safe version of strerror. */
  lpMessage = strerror_r(errno, lpBuffer, unBufferSize);
  if (!lpMessage)
  {
    /* Some standards says that lpMessage cannot be RT_NULL, some others not. */
    goto handle_error;
  }
  /* In the GNU strerror_r, the buffer might not be really used. */
  if (!RtChar_CopyString(lpMessage, lpBuffer, unBufferSize, lpOutputSize)) goto handle_error;
#endif

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtErrorMessage_WriteLast(RT_CHAR* lpPrefix)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  unWritten = 0;
  if (lpPrefix)
  {
    if (!RtChar_CopyString(lpPrefix, &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  }
  if (!RtErrorMessage_GetLast(                &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyStringWithSize(_R("\n"), 1, &lpBuffer[unWritten], RT_CHAR_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!RtConsole_WriteErrorWithSize(lpBuffer, unWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CDECL_API RtErrorMessage_WriteLastVariadic(void* lpNull, ...)
{
  va_list lpVaList;
  RT_B bResult;

  va_start(lpVaList, lpNull);
  bResult = RtErrorMessage_VWriteLast(lpVaList);
  va_end(lpVaList);

  return bResult;
}

RT_B RT_API RtErrorMessage_VWriteLast(va_list lpVaList)
{
  RT_CHAR lpBuffer[RT_CHAR_BIG_STRING_SIZE];
  RT_UN unOutputSize;
  RT_B bResult;

  if (!RtChar_VConcatStrings(lpVaList, lpBuffer, RT_CHAR_BIG_STRING_SIZE, &unOutputSize)) goto handle_error;
  if (!RtErrorMessage_WriteLast(lpBuffer)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
