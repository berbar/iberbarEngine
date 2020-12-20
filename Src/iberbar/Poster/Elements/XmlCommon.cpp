
#include "./XmlCommon.h"
#include <iberbar/Utility/String.h>



void iberbar::Poster::CXmlProcMessageBuffer::AddMessage( UXmlProcMessageCode code, const char* format, ... )
{
	va_list va;
	va_start( va, format );
	m_messages.push_back( UXmlProcMessage( code, StdFormatVa( format, va ) ) );
	va_end( va );
}