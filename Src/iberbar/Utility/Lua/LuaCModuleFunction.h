#pragma once

#include <iberbar/Base/Lua/LuaCModule.h>


namespace iberbar
{
	class CLuaCModuleFunction
		: public CLuaCModuleAbstract
	{
	public:
		CLuaCModuleFunction( void )
			: fns( nullptr )
		{
		}

	public:
		virtual void onRegister( lua_State* pLuaState, int tb ) override;

	public:
		const luaL_Reg* fns;
	};
}
