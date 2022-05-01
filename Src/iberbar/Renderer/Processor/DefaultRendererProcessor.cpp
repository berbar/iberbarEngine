
#include <iberbar/Renderer/Processor/DefaultRendererProcessor.h>
#include <iberbar/Renderer/TrianglesCommand.h>
#include <iberbar/Renderer/GroupCommand.h>
#include <iberbar/Renderer/MeshDrawCommand.h>
#include <iberbar/Renderer/Mesh.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/RHI/CommandContext.h>
#include <iberbar/RHI/Buffer.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/VertexDeclaration.h>
#include <iberbar/RHI/ShaderReflection.h>
#include <iberbar/RHI/Device.h>
#include <iberbar/Utility/String.h>



namespace iberbar
{
	namespace Renderer
	{

		class CDefaultRendererProcessor::_State
		{
		public:
			_State();
			~_State();

		public:
			//void SetVertexDeclaration( RHI::IVertexDeclaration* pVertexDeclaration );
			void Initial();
			bool AddCommand( CMeshDrawCommand* pCommand );
			void FlushRenderList();
			void SetShaderBindings();
			FORCEINLINE const std::vector<CMeshDrawCommand*>& GetRenderList() { return m_CommandList_Render; }
			FORCEINLINE uint32 GetVertexCount() const { return m_nVertexCount; }
			FORCEINLINE uint32 GetIndexCount() const { return m_nIndexCount; }
			FORCEINLINE RHI::IShaderState* GetUsingShaderState() const { return m_pUsingShaderState; }
			FORCEINLINE const CShaderVariableTable* GetUsingShaderVarTable( RHI::EShaderType eShaderType ) const { return m_pUsingShaderVarTables[ (int)eShaderType ]; }
			FORCEINLINE const CMaterial* GetMaterial() const { return m_pMaterial; }

		private:
			RHI::ICommandContext* m_pRhiContext;
			std::vector<CMeshDrawCommand*> m_CommandList_Render;
			RHI::IShaderState* m_pUsingShaderState;
			const CShaderVariableTable* m_pUsingShaderVarTables[ (int)RHI::EShaderType::__Count ];
			RHI::IUniformBuffer* m_UniformBuffers[ (int)RHI::EShaderType::__Count ][ 10 ];

			uint32 m_nVertexUsage;
			const CMaterial* m_pMaterial;
			uint32 m_nVertexCount;
			uint32 m_nIndexCount;
		};

		template < typename TRhiBuffer >
		class TRhiBufferLocker
		{
		public:
			TRhiBufferLocker()
				: m_pBuffer( nullptr )
				, m_bLocked( false )
			{
			}
			inline ~TRhiBufferLocker()
			{
				if ( m_bLocked == true )
				{

				}
			}
			inline CResult Lock( TRhiBuffer* pBuffer, uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags )
			{
				m_pBuffer = pBuffer;
				CResult Result = m_pBuffer->Lock( nOffset, nSize, ppOut, nFlags );
				if ( Result.IsOK() == true )
				{
					m_bLocked = true;
				}
			}

		private:
			TRhiBuffer* m_pBuffer;
			bool m_bLocked;
		};
	}
}







iberbar::Renderer::CDefaultRendererProcessor::CDefaultRendererProcessor()
	: m_pState( nullptr )
	, m_pCommandContext( nullptr )
	, m_pIndexBuffer( nullptr )
	, m_VertexBuffers()
{
	memset( m_VertexBuffers, 0, sizeof( m_VertexBuffers ) );
}


iberbar::Renderer::CDefaultRendererProcessor::~CDefaultRendererProcessor()
{
	SAFE_DELETE( m_pState );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
	for ( int i = 0; i < RHI::MaxVertexElementCount; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_VertexBuffers[ i ] );
	}
}


void iberbar::Renderer::CDefaultRendererProcessor::Initial()
{
	RHI::IDevice* pRhiDevice = CRenderer::sGetInstance()->GetRHIDevice();
	m_pCommandContext = pRhiDevice->GetDefaultContext();
	for ( int i = 0; i < RHI::MaxVertexElementCount; i++ )
	{
		pRhiDevice->CreateVertexBuffer( UINT16_MAX * 16, RHI::UBufferUsageFlags::AnyDynamic, &m_VertexBuffers[ i ] );
	}
	pRhiDevice->CreateIndexBuffer( sizeof(uint16), UINT16_MAX * 6 / 4, RHI::UBufferUsageFlags::AnyDynamic, &m_pIndexBuffer );

	m_pState = new CDefaultRendererProcessor::_State();
	m_pState->Initial();
}


void iberbar::Renderer::CDefaultRendererProcessor::VisitQueue( CRenderQueue* pQueue )
{
	pQueue->Sort();
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
	case URenderCommandType::DrawMesh:
	{
		CMeshDrawCommand* pTrianglesCommand = (CMeshDrawCommand*)pCommand;

		uint32 nVertexCountTotal = m_pState->GetVertexCount() + pTrianglesCommand->GetMesh()->GetVertexCount();
		uint32 nIndexCountTotal = m_pState->GetIndexCount() + pTrianglesCommand->GetMesh()->GetIndexCount();
		if ( nVertexCountTotal >= (UINT16_MAX * 16) ||
			nIndexCountTotal >= UINT16_MAX ||
			m_pState->AddCommand( pTrianglesCommand ) == false )
		{
			DrawBatchTriangles();
			m_pState->FlushRenderList();
			m_pState->AddCommand( pTrianglesCommand );
		}
		break;
	}

	case URenderCommandType::Callback:
		Flush();
		((CRenderCallbackCommand*)pCommand)->Execute();
		break;

	case URenderCommandType::Group:
		CRenderer::sGetInstance()->ProcessGroupCommand( (CRenderGroupCommand*)pCommand );
		break;

	default:
		//DefaultVisitCommand( pCommand );
		break;
	}
}


void iberbar::Renderer::CDefaultRendererProcessor::DrawBatchTriangles()
{
	auto& CommandList = m_pState->GetRenderList();
	if ( CommandList.empty() == true )
		return;

	const CMaterial* pMaterial = m_pState->GetMaterial();
	if ( pMaterial == nullptr || pMaterial->GetShaderState() == nullptr )
		return;

	//CTrianglesCommand* pCommand = nullptr;
	CMeshDrawCommand* pCommand = nullptr;
	const CMesh* pMesh = nullptr;
	uint32 nVertexCountInMesh = 0;
	uint32 nIndexCountInMesh = 0;
	const void* pVertexData = nullptr;
	const uint16* pIndexData = nullptr;
	//uint32 nVertexSize = m_pState->GetVertexSize();
	uint32 nVertexCount = m_pState->GetVertexCount();
	//uint32 nVertexSizeTotal = m_pState->GetVertexSizeTotal();
	uint32 nIndexCount = m_pState->GetIndexCount();
	//uint32 nIndexSizeTotal = m_pState->GetIndexSizeTotal();
	//uint32 nTriangleCount = m_pState->GetTriangleCount();

	//uint8* pVerticesTemp[ RHI::MaxVertexElementCount ];
	uint8* pVertexBytesBufferArray[ RHI::MaxVertexElementCount ];
	memset( pVertexBytesBufferArray, 0, sizeof( uint8* ) * RHI::MaxVertexElementCount );
	uint16* pIndicesTemp = nullptr;
	

	bool bLockAll = true;
	uint8 LockFlags[ RHI::MaxVertexElementCount + 1 ];
	memset( LockFlags, 0, sizeof( LockFlags ) );
	CResult cResultLockBuffer;
	const std::vector<uint32> StreamsUsed = pMaterial->GetStreams();
	const uint32* StreamsStride = pMaterial->GetShaderState()->GetVertexDeclaration()->GetStrides();
	for ( uint32 nStreamIndex : StreamsUsed )
	{
		cResultLockBuffer = m_VertexBuffers[ nStreamIndex ]->Lock( 0, nVertexCount * StreamsStride[ nStreamIndex ], (void**)(&(pVertexBytesBufferArray[ nStreamIndex ])) );
		if ( cResultLockBuffer.IsOK() == false )
		{
			bLockAll = false;
			break;
		}
	}
	if ( bLockAll == true )
	{
		cResultLockBuffer = m_pIndexBuffer->Lock( 0, nIndexCount * sizeof( uint16 ), (void**)&pIndicesTemp );
		if ( cResultLockBuffer.IsOK() == false )
		{
			bLockAll = false;
		}
	}

	if ( bLockAll == true )
	{
		uint16 nIndexOffset = 0;
		for ( size_t i = 0, s = CommandList.size(); i < s; i++ )
		{
			pCommand = CommandList[ i ];
			pMesh = pCommand->GetMesh();

			nVertexCountInMesh = pMesh->GetVertexCount();
			nIndexCountInMesh = pMesh->GetIndexCount();
			pIndexData = (const uint16*)pMesh->GetIndexData();

			// 拷贝顶点数据
			for ( uint32 nStreamIndex : StreamsUsed )
			{
				pVertexData = pMesh->GetVertexData( nStreamIndex );
				uint32 nVertexDataSize = nVertexCountInMesh * StreamsStride[ nStreamIndex ];
				if ( pVertexData == nullptr )
				{
					memset( pVertexBytesBufferArray[ nStreamIndex ], 0, nVertexDataSize );
				}
				else
				{
					memcpy_s( pVertexBytesBufferArray[ nStreamIndex ], nVertexDataSize, pVertexData, nVertexDataSize );
				}
				pVertexBytesBufferArray[ nStreamIndex ] += nVertexDataSize;
			}

			// 拷贝索引数据
			// 索引需要偏移
			for ( uint32 n = 0; n < nIndexCountInMesh; n++ )
			{
				pIndicesTemp[ n ] = pIndexData[ n ] + nIndexOffset;
			}
			pIndicesTemp += nIndexCountInMesh;
			nIndexOffset += nVertexCountInMesh;
		}
	}

	for ( uint32 nStreamIndex : StreamsUsed )
	{
		cResultLockBuffer = m_VertexBuffers[ nStreamIndex ]->Unlock();
	}
	m_pIndexBuffer->Unlock();

	if ( bLockAll == false )
	{
		return;
	}
	//CResult retLV = m_pVertexBuffer->Lock( 0, nVertexSizeTotal, (void**)&pVerticesTemp );
	//CResult retLI = m_pIndexBuffer->Lock( 0, nIndexSizeTotal, (void**)&pIndicesTemp );
	//if ( retLV.IsOK() && retLI.IsOK() )
	//{
	//	//const CTrianglesCommand::UTriangles* pTriangles = nullptr;
	//	uint16 nIndexOffset = 0;
	//	for ( size_t i = 0, s = CommandList.size(); i < s; i++ )
	//	{
	//		pCommand = CommandList[ i ];
	//		pMesh = pCommand->GetMeshData();

	//		
	//		nVertexCountInMesh = pMesh->GetVertexCount();
	//		nIndexCountInMesh = pMesh->GetIndexCount();
	//		pIndexData = (const uint16*)pMesh->GetIndexData();

	//		for ( uint32 nSlotTemp = 0, nSlotCount = pMesh->GetVertexSlotCount(); nSlotTemp < nSlotCount; nSlotTemp ++ )
	//		{
	//			pVertexData = pMesh->GetVertexData( nSlotTemp );

	//			memcpy_s( pVerticesTemp, nVertexSizeTotal, pVertexData, pTriangles->vertexSizeTotal );
	//			pVerticesTemp += pTriangles->vertexSizeTotal;
	//		}

	//		memcpy_s( pVerticesTemp, nVertexSizeTotal, pTriangles->vertices, pTriangles->vertexSizeTotal );
	//		pVerticesTemp += pTriangles->vertexSizeTotal;

	//		// 索引需要偏移
	//		for ( uint32 n = 0; n < nIndexCountInMesh; n++ )
	//		{
	//			pIndicesTemp[ n ] = pIndexData[ n ] + nIndexOffset;
	//		}
	//		pIndicesTemp += nIndexCountInMesh;
	//		nIndexOffset += nVertexCountInMesh;
	//	}
	//}
	//m_pVertexBuffer->Unlock();
	//m_pIndexBuffer->Unlock();

	m_pCommandContext->SetShaderState( m_pState->GetUsingShaderState() );
	m_pState->SetShaderBindings();
	
	for ( uint32 nStreamIndex : StreamsUsed )
	{
		m_pCommandContext->SetVertexBuffer( nStreamIndex, m_VertexBuffers[ nStreamIndex ], 0 );
	}
	m_pCommandContext->SetIndexBuffer( m_pIndexBuffer, 0 );
	m_pCommandContext->SetPrimitiveTopology( RHI::UPrimitiveType::Triangle );
	m_pCommandContext->DrawIndexed( 0, nIndexCount, 0 );
	//m_pCommandContext->Draw
	//m_pCommandContext->DrawPrimitive( 0, 1 );
}


void iberbar::Renderer::CDefaultRendererProcessor::DrawOneTriangles( CTrianglesCommand* pCommand )
{
	//RHI::IShaderState* pShaderState = pCommand->GetShaderState();
	//const CShaderVariableTable* pShaderVarTable_Vertex = &pCommand->GetShaderVariableTables()[ (int)RHI::EShaderType::VertexShader ];
	//const CShaderVariableTable* pShaderVarTable_Pixel = &pCommand->GetShaderVariableTables()[ (int)RHI::EShaderType::PixelShader ];
	//if ( pShaderState == nullptr ||
	//	pShaderVarTable_Vertex == nullptr ||
	//	pShaderVarTable_Pixel == nullptr )
	//	return;

	//auto pTriangles = pCommand->GetTriangles();

	//CResult ret;
	//void* pDataTemp = nullptr;
	//uint32 nDataSize = 0;

	//nDataSize = pTriangles->vertexCount * pTriangles->vertexSize;
	//ret = m_pVertexBuffer->Lock( 0, nDataSize, &pDataTemp );
	//if ( ret.IsOK() == false )
	//	return;
	//memcpy_s( pDataTemp, nDataSize, pTriangles->vertices, nDataSize );
	//m_pVertexBuffer->Unlock();

	//nDataSize = pTriangles->indexCount * 2;
	//ret = m_pIndexBuffer->Lock( 0, nDataSize, &pDataTemp );
	//if ( ret.IsOK() == false )
	//	return;
	//memcpy_s( pDataTemp, nDataSize, pTriangles->indices, nDataSize );
	//m_pIndexBuffer->Unlock();

	//SetShaderBindings();
	//m_pCommandContext->SetVertexBuffer( 0, m_pVertexBuffer, 0 );
	//m_pCommandContext->SetIndexBuffer( m_pIndexBuffer, 0 );
	//m_pCommandContext->SetShaderState( pShaderState );
	//m_pCommandContext->SetPrimitiveTopology( RHI::UPrimitiveType::Triangle );
	//m_pCommandContext->DrawIndexed( 0, 0, pTriangles->nTrianglesCount );
}


void iberbar::Renderer::CDefaultRendererProcessor::Flush()
{
	DrawBatchTriangles();
	m_pState->FlushRenderList();
}


//void iberbar::Renderer::CDefaultRendererProcessor::ProcessGroupCommand( CRenderGroupCommand* pCommand )
//{
//	Flush();
//
//	int nQueueId = pCommand->GetQueueId();
//
//	VisitQueue( m_RenderQueue[ nQueueId ] );
//}













iberbar::Renderer::CDefaultRendererProcessor::_State::_State()
	: m_CommandList_Render()
	, m_pUsingShaderState( nullptr )
	, m_pUsingShaderVarTables()
	, m_nVertexCount( 0 )
	, m_nIndexCount( 0 )
{
	memset( m_pUsingShaderVarTables, 0, sizeof( m_pUsingShaderVarTables ) );
	memset( m_UniformBuffers, 0, sizeof( m_UniformBuffers ) );
}


iberbar::Renderer::CDefaultRendererProcessor::_State::~_State()
{
	for ( int i = 0; i < (int)RHI::EShaderType::__Count; i++ )
	{
		for ( int j = 0; j < 10; j++ )
		{
			UNKNOWN_SAFE_RELEASE_NULL( m_UniformBuffers[ i ][ j ] );
		}
	}
}


void iberbar::Renderer::CDefaultRendererProcessor::_State::Initial()
{
	RHI::IDevice* pRhiDevice = CRenderer::sGetInstance()->GetRHIDevice();
	m_pRhiContext = pRhiDevice->GetDefaultContext();
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		for ( int j = 0; j < 10; j++ )
		{
			pRhiDevice->CreateUniformBuffer( &m_UniformBuffers[ i ][ j ], 4096 );
			m_UniformBuffers[ i ][ j ]->SetName( StdFormat( "Renderer::CDefaultRendererProcessor::UniformBuffer_%d_%d", i, j ) );
		}
	}
}


bool iberbar::Renderer::CDefaultRendererProcessor::_State::AddCommand( CMeshDrawCommand* pCommand )
{
	if ( pCommand->GetMaterial() == nullptr || pCommand->GetMaterial()->GetShaderState() == nullptr )
		return true;

	const CMesh* pMesh = pCommand->GetMesh();

	if ( m_CommandList_Render.empty() == true )
	{
		m_pUsingShaderState = pCommand->GetMaterial()->GetShaderState();

		CShaderVariableTable *const *const pTables = pCommand->GetMaterial()->GetShaderVariableTables();
		for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
		{
			//if ( pTables[ i ] == nullptr )
			//	continue;
			m_pUsingShaderVarTables[ i ] = (pTables[ i ]);
		}

		m_CommandList_Render.push_back( pCommand );
		m_nVertexCount = pMesh->GetVertexCount();
		m_nIndexCount = pMesh->GetIndexCount();
		m_pMaterial = pCommand->GetMaterial();

		return true;
	}

	if ( pCommand->GetMaterial()->GetShaderState() != m_pUsingShaderState )
		return false;

	const CShaderVariableTable* pTableUsing = nullptr;
	const CShaderVariableTable* pTableOther = nullptr;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		pTableUsing = m_pUsingShaderVarTables[ i ];
		pTableOther = (pCommand->GetMaterial()->GetShaderVariableTables()[ i ]);
		if ( pTableUsing == nullptr && pTableOther == nullptr )
			continue;
		if ( pTableUsing != nullptr && pTableOther != nullptr && pTableUsing->Compare( pTableOther ) == false )
			return false;
	}

	m_CommandList_Render.push_back( pCommand );
	m_nVertexCount += pMesh->GetVertexCount();
	m_nIndexCount += pMesh->GetIndexCount();
	m_pMaterial = pCommand->GetMaterial();

	return true;

}


void iberbar::Renderer::CDefaultRendererProcessor::_State::FlushRenderList()
{
	m_pUsingShaderState = nullptr;
	memset( m_pUsingShaderVarTables, 0, sizeof( m_pUsingShaderVarTables ) );
	m_CommandList_Render.clear();
	m_nVertexCount = 0;
	m_nIndexCount = 0;
}


void iberbar::Renderer::CDefaultRendererProcessor::_State::SetShaderBindings()
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
		pShaderVariableTable = m_pUsingShaderVarTables[ (int)nShaderType ];
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
			RHI::IUniformBuffer** UniformBuffers = m_UniformBuffers[ i ];
			for ( uint32 nBufferIndex = 0; nBufferIndex < nBufferCount; nBufferIndex++ )
			{
				pShaderReflectionBuffer = pShaderReflection->GetBufferByIndex( nBufferIndex );
				// 检查用户定义的BindPoint
				if ( pShaderReflectionBuffer->GetBindPoint() > g_nRhiUniformBindPoint_UserMax )
				{
					continue;
				}

				pUniformBuffer = UniformBuffers[ nBufferIndex ];
				pUniformBuffer->UpdateContents( pVariablesMemory + pShaderReflectionBuffer->GetOffset(), pShaderReflectionBuffer->GetSize() );
				m_pRhiContext->SetUniformBuffer( nShaderType, pShaderReflectionBuffer->GetBindPoint(), pUniformBuffer );
			}
		}

		const std::vector<RHI::ITexture*>& Textures = pShaderVariableTable->GetTextures();
		nTextureCount = Textures.size();
		for ( uint32 nTextureIndex = 0; nTextureIndex < nTextureCount; nTextureIndex++ )
		{
			m_pRhiContext->SetTexture( nShaderType, nTextureIndex, Textures[ nTextureIndex ] );
		}

		const std::vector<RHI::ISamplerState*>& SamplerStates = pShaderVariableTable->GetSamplerStates();
		nSamplerStateCount = SamplerStates.size();
		for ( uint32 nSamplerIndex = 0; nSamplerIndex < nSamplerStateCount; nSamplerIndex++ )
		{
			m_pRhiContext->SetSamplerState( nShaderType, nSamplerIndex, SamplerStates[ nSamplerIndex ] );
		}
	}

}

