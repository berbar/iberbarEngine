
#include <iberbar/LuaCppApis/Utility.h>
#include <iberbar/LuaCppApis/Utility_Names.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/FileHelper.h>

#include <filesystem>




namespace iberbar
{




	int LuaCFunction_ReadFileAsText( lua_State* pLuaState );
	int LuaCFunction_FileSystem_IsFileExist( lua_State* pLuaState );
	//int LuaCFunction_JsonDecode( lua_State* pLuaState );


	int LuaCppFunction_Ref_RefCount( lua_State* pLuaState );
	int LuaCppFunction_Ref___tostring( lua_State* pLuaState );


	static const luaL_Reg LuaCppMethods_Ref[] =
	{
		{ "RefCount", &LuaCppFunction_Ref_RefCount },
		{ Lua::s_ClassStandardMethod_ToString, &LuaCppFunction_Ref___tostring },
		{ nullptr, nullptr }
	};
	static const Lua::UClassDefinition LuaCppDef_Ref =
	{
		LuaCppNames::s_ClassName_Ref,
		LuaCppNames::s_ClassName_Ref_FullName,
		nullptr,
		nullptr,
		nullptr,
		LuaCppMethods_Ref,
	};
}






void iberbar::RegisterLuaCpp_ForUtility( lua_State* pLuaState )
{
	Lua::CBuilder builder( pLuaState );
	builder.ResolveScope( []( Lua::CScopeBuilder* scope )
		{
			scope->AddFunctionOne( "ReadFileAsText", &LuaCFunction_ReadFileAsText );

			scope->AddClass( LuaCppDef_Ref );

			scope->AddEnum( "UAlignHorizontal",
				[]( Lua::CEnumBuilder* pEnum )
				{
					pEnum->AddValueInt( "Left", (lua_Integer)UAlignHorizental::Left );
					pEnum->AddValueInt( "Center", (lua_Integer)UAlignHorizental::Center );
					pEnum->AddValueInt( "Right", (lua_Integer)UAlignHorizental::Right );
				} );

			scope->AddEnum( "UAlignVertical",
				[]( Lua::CEnumBuilder* pEnum )
				{
					pEnum->AddValueInt( "Top", (lua_Integer)UAlignVertical::Top );
					pEnum->AddValueInt( "Center", (lua_Integer)UAlignVertical::Center );
					pEnum->AddValueInt( "Bottom", (lua_Integer)UAlignVertical::Bottom );
				} );
		}, "iberbar" );

	builder.ResolveScope( []( Lua::CScopeBuilder* scope )
		{
			scope->AddFunctionOne( "IsFileExist", &LuaCFunction_FileSystem_IsFileExist );
		}, "iberbar.FileSystem" );
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


int iberbar::LuaCFunction_FileSystem_IsFileExist( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	const char* strFilePath = lua_tostring( pLuaState, 1 );
	if ( StringIsNullOrEmpty( strFilePath ) )
	{
		lua_pushboolean( pLuaState, 0 );
		return 1;
	}

	std::filesystem::path fsPath = strFilePath;
	if ( std::filesystem::is_regular_file( fsPath ) == false )
	{
		lua_pushboolean( pLuaState, 0 );
		return 1;
	}

	lua_pushboolean( pLuaState, std::filesystem::exists( fsPath ) ? 1 : 0 );
	return 1;
}



int iberbar::LuaCppFunction_Ref_RefCount( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	CRef* pRef = lua_tocppobject( pLuaState, 1, CRef );
	if ( pRef == nullptr )
	{
		lua_pushinteger( pLuaState, 1 );
		return 1;
	}

	lua_pushinteger( pLuaState, (lua_Integer)pRef->Refcount() );

	return 1;
}


int iberbar::LuaCppFunction_Ref___tostring( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	CRef* pRef = lua_tocppobject( pLuaState, 1, CRef );
	if ( pRef == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushstring( pLuaState, pRef->ToString().c_str() );
	//lua_pushstring( pLuaState, "" );

	return 1;
}
