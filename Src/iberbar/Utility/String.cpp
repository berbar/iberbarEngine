

#include <iberbar/Utility/String.h>



//std::wstring iberbar::Utf8ToUnicode( const char* str )
//{
//	std::wstring buf;
//#ifdef _WIN32
//	int size = utf8_to_unicode_calculate( str, -1 ) + 1;
//	if ( size <= 0 )
//		return std::wstring();
//	std::wstring buffer( size, '\0' );
//	utf8_to_unicode( str, -1, &buffer.front(), size );
//	return buffer;
//#endif
//
//#ifdef __ANDROID__
//#endif
//	return buf;
//}
//
//std::wstring iberbar::AsciiToUnicode( const char* str )
//{
//	int size = ascii_to_unicode_calculate( str, -1 ) + 1;
//	if ( size <= 0 )
//		return std::wstring();
//	std::wstring buffer( size, '\0' );
//	ascii_to_unicode( str, -1, &buffer.front(), size );
//	return buffer;
//}

//std::string iberbar::UnicodeToUtf8( const wchar_t* str )
//{
//	int size = unicode_to_utf8_calculate( str, -1 ) + 1;
//	if ( size <= 0 )
//		return std::string();
//	std::string buffer( size, '\0' );
//	unicode_to_utf8( str, -1, &buffer.front(), size );
//	return buffer;
//}
//
//std::string iberbar::UnicodeToAscii( const wchar_t* str )
//{
//	int size = unicode_to_ascii_calculate( str, -1 ) + 1;
//	if ( size <= 0 )
//		return std::string();
//	std::string buffer( size, '\0' );
//	unicode_to_ascii( str, -1, &buffer.front(), size );
//	return buffer;
//}


std::string iberbar::StdFormat( const char* format, ... )
{
	std::string str;
	int charCount = 0;
	va_list va;
	va_start( va, format );
	charCount = std::vsnprintf( nullptr, 0, format, va );
	va_end( va );
	if ( charCount == 0 )
		return "";

	str.resize( charCount + 1 );
	va_start( va, format );
	std::vsnprintf( &str.front(), charCount + 1, format, va );
	va_end( va );

	return str;
}

std::string iberbar::StdFormatVa( const char* format, va_list va )
{
	std::string str;
	int charCount = 0;
	charCount = std::vsnprintf( nullptr, 0, format, va );
	if ( charCount == 0 )
		return "";

	str.resize( charCount + 1 );
	std::vsnprintf( &str.front(), charCount + 1, format, va );

	return str;
}

void iberbar::StdFormatRef( std::string& str, const char* format, ... )
{
	int charCount = 0;
	va_list va;
	va_start( va, format );
	charCount = std::vsnprintf( nullptr, 0, format, va );
	va_end( va );
	if ( charCount == 0 )
		return;

	str.resize( charCount + 1 );
	va_start( va, format );
	std::vsnprintf( &str.front(), charCount + 1, format, va );
	va_end( va );
}


std::string iberbar::StringToLower( const char* str )
{
	int len = strlen( str );
	std::string strLower;
	strLower.resize( len + 1 );
	int i = 0;
	while ( *str != 0 )
	{
		strLower[ i ] = tolower( *str );
		str++;
		i++;
	}
	strLower[ len ] = 0;
	return strLower;
}


std::string iberbar::StringToUpper( const char* str )
{
	int len = strlen( str );
	std::string strLower;
	strLower.resize( len + 1 );
	int i = 0;
	while ( *str != 0 )
	{
		strLower[ i ] = toupper( *str );
		str++;
	}
	strLower[ len ] = 0;
	return strLower;
}


std::wstring iberbar::WStringToLower( const wchar_t* str )
{
	int len = wcslen( str );
	std::wstring strLower;
	strLower.resize( len + 1 );
	int i = 0;
	while ( *str != 0 )
	{
		strLower[ i ] = towlower( *str );
		str++;
	}
	strLower[ len ] = 0;
	return strLower;
}


std::wstring iberbar::WStringToUpper( const wchar_t* str )
{
	int len = wcslen( str );
	std::wstring strLower;
	strLower.resize( len + 1 );
	int i = 0;
	while ( *str != 0 )
	{
		strLower[ i ] = towupper( *str );
		str++;
	}
	strLower[ len ] = 0;
	return strLower;
}



std::string iberbar::StdStringToLower( std::string& str )
{
	char* ptr = &str.front();
	while ( *ptr != 0 )
	{
		ptr[ 0 ] = tolower( *ptr );
		ptr++;
	}
	return str;
}


std::string iberbar::StdStringToUpper( std::string& str )
{
	char* ptr = &str.front();
	while ( *ptr != 0 )
	{
		ptr[ 0 ] = toupper( *ptr );
		ptr++;
	}
	return str;
}


std::wstring iberbar::StdWStringToLower( std::wstring& str )
{
	wchar_t* ptr = &str.front();
	while ( *ptr != 0 )
	{
		ptr[ 0 ] = towlower( *ptr );
		ptr++;
	}
	return str;
}


std::wstring iberbar::StdWStringToUpper( std::wstring& str )
{
	wchar_t* ptr = &str.front();
	while ( *ptr != 0 )
	{
		ptr[ 0 ] = towupper( *ptr );
		ptr++;
	}
	return str;
}
