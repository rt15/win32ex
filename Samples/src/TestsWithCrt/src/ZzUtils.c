#include "ZzUtils.h"

RT_B RT_CALL ZzStartChrono(RT_CHRONO* lpChrono)
{
  return RtChrono_Create(lpChrono);
}

RT_B RT_CALL ZzStopChrono(RT_CHAR* lpIdentifier, RT_CHRONO* lpChrono)
{
  RT_ARRAY zzMessage;
  RT_CHAR lpMessage[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_ARRAY zzConversionBuffer;
  RT_CHAR lpConversionBuffer[64];
  RT_UN unDuration;
  RT_B bResult;

  if (!RtChrono_GetDuration(lpChrono, &unDuration)) goto handle_error;
  unDuration = unDuration / 1000;
  RtArray_Create(&zzConversionBuffer, lpConversionBuffer, sizeof(RT_CHAR), 64);
  if (!RtChar_ConvertUnsignedIntegerToString(unDuration, &zzConversionBuffer)) goto handle_error;

  RtArray_Create(&zzMessage, lpMessage, sizeof(RT_CHAR), RT_CHAR_THIRD_BIG_STRING_SIZE);
  if (!RtChar_AppendCString(&zzMessage, lpIdentifier)) goto handle_error;
  if (!RtChar_AppendCString(&zzMessage, _R(": "))) goto handle_error;
  if (!RtArray_Append(&zzMessage, &zzConversionBuffer)) goto handle_error;
  if (!RtChar_AppendCString(&zzMessage, _R(" ms\n"))) goto handle_error;
  if (!RtConsole_WriteString(&zzMessage)) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
