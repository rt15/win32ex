#include "ZzLuaUtils.h"

RT_EXPORT void RT_STDCALL ZzPushLastErrorMessage(lua_State* lpLuaState)
{
  RT_CHAR lpLastErrorMessage[RT_CHAR_QUARTER_BIG_STRING_SIZE];
  RT_CHAR8 lpLastErrorMessage8[RT_CHAR_QUARTER_BIG_STRING_SIZE];
  RT_UN unWritten;

  unWritten = 0;
  if (!RtGetLastErrorMessage(lpLastErrorMessage, RT_CHAR_QUARTER_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  unWritten = RtEncoding_EncodeWithBuffer(lpLastErrorMessage, unWritten, 0, lpLastErrorMessage8, RT_CHAR_QUARTER_BIG_STRING_SIZE);
  if (unWritten == RT_TYPE_MAX_UN) goto handle_error;

  lua_pushstring(lpLuaState, lpLastErrorMessage8);

free_resources:
  return;

handle_error:
  lua_pushstring(lpLuaState, "Unexpected error.");
  goto free_resources;
}
