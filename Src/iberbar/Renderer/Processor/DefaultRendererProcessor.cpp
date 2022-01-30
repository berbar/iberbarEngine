
#include <iberbar/Renderer/Processor/DefaultRendererProcessor.h>
#include <iberbar/Renderer/TrianglesCommand.h>
#include <iberbar/Renderer/GroupCommand.h>
#include <iberbar/Renderer/MeshDrawCommand.h>
#include <iberbar/Renderer/Mesh.h>
#include <iberbar/RHI/CommandContext.h>
#include <iberbar/RHI/Buffer.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/VertexDeclaration.h>




namespace iberbar
{
	namespace Renderer
	{
		class CRenderer2dState
		{
		public:
			CRenderer2dState();
			~CRenderer2dState();

		public:
			bool AddCommand( CMeshDrawCommand* pCommand );
			void FlushRenderList();
			void SetShaderBindings();
			FORCEINLINE const std::vector<CMeshDrawCommand*>& GetRenderList() { return m_CommandList_Render; }
			FORCEINLINE uint32 GetVertexSize() const { return m_nVertexSize; }
			FORCEINLINE uint32 GetVertexCount() const { return m_nVertexCount; }
			FORCEINLINE uint32 GetVertexSizeTotal() const { return m_nVertexSizeTotal; }
			FORCEINLINE uint32 GetIndexCount() const { return m_nIndexCount; }
			FORCEINLINE uint32 GetIndexSizeTotal() const { return m_nIndexSizeTotal; }
			FORCEINLINE uint32 GetTriangleCount() const { return m_nTriangleCount; }
			FORCEINLINE RHI::IShaderState* GetUsingShaderState() const { return m_pUsingShaderState; }
			FORCEINLINE const CShaderVariableTable* GetUsingShaderVarTable( RHI::EShaderType eShaderType ) const { return m_pUsingShaderVarTables[ (int)eShaderType ]; }

		private:
			RHI::ICommandContext* m_pRhiContext;
			std::vector<CMeshDrawCommand*> m_CommandList_Render;
			RHI::IShaderState* m_pUsingShaderState;
			const CShaderVariableTable* m_pUsingShaderVarTables[ (int)RHI::EShaderType::__Count ];
			uint32 m_nVertexSlotCount;
			uint32 m_nVertexSize;
			uint32 m_nVertexCount;
			uint32 m_nVertexSizes[ RHI::MaxVertexElementCount ];
			//uint32 m_nVertexSizeTotal;
			uint32 m_nIndexCount;
			uint32 m_nIndexSizeTotal;
			uint32 m_nTriangleCount;
		};
	}
}


void iberbar::Renderer::CDefaultRendererProcessor::VisitQueue( CRenderQueue* pQueue )
{
	VisitCommandList( pQueue->GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Negative ) );
	VisitCommandList( pQueue->GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Zero ) );
	VisitCommandList( pQueue->GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Positive ) );
}


void iberbar::Renderer::CDefaultRendererProcessor::VisitCommandList( const URenderCommandList& commandList )
{
	if ( commandList.empty() == true )
		return;

	size_t nLen = commandList.size();
	for ( size_t i = 0; i < nLen; i++ )
	{
		VisitCommand( commandList[ i ] );
	}
	Flush();
}


void iberbar::Renderer::CDefaultRendererProcessor::VisitCommand( CRenderCommand* pCommand )
{
	URenderCommandType nCommandType = pCommand->GetCommandType();
	switch ( nCommandType )
	{
	case URenderCommandType::Triangles:
	{
		CTrianglesCommand* pTrianglesCommand = (CTrianglesCommand*)pCommand;

		uint32 nVertexSizeTotal_New = m_pState->GetVertexSizeTotal() + pTrianglesCommand->GetTriangles()->vertexSizeTotal;
		uint32 nIndexSizeTotal_New = m_pState->GetIndexSizeTotal() + pTrianglesCommand->GetTriangles()->indexSizeTotal;
		if ( nVertexSizeTotal_New >= m_pVertexBuffer->GetInSize() ||
			nIndexSizeTotal_New >= m_pIndexBuffer->GetInSize() ||
			m_pState->AddCommand( pTrianglesCommand ) == false )
		{
			DrawBatchTriangles();
			m_pState->AddCommand( pTrianglesCommand );
		}
		break;
	}

	default:
		DefaultVisitCommand( pCommand );
		break;
	}
}


void iberbar::Renderer::CDefaultRendererProcessor::DrawBatchTriangles()
{
	auto& CommandList = m_pState->GetRenderList();
	if ( CommandList.empty() == true )
		return;

	//CTrianglesCommand* pCommand = nullptr;
	CMeshDrawCommand* pCommand = nullptr;
	const IMesh* pMesh = nullptr;
	uint32 nVertexCountInMesh = 0;
	uint32 nIndexCountInMesh = 0;
	const void* pVertexData = nullptr;
	const uint16* pIndexData = nullptr;
	//uint32 nVertexSize = m_pState->GetVertexSize();
	//uint32 nVertexCount = m_pState->GetVertexCount();
	//uint32 nVertexSizeTotal = m_pState->GetVertexSizeTotal();
	//uint32 nIndexCount = m_pState->GetIndexCount();
	//uint32 nIndexSizeTotal = m_pState->GetIndexSizeTotal();
	//uint32 nTriangleCount = m_pState->GetTriangleCount();

	uint8* pVerticesTemp = nullptr;
	uint16* pIndicesTemp = nullptr;

	CResult retLV = m_pVertexBuffer->Lock( 0, nVertexSizeTotal, (void**)&pVerticesTemp );
	CResult retLI = m_pIndexBuffer->Lock( 0, nIndexSizeTotal, (void**)&pIndicesTemp );
	if ( retLV.IsOK() && retLI.IsOK() )
	{
		//const CTrianglesCommand::UTriangles* pTriangles = nullptr;
		uint16 nIndexOffset = 0;
		for ( size_t i = 0, s = CommandList.size(); i < s; i++ )
		{
			pCommand = CommandList[ i ];
			pMesh = pCommand->GetMeshData();

			
			nVertexCountInMesh = pMesh->GetVertexCount();
			nIndexCountInMesh = pMesh->GetIndexCount();
			pIndexData = (const uint16*)pMesh->GetIndexData();

			for ( uint32 nSlotTemp = 0, nSlotCount = pMesh->GetVertexSlotCount(); nSlotTemp < nSlotCount; nSlotTemp ++ )
			{
				pVertexData = pMesh->GetVertexData( nSlotTemp );

				memcpy_s( pVerticesTemp, nVertexSizeTotal, pVertexData, pTriangles->vertexSizeTotal );
				pVerticesTemp += pTriangles->vertexSizeTotal;
			}

			memcpy_s( pVerticesTemp, nVertexSizeTotal, pTriangles->vertices, pTriangles->vertexSizeTotal );
			pVerticesTemp += pTriangles->vertexSizeTotal;

			// Ë÷ÒýÐèÒªÆ«ÒÆ
			for ( uint32 n = 0; n < nIndexCountInMesh; n++ )
			{
				pIndicesTemp[ n ] = pIndexData[ n ] + nIndexOffset;
			}
			pIndicesTemp += nIndexCountInMesh;
			nIndexOffset += nVertexCountInMesh;
		}
	}
	m_pVertexBuffer->Unlock();
	m_pIndexBuffer->Unlock();

	SetShaderBindings();
	m_pCommandContext->SetVertexBuffer( 0, m_pVertexBuffer, 0 );
	m_pCommandContext->SetIndexBuffer( m_pIndexBuffer, 0 );
	m_pCommandContext->SetShaderState( m_pState->GetUsingShaderState() );
	m_pCommandContext->SetPrimitiveTopology( RHI::UPrimitiveType::Triangle );
	m_pCommandContext->DrawIndexed( 0, nIndexCount, 0 );

	m_pState->FlushRenderList();
}


void iberbar::Renderer::CDefaultRendererProcessor::DrawOneTriangles( CTrianglesCommand* pCommand )
{
	RHI::IShaderState* pShaderState = pCommand->GetShaderState();
	const CShaderVariableTable* pShaderVarTable_Vertex = &pCommand->GetShaderVariableTables()[ (int)RHI::EShaderType::VertexShader ];
	const CShaderVariableTable* pShaderVarTable_Pixel = &pCommand->GetShaderVariableTables()[ (int)RHI::EShaderType::PixelShader ];
	if ( pShaderState == nullptr ||
		pShaderVarTable_Vertex == nullptr ||
		pShaderVarTable_Pixel == nullptr )
		return;

	auto pTriangles = pCommand->GetTriangles();

	CResult ret;
	void* pDataTemp = nullptr;
	uint32 nDataSize = 0;

	nDataSize = pTriangles->vertexCount * pTriangles->vertexSize;
	ret = m_pVertexBuffer->Lock( 0, nDataSize, &pDataTemp );
	if ( ret.IsOK() == false )
		return;
	memcpy_s( pDataTemp, nDataSize, pTriangles->vertices, nDataSize );
	m_pVertexBuffer->Unlock();

	nDataSize = pTriangles->indexCount * 2;
	ret = m_pIndexBuffer->Lock( 0, nDataSize, &pDataTemp );
	if ( ret.IsOK() == false )
		return;
	memcpy_s( pDataTemp, nDataSize, pTriangles->indices, nDataSize );
	m_pIndexBuffer->Unlock();

	SetShaderBindings();
	m_pCommandContext->SetVertexBuffer( 0, m_pVertexBuffer, 0 );
	m_pCommandContext->SetIndexBuffer( m_pIndexBuffer, 0 );
	m_pCommandContext->SetShaderState( pShaderState );
	m_pCommandContext->SetPrimitiveTopology( RHI::UPrimitiveType::Triangle );
	m_pCommandContext->DrawIndexedPrimitive( 0, 0, pTriangles->nTrianglesCount );
}


void iberbar::Renderer::CDefaultRendererProcessor::Flush()
{
	DrawBatchTriangles();
}


//void iberbar::Renderer::CDefaultRendererProcessor::ProcessGroupCommand( CRenderGroupCommand* pCommand )
//{
//	Flush();
//
//	int nQueueId = pCommand->GetQueueId();
//
//	VisitQueue( m_RenderQueue[ nQueueId ] );
//}













iberbar::Renderer::CRenderer2dState::CRenderer2dState()
	: m_CommandList_Render()
	, m_pUsingShaderState( nullptr )
	, m_pUsingShaderVarTables()
	, m_nVertexSize( 0 )
	, m_nVertexCount( 0 )
	, m_nVertexSizeTotal( 0 )
	, m_nIndexCount( 0 )
	, m_nIndexSizeTotal( 0 )
	, m_nTriangleCount( 0 )
{
	memset( m_pUsingShaderVarTables, 0, sizeof( m_pUsingShaderVarTables ) );
}


iberbar::Renderer::CRenderer2dState::~CRenderer2dState()
{
}


bool iberbar::Renderer::CRenderer2dState::AddCommand( CMeshDrawCommand* pCommand )
{
	if ( pCommand->GetShaderState() == nullptr )
		return true;

	const IMesh* pMesh = pCommand->GetMeshData();

	if ( m_CommandList_Render.empty() == true )
	{
		m_CommandList_Render.push_back( pCommand );
		m_pUsingShaderState = pCommand->GetShaderState();
		m_nVertexSlotCount = m_pUsingShaderState->GetVertexDeclaration()->GetSlotCount();

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


void iberbar::Renderer::CRenderer2dState::FlushRenderList()
{
	m_pUsingShaderState = nullptr;
	memset( m_pUsingShaderVarTables, 0, sizeof( m_pUsingShaderVarTables ) );
	m_CommandList_Render.clear();
	m_nVertexSize = 0;
	m_nVertexCount = 0;
	m_nVertexSizeTotal = 0;
	m_nIndexCount = 0;
	m_nIndexSizeTotal = 0;
	m_nTriangleCount = 0;
}


void iberbar::Renderer::CRenderer2d::SetShaderBindings()
{
	const CShaderVariableTable* pShaderVariableTable;
	//RHI::IShader* pShader;
	const RHI::IShaderReflection* pShaderReflection;
	const RHI::IShaderReflectionBuffer* pShaderReflectionBuffer;
	RHI::IUniformBuffer* pUniformBuffer;
	uint32 nTextureCount;
	uint32 nSamplerStateCount;
	uint32 nBufferCount;
	const uint8* pVariablesMemory = nullptr;
	RHI::EShaderType nShaderType;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		nShaderType = (RHI::EShaderType)i;
		pShaderVariableTable = m_pState->GetUsingShaderVarTable( nShaderType );
		if ( pShaderVariableTable == nullptr )
		{
			continue;
		}

		pShaderReflection = pShaderVariableTable->GetShaderReflection();
		assert( pShaderReflection != nullptr );

		pVariablesMemory = pShaderVariableTable->GetMemory();
		nBufferCount = pShaderReflection->GetBufferCount();
		if ( pVariablesMemory != nullptr && nBufferCount > 0 )
		{
			auto UniformBuffers = m_pState->GetUsingShaderState()->GetUniformBuffers( nShaderType );
			for ( uint32 nBufferIndex = 0; nBufferIndex < nBufferCount; nBufferIndex++ )
			{
				pShaderReflectionBuffer = pShaderReflection->GetBufferByIndex( nBufferIndex );
				pUniformBuffer = UniformBuffers[ nBufferIndex ];
				if ( pShaderReflectionBuffer == nullptr || pUniformBuffer == nullptr )
				{
					m_pCommandContext->SetUniformBuffer( nShaderType, nBufferIndex, nullptr );
					continue;
				}

				pUniformBuffer->UpdateContents( pVariablesMemory + pShaderReflectionBuffer->GetOffset(), pShaderReflectionBuffer->GetSize() );
				m_pCommandContext->SetUniformBuffer( nShaderType, nBufferIndex, pUniformBuffer );
			}
		}

		const std::vector<RHI::ITexture*>& Textures = pShaderVariableTable->GetTextures();
		nTextureCount = Textures.size();
		for ( uint32 nTextureIndex = 0; nTextureIndex < nTextureCount; nTextureIndex++ )
		{
			m_pCommandContext->SetTexture( nShaderType, nTextureIndex, Textures[ nTextureIndex ] );
		}

		const std::vector<RHI::ISamplerState*>& SamplerStates = pShaderVariableTable->GetSamplerStates();
		nSamplerStateCount = SamplerStates.size();
		for ( uint32 nSamplerIndex = 0; nSamplerIndex < nSamplerStateCount; nSamplerIndex++ )
		{
			m_pCommandContext->SetSamplerState( nShaderType, nSamplerIndex, SamplerStates[ nSamplerIndex ] );
		}
	}

}

