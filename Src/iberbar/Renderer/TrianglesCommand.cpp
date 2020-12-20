
#include "TrianglesCommand.h"
#include <iberbar/RHI/Texture.h>


iberbar::Renderer::CTrianglesCommand::CTrianglesCommand()
	: CRenderCommand()
	, m_Triangles()
{
	m_nCommandType = URenderCommandType::Triangles;
}
