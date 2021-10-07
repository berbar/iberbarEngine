
#include <iberbar/RHI/D3D11/Effect.h>
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/Device.h>



namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			template < typename TD3DShader >
			CResult GenerateD3DShaderDesc( EShaderType eShaderType );
		}
	}
}



iberbar::RHI::D3D11::CEffect::CEffect()
	: m_pDevice( nullptr )
	, m_pShaderSlots()
{
	memset( m_pShaderSlots, 0, ARRAYSIZE( m_pShaderSlots ) );
}


iberbar::RHI::D3D11::CEffect::~CEffect()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
	for ( int i = 0, s = (int)EShaderType::__Count; i < s; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderSlots[i] );
	}
}


void iberbar::RHI::D3D11::CEffect::SetShader( EShaderType eShaderType, IShader* pShader )
{
	if ( m_pShaderSlots[ (int)eShaderType ] == pShader )
		return;

	if ( pShader )
	{
		assert( pShader->GetShaderType() == eShaderType );
	}

	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderSlots[(int)eShaderType] );
	m_pShaderSlots[(int)eShaderType] = (CShader*)pShader;
	UNKNOWN_SAFE_ADDREF( m_pShaderSlots[(int)eShaderType] );
}


iberbar::CResult iberbar::RHI::D3D11::CEffect::Generate()
{

}


iberbar::CResult iberbar::RHI::D3D11::CEffect::GenerateShaderDesc( EShaderType eShaderType )
{
	
	
}

