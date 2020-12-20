
#include <iberbar/Lua/LoggingHelper.h>
#include <iberbar/Utility/Log/OutputDevice.h>
#include <iberbar/Utility/String.h>


#ifdef _DEBUG
iberbar::Logging::ULevel iberbar::Lua::CLoggingHelper::sm_nLevel = iberbar::Logging::ULevel::Trace;
#else
iberbar::Logging::ULevel iberbar::Lua::CLoggingHelper::sm_nLevel = iberbar::Logging::ULevel::Info;
#endif
iberbar::Logging::COutputDevice* iberbar::Lua::CLoggingHelper::sm_pOutputDevice = nullptr;


void iberbar::Lua::CLoggingHelper::sInitial( Logging::COutputDevice* pOutputDevice )
{
	UNKNOWN_SAFE_RELEASE_NULL( sm_pOutputDevice );
	sm_pOutputDevice = pOutputDevice;
	UNKNOWN_SAFE_ADDREF( sm_pOutputDevice );
}


void iberbar::Lua::CLoggingHelper::sDestroy()
{
	UNKNOWN_SAFE_RELEASE_NULL( sm_pOutputDevice );
}


void iberbar::Lua::CLoggingHelper::sLog( Logging::ULevel nLevel, const char* strText )
{
	if ( sm_pOutputDevice == nullptr )
		return;

	sm_pOutputDevice->Serialize( nLevel, strText, "Lua" );
}


void iberbar::Lua::CLoggingHelper::sLogFormat( Logging::ULevel nLevel, const char* strFormat, ... )
{
	if ( sm_pOutputDevice == nullptr )
		return;

	if ( nLevel < sm_nLevel )
		return;

	std::string Buffer;
	va_list va;
	va_start( va, strFormat );
	StringFormatVa( Buffer, strFormat, va );
	va_end( va );

	sm_pOutputDevice->Serialize( nLevel, Buffer.c_str(), "Lua" );
}

