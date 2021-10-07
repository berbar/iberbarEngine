
#include <iberbar/LuaCppApis/Rhi.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Lua/LuaCppBuilder.h>

#include <iberbar/RHI/Texture.h>



//#define LuaCppNamespace "iberbar.Rhi"
//
//#define LuaCppName_Texture "CTexture"
//#define LuaCppName_Texture_FullName LuaCppNamespace "." LuaCppName_Texture



namespace iberbar
{
	namespace RHI
	{
		int LuaCppFunction_Texture_GetSize( lua_State* pLuaState );
		int LuaCppFunction_Texture_SaveToFile( lua_State* pLuaState );


		void LuaCppClassBuilder_Texture( const char*, Lua::CClassBuilder* cb );
	}
}





void iberbar::RHI::RegisterLuaCpp( lua_State* pLuaState )
{
	Lua::CBuilder builder( pLuaState );
	builder.ResolveScope( []( Lua::CScopeBuilder* scope )
		{
			scope->AddClass( LuaCppName_Texture, &LuaCppClassBuilder_Texture );
		}, LuaCppNamespace );
}






int iberbar::RHI::LuaCppFunction_Texture_GetSize( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	ITexture* pTexture = lua_tocppobject( pLuaState, 1, ITexture );
	if ( pTexture != nullptr )
	{
		const CSize2i& Size = pTexture->GetSize();
		lua_newtable( pLuaState );
		int nLuaTable = lua_gettop( pLuaState );
		lua_pushstring( pLuaState, "w" );
		lua_pushnumber( pLuaState, (lua_Number)Size.w );
		lua_settable( pLuaState, nLuaTable );
		lua_pushstring( pLuaState, "h" );
		lua_pushnumber( pLuaState, (lua_Number)Size.h );
		lua_settable( pLuaState, nLuaTable );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::RHI::LuaCppFunction_Texture_SaveToFile( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	ITexture* pTexture = lua_tocppobject( pLuaState, 1, ITexture );
	const char* strFile = lua_tostring( pLuaState, 2 );
	if ( pTexture != nullptr )
	{
		pTexture->SaveToFileA( strFile );
	}
	else
	{
	}

	return 0;
}






void iberbar::RHI::LuaCppClassBuilder_Texture( const char*, Lua::CClassBuilder* cb )
{
	cb->AddMemberMethod( "GetSize", &LuaCppFunction_Texture_GetSize );
	cb->AddMemberMethod( "SaveToFile", &LuaCppFunction_Texture_SaveToFile );

	cb->AddDistructor( &Lua::Class_Unknown_Distructor_Release<ITexture> );
}


