
#include <iberbar/Lua/StackChecker.h>
#include <iberbar/Lua/LoggingHelper.h>
#include <iberbar/Utility/String.h>


void iberbar::Lua::CStackSnapshot::Diff_Error( int nDiff, const char* strFile, int nLine )
{
	if ( Diff() != nDiff )
	{
		std::string strText = StdFormat( "error stack at %s(%d)", strFile, nLine );
		CLoggingHelper::sLog( Logging::ULevel::Error, strText.c_str() );
		luaL_error( m_pLuaState, strText.c_str() );
	}
}


void iberbar::Lua::CStackSnapshot::Diff_Error_Format( int nDiff, const char* strFile, int nLine, const char* strFormat, ... )
{
	if ( Diff() != nDiff )
	{
		std::string strText;
		va_list va;
		va_start( va, strFormat );
		strText = StdFormatVa( strFormat, va );
		va_end( va );
		strText = StdFormat( "error stack at %s(%d): %s", strFile, nLine, strText.c_str() );
		CLoggingHelper::sLog( Logging::ULevel::Error, strText.c_str() );
		luaL_error( m_pLuaState, strText.c_str() );
	}
}
