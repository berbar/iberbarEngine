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



}