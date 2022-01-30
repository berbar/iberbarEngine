
#include <iberbar/RHI/Shader.h>



iberbar::RHI::IShaderProgram::IShaderProgram(
	RHI::IShader* pVS,
	RHI::IShader* pPS,
	RHI::IShader* pHS,
	RHI::IShader* pGS,
	RHI::IShader* pDS )
	: IResource( UResourceType::ShaderProgram )
	, m_pShaders()
{
	memset( m_pShaders, 0, sizeof( m_pShaders ) );

	if ( pVS && pVS->GetShaderType() == EShaderType::VertexShader )
	{
		m_pShaders[ (int)EShaderType::VertexShader ] = pVS;
		m_pShaders[ (int)EShaderType::VertexShader ]->AddRef();
	}
	
	if ( pPS && pPS->GetShaderType() == EShaderType::PixelShader )
	{
		m_pShaders[ (int)EShaderType::PixelShader ] = pPS;
		m_pShaders[ (int)EShaderType::PixelShader ]->AddRef();
	}
	
	if ( pHS && pHS->GetShaderType() == EShaderType::HullShader )
	{
		m_pShaders[ (int)EShaderType::HullShader ] = pHS;
		m_pShaders[ (int)EShaderType::HullShader ]->AddRef();
	}
	
	if ( pGS && pGS->GetShaderType() == EShaderType::GeometryShader )
	{
		m_pShaders[ (int)EShaderType::GeometryShader ] = pGS;
		m_pShaders[ (int)EShaderType::GeometryShader ]->AddRef();
	}

	if ( pDS && pDS->GetShaderType() == EShaderType::DomainShader )
	{
		m_pShaders[ (int)EShaderType::DomainShader ] = pDS;
		m_pShaders[ (int)EShaderType::DomainShader ]->AddRef();
	}
}


iberbar::RHI::IShaderProgram::~IShaderProgram()
{
	for ( int i = 0, s = (int)EShaderType::__Count; i < s; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaders[ i ] );
	}
}
