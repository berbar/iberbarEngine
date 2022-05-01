
#include <iberbar/Renderer/Effects/EffectMatrices.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/RHI/Buffer.h>
#include <iberbar/RHI/CommandContext.h>
#include <iberbar/RHI/Device.h>


iberbar::CResult iberbar::Renderer::CEffectMatrices::Initial()
{
	return InitialRhiUniformBuffer( sizeof( _Data ) );
}


void iberbar::Renderer::CEffectMatrices::Apply()
{
	if ( m_DirtyData != 0 )
	{
		DirectX::XMMATRIX ViewMatrix = DirectX::XMLoadFloat4x4( &m_Data.ViewMatrix );
		DirectX::XMMATRIX ProjectionMatrix = DirectX::XMLoadFloat4x4( &m_Data.ProjectionMatrix );
		
		DirectX::XMMATRIX ViewProjectionMatrix = DirectX::XMMatrixMultiply( ViewMatrix, ProjectionMatrix );
		//DirectX::XMMATRIX ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
		//m_matViewProjectionTranspose = DirectX::XMMatrixTranspose( m_matViewProjection );
		DirectX::XMStoreFloat4x4( &m_Data.ViewProjectionMatrix, ViewProjectionMatrix );

		m_pRhiUniformBuffer->UpdateContents( &m_Data, sizeof( _Data ) );

		m_DirtyData = 0;
	}

	RHI::ICommandContext* pRhiContext = CRenderer::sGetInstance()->GetRHIDevice()->GetDefaultContext();
	uint32 nBindPoint = g_nRhiUniformBindPoint_Matrices;
	pRhiContext->SetUniformBuffer( RHI::EShaderType::VertexShader, nBindPoint, m_pRhiUniformBuffer );
	pRhiContext->SetUniformBuffer( RHI::EShaderType::PixelShader, nBindPoint, m_pRhiUniformBuffer );
	pRhiContext->SetUniformBuffer( RHI::EShaderType::GeometryShader, nBindPoint, m_pRhiUniformBuffer );
	pRhiContext->SetUniformBuffer( RHI::EShaderType::DomainShader, nBindPoint, m_pRhiUniformBuffer );
	pRhiContext->SetUniformBuffer( RHI::EShaderType::HullShader, nBindPoint, m_pRhiUniformBuffer );
	pRhiContext->SetUniformBuffer( RHI::EShaderType::ComputeShader, nBindPoint, m_pRhiUniformBuffer );
}
