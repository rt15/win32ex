#include "ZzFileSystem.h"

#include <lauxlib.h>

void RT_STDCALL ZzCheckPath(lua_State* lpLuaState, RT_UN unType)
{
  RT_CHAR lpPath[RT_FILE_SYSTEM_MAX_FILE_PATH];
  RT_CHAR8* lpPath8;
  RT_UN unWritten;

  lpPath8 = (RT_CHAR8*)luaL_checkstring(lpLuaState, 1);
  if (!lpPath8)
  {
    RtError_SetLast(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }
  unWritten = RtEncoding_DecodeWithBuffer(lpPath8, -1, 0, lpPath, RT_FILE_SYSTEM_MAX_FILE_PATH);
  if (unWritten == -1) goto handle_error;

  if (!RtCheckPath(lpPath, unType)) goto handle_error;

  lua_pushboolean(lpLuaState, 1);
free_resources:
  return;
handle_error:
  lua_pushboolean(lpLuaState, 0);
  goto free_resources;
}

RT_EXPORT RT_N32 RT_CDECL ZzIsDirectory(lua_State* lpLuaState)
{
  ZzCheckPath(lpLuaState, RT_FILE_SYSTEM_TYPE_DIRECTORY);
  return 1;
}

RT_EXPORT RT_N32 RT_CDECL ZzIsFile(lua_State* lpLuaState)
{
  ZzCheckPath(lpLuaState, RT_FILE_SYSTEM_TYPE_FILE);
  return 1;
}

RT_EXPORT RT_N32 RT_CDECL ZzIsFileOrDirectory(lua_State* lpLuaState)
{
  ZzCheckPath(lpLuaState, RT_FILE_SYSTEM_TYPE_FILE | RT_FILE_SYSTEM_TYPE_DIRECTORY);
  return 1;
}
