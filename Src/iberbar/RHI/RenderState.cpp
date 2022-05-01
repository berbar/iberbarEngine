
#include <iberbar/RHI/RenderState.h>


iberbar::RHI::IBlendState::IBlendState( const UBlendDesc& Desc )
	: IResource( UResourceType::BlendState )
	, m_Desc()
{
	memcpy_s( &m_Desc, sizeof( UBlendDesc ), &Desc, sizeof( UBlendDesc ) );
}





iberbar::RHI::IDepthStencilState::IDepthStencilState(const UDepthStencilDesc& Desc)
	: IResource(UResourceType::DepthStencilState)
	, m_Desc()
{
	memcpy_s(&m_Desc, sizeof(UDepthStencilDesc), &Desc, sizeof(UDepthStencilDesc));
}





iberbar::RHI::ISamplerState::ISamplerState( const UTextureSamplerState& SamplerStateDesc )
	: IResource( UResourceType::SamplerState )
	, m_SamplerStateDesc()
{
	memcpy_s( &m_SamplerStateDesc, sizeof( UTextureSamplerState ), &SamplerStateDesc, sizeof( UTextureSamplerState ) );
}
