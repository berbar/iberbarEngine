
#include "RenderCommand.h"
#include <iberbar/RHI/ShaderState.h>


iberbar::Renderer::CRenderCommand::CRenderCommand()
	: m_nCommandType( URenderCommandType::Unknown )
	, m_nZOrder( 0 )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTable( nullptr )
{
}

iberbar::Renderer::CRenderCommand::~CRenderCommand()
{
	//UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
}
