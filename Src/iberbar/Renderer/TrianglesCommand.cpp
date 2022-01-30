
#include "TrianglesCommand.h"
#include <iberbar/RHI/Texture.h>


iberbar::Renderer::CTrianglesCommand::CTrianglesCommand()
	: CRenderCommand( URenderCommandType::Triangles )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTables( nullptr )
	, m_Triangles()
{
}
