
#include <iberbar/Utility/RefStatistics.h>



iberbar::CRefStatistics iberbar::CRefStatistics::sm_pSharedInstance;


iberbar::CRefStatistics::CRefStatistics()
	: m_Refs()
{
}


iberbar::CRefStatistics::~CRefStatistics()
{
	//auto iter = m_Refs.begin();
	//auto end = m_Refs.end();
	//for ( ; iter != end; iter++ )
	//{
	//	SAFE_DELETE( iter->second );
	//}
	//m_Refs.clear();
}


void iberbar::CRefStatistics::ForEach( std::function<void( CRef* )> func ) const
{
	auto iter = m_Refs.begin();
	auto end = m_Refs.end();
	for ( ; iter != end; iter++ )
	{
		func( iter->second );
	}
}
