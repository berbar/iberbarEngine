
#include <iberbar/Lua/LuaCallback.h>
#include <iberbar/Lua/LuaDevice.h>




lua_Integer iberbar::Lua::ToLuaCallback( lua_State* pLuaState, int nArg )
{
	if ( lua_isfunction( pLuaState, nArg ) == false )
		return 0;

	static lua_Integer s_function_ref_id = 0;
	s_function_ref_id++;

	lua_pushstring( pLuaState, RefId_Mapping_Function );
	lua_rawget( pLuaState, LUA_REGISTRYINDEX );
	lua_pushinteger( pLuaState, s_function_ref_id );
	lua_pushvalue( pLuaState, nArg );

	lua_rawset( pLuaState, -3 );
	lua_pop( pLuaState, 1 );

	return s_function_ref_id;
}


lua_Integer iberbar::Lua::ToLuaCallbackExtParam( lua_State* pLuaState, int nArg )
{
	if ( lua_istable( pLuaState, nArg ) == false )
		return 0;

	static lua_Integer s_function_extparams_ref_id = 0;
	s_function_extparams_ref_id++;

#ifdef _DEBUG
	lua_getglobal( pLuaState, RefId_Mapping_Function_ExtParams );
#else
	lua_pushstring( pLuaState, RefId_Mapping_Function_ExtParams );
	lua_rawget( pLuaState, LUA_REGISTRYINDEX );
#endif
	
	lua_pushinteger( pLuaState, s_function_extparams_ref_id );
	lua_pushvalue( pLuaState, nArg );

	lua_rawset( pLuaState, -3 );
	lua_pop( pLuaState, 1 );

	return s_function_extparams_ref_id;
}


bool iberbar::Lua::PushLuaCallback( lua_State* pLuaState, lua_Integer nLuaCallback )
{
	lua_pushstring( pLuaState, RefId_Mapping_Function );
	lua_rawget( pLuaState, LUA_REGISTRYINDEX );
	lua_pushinteger( pLuaState, nLuaCallback );
	lua_rawget( pLuaState, -2 );
	lua_remove( pLuaState, -2 );

	if ( !lua_isfunction( pLuaState, -1 ) )
	{
		lua_pop( pLuaState, 1 );
		return false;
	}

	return true;
}


bool iberbar::Lua::PushLuaCallbackExtParam( lua_State* pLuaState, lua_Integer nLuaExt )
{
#ifdef _DEBUG
	lua_getglobal( pLuaState, RefId_Mapping_Function_ExtParams );
#else
	lua_pushstring( pLuaState, RefId_Mapping_Function_ExtParams );
	lua_rawget( pLuaState, LUA_REGISTRYINDEX );
#endif

	lua_pushinteger( pLuaState, nLuaExt );
	lua_rawget( pLuaState, -2 );
	lua_remove( pLuaState, -2 );

	if ( !lua_istable( pLuaState, -1 ) )
	{
		lua_pop( pLuaState, 1 );
		return false;
	}

	return true;
}



iberbar::Lua::CLuaCallbackHandler::~CLuaCallbackHandler()
{
	if ( m_pLuaState != nullptr && GetLuaDevice( m_pLuaState ) )
	{
		if ( m_nLuaCallback > 0 )
		{
			lua_pushstring( m_pLuaState, RefId_Mapping_Function );
			lua_rawget( m_pLuaState, LUA_REGISTRYINDEX );
			lua_pushinteger( m_pLuaState, m_nLuaCallback );
			lua_pushnil( m_pLuaState );
			lua_rawset( m_pLuaState, -3 );
			lua_pop( m_pLuaState, 1 );
		}

		if ( m_nLuaExt > 0 )
		{
#ifdef _DEBUG
			lua_getglobal( m_pLuaState, RefId_Mapping_Function_ExtParams );
#else
			lua_pushstring( m_pLuaState, RefId_Mapping_Function_ExtParams );
			lua_rawget( m_pLuaState, LUA_REGISTRYINDEX );
#endif;
			lua_pushinteger( m_pLuaState, m_nLuaExt );
			lua_pushnil( m_pLuaState );
			lua_rawset( m_pLuaState, -3 );
			lua_pop( m_pLuaState, 1 );
		}
	}
}


//bool iberbar::Lua::CLuaCallbackHandler::PushFunction() const
//{
//	lua_pushstring( m_pLuaState, TOLUA_REFID_FUNCTION_MAPPING );
//	lua_rawget( m_pLuaState, LUA_REGISTRYINDEX );
//	lua_pushinteger( m_pLuaState, m_nLuaCallback );
//	lua_rawget( m_pLuaState, -2 );
//	lua_remove( m_pLuaState, -2 );
//
//	if ( !lua_isfunction( m_pLuaState, -1 ) )
//	{
//		lua_pop( m_pLuaState, 1 );
//		return false;
//	}
//
//	return true;
//}