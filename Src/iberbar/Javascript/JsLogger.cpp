
#include <iberbar/Javascript/JsLogger.h>


void iberbar::iJavascript::CDefaultLogger::Log( const std::string& strMessage ) const
{
	printf_s( strMessage.c_str() );
}

void iberbar::iJavascript::CDefaultLogger::Info( const std::string& strMessage ) const
{
	printf_s( strMessage.c_str() );
}

void iberbar::iJavascript::CDefaultLogger::Warn( const std::string& strMessage ) const
{
	printf_s( strMessage.c_str() );
}

void iberbar::iJavascript::CDefaultLogger::Error( const std::string& strMessage ) const
{
	printf_s( strMessage.c_str() );
}