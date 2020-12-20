

#pragma

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#define IBERBAR_LUA_API_BEGIN     extern "C" {
#define IBERBAR_LUA_API( func )   int func( lua_State* pLuaState )
#define IBERBAR_LUA_API_END       }



#define IBERBAR_LUAOPEN( mod ) int luaopen_##mod( lua_State* pLuaState )
#define IBERBAR_LUAOPEN_API( mod, api ) int luaopenapi_##mod##_##api( lua_State* pLuaState )
