
#include <iberbar/Renderer/MeshDrawCommand.h>



iberbar::Renderer::CMeshDrawCommand::CMeshDrawCommand()
	: CRenderCommand( URenderCommandType::DrawMesh )
	, m_pMesh( nullptr )
	, m_pMaterial( nullptr )
{
}
