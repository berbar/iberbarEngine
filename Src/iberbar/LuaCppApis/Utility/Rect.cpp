

#include <iberbar/LuaCppApis/Utility/Rect.h>
#include <iberbar/Lua/LuaTable.h>


iberbar::CPoint2i iberbar::Lua_ToPoint( lua_State* L, int idx )
{
	if ( lua_istable( L, idx ) )
	{
		CLuaTable tbl = CLuaTable( L, idx );
		return CPoint2i(
			tbl.GetInteger_KeyStr( "x" ),
			tbl.GetInteger_KeyStr( "y" ) );
	}
	return CPoint2i();
}

iberbar::CSize2i iberbar::Lua_ToSize( lua_State* L, int idx )
{
	if ( lua_istable( L, idx ) )
	{
		CLuaTable tbl = CLuaTable( L, idx );
		return CSize2i(
			tbl.GetInteger_KeyStr( "w" ),
			tbl.GetInteger_KeyStr( "h" ) );
	}
	return CSize2i();
}

iberbar::CRect2i iberbar::Lua_ToRect( lua_State* L, int idx )
{
	if ( lua_istable( L, idx ) )
	{
		CLuaTable tblRect = CLuaTable( L, idx );
		return CRect2i(
			tblRect.GetInteger_KeyStr( "l" ),
			tblRect.GetInteger_KeyStr( "t" ),
			tblRect.GetInteger_KeyStr( "r" ),
			tblRect.GetInteger_KeyStr( "b" ) );
	}
	return CRect2i();
}

iberbar::CCircle iberbar::Lua_ToCircle( lua_State* L, int idx )
{
	if ( lua_istable( L, idx ) )
	{
		CLuaTable tbl = CLuaTable( L, idx );
		return CCircle(
			tbl.GetInteger_KeyStr( "x" ),
			tbl.GetInteger_KeyStr( "y" ),
			tbl.GetInteger_KeyStr( "r" ) );
	}
	return CCircle();
}

iberbar::CColor4B iberbar::Lua_ToColor4B( lua_State* L, int idx )
{
	if ( lua_istable( L, idx ) )
	{
		CLuaTable tblRect = CLuaTable( L, idx );
		return CColor4B(
			tblRect.GetInteger_KeyStr( "r" ),
			tblRect.GetInteger_KeyStr( "g" ),
			tblRect.GetInteger_KeyStr( "b" ),
			tblRect.GetInteger_KeyStr( "a" ) );
	}
	else if ( lua_isinteger( L, idx ) )
	{
		return CColor4B( lua_tointeger( L, idx ) );
	}
	return CColor4B();
}
