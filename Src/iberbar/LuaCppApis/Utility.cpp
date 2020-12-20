
#include <iberbar/LuaCppApis/Utility.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/FileHelper.h>




namespace iberbar
{
	int LuaCFunction_ReadFileAsText( lua_State* pLuaState );
}



void iberbar::RegisterLuaCpp_ForUtility( lua_State* pLuaState )
{
	LuaCpp::CBuilder builder( pLuaState );
	builder.ResolveScope( []( LuaCpp::CScopeBuilder* scope )
		{
			scope->AddFunctionOne( "ReadFileAsText", &LuaCFunction_ReadFileAsText );
		}, "iberbar" );
}


int iberbar::LuaCFunction_ReadFileAsText( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	const char* strFile = lua_tostring( pLuaState, 1 );
	if ( StringIsNullOrEmpty( strFile ) )
	{
		lua_pushboolean( pLuaState, false );
		lua_pushstring( pLuaState, "" );
	}
	else
	{
		CFileHelper FileHelper;
		bool ret = FileHelper.OpenFileA( strFile, "r" );
		if ( ret == true )
		{
			lua_pushboolean( pLuaState, ret );
			lua_pushstring( pLuaState, FileHelper.ReadAsText().c_str() );
		}
		else
		{
			lua_pushboolean( pLuaState, ret );
			lua_pushstring( pLuaState, "" );
		}
		FileHelper.CloseFile();
	}

	return 2;
}