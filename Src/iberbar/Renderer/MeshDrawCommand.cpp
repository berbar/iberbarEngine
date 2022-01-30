
#include <iberbar/Renderer/MeshDrawCommand.h>



iberbar::Renderer::CMeshDrawCommand::CMeshDrawCommand()
	: CRenderCommand( URenderCommandType::DrawMesh )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTables( nullptr )
	, m_pMesh( nullptr )
{
}
