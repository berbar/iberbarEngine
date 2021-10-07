

#include <iberbar/RHI/D3D11/RenderState.h>
#include <iberbar/RHI/D3D11/Device.h>
#include <iberbar/RHI/D3D11/Types.h>

iberbar::CResult iberbar::RHI::D3D11::CBlendState::Create( CDevice* pDevice )
{
	D3D11_BLEND_DESC Desc;
	memset( &Desc, 0, sizeof( D3D11_BLEND_DESC ) );
	Desc.AlphaToCoverageEnable = m_Desc.AlphaToCoverageEnable;
	Desc.IndependentBlendEnable = m_Desc.IndependentBlendEnable;
	for ( int i = 0; i < 8; i++ )
	{
		Desc.RenderTarget[ i ].BlendEnable = m_Desc.RenderTargets[ i ].BlendEnable;
		Desc.RenderTarget[ i ].SrcBlend = ConvertBlend( m_Desc.RenderTargets[ i ].SrcBlend );
		Desc.RenderTarget[ i ].DestBlend = ConvertBlend( m_Desc.RenderTargets[ i ].DestBlend );
		Desc.RenderTarget[ i ].BlendOp = ConvertBlendOP( m_Desc.RenderTargets[ i ].BlendOp );
		Desc.RenderTarget[ i ].SrcBlendAlpha = ConvertBlend( m_Desc.RenderTargets[ i ].SrcBlendAlpha );
		Desc.RenderTarget[ i ].DestBlendAlpha = ConvertBlend( m_Desc.RenderTargets[ i ].DestBlendAlpha );
		Desc.RenderTarget[ i ].BlendOpAlpha = ConvertBlendOP( m_Desc.RenderTargets[ i ].BlendOpAlpha );
		Desc.RenderTarget[ i ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	HRESULT hResult = pDevice->GetD3DDevice()->CreateBlendState( &Desc, &m_pD3DBlendState );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CSamplerState::Create( CDevice* pDevice )
{
	CD3D11_SAMPLER_DESC Desc = CD3D11_SAMPLER_DESC( CD3D11_DEFAULT() );
	Desc.Filter = EncodeTextureFilter( m_SamplerStateDesc.nMinFilter, m_SamplerStateDesc.nMagFilter, m_SamplerStateDesc.nMipFilter );
	Desc.AddressU = ConvertTextureAddressMode( m_SamplerStateDesc.nAddressU );
	Desc.AddressV = ConvertTextureAddressMode( m_SamplerStateDesc.nAddressV );
	Desc.AddressW = ConvertTextureAddressMode( m_SamplerStateDesc.nAddressW );
	HRESULT hResult = pDevice->GetD3DDevice()->CreateSamplerState( &Desc, &m_pD3DSamplerState );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );
	return CResult();
}
