#pragma once


#include <iberbar/Utility/Platform.h>

namespace iberbar
{
	namespace Paths
	{
		std::string GetExtensionA( const std::string& strPath );
		std::wstring GetExtensionW( const std::wstring& strPath );

		std::string CombineA( const std::string& strPath1, const std::string& strPath2 );
	}
}


inline std::string iberbar::Paths::GetExtensionA( const std::string& strPath )
{
	int nDotPos = strPath.rfind( '.' );
	if ( nDotPos == std::string::npos )
		return "";
	return strPath.substr( nDotPos );
}


inline std::wstring iberbar::Paths::GetExtensionW( const std::wstring& strPath )
{
	int nDotPos = strPath.rfind( L'.' );
	if ( nDotPos == std::string::npos )
		return L"";
	return strPath.substr( nDotPos );
}

inline std::string iberbar::Paths::CombineA( const std::string& strPath1, const std::string& strPath2 )
{
	if ( strPath1.empty() )
		return strPath2;
	char LastChar = *(strPath1.rbegin());
	char FirstChar = *(strPath1.begin());
	if ( LastChar == '/' || LastChar == '\\' || FirstChar == '/' || FirstChar == '\\' )
		return strPath1 + strPath2;
	return strPath1 + "/" + strPath2;
}