#pragma once

#include <iberbar/LuaCppApis/Headers.h>


//#define LuaCppNamespace "iberbar.Rhi"
//
//#define LuaCppName_Texture "CTexture"
//#define LuaCppName_Texture_FullName LuaCppNamespace "." LuaCppName_Texture


namespace iberbar
{
	namespace RHI
	{
		static const char LuaCppNamespace[] = "iberbar.Rhi";

		static const char LuaCppName_Texture[] = "CTexture";
		static const char LuaCppName_Texture_FullName[] = "iberbar.Rhi.CTexture";

		void RegisterLuaCpp( lua_State* pLuaState );
	}
}