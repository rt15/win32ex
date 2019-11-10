#include "ZzLuaUtils.h"

RT_EXPORT void RT_STDCALL ZzPushLastErrorMessage(lua_State* lpLuaState)
{
  RT_CHAR lpLastErrorMessage[RT_CHAR_QUARTER_BIG_STRING_SIZE];
  RT_CHAR8 lpLastErrorMessage8[RT_CHAR_QUARTER_BIG_STRING_SIZE];
  RT_UN unOutputSize;

  if (!RtErrorMessage_GetLast(lpLastErrorMessage, RT_CHAR_QUARTER_BIG_STRING_SIZE, &unOutputSize)) goto handle_error;
  unOutputSize = RtEncoding_EncodeWithBuffer(lpLastErrorMessage, unOutputSize, 0, lpLastErrorMessage8, RT_CHAR_QUARTER_BIG_STRING_SIZE);
  if (unOutputSize == RT_TYPE_MAX_UN) goto handle_error;

  lua_pushstring(lpLuaState, lpLastErrorMessage8);

free_resources:
  return;

handle_error:
  lua_pushstring(lpLuaState, "Unexpected error.");
  goto free_resources;
}
