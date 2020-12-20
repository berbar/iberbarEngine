#pragma once

#include <iberbar/Base/Lua/LuaCModule.h>
#include <iberbar/Base/Lua/LuaTypes.h>


namespace iberbar
{

	class CLuaCModuleClass
		: public CLuaCModuleAbstract
	{
	public:
		struct Reg
		{
			const char* clsname;
			lua_CFunction fn_constructor;
			lua_CFunction fn_gc;
			lua_CFunction fn_tostring;
			const luaL_Reg* fns;
		};

	public:
		CLuaCModuleClass()
		{

		}

	public:
		virtual void onRegister( lua_State* pLuaState, int tb ) override;

	public:
		const Reg* classes;
	};
}

#define IBERBAR_LUA_CMODULE_CLS( type )           LuaCModuleClsReg_##type
#define IBERBAR_LUA_CMODULE_CLS_DECLARE( type )   static const iberbar::CLuaCModuleClass::Reg IBERBAR_LUA_CMODULE_CLS(type)[]
#define IBERBAR_LUA_CMODULE_CLS_DECLARE_END       { nullptr }
