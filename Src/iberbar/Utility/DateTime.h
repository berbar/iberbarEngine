#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{
	class CDateTime
	{
	public:
		CDateTime();
		CDateTime( const int64& nTimeStamp );
		CDateTime( const tm& Time );

	public:
		std::string ToString();
		std::string ToString( const char* format );

	private:
		tm m_Time;


	public:
		static CDateTime sNow();
	};
}


inline iberbar::CDateTime::CDateTime()
	: m_Time()
{
}


inline iberbar::CDateTime::CDateTime( const int64& nTimeStamp )
	: m_Time()
{
	localtime_s( &m_Time, &nTimeStamp );
}


inline iberbar::CDateTime::CDateTime( const tm& Time )
	: m_Time( Time )
{
}


inline std::string iberbar::CDateTime::ToString()
{
	return ToString( "%Y-%m-%d %H:%M:%S" );
}


inline std::string iberbar::CDateTime::ToString( const char* format )
{
	char strBuffer[ 64 ];
	strftime( strBuffer, sizeof( strBuffer ), "%Y-%m-%d %H:%M:%S", &m_Time );
	return strBuffer;
}


inline iberbar::CDateTime iberbar::CDateTime::sNow()
{
	time_t nTimeStamp;
	time( &nTimeStamp );
	return CDateTime( nTimeStamp );
}
