
#include <iberbar/Renderer/Effect.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/RHI/Buffer.h>
#include <iberbar/RHI/Device.h>


iberbar::Renderer::CEffectBase::CEffectBase()
	: m_pRhiUniformBuffer( nullptr )
{

}


iberbar::Renderer::CEffectBase::~CEffectBase()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pRhiUniformBuffer );
}


iberbar::CResult iberbar::Renderer::CEffectBase::InitialRhiUniformBuffer( uint32 nSize )
{
	RHI::IDevice* pRhiDevice = CRenderer::sGetInstance()->GetRHIDevice();
	return pRhiDevice->CreateUniformBuffer( &m_pRhiUniformBuffer, nSize );
}
