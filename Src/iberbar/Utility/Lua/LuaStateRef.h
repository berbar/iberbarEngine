#pragma once


#include <iberbar\Base\Lua\LuaBase.h>
#include <iberbar\Base\Unknown.h>


namespace iberbar
{
	class CLuaStateRef
		: public CUnknown
	{
		IBERBAR_UNKNOWN_CLONE_DISABLED( CLuaStateRef );

	public:
		CLuaStateRef( void );
		~CLuaStateRef();

		inline lua_State* get() { return m_pLuaState; }

		void initial()
		{
			m_pLuaState = luaL_newstate();
			luaL_openlibs( m_pLuaState );
		}

		void destroy()
		{
			if ( m_pLuaState )
			{
				lua_close( m_pLuaState );
				m_pLuaState = NULL;
			}
		}

	private:
		lua_State * m_pLuaState;
	};

	IBERBAR_UNKNOWN_PTR_DECLARE( CLuaStateRef );
}


