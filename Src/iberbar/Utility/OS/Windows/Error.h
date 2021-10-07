#pragma once

#include <iberbar/Utility/Result.h>

namespace iberbar
{
	namespace OS
	{

#ifdef WIN32
		inline void ConvertHResultA( std::string& str, HRESULT hr )
		{
			HLOCAL lpMsgBuf;
			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_MAX_WIDTH_MASK,
				NULL,
				hr,
				MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
				(LPSTR)&lpMsgBuf,
				0, NULL );
			str = (LPSTR)lpMsgBuf;
			LocalFree( lpMsgBuf );
		}

		inline CResult ConvertHResultA( HRESULT hr )
		{
			char strMsgBuf[256];
			memset( strMsgBuf, 0, sizeof( strMsgBuf ) );
			FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				hr,
				MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
				strMsgBuf,
				256, NULL );
			CResult Ret;
			Ret.code = ResultCode::Bad;
			Ret.data = strMsgBuf;
			return Ret;
		}

		inline void ConvertHResultW( std::wstring& str, HRESULT hr )
		{
			HLOCAL lpMsgBuf;
			FormatMessageW(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_MAX_WIDTH_MASK,
				NULL,
				hr,
				MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
				(LPWSTR)&lpMsgBuf,
				0, NULL );
			str = (LPWSTR)lpMsgBuf;
			LocalFree( lpMsgBuf );
		}

		//	inline void ConvertHResult( std::tstring& str, HRESULT hr )
		//	{
		//#ifdef UNICODE
		//		ConvertHResultW( str, hr );
		//#else
		//		ConvertHResultA( str, hr );
		//#endif
		//	}

#endif
	}
}