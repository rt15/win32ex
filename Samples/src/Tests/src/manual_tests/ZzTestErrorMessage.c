#include <RtWin32Ex.h>

RT_B RT_CALL ZzDisplayErrorMessage(RT_CHAR* lpErrorName, RT_UN unError)
{
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  RtError_SetLast(unError);

  unWritten = 0;
  if (!RtChar_CopyString(lpErrorName, &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtChar_CopyString(_R(" = "),   &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtErrorMessage_GetLast(        &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (unWritten != RtChar_GetCStringSize(lpBuffer)) goto handle_error;

  if (!RtChar_CopyString(_R("\n"), &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (!RtConsole_WriteStringWithSize(lpBuffer, unWritten));

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzTestErrorMessage()
{
  RT_CHAR lpBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_UN unOutputSize;
  RT_B bResult;

  if (!RtConsole_WriteCString(_R("## Error messages:\n\n"))) goto handle_error;

  if (RtChar_CopyString(_R("Long string."), lpBuffer, 3, &unOutputSize)) goto handle_error;
  if (unOutputSize != 2) goto handle_error;

  unWritten = 0;
  if (!RtChar_CopyString(_R("Error: \""), &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtErrorMessage_GetLast(            &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;

  if (unWritten != RtChar_GetCStringSize(lpBuffer)) goto handle_error;

  if (!RtChar_CopyString(_R("\"\n"), &lpBuffer[unWritten], RT_CHAR_HALF_BIG_STRING_SIZE - unWritten, &unOutputSize)) goto handle_error; unWritten += unOutputSize;
  if (!RtConsole_WriteStringWithSize(lpBuffer, unWritten));

  if (!RtConsole_WriteCString(_R("\n"))) goto handle_error;

  if (!ZzDisplayErrorMessage(_R("RT_ERROR_SUCCESS"),             RT_ERROR_SUCCESS))             goto handle_error;
  if (!ZzDisplayErrorMessage(_R("RT_ERROR_INSUFFICIENT_BUFFER"), RT_ERROR_INSUFFICIENT_BUFFER)) goto handle_error;
  if (!ZzDisplayErrorMessage(_R("RT_ERROR_BAD_ARGUMENTS"),       RT_ERROR_BAD_ARGUMENTS))       goto handle_error;
  if (!ZzDisplayErrorMessage(_R("RT_ERROR_NOT_ENOUGH_MEMORY"),   RT_ERROR_NOT_ENOUGH_MEMORY))   goto handle_error;
  if (!ZzDisplayErrorMessage(_R("RT_ERROR_ALREADY_EXISTS"),      RT_ERROR_ALREADY_EXISTS))      goto handle_error;
  if (!ZzDisplayErrorMessage(_R("RT_ERROR_FUNCTION_FAILED"),     RT_ERROR_FUNCTION_FAILED))     goto handle_error;
  if (!ZzDisplayErrorMessage(_R("RT_ERROR_ARITHMETIC_OVERFLOW"), RT_ERROR_ARITHMETIC_OVERFLOW)) goto handle_error;
  if (!ZzDisplayErrorMessage(_R("RT_ERROR_WOULD_BLOCK"),         RT_ERROR_WOULD_BLOCK))         goto handle_error;
  if (!ZzDisplayErrorMessage(_R("RT_ERROR_SOCKET_WOULD_BLOCK"),  RT_ERROR_SOCKET_WOULD_BLOCK))  goto handle_error;

  if (!RtConsole_WriteCString(_R("\n"))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
