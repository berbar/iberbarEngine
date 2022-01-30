
#include <iberbar/Renderer/Shader.h>
#include <iberbar/RHI/Shader.h>


iberbar::Renderer::CShaderProgram::CShaderProgram(
	RHI::IShader* pVS,
	RHI::IShader* pPS,
	RHI::IShader* pHS,
	RHI::IShader* pGS,
	RHI::IShader* pDS )
	: CRenderResource()
	, m_pShaders()
{
	memset( m_pShaders, 0, sizeof( m_pShaders ) );
}


iberbar::Renderer::CShaderProgram::~CShaderProgram()
{
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaders[ i ] );
	}
}