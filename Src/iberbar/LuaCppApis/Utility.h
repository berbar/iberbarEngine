#pragma once

#include <iberbar/Lua/LuaBase.h>
#include <iberbar/Utility/Rect.h>


namespace iberbar
{
	void RegisterLuaCpp_ForUtility( lua_State* L );

	void LuaApiUtil_PushPoint2i( lua_State* pLuaState, const CPoint2i& Point );
	void LuaApiUtil_PushPoint2f( lua_State* pLuaState, const CPoint2f& Point );
	void LuaApiUtil_PushRect2i( lua_State* pLuaState, const CRect2i& Rect );
	void LuaApiUtil_PushRect2f( lua_State* pLuaState, const CRect2f& Rect );

	CPoint2i LuaApiUtil_ToPoint2i( lua_State* pLuaState, int nArg );
	CPoint2f LuaApiUtil_ToPoint2f( lua_State* pLuaState, int nArg );
	CRect2i LuaApiUtil_ToRect2i( lua_State* pLuaState, int nArg );
	CRect2f LuaApiUtil_ToRect2f( lua_State* pLuaState, int nArg );
}



inline void iberbar::LuaApiUtil_PushPoint2i( lua_State* pLuaState, const CPoint2i& Point )
{
	lua_createtable( pLuaState, 0, 2 );
	lua_pushstring( pLuaState, "x" );
	lua_pushinteger( pLuaState, (lua_Integer)Point.x );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "y" );
	lua_pushinteger( pLuaState, (lua_Integer)Point.y );
	lua_settable( pLuaState, -3 );
}


inline void iberbar::LuaApiUtil_PushPoint2f( lua_State* pLuaState, const CPoint2f& Point )
{
	lua_createtable( pLuaState, 0, 2 );
	lua_pushstring( pLuaState, "x" );
	lua_pushnumber( pLuaState, (lua_Integer)Point.x );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "y" );
	lua_pushnumber( pLuaState, (lua_Integer)Point.y );
	lua_settable( pLuaState, -3 );
}


inline void iberbar::LuaApiUtil_PushRect2i( lua_State* pLuaState, const CRect2i& Rect )
{
	lua_createtable( pLuaState, 0, 4 );
	lua_pushstring( pLuaState, "left" );
	lua_pushinteger( pLuaState, Rect.l );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "top" );
	lua_pushinteger( pLuaState, Rect.t );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "right" );
	lua_pushinteger( pLuaState, Rect.r );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "bottom" );
	lua_pushinteger( pLuaState, Rect.b );
	lua_settable( pLuaState, -3 );
}


inline void iberbar::LuaApiUtil_PushRect2f( lua_State* pLuaState, const CRect2f& Rect )
{
	lua_createtable( pLuaState, 0, 4 );
	lua_pushstring( pLuaState, "left" );
	lua_pushnumber( pLuaState, Rect.l );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "top" );
	lua_pushnumber( pLuaState, Rect.t );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "right" );
	lua_pushnumber( pLuaState, Rect.r );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "bottom" );
	lua_pushnumber( pLuaState, Rect.b );
	lua_settable( pLuaState, -3 );
}


inline iberbar::CPoint2i iberbar::LuaApiUtil_ToPoint2i( lua_State* pLuaState, int nArg )
{
	return CPoint2i();
}


inline iberbar::CPoint2f iberbar::LuaApiUtil_ToPoint2f( lua_State* pLuaState, int nArg )
{
	return CPoint2f();
}


inline iberbar::CRect2i iberbar::LuaApiUtil_ToRect2i( lua_State* pLuaState, int nArg )
{
	return CRect2i();
}


inline iberbar::CRect2f iberbar::LuaApiUtil_ToRect2f( lua_State* pLuaState, int nArg )
{
	return CRect2f();
}


