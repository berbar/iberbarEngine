
#include "DynamicLibrary.h"
#include <dlfcn.h>


iberbar::OS::CDynamicLibrary::CDynamicLibrary()
	: m_strLibName( "" )
	, m_pModule( nullptr )
{
}


iberbar::OS::CDynamicLibrary::~CDynamicLibrary()
{
	Free();
}


iberbar::CResult iberbar::OS::CDynamicLibrary::Load( const char* strLibName )
{
	assert( m_pModule == nullptr );

	std::string strLibPath = strLibName;
	strLibPath += ".so";

	m_pModule = dlopen( strLibName, RTLD_NOW );

	if ( m_pModule == nullptr )
		return MakeResult( ResultCode::Bad, dlerror() );

	m_strLibName = strLibName;

	return CResult();
}


void iberbar::OS::CDynamicLibrary::Free()
{
	if ( m_pModule )
	{
		::dlclose( m_pModule );
		m_pModule = nullptr;
	}
}


void* iberbar::OS::CDynamicLibrary::GetProcAddress( const char* strProcName )
{
	return ::dlsym( m_pModule, strProcName );
}

