
#include <iberbar/Renderer/Effects/EffectMatrices.h>
#include <iberbar/RHI/Buffer.h>


void iberbar::Renderer::CEffectMatrices::Apply()
{
	if ( m_DirtyData != 0 )
	{
		m_pRhiUniformBuffer->UpdateContents( &m_Data, sizeof( _Data ) );
	}

	//C->SetUniformBuffer( nShaderType, pShaderReflectionBuffer->GetBindPoint(), pUniformBuffer );
}
