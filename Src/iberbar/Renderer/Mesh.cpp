
#include <iberbar/Renderer/Mesh.h>

iberbar::Renderer::IMesh::IMesh()
	: m_VertexDataSlots()
{
	memset( m_VertexDataSlots, 0, sizeof( m_VertexDataSlots ) );
}





iberbar::Renderer::CMesh_V3F_C4F_T2F::CMesh_V3F_C4F_T2F()
	: m_MeshVertices()
	, m_MeshIndices()
{
	memset( m_MeshVertices, 0, sizeof( m_MeshVertices ) );

	m_MeshIndices[0] = 0;
	m_MeshIndices[1] = 1;
	m_MeshIndices[2] = 2;
	m_MeshIndices[3] = 1;
	m_MeshIndices[4] = 3;
	m_MeshIndices[5] = 2;

	m_nVertexSlotCount = 1;
	m_VertexDataSlots[ 0 ] = m_MeshVertices;
	m_IndexData = m_MeshIndices;

	//RHI::UVertexDeclarationDesc m_VertexDeclarationDesc;
	m_VertexDeclarationDesc.nSlotCount = 1;
	m_VertexDeclarationDesc.nVertexElementsCount = 3;
	m_VertexDeclarationDesc.Strides[ 0 ] = sizeof( Renderer::UVertex_V3F_C4B_T2F );
	m_VertexDeclarationDesc.VertexElements[ 0 ] = { 0, RHI::UVertexDeclareUsage::Position, 0, RHI::UVertexFormat::FLOAT3, offsetof( Renderer::UVertex_V3F_C4B_T2F, position ) };
	m_VertexDeclarationDesc.VertexElements[ 1 ] = { 0, RHI::UVertexDeclareUsage::Color, 0, RHI::UVertexFormat::FLOAT4, offsetof( Renderer::UVertex_V3F_C4B_T2F, color ) };
	m_VertexDeclarationDesc.VertexElements[ 2 ] = { 0, RHI::UVertexDeclareUsage::TexCoord, 0, RHI::UVertexFormat::FLOAT2, offsetof( Renderer::UVertex_V3F_C4B_T2F, texcoord ) };
}