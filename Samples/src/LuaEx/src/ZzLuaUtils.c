#include "ZzLuaUtils.h"

RT_EXPORT void RT_STDCALL ZzPushLastErrorMessage(lua_State *lpLuaState)
{
	rt_char lpLastErrorMessage[RT_CHAR_QUARTER_BIG_STRING_SIZE];
	rt_char8 lpLastErrorMessage8[RT_CHAR_QUARTER_BIG_STRING_SIZE];
	rt_un output_size;

	if (!rt_error_message_get_last(lpLastErrorMessage, RT_CHAR_QUARTER_BIG_STRING_SIZE, &output_size)) goto error;
	output_size = rt_encoding_encode_with_buffer(lpLastErrorMessage, output_size, 0, lpLastErrorMessage8, RT_CHAR_QUARTER_BIG_STRING_SIZE);
	if (output_size == RT_TYPE_MAX_UN) goto error;

	lua_pushstring(lpLuaState, lpLastErrorMessage8);

free:
	return;

error:
	lua_pushstring(lpLuaState, "Unexpected error.");
	goto free;
}
