
#include "TrianglesCommand.h"
#include <iberbar/RHI/Texture.h>


iberbar::Renderer::CTrianglesCommand::CTrianglesCommand()
	: CRenderCommand( URenderCommandType::Triangles )
	, m_pMaterial( nullptr )
	, m_Triangles()
{
}
