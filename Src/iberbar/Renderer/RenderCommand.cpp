
#include "RenderCommand.h"
#include <iberbar/RHI/ShaderState.h>


iberbar::Renderer::CRenderCommand::CRenderCommand( URenderCommandType nType )
	: m_nCommandType( nType )
	, m_nZOrder( 0 )
{
}


iberbar::Renderer::CRenderCommand::~CRenderCommand()
{
}
