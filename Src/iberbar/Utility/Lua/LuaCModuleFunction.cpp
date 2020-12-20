
#include <iberbar/Base/Lua/LuaCModuleFunction.h>

void iberbar::CLuaCModuleFunction::onRegister( lua_State* pLuaState, int tb )
{
	if ( tb < 0 )
	{
		const luaL_Reg* fn = fns;
		while ( fn->name )
		{
			lua_pushcclosure( pLuaState, fn->func, 0 );
			lua_setglobal( pLuaState, fn->name );
			fn ++;
		}
	}
	else
	{
		const luaL_Reg* fn = fns;
		while ( fn->name )
		{
			lua_pushcclosure( pLuaState, fn->func, 0 );
			lua_pushstring( pLuaState, fn->name );
			lua_insert( pLuaState, -2 );  // swap value and key
			lua_settable( pLuaState, tb );
			fn ++;
		}
	}
}