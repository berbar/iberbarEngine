
#include <iberbar/Base/Lua/LuaStateRef.h>


iberbar::CLuaStateRef::CLuaStateRef( void )
	: m_pLuaState( NULL )
{
}


iberbar::CLuaStateRef::~CLuaStateRef()
{
	if ( m_pLuaState )
	{
		lua_close( m_pLuaState );
		m_pLuaState = NULL;
	}
}