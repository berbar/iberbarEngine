
#include <iberbar/RHI/Trace.h>


iberbar::RHI::CTrace iberbar::RHI::CTrace::sm_Instance;

iberbar::RHI::CTrace::CTrace()
	: m_ResourcesMap()
{
}


void iberbar::RHI::CTrace::AddResource( IResource* pResource )
{
	m_ResourcesMap.insert( std::make_pair( (uintptr_t)pResource, pResource ) );
}


void iberbar::RHI::CTrace::RemoveResource( IResource* pResource )
{
	m_ResourcesMap.erase( (uintptr_t)pResource );
}


void iberbar::RHI::CTrace::ForeachResource( std::function<void( IResource* )> Proc )
{
	auto iter = m_ResourcesMap.begin();
	auto end = m_ResourcesMap.end();
	for ( ; iter != end; iter++ )
	{
		Proc( iter->second );
	}
}
