#include "ZzFileSystem.h"

#include <lauxlib.h>

void RT_STDCALL ZzCheckPath(lua_State *lpLuaState, rt_un type)
{
	rt_char path[RT_FILE_PATH_SIZE];
	rt_char8 *path8;
	rt_un written;

	path8 = (rt_char8*)luaL_checkstring(lpLuaState, 1);
	if (!path8) {
		rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
		goto error;
	}
	written = rt_encoding_decode_with_buffer(path8, -1, 0, path, RT_FILE_PATH_SIZE);
	if (written == -1) goto error;

	if (!rt_file_system_CheckPath(path, type)) goto error;

	lua_pushboolean(lpLuaState, 1);
free:
	return;
error:
	lua_pushboolean(lpLuaState, 0);
	goto free;
}

RT_EXPORT rt_n32 RT_CDECL ZzIsDir(lua_State *lpLuaState)
{
	ZzCheckPath(lpLuaState, RT_FILE_PATH_TYPE_DIR);
	return 1;
}

RT_EXPORT rt_n32 RT_CDECL ZzIsFile(lua_State *lpLuaState)
{
	ZzCheckPath(lpLuaState, RT_FILE_PATH_TYPE_FILE);
	return 1;
}

RT_EXPORT rt_n32 RT_CDECL ZzIsFileOrDir(lua_State *lpLuaState)
{
	ZzCheckPath(lpLuaState, RT_FILE_PATH_TYPE_FILE | RT_FILE_PATH_TYPE_DIR);
	return 1;
}
