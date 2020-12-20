
#pragma once

#include <iberbar/LuaCppApis/Headers.h>
#include <iberbar/Utility/Point.h>
#include <iberbar/Utility/Size.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Circle.h>
#include <iberbar/Utility/Color.h>

namespace iberbar
{
	CPoint2i __iberbarLuaApisApi__ Lua_ToPoint( lua_State* L, int idx );

	CSize2i __iberbarLuaApisApi__ Lua_ToSize( lua_State* L, int idx );

	CRect2i __iberbarLuaApisApi__ Lua_ToRect( lua_State* L, int idx );

	CCircle __iberbarLuaApisApi__ Lua_ToCircle( lua_State* L, int idx );

	CColor4B __iberbarLuaApisApi__ Lua_ToColor4B( lua_State* L, int idx );


}
