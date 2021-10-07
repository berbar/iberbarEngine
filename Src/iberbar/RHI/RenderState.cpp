
#include <iberbar/RHI/RenderState.h>


iberbar::RHI::IBlendState::IBlendState( const UBlendDesc& Desc )
	: IResource( UResourceType::BlendState )
	, m_Desc()
{
	memcpy_s( &m_Desc, sizeof( UBlendDesc ), &Desc, sizeof( UBlendDesc ) );
}






iberbar::RHI::ISamplerState::ISamplerState( const UTextureSamplerState& SamplerStateDesc )
	: IResource( UResourceType::SamplerState )
	, m_SamplerStateDesc()
{
	memcpy_s( &m_SamplerStateDesc, sizeof( UTextureSamplerState ), &SamplerStateDesc, sizeof( UTextureSamplerState ) );
}
