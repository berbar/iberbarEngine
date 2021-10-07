
#include <iberbar/RHI/Resource.h>
#include <iberbar/RHI/Trace.h>


iberbar::RHI::IResource::IResource( UResourceType nResourceType )
	: m_nResourceType( nResourceType )
	, m_bIsManaged( false )
	, m_bIsLost( false )
{
	CTrace::sGetInstance()->AddResource( this );
}

iberbar::RHI::IResource::~IResource()
{
	CTrace::sGetInstance()->RemoveResource( this );
}
