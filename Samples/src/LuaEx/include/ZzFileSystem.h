#ifndef ZZ_FILE_SYSTEM_H
#define ZZ_FILE_SYSTEM_H

#include <rpr.h>

#include <lua.h>

RT_EXPORT rt_n32 RT_CDECL ZzIsDir(lua_State *lpLuaState);

RT_EXPORT rt_n32 RT_CDECL ZzIsFile(lua_State *lpLuaState);

RT_EXPORT rt_n32 RT_CDECL ZzIsFileOrDir(lua_State *lpLuaState);

#endif /* ZZ_FILE_SYSTEM_H */
