
#include <iberbar/Renderer/RendererState.h>
#include <iberbar/Renderer/MeshDrawCommand.h>
#include <iberbar/Renderer/Material.h>
#include <iberbar/RHI/ShaderState.h>



bool iberbar::Renderer::CRendererState::AddCommand( CMeshDrawCommand* pCommand )
{
	if ( pCommand->GetMaterial() == nullptr ||
		pCommand->GetMaterial()->GetShaderState() == nullptr )
		return true;

	const CMesh* pMesh = pCommand->GetMesh();

	if ( m_CommandList_Render.empty() == true )
	{
		m_CommandList_Render.push_back( pCommand );
		m_pUsingShaderState = pCommand->GetMaterial()->GetShaderState();
		//m_nVertexSlotCount = m_pUsingShaderState->GetVertexDeclaration()->GetSlotCount();

		memset( m_nVertexSizes, 0, sizeof( m_nVertexSizes ) );
		for ( uint32 nSlotIndex = 0; nSlotIndex < m_nVertexSlotCount; nSlotIndex++ )
		{
			m_nVertexSizes[ nSlotIndex ] = pMesh->GetVertexSize( nSlotIndex );
		}

		for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
		{
			m_pUsingShaderVarTables[ i ] = &(pCommand->GetShaderVariableTables()[ i ]);
		}

		m_nVertexCount = pMesh->GetVertexCount();
		m_nIndexCount = pMesh->GetIndexCount();

		return true;
	}

	if ( pCommand->GetShaderState() != m_pUsingShaderState )
		return false;

	for ( uint32 nSlotIndex = 0; nSlotIndex < m_nVertexSlotCount; nSlotIndex++ )
	{
		m_nVertexSizes[ nSlotIndex ] += pMesh->GetVertexSize( nSlotIndex );
	}

	const CShaderVariableTable* pTableUsing = nullptr;
	const CShaderVariableTable* pTableOther = nullptr;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		pTableUsing = m_pUsingShaderVarTables[ i ];
		pTableOther = &(pCommand->GetShaderVariableTables()[ i ]);
		if ( pTableUsing == nullptr && pTableOther == nullptr )
			continue;
		if ( pTableUsing != nullptr && pTableOther != nullptr && pTableUsing->Compare( pTableOther ) == false )
			return false;
	}

	m_CommandList_Render.push_back( pCommand );
	m_nVertexCount += pMesh->GetVertexCount();

	m_nIndexCount += pMesh->GetIndexCount();
	m_nIndexSizeTotal += pTriangles->indexSizeTotal;
	m_nTriangleCount += pTriangles->nTrianglesCount;

	return true;

}
