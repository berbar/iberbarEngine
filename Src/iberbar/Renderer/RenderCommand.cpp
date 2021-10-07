
#include "RenderCommand.h"
#include <iberbar/RHI/ShaderState.h>


iberbar::Renderer::CRenderCommand::CRenderCommand( URenderCommandType nType )
	: m_nCommandType( nType )
	, m_nZOrder( 0 )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTableArray()
{
	memset( m_pShaderVariableTableArray, 0, sizeof( m_pShaderVariableTableArray ) );
}


iberbar::Renderer::CRenderCommand::~CRenderCommand()
{
	//UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
}
