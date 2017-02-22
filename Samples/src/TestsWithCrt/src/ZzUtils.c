#include "ZzUtils.h"

RT_B RT_CALL ZzStartChrono(RT_CHRONO* lpChrono)
{
  return RtCreateChrono(lpChrono);
}

RT_B RT_CALL ZzStopChrono(RT_CHAR* lpIdentifier, RT_CHRONO* lpChrono)
{
  RT_UN unDuration;
  RT_UN unWritten;
  RT_CHAR lpBuffer[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_B bResult;

  if (!RtGetChrono(lpChrono, &unDuration)) goto handle_error;
  unDuration = unDuration / 1000;

  unWritten = 0;
  if (!RtCopyString(lpIdentifier,            &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R(": "),                &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtConvertUIntegerToString(unDuration, &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtCopyString(_R(" ms\n"),             &lpBuffer[unWritten], RT_CHAR_THIRD_BIG_STRING_SIZE - unWritten, &unWritten)) goto handle_error;
  if (!RtWriteStringToConsoleWithSize(lpBuffer, unWritten)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
