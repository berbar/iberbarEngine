#pragma once


#include <iberbar/Lua/LuaBase.h>
#include <functional>


namespace iberbar
{
	namespace Lua
	{
		class __iberbarLuaApi__ CFunctionHelper
		{
		public:
			static void sExecuteFunction( lua_State* pLuaState, int nArgs );
			static int  sExecuteFunction( lua_State* pLuaState, int nArgs, int nReturns );
			//void ExecuteFunction( int nArgs, int nReturns, std::function<void()> funcGetReturns );
			static void sExecuteGlobalFunction( lua_State* pLuaState, const char* strFunctionName );
		};
	}
}
