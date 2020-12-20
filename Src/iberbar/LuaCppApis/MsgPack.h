#pragma once


struct lua_State;


namespace iberbar
{
	namespace MsgPack
	{
		void RegisterLuaCpp( lua_State* pLuaState );
	}
}
