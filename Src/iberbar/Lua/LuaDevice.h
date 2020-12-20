#pragma once

#include <iberbar/Lua/LuaBase.h>


namespace iberbar
{
	class __iberbarLuaApi__ CLuaDevice
		: public CRef
	{
	public:
		CLuaDevice();
		~CLuaDevice();



	public:
		void Initial();
		void AddLuaPath( const char* strFile );
		CResult ExecuteFile( const char* strFile );
		CResult ExecuteSource( const char* strSource );
		lua_State* GetLuaState() { return m_pLuaState; }

	private:
		lua_State* m_pLuaState;
		
	};


	namespace Lua
	{
		CLuaDevice* GetLuaDevice( lua_State* pLuaState );
	}
}

