#pragma once

#include <iberbar/Base/Lua/LuaBase.h>
#include <iberbar/Base/Result.h>

namespace iberbar
{
	typedef std::vector< std::string > LuaCModuleNamespaceList;

	class CLuaCModuleAbstract abstract
	{
	public:
		virtual void onRegister( lua_State* pLuaState, int tb ) = 0;
	};

	Result LuaCModuleRegister( lua_State* pLuaState, const char* name, CLuaCModuleAbstract* mod );
}