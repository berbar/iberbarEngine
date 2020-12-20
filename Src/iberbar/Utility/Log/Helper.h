#pragma once

#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Log/BaseTypes.h>

namespace iberbar
{
	namespace Logging
	{
		std::string FormatLogText( ULevel nLevel, const char* strText, const char* strCategory, int64 nTime );
	}
}