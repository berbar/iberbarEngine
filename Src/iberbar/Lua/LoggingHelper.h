#pragma once

#include <iberbar/Lua/LuaBase.h>
#include <iberbar/Utility/Log/BaseTypes.h>

namespace iberbar
{
	namespace Logging
	{
		class COutputDevice;
	}

	namespace Lua
	{
		class __iberbarLuaApi__ CLoggingHelper
		{
		public:
			static void sInitial( Logging::COutputDevice* pOutputDevice );
			static void sDestroy();
			static void sSetLevel( Logging::ULevel nLevel ) { sm_nLevel = nLevel; }
			static void sLog( Logging::ULevel nLevel, const char* strText );
			static void sLogFormat( Logging::ULevel nLevel, const char* strFormat, ... );

		private:
			static Logging::COutputDevice* sm_pOutputDevice;
			static Logging::ULevel sm_nLevel;
		};
	}
}

