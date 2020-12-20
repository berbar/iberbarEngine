#pragma once

#include "../IO.h"
#include <spdlog/logger.h>


namespace iberbar
{
	namespace IO
	{
		class CLogger_UseSpdlog
			: public ILogger
		{
		public:
			CLogger_UseSpdlog();
			virtual ~CLogger_UseSpdlog();

			virtual void Write( ULoggerLevel nLevel, const char* strTags, const char* strFormat, ... ) override;
			virtual void Write( ULoggerLevel nLevel, const char* strTags, const char* strText ) override;
			void WriteInner( ULoggerLevel nLevel, const char* strText );

		public:
			CResult Create( const ULoggerOptions& Options );
			bool CompareLogger( const char* strName );

		private:
			std::shared_ptr<spdlog::logger> m_spdlog;
		};
	}
}
