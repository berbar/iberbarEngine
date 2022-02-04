
#include <iberbar/RHI/D3D11/ShaderState.h>
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/VertexDeclaration.h>
#include <iberbar/RHI/D3D11/Buffer.h>
#include <iberbar/RHI/D3D11/Device.h>


iberbar::RHI::D3D11::CShaderState::CShaderState(
	CDevice* pDevice,
	CVertexDeclaration* pVertexDeclaration,
	CShaderProgram* pShaderProgram )
	: m_pDevice( pDevice )
	, m_pVertexDeclaration( pVertexDeclaration )
	, m_pShaderProgram( pShaderProgram )
{
	assert( m_pDevice );

	assert( m_pVertexDeclaration );
	m_pVertexDeclaration->AddRef();

	assert( m_pShaderProgram );
	m_pShaderProgram->AddRef();
}


iberbar::RHI::D3D11::CShaderState::~CShaderState()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexDeclaration );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderProgram );

}


iberbar::RHI::IShaderProgram* iberbar::RHI::D3D11::CShaderState::GetShaderProgram()
{
	return m_pShaderProgram;
}


iberbar::RHI::IVertexDeclaration* iberbar::RHI::D3D11::CShaderState::GetVertexDeclaration()
{
	return m_pVertexDeclaration;
}


//iberbar::RHI::IUniformBuffer** iberbar::RHI::D3D11::CShaderState::GetUniformBuffers( EShaderType eShaderType )
//{
//	return (IUniformBuffer**)m_UniformBuffers[ (int)eShaderType ];
//}


iberbar::CResult iberbar::RHI::D3D11::CShaderState::Initial()
{
	if ( m_pShaderProgram->GetVertexShader() == nullptr )
		return MakeResult( ResultCode::Bad, "" );

	D3D11_INPUT_ELEMENT_DESC InputElementDescs[16];
	m_pVertexDeclaration->BuildD3DInputElementDescritions( InputElementDescs );

	HRESULT hResult = m_pDevice->GetD3DDevice()->CreateInputLayout(
		InputElementDescs, m_pVertexDeclaration->GetVertexElementsCount(),
		m_pShaderProgram->GetVertexShader()->GetCodePointer(), m_pShaderProgram->GetVertexShader()->GetCodeSize(),
		&m_pD3DInputLayout );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );




	return CResult();
}
