
#pragma once


#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Math.h>

#ifdef _WIN32
#else
#include <codecvt>
#include <iostream>
#endif

namespace iberbar
{

//#ifdef _MSC_VER
//#define snprintf _snprintf_s
//#endif

#ifndef _WIN32
#define wcsncpy_s( dst, dst_size, src, n ) wcsncpy( dst, src, n )
#define sprintf_s( buf, fmt, ... ) sprintf( buf, fmt, __VA_ARGS__ )
#endif

	//template < typename Ch >
	//int tstring_split( const Ch* str, Ch ch )
	//{
	//	assert( str );
	//	assert( ch != 0 );
	//	int nCount = 0;
	//	while ( str[0] )
	//	{
	//		if ( str[0] == ch )
	//			nCount++;
	//		str++;
	//	}
	//	return 0;
	//}


	template < typename Ch >
	const Ch* tstring_trim( const Ch* str, Ch ch )
	{
		while ( str[0] && str[0] != ch )
		{
			str++;
		}
		return str;
	}


	template< typename Ch >
	int tstring_find_ch_count( const Ch* str, Ch ch )
	{
		assert( str );
		assert( ch != 0 );

		int nCount = 0;
		while ( str[ 0 ] )
		{
			if ( str[ 0 ] == ch )
				nCount ++;
			str ++;
		}

		return nCount;
	}


	template< typename Ch >
	int tstring_find_ch( const Ch* str, Ch ch )
	{
		assert( str );
		assert( ch != 0 );

		int nPos = 0;
		while ( str[ 0 ] )
		{
			if ( str[ 0 ] == ch )
				return nPos;

			nPos ++;
			str ++;
		}

		return -1;
	}


	template< typename Ch >
	int tstring_rfind_ch( const Ch* str, Ch ch, int len )
	{
		assert( str );
		assert( ch != 0 );
		assert( len > 0 );

		int nPos = len - 1;
		str = &str[ nPos ];
		while ( nPos >= 0 )
		{
			if ( ( *str ) == ch )
				return nPos;

			nPos --;
			str --;
		}

		return -1;
	}


	template< typename Ch >
	int tstring_paste_endCh( const Ch* str, Ch ch, Ch* buf, int bufSize )
	{
		int nPos = tstring_find_ch( str, ch );
		if ( nPos < 0 )
			return nPos;

		int nPasteCount = (nPos>=bufSize) ? bufSize : nPos;
		memcpy_s( buf, bufSize, str, nPasteCount );
		buf[ nPasteCount ] = 0;

		return nPos;
	}

	template < typename Ch >
	bool tstring_start_with( const Ch* str, const Ch* com )
	{
		while ( *com != 0 )
		{
			if ( *str == 0 )
				return false;
			if ( *str != *com )
				return false;
			com++;
			str++;
		}
		return true;
	}


	//template< typename Ch = TCHAR >
	//inline void tstring_cut_fileext( Ch* buf, int bufSize, const Ch* str )
	//{
	//	int lc_wcslen = tstrlen( str );
	//	int lc_npos = tstring_rfind_ch( str, L'.', lc_wcslen );
	//	lc_npos = ( lc_npos < lc_wcslen ) ? lc_npos : lc_wcslen;
	//	lc_npos = ( lc_npos < (bufSize-1) ) ? lc_npos : (bufSize-1);
	//	memcpy( buf, /*sizeof( Ch ) * bufSize,*/ str, sizeof( Ch ) * lc_npos );
	//	buf[ lc_npos ] = 0;
	//}


	inline int string_find_ch_count( const char* str, char ch ) { return tstring_find_ch_count( str, ch ); }
	inline int string_find_ch( const char* str, char ch ) { return tstring_find_ch( str, ch ); }
	inline int string_rfind_ch( const char* str, char ch, int len ) { return tstring_rfind_ch( str, ch, len ); }

	inline int wstring_find_ch_count( const wchar_t* str, wchar_t ch ) { return tstring_find_ch_count( str, ch ); }
	inline int wstring_find_ch( const wchar_t* str, wchar_t ch ) { return tstring_find_ch( str, ch ); }
	inline int wstring_rfind_ch( const wchar_t* str, wchar_t ch, int len ) { return tstring_rfind_ch( str, ch, len ); }

	inline void wstring_cut_fileext( wchar_t* buf, int bufSize, const wchar_t* str )
	{
		int lc_wcslen = (int)wcslen( str );
		int lc_npos = tstring_rfind_ch( str, L'.', lc_wcslen );
		lc_npos = (lc_npos < lc_wcslen) ? lc_npos : lc_wcslen;
		lc_npos = (lc_npos < (bufSize - 1)) ? lc_npos : (bufSize - 1);
		wcsncpy_s( buf, bufSize, str, lc_npos );
		buf[lc_npos] = 0;
	}
	


//	inline int ascii_to_unicode( const char* pzMultiBytes, int nCount, wchar_t* pWideCharBuffer, int nBufferSize );
//	inline int ascii_to_unicode_calculate( const char* lpMultiBytes, int nCount );
//	inline int utf8_to_unicode( const char* pzMultiBytes, int nCount, wchar_t* pWideCharBuffer, int nBufferSize );
//	inline int utf8_to_unicode_calculate( const char* lpMultiBytes, int nCount );
//
//	inline int unicode_to_ascii( const wchar_t* lpWideChars, int nCount, char* pMultiByteBuffer, int nBufferSize );
//	inline int unicode_to_ascii_calculate( const wchar_t* lpWideChars, int nCount, char* pMultiByteBuffer );
//	inline int unicode_to_utf8( const wchar_t* lpWideChars, int nCount, char* pMultiByteBuffer, int nBufferSize );
//	inline int unicode_to_utf8_calculate( const wchar_t* lpWideChars, int nCount );
//
//
//
//	inline int ascii_to_unicode( const char* pzMultiBytes, int nCount, wchar_t* pWideCharBuffer, int nBufferSize )
//	{
//#ifdef WIN32
//		int lc_n = MultiByteToWideChar( CP_ACP, MB_COMPOSITE, pzMultiBytes, nCount, pWideCharBuffer, nBufferSize );
//		//pWideCharBuffer[ lc_n ] = 0;
//		return lc_n;
//#endif
//	}
//
//	inline int ascii_to_unicode_calculate( const char* lpMultiBytes, int nCount )
//	{
//		return ascii_to_unicode( lpMultiBytes, nCount, NULL, 0 );
//	}


//	inline int utf8_to_unicode( const char* pzMultiBytes, int nCount, wchar_t* pWideCharBuffer, int nBufferSize )
//	{
//#ifdef WIN32
//		int lc_n = MultiByteToWideChar( CP_UTF8, 0, pzMultiBytes, nCount, pWideCharBuffer, nBufferSize );
//		//pWideCharBuffer[ lc_n ] = 0;
//		return lc_n;
//#endif
//		std::wstring_convert<std::codecvt_utf8<wchar_t>> cvtUTF8;
//	}
//
//	inline int utf8_to_unicode_calculate( const char* lpMultiBytes, int nCount )
//	{
//		return ascii_to_unicode( lpMultiBytes, nCount, NULL, 0 );
//	}



//
//	inline int unicode_to_ascii( const wchar_t* lpWideChars, int nCount, char* pMultiByteBuffer, int nBufferSize )
//	{
//#ifdef WIN32
//		return WideCharToMultiByte( CP_ACP, 0, lpWideChars, nCount, pMultiByteBuffer, nBufferSize, NULL, NULL );
//#endif
//	}
//
//	inline int unicode_to_ascii_calculate( const wchar_t* lpWideChars, int nCount )
//	{
//		return unicode_to_ascii( lpWideChars, nCount, 0, 0 );
//	}
//
//
//
//	inline int unicode_to_utf8( const wchar_t* lpWideChars, int nCount, char* pMultiByteBuffer, int nBufferSize )
//	{
//#ifdef WIN32
//		return WideCharToMultiByte( CP_UTF8, 0, lpWideChars, nCount, pMultiByteBuffer, nBufferSize, NULL, NULL );
//#endif
//	}
//
//	inline int unicode_to_utf8_calculate( const wchar_t* lpWideChars, int nCount )
//	{
//		return unicode_to_utf8( lpWideChars, nCount, 0, 0 );
//	}

	std::wstring Utf8ToUnicode( const char* str );
	int Utf8ToUnicode( const char* str, wchar_t* pBuf, int nBufSize );
	std::u32string Utf8ToUtf32( const char* str );
	std::wstring AsciiToUnicode( const char* str );

	std::string UnicodeToUtf8( const wchar_t* str );
	std::string UnicodeToAscii( const wchar_t* str );

	template < typename Ch >
	inline bool StringIsNullOrEmpty( const Ch* s )
	{
		return s == nullptr || s[ 0 ] == 0;
	}


	template < typename Ch, int nBufferSize, int nBufferCount >
	class CStringEasySplitHelper
	{
	public:
		typedef char _Buffer[ nBufferSize ];

		CStringEasySplitHelper()
			: Buffers()
		{
			memset( Buffers, 0, sizeof( Buffers ) );
		}

		int Split( const Ch* strText, Ch nChar )
		{
			int nBufferNow = 0;
			int nCharIndex = 0;
			Ch* pBuffer = Buffers[ nBufferNow ];
			memset( pBuffer, 0, nBufferSize );
			const Ch* ptr = strText;
			while ( *ptr != 0 )
			{
				if ( *ptr == nChar )
				{
					pBuffer[ nCharIndex ] = 0;
					nBufferNow++;
					pBuffer = Buffers[ nBufferNow ];
					memset( pBuffer, 0, nBufferSize );
					nCharIndex = 0;
				}
				else
				{
					if ( nCharIndex < nBufferSize )
						pBuffer[ nCharIndex ] = *ptr;
					nCharIndex++;
				}
				ptr++;
			}
			pBuffer[ nCharIndex ] = 0;
			nBufferNow++;
			return nBufferNow;
		}

		inline const Ch* operator[]( int n )
		{
			return Buffers[ n ];
		}

	public:
		_Buffer Buffers[ nBufferCount ];
	};


	//template < typename Ch >
	//int TStringSplit( const Ch* strText, Ch nChar, Ch** ppBuffers, int nBufferSize, int nBufferCount )
	//{
	//	int nBufferNow = 0;
	//	int nCharIndex = 0;
	//	Ch* pBuffer = ppBuffers[ nBufferNow ];
	//	memset( pBuffer, 0, nBufferSize );
	//	const Ch* ptr = strText;
	//	while ( *ptr != 0 )
	//	{
	//		if ( *ptr == nChar )
	//		{
	//			pBuffer[ nCharIndex ] = 0;
	//			nBufferNow++;
	//			pBuffer = ppBuffers[ nBufferNow ];
	//			memset( pBuffer, 0, nBufferSize );
	//			nCharIndex = 0;
	//		}
	//		else
	//		{
	//			pBuffer[ nCharIndex ] = *ptr;
	//			nCharIndex++;
	//		}
	//		ptr++;
	//	}
	//	pBuffer[ nCharIndex ] = 0;
	//	nBufferNow++;
	//	return nBufferNow;
	//}

	
	inline bool IsEngLetter_Char32( char32_t ch )
	{
		return(ch >= U'a' && ch <= U'z') || (ch >= U'A' && ch <= U'Z');
	}

	inline bool IsEngLetter_Char8( char ch )
	{
		return(ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
	}

	inline bool Char32_IsWhiteSpace( char32_t ch )
	{
		return ch == U' ';
	}

	inline bool IsWrap_Char32( char32_t ch )
	{
		return ch == U'\n' || ch == U'\r';
	}


	template < typename TAlloc = std::allocator<char> >
	bool StringFormat( std::basic_string<char, std::char_traits<char>, TAlloc>& buffer, const char* format, ... );

	template < typename TAlloc = std::allocator<char> >
	bool StringFormatVa( std::basic_string<char, std::char_traits<char>, TAlloc>& buffer, const char* format, va_list va );

	std::string __iberbarUtilityApi__ StdFormat( const char* format, ... );
	std::string __iberbarUtilityApi__ StdFormatVa( const char* format, va_list va );
	void __iberbarUtilityApi__ StdFormatRef( std::string& str, const char* format, ... );
	std::string __iberbarUtilityApi__ StringToLower( const char* str );
	std::string __iberbarUtilityApi__ StringToUpper( const char* str );

	std::wstring __iberbarUtilityApi__ WStringToLower( const wchar_t* str );
	std::wstring __iberbarUtilityApi__ WStringToUpper( const wchar_t* str );

	std::tstring TStringToLower( const tchar* str );
	std::tstring TStringToUpper( const tchar* str );

	__iberbarUtilityApi__ std::string StdStringToLower( std::string& str );
	__iberbarUtilityApi__ std::string StdStringToUpper( std::string& str );
	std::string StdStringGetExt( const std::string& str );
	std::string StdStringGetExtLowerCase( const std::string& str );

	__iberbarUtilityApi__ std::wstring StdWStringToLower( std::wstring& str );
	__iberbarUtilityApi__ std::wstring StdWStringToUpper( std::wstring& str );
	std::wstring StdWStringGetExt( const std::wstring& str );
	std::wstring StdWStringGetExtLowerCase( const std::wstring& str );
}



inline std::wstring iberbar::Utf8ToUnicode( const char* str )
{
#ifdef _WIN32
	int nWcharCount = MultiByteToWideChar( CP_UTF8, 0, str, -1, nullptr, 0 );
	std::wstring strWchar;
	strWchar.resize( nWcharCount + 1 );
	MultiByteToWideChar( CP_UTF8, 0, str, -1, &strWchar.front(), nWcharCount + 1 );
	strWchar[ nWcharCount ] = 0;
	return strWchar;
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvtUTF8;
	return cvtUTF8.from_bytes( str );
#endif
}


inline int iberbar::Utf8ToUnicode( const char* str, wchar_t* pBuf, int nBufSize )
{
#ifdef _WIN32
	int nWcharCount = MultiByteToWideChar( CP_UTF8, 0, str, -1, nullptr, 0 );
	nWcharCount = tMin( nWcharCount, nBufSize - 1 );
	MultiByteToWideChar( CP_UTF8, 0, str, -1, pBuf, nWcharCount + 1 );
	pBuf[nWcharCount] = 0;
	return nWcharCount;
#else
#endif
}


inline std::string iberbar::UnicodeToUtf8( const wchar_t* str )
{
#ifdef _WIN32
	int nCharCount = WideCharToMultiByte( CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr );
	std::string strChar;
	strChar.resize( nCharCount + 1 );
	WideCharToMultiByte( CP_UTF8, 0, str, -1, &strChar.front(), nCharCount + 1, nullptr, nullptr );
	strChar[ nCharCount ] = 0;
	return strChar;
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvtUTF8;
	return cvtUTF8.to_bytes( str );
#endif
}





template < typename TAlloc >
bool iberbar::StringFormat( std::basic_string<char, std::char_traits<char>, TAlloc>& buffer, const char* format, ... )
{
	int charCount = 0;
	va_list va;
	va_start( va, format );
	charCount = std::vsnprintf( nullptr, 0, format, va );
	va_end( va );
	if ( charCount == 0 )
		return true;

	buffer.resize( charCount + 1 );
	va_start( va, format );
	std::vsnprintf( &buffer.front(), charCount + 1, format, va );
	va_end( va );

	return true;
}


template < typename TAlloc >
bool iberbar::StringFormatVa( std::basic_string<char, std::char_traits<char>, TAlloc>& buffer, const char* format, va_list va )
{
	int charCount = 0;
	charCount = std::vsnprintf( nullptr, 0, format, va );
	if ( charCount == 0 )
		return true;

	buffer.resize( charCount + 1 );
	std::vsnprintf( &buffer.front(), charCount + 1, format, va );

	return true;
}







inline std::tstring iberbar::TStringToLower( const tchar* str )
{
#ifdef UNICODE
	return WStringToLower( str );
#else
	return StringToLower( str );
#endif
}

inline std::tstring iberbar::TStringToUpper( const tchar* str )
{
#ifdef UNICODE
	return WStringToUpper( str );
#else
	return StringToUpper( str );
#endif
}



inline std::string iberbar::StdStringGetExt( const std::string& str )
{
	int nPos = str.find_last_of( '.' );
	if ( nPos == std::string::npos )
		return "";
	return str.substr( nPos );
}


inline std::string iberbar::StdStringGetExtLowerCase( const std::string& str )
{
	std::string strExt = StdStringGetExt( str );
	return StdStringToLower( strExt );
}


inline std::wstring iberbar::StdWStringGetExt( const std::wstring& str )
{
	int nPos = str.find_last_of( L'.' );
	if ( nPos == std::string::npos )
		return L"";
	return str.substr( nPos );
}


inline std::wstring iberbar::StdWStringGetExtLowerCase( const std::wstring& str )
{
	std::wstring strExt = StdWStringGetExt( str );
	return StdWStringToLower( strExt );
}

