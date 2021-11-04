
#include <iberbar/RHI/D3D11/StateCache.h>
#include <iberbar/RHI/D3D11/Device.h>


iberbar::RHI::D3D11::CStateCache::CStateCache( CDevice* pDevice )
	: m_pD3DDeviceContext( pDevice->GetD3DDeviceContext() )
	, m_BlendFactor()
	, m_BlendSampleMask( 0 )
	, m_pD3DBlendState( nullptr )

	, m_pD3DDepthStencilState( nullptr )
	, m_nDepthStencilRef( 0 )

	, m_D3DSamplerStates()

	, m_pD3DInputLayout( nullptr )
	, m_pD3DVertexShader( nullptr )
	, m_pD3DPixelShader( nullptr )
	, m_pD3DGeometryShader( nullptr )
	, m_pD3DDomainShader( nullptr )
	, m_pD3DHullShader( nullptr )
	, m_pD3DComputeShader( nullptr )

	, m_StreamStrides()

	, m_VertexBufferStates()

	, m_pD3DIndexBuffer( nullptr )
	, m_nIndexFormat( DXGI_FORMAT_UNKNOWN )
	, m_nIndexOffset( -1 )
{
	assert( m_pD3DDeviceContext );
	m_pD3DDeviceContext->AddRef();

	memset( m_StreamStrides, 0, sizeof( m_StreamStrides ) );
	memset( m_VertexBufferStates, 0, sizeof( m_VertexBufferStates ) );
}


iberbar::RHI::D3D11::CStateCache::~CStateCache()
{
	D3D_SAFE_RELEASE( m_pD3DDeviceContext );
}