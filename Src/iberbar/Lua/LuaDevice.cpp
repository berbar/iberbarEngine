

#include <iberbar/Lua/LuaDevice.h>
#include <iberbar/Lua/LuaCallback.h>
#include <iberbar/Lua/LoggingHelper.h>
#include <luaconf.h>


using namespace iberbar::Lua;


namespace iberbar
{
	namespace Lua
	{
		CLuaDevice* g_pUniqueLuaDevice = nullptr;
	}
}



iberbar::CLuaDevice::CLuaDevice()
	: CRef()
	, m_pLuaState( nullptr )
{
	g_pUniqueLuaDevice = this;
}

iberbar::CLuaDevice::~CLuaDevice()
{
	if ( m_pLuaState != nullptr )
	{
		lua_close( m_pLuaState );
		m_pLuaState = nullptr;
	}

	g_pUniqueLuaDevice = nullptr;
}

void iberbar::CLuaDevice::Initial()
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs( m_pLuaState );
	//lua_pushcfunction( m_pLuaState, luaopen_io );
	//lua_pushstring( m_pLuaState, LUA_IOLIBNAME );
	//lua_pcall( m_pLuaState, 1, 0, 0 );

	// 开启Lua回调模式
	lua_pushstring( m_pLuaState, Lua::RefId_Mapping_Function );
	lua_newtable( m_pLuaState );
	lua_rawset( m_pLuaState, LUA_REGISTRYINDEX );

#ifdef _DEBUG
	lua_newtable( m_pLuaState );
	lua_setglobal( m_pLuaState, Lua::RefId_Mapping_Function_ExtParams );
#else
	lua_pushstring( m_pLuaState, Lua::RefId_Mapping_Function_ExtParams );
	lua_newtable( m_pLuaState );
	lua_rawset( m_pLuaState, LUA_REGISTRYINDEX );
#endif
}


void iberbar::CLuaDevice::AddLuaPath( const char* path )
{
	lua_getglobal( m_pLuaState, "package" );
	lua_getfield( m_pLuaState, -1, "path" ); // get field "path" from table at top of stack (-1)
	std::string cur_path = lua_tostring( m_pLuaState, -1 ); // grab path string from top of stack
	cur_path.append( ";" ); // do your path magic here
	cur_path.append( path );
	lua_pop( m_pLuaState, 1 ); // get rid of the string on the stack we just pushed on line 5
	lua_pushstring( m_pLuaState, cur_path.c_str() ); // push the new one
	lua_setfield( m_pLuaState, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
	lua_pop( m_pLuaState, 1 ); // get rid of package table from top of stack
}


iberbar::CResult iberbar::CLuaDevice::ExecuteFile( const char* strFile )
{
	const char* err = nullptr;
	int nRet;
	nRet = luaL_loadfile( m_pLuaState, strFile );
	if ( nRet != 0 )
	{
		int type = lua_type( m_pLuaState, -1 );
		err = lua_tostring( m_pLuaState, -1 );
		Lua::CLoggingHelper::sLog( Logging::ULevel::Error, err );
		lua_pop( m_pLuaState, 1 );
		return MakeResult( ResultCode::Bad, err );
	}

	nRet = lua_pcall( m_pLuaState, 0, 0, 0 );
	if ( nRet != 0 )
	{
		int type = lua_type( m_pLuaState, -1 );
		err = lua_tostring( m_pLuaState, -1 );
		Lua::CLoggingHelper::sLog( Logging::ULevel::Error, err );
		//lua_pop( m_pLuaState, 1 );
		lua_pop( m_pLuaState, 2 );

		return MakeResult( ResultCode::Bad, err );
	}

	//lua_getglobal( m_pLuaState, "Main" );
	//if ( lua_isfunction( m_pLuaState, -1 ) == false )
	//{
	//	lua_pop( m_pLuaState, 1 );
	//}
	//else
	//{
	//	int functionIndex = lua_gettop( m_pLuaState );
	//	int traceCallback = 0;
	//	lua_getglobal( m_pLuaState, "__G__TRACKBACK__" );                        /* L: ... func arg1 arg2 ... G */
	//	if ( !lua_isfunction( m_pLuaState, -1 ) )
	//	{
	//		lua_pop( m_pLuaState, 1 );                                           /* L: ... func arg1 arg2 ... */
	//	}
	//	else
	//	{
	//		lua_rotate( m_pLuaState, functionIndex, 1 );                         /* L: ... G func arg1 arg2 ... */
	//		traceCallback = functionIndex;
	//	}
	//	nRet = lua_pcall( m_pLuaState, 0, 0, traceCallback );
	//	if ( nRet != 0 )
	//	{
	//		int type = lua_type( m_pLuaState, -1 );
	//		err = lua_tostring( m_pLuaState, -1 );
	//		Lua::CLoggingHelper::sLog( Logging::ULevel::Error, err );
	//		//lua_pop( m_pLuaState, 1 );
	//		lua_pop( m_pLuaState, 2 );

	//		return MakeResult( ResultCode::Bad, err );
	//	}
	//}
	return CResult();
}


iberbar::CResult iberbar::CLuaDevice::ExecuteSource( const char* strSource )
{
	const char* err = nullptr;
	if ( luaL_dostring( m_pLuaState, strSource ) )
	{
		err = lua_tostring( m_pLuaState, -1 );
		lua_pop( m_pLuaState, 1 );
		return MakeResult( ResultCode::Bad, err );
	}
	return CResult();
}


//void iberbar::CLuaDevice::ExecuteFunction( int nArgs )
//{
//	int nFunctionIndex = -(nArgs+1);
//	if ( lua_isfunction( m_pLuaState, nFunctionIndex ) == false )
//	{
//		lua_pop( m_pLuaState, 1 );
//		return;
//	}
//
//	int nTraceCallback = 0;
//	lua_getglobal( m_pLuaState, "__G__TRACKBACK__" );
//	if ( !lua_isfunction( m_pLuaState, -1 ) )
//	{
//		lua_pop( m_pLuaState, 1 );
//	}
//	else
//	{
//		lua_rotate( m_pLuaState, nFunctionIndex, 1 );
//		nTraceCallback = nFunctionIndex;
//	}
//	int nRet = lua_pcall( m_pLuaState, nArgs, 0, nTraceCallback );
//	if ( nRet != 0 )
//	{
//		if ( nTraceCallback == 0 )
//		{
//			int type = lua_type( m_pLuaState, -1 );
//			const char* err = lua_tostring( m_pLuaState, -1 );
//			Lua::CLoggingHelper::sLog( Logging::ULevel::Error, err );
//			lua_pop( m_pLuaState, 1 );
//		}
//		else
//		{
//			lua_pop( m_pLuaState, 2 );
//		}
//	}
//}







iberbar::CLuaDevice* iberbar::Lua::GetLuaDevice( lua_State* pLuaState )
{
	if ( g_pUniqueLuaDevice && g_pUniqueLuaDevice->GetLuaState() == pLuaState )
		return g_pUniqueLuaDevice;

	return nullptr;
}

