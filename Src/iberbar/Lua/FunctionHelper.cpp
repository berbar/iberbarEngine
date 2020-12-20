
#include <iberbar/Lua/FunctionHelper.h>
#include <iberbar/Lua/LoggingHelper.h>


void iberbar::Lua::CFunctionHelper::sExecuteFunction( lua_State* pLuaState, int nArgs )
{
	int nFunctionIndex = -(nArgs + 1);

	int nTraceCallback = 0;
	lua_getglobal( pLuaState, "__G__TRACKBACK__" );
	if ( !lua_isfunction( pLuaState, -1 ) )
	{
		lua_pop( pLuaState, 1 );
	}
	else
	{
		lua_rotate( pLuaState, nFunctionIndex - 1, 1 );
		nTraceCallback = nFunctionIndex - 1;
	}
	int nRet = lua_pcall( pLuaState, nArgs, 0, nTraceCallback );
	if ( nRet != 0 )
	{
		if ( nTraceCallback == 0 )
		{
			int type = lua_type( pLuaState, -1 );
			const char* err = lua_tostring( pLuaState, -1 );
			Lua::CLoggingHelper::sLog( Logging::ULevel::Error, err );
			lua_pop( pLuaState, 1 );
		}
		else
		{
			lua_pop( pLuaState, 2 );
		}
	}
}


void iberbar::Lua::CFunctionHelper::sExecuteGlobalFunction( lua_State* pLuaState, const char* strFunctionName )
{
	lua_getglobal( pLuaState, strFunctionName );
	if ( lua_isfunction( pLuaState, -1 ) == false )
	{
		lua_pop( pLuaState, 1 );
		return;
	}
	sExecuteFunction( pLuaState, 0 );
}
