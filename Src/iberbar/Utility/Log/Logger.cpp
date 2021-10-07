
#include <iberbar/Utility/Log/Logger.h>


iberbar::Logging::CLogger::CLogger()
	: m_pOutputDevice( nullptr )
#ifdef _DEBUG
	, m_nLevel( ULevel::Debug )
#else
	, m_nLevel( ULevel::Info )
#endif
{

}


iberbar::Logging::CLogger::CLogger( COutputDevice* pDevice )
	: m_pOutputDevice( pDevice )
#ifdef _DEBUG
	, m_nLevel( ULevel::Debug )
#else
	, m_nLevel( ULevel::Info )
#endif
{
	UNKNOWN_SAFE_ADDREF( m_pOutputDevice );
}


iberbar::Logging::CLogger::CLogger( const CLogger& Other )
	: m_pOutputDevice( Other.m_pOutputDevice )
	, m_nLevel( Other.m_nLevel )
{
	UNKNOWN_SAFE_ADDREF( m_pOutputDevice );
}


iberbar::Logging::CLogger::~CLogger()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pOutputDevice );
}