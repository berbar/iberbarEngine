#pragma once

struct lua_State;

namespace iberbar
{
	namespace Xml
	{
		void RegisterLuaCpp( lua_State* pLuaState );
	}
}