
#include <iberbar/RHI/D3D11/ShaderState.h>
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/VertexDeclaration.h>
#include <iberbar/RHI/D3D11/Device.h>


iberbar::RHI::D3D11::CShaderState::CShaderState(
	CDevice* pDevice,
	CVertexDeclaration* pVertexDeclaration,
	CVertexShader* pVertexShader,
	CPixelShader* pPixelShader,
	CHullShader* pHullShader,
	CGeometryShader* pGeometryShader,
	CDomainShader* pDomainShader )
	: m_pDevice( pDevice )
{
	assert( m_pDevice );
	m_pDevice->AddRef();

	assert( pVertexDeclaration );
	m_pVertexDeclaration = pVertexDeclaration;
	m_pVertexDeclaration->AddRef();

	m_pVertexShader = pVertexShader;
	UNKNOWN_SAFE_ADDREF( m_pVertexShader );

	m_pPixelShader = pPixelShader;
	UNKNOWN_SAFE_ADDREF( m_pPixelShader );

	m_pHullShader = pHullShader;
	UNKNOWN_SAFE_ADDREF( m_pHullShader );

	m_pGeometryShader = pGeometryShader;
	UNKNOWN_SAFE_ADDREF( m_pGeometryShader );

	m_pDomainShader = pDomainShader;
	UNKNOWN_SAFE_ADDREF( m_pDomainShader );
}


iberbar::RHI::D3D11::CShaderState::~CShaderState()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexDeclaration );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pPixelShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pHullShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pGeometryShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDomainShader );
}


iberbar::RHI::IShader* iberbar::RHI::D3D11::CShaderState::GetShader( EShaderType eShaderType )
{
	return m_pShaders[ (int)eShaderType ];
}


iberbar::CResult iberbar::RHI::D3D11::CShaderState::Initial()
{
	if ( m_pVertexShader == nullptr )
		return MakeResult( ResultCode::Bad, "" );

	D3D11_INPUT_ELEMENT_DESC InputElementDescs[16];
	m_pVertexDeclaration->BuildD3DInputElementDescritions( InputElementDescs );

	HRESULT hResult = m_pDevice->GetD3DDevice()->CreateInputLayout(
		InputElementDescs, m_pVertexDeclaration->GetVertexElementsCount(),
		m_pVertexShader->GetCodePointer(), m_pVertexShader->GetCodeSize(),
		&m_pD3DInputLayout );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );

	return CResult();
}
