#pragma once

#include <iberbar/Poster/MathTypes.h>
#include <iberbar/Lua/LuaBase.h>


namespace iberbar
{
	namespace Poster
	{
		void __iberbarExports__ RegisterCFunctions( lua_State* L, const char* moduleName = nullptr );
	}
}

