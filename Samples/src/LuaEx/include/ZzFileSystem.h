#ifndef ZZ_FILE_SYSTEM_H
#define ZZ_FILE_SYSTEM_H

#include <RtWin32Ex.h>

#include <lua.h>

RT_EXPORT RT_N32 RT_CDECL ZzIsDirectory(lua_State* lpLuaState);

RT_EXPORT RT_N32 RT_CDECL ZzIsFile(lua_State* lpLuaState);

RT_EXPORT RT_N32 RT_CDECL ZzIsFileOrDirectory(lua_State* lpLuaState);

#endif /* ZZ_FILE_SYSTEM_H */
