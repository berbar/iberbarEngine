
#include "Logger.h"
#include "Device.h"
#include "MsgQueue.h"
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>



namespace iberbar
{
	namespace IO
	{
		spdlog::level::level_enum ToSpdlogLevel( ULoggerLevel nLevel )
		{
			switch ( nLevel )
			{
				case ULoggerLevel::Trace:
					return spdlog::level::trace;

				case ULoggerLevel::Debug:
					return spdlog::level::debug;

				case ULoggerLevel::Info:
					return spdlog::level::info;

				case ULoggerLevel::Warn:
					return spdlog::level::warn;

				case ULoggerLevel::Error:
					return spdlog::level::err;

				case ULoggerLevel::Critical:
					return spdlog::level::critical;

				case ULoggerLevel::Fatal:
					return spdlog::level::critical;

				default:
					return spdlog::level::info;
			}
		}
	}
}





iberbar::IO::CLogger_UseSpdlog::CLogger_UseSpdlog()
	: m_spdlog( nullptr )
{

}


iberbar::IO::CLogger_UseSpdlog::~CLogger_UseSpdlog()
{
}


void iberbar::IO::CLogger_UseSpdlog::Write( ULoggerLevel nLevel, const char* strTags, const char* strFormat, ... )
{
	ULoggerMsg Msg;
	Msg.pLogger = this;
	Msg.nLevel = nLevel;
	Msg.strText = IOString( g_pDevice->GetMemoryAllocator<char>() );
	va_list va;
	va_start( va, strFormat );
	IOStringFormatVa( Msg.strText, strFormat, va );
	va_end( va );
	g_pMsgQueues->GetQueue_Logger()->AddMsg( Msg );
}


void iberbar::IO::CLogger_UseSpdlog::Write( ULoggerLevel nLevel, const char* strTags, const char* strText )
{
	ULoggerMsg Msg;
	Msg.pLogger = this;
	Msg.nLevel = nLevel;
	Msg.strText = IOString( strText, g_pDevice->GetMemoryAllocator<char>() );
	g_pMsgQueues->GetQueue_Logger()->AddMsg( Msg );
}


void iberbar::IO::CLogger_UseSpdlog::WriteInner( ULoggerLevel nLevel, const char* strText )
{
	if ( m_spdlog == nullptr )
		return;

	m_spdlog->log( ToSpdlogLevel( nLevel ), strText );
}


iberbar::CResult iberbar::IO::CLogger_UseSpdlog::Create( const ULoggerOptions& Options  )
{
	if ( Options.nType == (int)ULoggerType::File )
	{
		m_spdlog = spdlog::basic_logger_mt( Options.strName, Options.strFileName );
	}
	
	if ( m_spdlog == nullptr )
	{
		return MakeResult( ResultCode::Bad, "failed to" );
	}

	return CResult();
}


bool iberbar::IO::CLogger_UseSpdlog::CompareLogger( const char* strName )
{
	if ( strcmp( m_spdlog->name().c_str(), strName ) == 0 )
		return true;

	return false;
}

