#pragma once


#include <iberbar/Lua/LuaBase.h>


#ifndef __iberbarLuaApisApi__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarLuaApisExport__ )
#			define __iberbarLuaApisApi__ _declspec( dllexport )
#		else
#			define __iberbarLuaApisApi__ _declspec( dllimport )
#		endif
#	else
#		define __iberbarLuaApisApi__
#	endif
#endif

