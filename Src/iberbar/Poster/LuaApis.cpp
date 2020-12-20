
#include <iberbar/Lua/LuaTable.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Lua/LuaCppFunctionParameters.h>
#include <iberbar/Poster/LuaApis.h>
#include <iberbar/Poster/Surface.h>
#include <iberbar/Poster/TextureScale9.h>
#include <iberbar/Poster/Sprite.h>
#include <iberbar/LuaCppApis/Utility.h>

using namespace iberbar;
using namespace iberbar::Poster;



namespace iberbar
{
	namespace Poster
	{
		extern "C" {

			int LuaCppFunction_Surface_Constructor( lua_State* L );

			int LuaCppFunction_Surface_Load( lua_State* L );

			int LuaCppFunction_Surface_Write( lua_State* L );


			int LuaCppFunction_TextureScale9_Constructor( lua_State* L );



			int LuaCFunction_DrawTexture( lua_State* L );

			int LuaCFunction_DrawTextureScale9( lua_State* L );

			int LuaCFunction_DrawTextureInCircle( lua_State* L );

			int LuaCFunction_DrawTextureInCornersBox( lua_State* L );

			int LuaCFunction_DrawLine( lua_State* L );

			int LuaCFunction_DrawRectangle( lua_State* L );
		}


		ULineRenderOptions Lua_ToLineRenderOptions( lua_State* L, int idx );

		UTextureScale9Paddings Lua_ToTextureScale9Paddings( lua_State* L, int idx );

		void RegisterClass_Surface( const char*, LuaCpp::CClassBuilder* classBuilder );

		void RegisterClass_TextureScale9( const char*, LuaCpp::CClassBuilder* classBuilder );
	}
}


extern "C"
{
	int iberbar::Poster::LuaCppFunction_Surface_Constructor( lua_State* L )
	{
		const char* className = nullptr;
		int metatable = 0;
		int ret = LuaCpp::Class_Default_Constructor_New_GetMetatable( L, &className, &metatable );
		if ( ret > 0 ) return ret;

		int at1 = lua_type( L, 1 );
		int at2 = lua_type( L, 2 );
		int at3 = lua_type( L, 3 );
		int at4 = lua_type( L, 4 );

		int top = lua_gettop( L );
		CSurface* surface = nullptr;
		if ( top == 3 )
		{
			surface = new CSurface( lua_tointeger( L, 1 ), lua_tointeger( L, 2 ) );
		}
		else if ( top == 4 )
		{
			surface = new CSurface( lua_tointeger( L, 1 ), lua_tointeger( L, 2 ), Lua_ToColor4B( L, 3 ) );
		}
		else
		{
			surface = new CSurface();
		}
		LuaCpp::Class_Default_PushObject( L, metatable, surface, className );
		return 1;
	}

	int iberbar::Poster::LuaCppFunction_Surface_Load( lua_State* L )
	{
		int ntop = lua_gettop( L );
		if ( ntop != 2 )
		{
			return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );
		}
		CSurface* surface = lua_self_tocppobject( L, CSurface );
		if ( surface == nullptr )
			return iberbar_LuaError_SelfIsNullPointer( L );
		const char* filepath = lua_tostring( L, 2 );
		surface->Load( filepath );
		return 0;
	}

	int iberbar::Poster::LuaCppFunction_Surface_Write( lua_State* L )
	{
		int ntop = lua_gettop( L );
		if ( ntop != 2 )
		{
			return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );
		}
		CSurface* surface = lua_self_tocppobject( L, CSurface );
		if ( surface == nullptr )
			return iberbar_LuaError_SelfIsNullPointer( L );
		const char* filepath = lua_tostring( L, 2 );
		surface->Write( filepath );
		return 0;
	}

	int iberbar::Poster::LuaCppFunction_TextureScale9_Constructor( lua_State* L )
	{
		const char* className = nullptr;
		int metatable = 0;
		int ret = LuaCpp::Class_Default_Constructor_New_GetMetatable( L, &className, &metatable );
		if ( ret > 0 ) return ret;

		int top = lua_gettop( L );
		if ( top != 3 )
		{
			return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );
		}

		CSurface* surface = lua_tocppobject( L, 1, CSurface );
		UTextureScale9Paddings paddings = Lua_ToTextureScale9Paddings( L, 2 );

		CTextureScale9* texture = new CTextureScale9( *surface, paddings );

		LuaCpp::Class_Default_PushObject( L, metatable, texture, className );
		return 1;

		return 1;
	}

	int iberbar::Poster::LuaCFunction_DrawTexture( lua_State* L )
	{
		int top = lua_gettop( L );
		if ( top != 3 && top != 4 )
			return iberbar_LuaError_ArgumentsCountIsNotMatch2( L, 3, 4 );

		CSurface* surface = lua_tocppobject( L, 1, CSurface );
		if ( surface == nullptr )
			return 0;

		CSurface* texture = lua_tocppobject( L, 3, CSurface );
		if ( texture == nullptr )
			return 0;

		CRect2i dest = Lua_ToRect( L, 2 );
		if ( top == 3 || lua_isnil( L, 4 ) == true )
		{
			DrawTexture( surface, dest, texture );
		}
		else
		{
			CRect2i src = Lua_ToRect( L, 4 );
			DrawTexture( surface, dest, texture, &src );
		}

		return 0;
	}

	int iberbar::Poster::LuaCFunction_DrawTextureScale9( lua_State* L )
	{
		return 0;
	}

	int iberbar::Poster::LuaCFunction_DrawTextureInCircle( lua_State* L )
	{
		int top = lua_gettop( L );
		if ( top != 3 && top != 4 )
			return iberbar_LuaError_ArgumentsCountIsNotMatch2( L, 3, 4 );

		CSurface* surface = lua_tocppobject( L, 1, CSurface );
		if ( surface == nullptr )
			return 0;

		CSurface* texture = lua_tocppobject( L, 3, CSurface );
		if ( texture == nullptr )
			return 0;

		CCircle dest = Lua_ToCircle( L, 2 );
		if ( top == 3 || lua_isnil( L, 4 ) == true )
		{
			DrawTextureInCircle( surface, dest, texture );
		}
		else
		{
			CRect2i src = Lua_ToRect( L, 4 );
			DrawTextureInCircle( surface, dest, texture, &src );
		}

		return 0;
	}

	int iberbar::Poster::LuaCFunction_DrawTextureInCornersBox( lua_State* L )
	{
		int top = lua_gettop( L );
		if ( top != 4 && top != 5 )
			return iberbar_LuaError_ArgumentsCountIsNotMatch2( L, 4, 5 );

		CSurface* surface = lua_tocppobject( L, 1, CSurface );
		if ( surface == nullptr )
			return 0;

		CSurface* texture = lua_tocppobject( L, 3, CSurface );
		if ( texture == nullptr )
			return 0;

		CRect2i dest = Lua_ToRect( L, 2 );
		int radius = lua_tointeger( L, 3 );
		if ( top == 4 || lua_isnil( L, 5 ) == true )
		{
			DrawTextureInCornersBox( surface, dest, radius, texture );
		}
		else
		{
			CRect2i src = Lua_ToRect( L, 5 );
			DrawTextureInCornersBox( surface, dest, radius, texture, &src );
		}

		return 0;
	}

	int iberbar::Poster::LuaCFunction_DrawLine( lua_State* L )
	{
		int top = lua_gettop( L );
		if ( top != 4 && top != 5 )
			return iberbar_LuaError_ArgumentsCountIsNotMatch2( L, 4, 5 );

		CSurface* surface = lua_tocppobject( L, 1, CSurface );
		if ( surface == nullptr )
			return 0;

		CPoint2i pt1 = Lua_ToPoint( L, 2 );
		CPoint2i pt2 = Lua_ToPoint( L, 3 );
		CColor4B color = Lua_ToColor4B( L, 4 );
		if ( top == 4 || lua_isnil( L, 5 ) == true )
		{
			DrawLine2( surface, pt1, pt2, color );
		}
		else
		{
			ULineRenderOptions options = Lua_ToLineRenderOptions( L, 5 );
			DrawLine2( surface, pt1, pt2, color, &options );
		}

		return 0;
	}

	int iberbar::Poster::LuaCFunction_DrawRectangle( lua_State* L )
	{
		int top = lua_gettop( L );
		if ( top != 4 )
			return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 4 );

		CSurface* surface = lua_tocppobject( L, 1, CSurface );
		if ( surface == nullptr )
			return 0;

		CRect2i rect = Lua_ToRect( L, 2 );
		CColor4B color = Lua_ToColor4B( L, 3 );
		int thickness = lua_tointeger( L, 4 );
		DrawRectagle( surface, rect, color, thickness );
		
		return 0;
	}
}



ULineRenderOptions iberbar::Poster::Lua_ToLineRenderOptions( lua_State* L, int idx )
{
	ULineRenderOptions options;
	CLuaTable table( L, idx );
	options.width = table.GetInteger_KeyStr( "width" );
	options.style = table.GetInteger_KeyStr( "style" );
	return options;
}

UTextureScale9Paddings iberbar::Poster::Lua_ToTextureScale9Paddings( lua_State* L, int idx )
{
	UTextureScale9Paddings paddings;
	CLuaTable table( L, idx );
	paddings.left = table.GetInteger_KeyStr( "l" );
	paddings.top = table.GetInteger_KeyStr( "t" );
	paddings.right = table.GetInteger_KeyStr( "r" );
	paddings.bottom = table.GetInteger_KeyStr( "b" );
	return paddings;
}


void iberbar::Poster::RegisterCFunctions( lua_State* L, const char* moduleName )
{
	LuaCpp::CBuilder builder( L );
	builder.ResolveScope( []( LuaCpp::CScopeBuilder* scope )
	{
		scope->AddClass( "CSurface", &RegisterClass_Surface );
		scope->AddClass( "CTextureScale9", &RegisterClass_TextureScale9 );
		scope->AddFunctionOne( "DrawRectangle", &LuaCFunction_DrawRectangle );
		scope->AddFunctionOne( "DrawLine", &LuaCFunction_DrawLine );
		scope->AddFunctionOne( "DrawTexture", &LuaCFunction_DrawTexture );
		scope->AddFunctionOne( "DrawTextureInCircle", &LuaCFunction_DrawTextureInCircle );
		scope->AddFunctionOne( "DrawTextureInCornersBox", &LuaCFunction_DrawTextureInCornersBox );
		scope->AddFunctionOne( "DrawTextureScale9", &LuaCFunction_DrawTextureScale9 );
	}, ( moduleName == nullptr ) ? "iberbar.Poster" : moduleName );
}


void iberbar::Poster::RegisterClass_Surface( const char*, LuaCpp::CClassBuilder* classBuilder )
{
	classBuilder->AddConstructor( &LuaCppFunction_Surface_Constructor );
	classBuilder->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CSurface> );
	classBuilder->AddMemberMethod( "Load", &LuaCppFunction_Surface_Load );
	classBuilder->AddMemberMethod( "Write", &LuaCppFunction_Surface_Write );
}

void iberbar::Poster::RegisterClass_TextureScale9( const char*, LuaCpp::CClassBuilder* classBuilder )
{
	classBuilder->AddConstructor( &LuaCppFunction_TextureScale9_Constructor );
	classBuilder->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CTextureScale9> );
}

