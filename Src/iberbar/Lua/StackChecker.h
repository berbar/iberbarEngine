#pragma once

#include <iberbar/Lua/LuaBase.h>

namespace iberbar
{
	namespace Lua
	{
		class __iberbarLuaApi__ CStackSnapshot
		{
		public:
			CStackSnapshot( lua_State* pLuaState );

			int Diff();
			void Diff_Error( int nDiff, const char* strFile, int nLine );
			void Diff_Error_Format( int nDiff, const char* strFile, int nLine, const char* strFormat, ... );

		private:
			lua_State* m_pLuaState;
			int m_nTopTemp;
		};
	}
}


#define iberbar_Lua_StackSnapshot_DiffError( sss, diff ) sss.Diff_Error( diff, __FILE__, __LINE__ )
#define iberbar_Lua_StackSnapshot_DiffError_Format( sss, diff, format, ... ) sss.Diff_Error_Format( diff, __FILE__, __LINE__, format, __VA_ARGS__ )



inline iberbar::Lua::CStackSnapshot::CStackSnapshot( lua_State* pLuaState )
	: m_pLuaState( pLuaState )
	, m_nTopTemp( lua_gettop( pLuaState ) )
{
}


inline int iberbar::Lua::CStackSnapshot::Diff()
{
	int nTop = lua_gettop( m_pLuaState );
	int nDiff = nTop - m_nTopTemp;
	m_nTopTemp = nTop;
	return nDiff;
}




