#pragma once

#include <iberbar/Utility/Platform.h>
#include <optional>

namespace iberbar
{
	enum class ResultCode
	{
		Bad = -1,
		Okay = 0,
	};

	class __iberbarUtilityApi__ CResult
	{
	public:
		ResultCode code;
		std::string data;

	public:
		CResult()
			: code( ResultCode::Okay )
			, data()
		{
		}

		CResult( ResultCode code, const char* data )
			: code( code )
			, data( data )
		{
		}

		~CResult()
		{
		}

	public:
		bool IsOK() const
		{
			return ( this->code == ResultCode::Okay ) ? true : false;
		}
	};



	inline CResult MakeResult( ResultCode code, const char* fmt, ... )
	{
		if ( fmt == nullptr )
			return CResult( code, "" );

		CResult ret( code, "" );
		int charCount = 0;
		va_list va;
		va_start( va, fmt );
		charCount = std::vsnprintf( nullptr, 0, fmt, va );
		va_end( va );
		if ( charCount == 0 )
			return ret;

		ret.data.resize( charCount + 1 );
		va_start( va, fmt );
		std::vsnprintf( &ret.data.front(), charCount + 1, fmt, va );
		va_end( va );
		
		return ret;
	}


	//template < typename T >
	//struct TResult
	//{
	//	ResultCode code;
	//	std::string erro;
	//	std::optional<T> data;

	//	TResult( ResultCode code )
	//		: code( code )
	//		, erro()
	//		, data()
	//	{
	//	}

	//	bool IsOK() const
	//	{
	//		return (this->code == ResultCode::Okay) ? true : false;
	//	}
	//};

	//template < typename T>
	//inline TResult<T> MakeResultBad( const char* fmt, ... )
	//{
	//	TResult<T> ret( ResultCode::Bad );
	//	int charCount = 0;
	//	va_list va;
	//	va_start( va, fmt );
	//	charCount = std::vsnprintf( nullptr, 0, fmt, va );
	//	va_end( va );
	//	if (charCount == 0)
	//		return ret;

	//	ret.erro.resize( charCount + 1 );
	//	va_start( va, fmt );
	//	std::vsnprintf( &ret.erro.front(), charCount + 1, fmt, va );
	//	va_end( va );

	//	return ret;
	//}

	//template < typename T >
	//inline TResult<T> MakeResultOkay()
	//{
	//	return TResult<T>( ResultCode::Okay );
	//}

	//template < typename T >
	//inline TResult<T> MakeResultOkay( const T& data )
	//{
	//	TResult<T> ret( ResultCode::Okay );
	//	ret.data = data;
	//	return ret;
	//}


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