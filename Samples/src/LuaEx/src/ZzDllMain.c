#include "ZzDllMain.h"

#include <lauxlib.h>
#include <lualib.h>

#include "ZzReplaceInFile.h"
#include "ZzFileSystem.h"

luaL_Reg zz_lpLibFunctions[] = {
	{ "ReplaceInFile", ZzReplaceInFile },
	{ "IsDir", ZzIsDir },
	{ "IsFile", ZzIsFile },
	{ "IsFileOrDir", ZzIsFileOrDir },
	{ RT_NULL, RT_NULL }
};

RT_EXPORT rt_n32 RT_CDECL ZzOpenLib(lua_State *lpLuaState)
{
	luaL_newlib(lpLuaState, zz_lpLibFunctions);
	lua_setglobal(lpLuaState, "LuaEx");
	return 1;
}
