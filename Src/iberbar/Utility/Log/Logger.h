#pragma once

#include <iberbar/Utility/Log/OutputDevice.h>
#include <iberbar/Utility/String.h>

namespace iberbar
{
	namespace Logging
	{
		class __iberbarUtilityApi__ CLogger
		{
		public:
			CLogger();
			CLogger( COutputDevice* pDevice );
			CLogger( const CLogger& Other );
			~CLogger();

		public:
			void SetLevel( ULevel nLevel );
			ULevel GetLevel() const;

			void Write( ULevel nLevel, const char* strCategory, const char* strText, int64 nTime = -1 );
			void WriteFormat( ULevel nLevel, const char* strCategory, const char* strFormat, ... );

		private:
			COutputDevice* m_pOutputDevice;
			ULevel m_nLevel;
		};
	}
}



inline void iberbar::Logging::CLogger::SetLevel( ULevel nLevel )
{
	m_nLevel = nLevel;
}


inline iberbar::Logging::ULevel iberbar::Logging::CLogger::GetLevel() const
{
	return m_nLevel;
}


inline void iberbar::Logging::CLogger::Write( ULevel nLevel, const char* strCategory, const char* strText, int64 nTime )
{
	if ( nLevel < m_nLevel )
		return;
	if ( m_pOutputDevice == nullptr )
		return;
	m_pOutputDevice->Serialize( nLevel, strText, strCategory, nTime );
}


inline void iberbar::Logging::CLogger::WriteFormat( ULevel nLevel, const char* strCategory, const char* strFormat, ... )
{
	if ( nLevel < m_nLevel )
		return;
	if ( m_pOutputDevice == nullptr )
		return;
	va_list va;
	va_start( va, strFormat );
	std::string strText = StdFormatVa( strFormat, va );
	va_end( va );
	m_pOutputDevice->Serialize( nLevel, strText.c_str(), strCategory );
}
