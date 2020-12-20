
#include <iberbar/Utility/Log/Helper.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/DateTime.h>


namespace iberbar
{
	namespace Logging
	{
		static const int s_LevelNameLength = 8;

		const char s_LevelNames[ (int)ULevel::__Count ][ s_LevelNameLength + 1 ] =
		{
			"Verbose ",
			"Debug   ",
			"Info    ",
			"Warn    ",
			"Error   ",
			"Critical",
			"Fatal   ",
		};
	}
}


std::string iberbar::Logging::FormatLogText( ULevel nLevel, const char* strText, const char* strCategory, int64 nTime )
{
	std::string Buffer;
	std::string DateTime;
	if ( nTime < 0 )
		DateTime = CDateTime::sNow().ToString();
	else
		DateTime = CDateTime( nTime ).ToString();
	StringFormat( Buffer, "%s %s [%s] %s\n", DateTime.c_str(), s_LevelNames[ (int)nLevel ], strCategory, strText );
	return Buffer;
}


