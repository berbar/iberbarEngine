
#include "DynamicLibrary.h"


iberbar::OS::CDynamicLibrary::CDynamicLibrary()
	: m_strLibName( "" )
	, m_hModule( nullptr )
{
}


iberbar::OS::CDynamicLibrary::~CDynamicLibrary()
{
	Free();
}


iberbar::CResult iberbar::OS::CDynamicLibrary::Load( const char* strLibName )
{
	assert( m_hModule == nullptr );

	CResult Ret;

	std::string strLibPath = strLibName;
	strLibPath += ".dll";

	m_hModule = ::LoadLibraryA( strLibPath.c_str() );
	if ( m_hModule == nullptr )
	{
		LPVOID lpMsgBuf = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
			(LPSTR)&lpMsgBuf,
			0,
			NULL
		);
		Ret = MakeResult( ResultCode::Bad, (LPSTR)lpMsgBuf );
		LocalFree( lpMsgBuf );
		return Ret;
	}

	return CResult();
}


void iberbar::OS::CDynamicLibrary::Free()
{
	if ( m_hModule )
	{
		::FreeLibrary( m_hModule );
		m_hModule = nullptr;
	}
}


void* iberbar::OS::CDynamicLibrary::GetProcAddress( const char* strProcName )
{
	return ::GetProcAddress( m_hModule, strProcName );
}

