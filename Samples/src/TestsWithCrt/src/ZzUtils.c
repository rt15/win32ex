#include "ZzUtils.h"

RT_B RT_CALL ZzStartChrono(LARGE_INTEGER* lpStartCounter)
{
  RT_B bResult;

  if (!QueryPerformanceCounter(lpStartCounter)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:  
  return bResult;
  
handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_CALL ZzStopChrono(RT_CHAR* lpIdentifier, LARGE_INTEGER* lpStartCounter)
{
  LARGE_INTEGER rtFrequency;
  LARGE_INTEGER rtEndCounter;
  RT_UN unDuration;
  RT_UN unWritten;
  RT_CHAR lpBuffer[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_B bResult;

  if (!QueryPerformanceCounter(&rtEndCounter)) goto handle_error;

  if (!QueryPerformanceFrequency(&rtFrequency)) goto handle_error;

  unDuration = (RT_UN)((rtEndCounter.QuadPart  - lpStartCounter->QuadPart) * 1000 / rtFrequency.QuadPart);

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
