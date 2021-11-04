

#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/RenderCommand.h>
#include <iberbar/Renderer/TrianglesCommand.h>
#include <iberbar/Renderer/CallbackCommand.h>
#include <iberbar/Renderer/GroupCommand.h>
#include <iberbar/Renderer/ShaderVariables.h>
#include <iberbar/Renderer/Vertex.h>
#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/CommandContext.h>
#include <iberbar/RHI/Buffer.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/ShaderReflection.h>
#include <iterator>






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
			bool AddCommand( CTrianglesCommand* pCommand );
			void FlushRenderList();
			void SetShaderBindings();
			FORCEINLINE const std::vector<CTrianglesCommand*>& GetRenderList() { return m_CommandList_Render; }
			FORCEINLINE uint32 GetVertexSize() const { return m_nVertexSize; }
			FORCEINLINE uint32 GetVertexCount() const { return m_nVertexCount; }
			FORCEINLINE uint32 GetVertexSizeTotal() const { return m_nVertexSizeTotal; }
			FORCEINLINE uint32 GetIndexCount() const { return m_nIndexCount; }
			FORCEINLINE uint32 GetIndexSizeTotal() const { return m_nIndexSizeTotal; }
			FORCEINLINE uint32 GetTriangleCount() const { return m_nTriangleCount; }
			FORCEINLINE RHI::IShaderState* GetUsingShaderState() const { return m_pUsingShaderState; }
			FORCEINLINE CShaderVariableTable* GetUsingShaderVarTable( RHI::EShaderType eShaderType ) const { return m_pUsingShaderVarTables[ (int)eShaderType ]; }

		private:
			RHI::ICommandContext* m_pRhiContext;
			std::vector<CTrianglesCommand*> m_CommandList_Render;
			RHI::IShaderState* m_pUsingShaderState;
			CShaderVariableTable* m_pUsingShaderVarTables[ (int)RHI::EShaderType::__Count ];
			uint32 m_nVertexSize;
			uint32 m_nVertexCount;
			uint32 m_nVertexSizeTotal;
			uint32 m_nIndexCount;
			uint32 m_nIndexSizeTotal;
			uint32 m_nTriangleCount;
		};
	}
}





static bool CompareRenderCommand( iberbar::Renderer::CRenderCommand* a, iberbar::Renderer::CRenderCommand* b )
{
	return a->GetZOrder() < b->GetZOrder();
}

void iberbar::Renderer::CRenderQueue::PushBack( CRenderCommand* pCommand )
{
	int nZIndex = pCommand->GetZOrder();
	if ( nZIndex < 0 )
	{
		m_CommandLists[ (int)UQueueGroup::Zindex_Negative ].push_back( pCommand );
	}
	else if ( nZIndex > 0 )
	{
		m_CommandLists[ (int)UQueueGroup::Zindex_Positive ].push_back( pCommand );
	}
	else
	{
		m_CommandLists[ (int)UQueueGroup::Zindex_Zero ].push_back( pCommand );
	}	
}

void iberbar::Renderer::CRenderQueue::Sort()
{
	std::stable_sort(
		std::begin( m_CommandLists[ (int)UQueueGroup::Zindex_Negative ] ),
		std::end( m_CommandLists[ (int)UQueueGroup::Zindex_Negative ] ),
		CompareRenderCommand );
	std::stable_sort(
		std::begin( m_CommandLists[ (int)UQueueGroup::Zindex_Positive ] ),
		std::end( m_CommandLists[ (int)UQueueGroup::Zindex_Positive ] ),
		CompareRenderCommand );
}

void iberbar::Renderer::CRenderQueue::Clear()
{
	for ( int i = 0; i < (int)UQueueGroup::Count; ++i )
	{
		m_CommandLists[ i ].clear();
	}
}





iberbar::Renderer::CRenderer2d::CRenderer2d()
	: m_RenderQueue()
	, m_CommandGroupStack()
	, m_bIsRendering( false )
	, m_pDevice( nullptr )
	//, m_pCommandContext( nullptr )
	, m_pVertexBuffer( nullptr )
	, m_pIndexBuffer( nullptr )
	, m_pState( new CRenderer2dState() )
	, m_pRenderGroupCommandManager( new CRenderGroupCommandManager( this ) )
{
	m_RenderQueue.push_back( CRenderQueue() );
	m_CommandGroupStack.push( 0 );
}

iberbar::Renderer::CRenderer2d::~CRenderer2d()
{
	m_RenderQueue.clear();
	SAFE_DELETE( m_pRenderGroupCommandManager );
	SAFE_DELETE( m_pState );
	//UNKNOWN_SAFE_RELEASE_NULL( m_pCommandContext );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}

void iberbar::Renderer::CRenderer2d::Init( RHI::IDevice* pDevice )
{
	assert( pDevice );

	//m_pDevice = pDevice;
	//m_pDevice->AddRef();
	//m_pDevice->CreateCommandContext( &m_pCommandContext );
	m_pCommandContext = m_pDevice->GetDefaultContext();
	m_pDevice->CreateVertexBuffer( UINT16_MAX * sizeof( UVertex_V3F_C4B_T2F ), RHI::UBufferUsageFlags::AnyDynamic, &m_pVertexBuffer );
	m_pDevice->CreateIndexBuffer( 0, UINT16_MAX * 6 / 4, RHI::UBufferUsageFlags::AnyDynamic, &m_pIndexBuffer );
}


void iberbar::Renderer::CRenderer2d::AddCommand( CRenderCommand* pCommand )
{
	int nQueueId = m_CommandGroupStack.top();
	AddCommand( pCommand, nQueueId );
}


void iberbar::Renderer::CRenderer2d::AddCommand( CRenderCommand* pCommand, int nQueueId )
{
	assert( pCommand );
	assert( nQueueId >= 0 );
	assert( m_bIsRendering == false );

	m_RenderQueue[ nQueueId ].PushBack( pCommand );
}

void iberbar::Renderer::CRenderer2d::CleanupCommands()
{

}


int iberbar::Renderer::CRenderer2d::CreateRenderQueue()
{
	CRenderQueue queue;
	m_RenderQueue.push_back( queue );
	return (int)m_RenderQueue.size() - 1;
}


void iberbar::Renderer::CRenderer2d::PushRenderQueue( int nQueueId )
{
	assert( m_bIsRendering == false );
	m_CommandGroupStack.push( nQueueId );
}


void iberbar::Renderer::CRenderer2d::PopRenderQueue()
{
	assert( m_bIsRendering == false );
	m_CommandGroupStack.pop();
}


void iberbar::Renderer::CRenderer2d::Clear()
{
	size_t n = m_RenderQueue.size();
	for ( size_t i = 0; i < n; i++ )
	{
		m_RenderQueue[ i ].Clear();
	}
}

void iberbar::Renderer::CRenderer2d::Render()
{
	m_bIsRendering = true;

	for ( int i = 0; i < m_RenderQueue.size(); i++ )
	{
		m_RenderQueue[ i ].Sort();
	}

	m_pCommandContext->SetVertexBuffer( 0, m_pVertexBuffer, 0 );
	m_pCommandContext->SetIndexBuffer( m_pIndexBuffer, 0 );

	VisitQueue( m_RenderQueue[ 0 ] );

	Clear();

	m_bIsRendering = false;
}


void iberbar::Renderer::CRenderer2d::OnRhiLost()
{
	//m_pCommandContext->SetVertexBuffer( nullptr );
	//m_pCommandContext->SetIndexBuffer( nullptr );
	//m_pCommandContext->SetShaderState( nullptr );
	//for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i ++ )
	//	m_pCommandContext->SetShaderVariableTable( (RHI::EShaderType)i, nullptr );

	if ( m_pVertexBuffer )
	{
		m_pVertexBuffer->OnLost();
	}

	if ( m_pIndexBuffer )
	{
		m_pIndexBuffer->OnLost();
	}
}


iberbar::CResult iberbar::Renderer::CRenderer2d::OnRhiReset()
{
	CResult Ret;

	if ( m_pVertexBuffer )
	{
		Ret = m_pVertexBuffer->OnReset();
		if ( Ret.IsOK() == false )
			return Ret;
	}

	if ( m_pIndexBuffer )
	{
		Ret = m_pIndexBuffer->OnReset();
		if ( Ret.IsOK() == false )
			return Ret;
	}

	return Ret;
}


void iberbar::Renderer::CRenderer2d::VisitQueue( CRenderQueue& queue )
{
	VisitCommandList( queue.GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Negative ) );
	VisitCommandList( queue.GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Zero ) );
	VisitCommandList( queue.GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Positive ) );
}

void iberbar::Renderer::CRenderer2d::VisitCommandList( const URenderCommandList& commandList )
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


void iberbar::Renderer::CRenderer2d::VisitCommand( CRenderCommand* pCommand )
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
		
		case URenderCommandType::Callback:
		{
			Flush();
			((CRenderCallbackCommand*)pCommand)->Execute();
			break;
		}

		case  URenderCommandType::Group:
		{
			ProcessGroupCommand( (CRenderGroupCommand*)pCommand );
			break;
		}

		default:
			break;
	}
}


void iberbar::Renderer::CRenderer2d::DrawBatchTriangles()
{
	auto& CommandList = m_pState->GetRenderList();
	if ( CommandList.empty() == true )
		return;

	CTrianglesCommand* pCommand = nullptr;
	uint32 nVertexSize = m_pState->GetVertexSize();
	uint32 nVertexCount = m_pState->GetVertexCount();
	uint32 nVertexSizeTotal = m_pState->GetVertexSizeTotal();
	uint32 nIndexCount = m_pState->GetIndexCount();
	uint32 nIndexSizeTotal = m_pState->GetIndexSizeTotal();
	uint32 nTriangleCount = m_pState->GetTriangleCount();

	uint8* pVerticesTemp = nullptr;
	uint16* pIndicesTemp = nullptr;

	CResult retLV = m_pVertexBuffer->Lock( 0, nVertexSizeTotal, (void**)&pVerticesTemp );
	CResult retLI = m_pIndexBuffer->Lock( 0, nIndexSizeTotal, (void**)&pIndicesTemp );
	if ( retLV.IsOK() && retLI.IsOK() )
	{
		const CTrianglesCommand::UTriangles* pTriangles = nullptr;
		uint16 nIndexOffset = 0;
		for ( size_t i = 0, s = CommandList.size(); i < s; i++ )
		{
			pCommand = CommandList[ i ];
			pTriangles = pCommand->GetTriangles();

			memcpy_s( pVerticesTemp, nVertexSizeTotal, pTriangles->vertices, pTriangles->vertexSizeTotal );
			pVerticesTemp += pTriangles->vertexSizeTotal;

			// Ë÷ÒýÐèÒªÆ«ÒÆ
			for ( uint32 n = 0, t = pTriangles->indexCount; n < t; n++ )
			{
				pIndicesTemp[ n ] = pTriangles->indices[ n ] + nIndexOffset;
			}
			pIndicesTemp += pTriangles->indexCount;
			nIndexOffset += pTriangles->vertexCount;
		}
	}
	m_pVertexBuffer->Unlock();
	m_pIndexBuffer->Unlock();

	SetShaderBindings();
	m_pCommandContext->SetVertexBuffer( 0, m_pVertexBuffer, 0 );
	m_pCommandContext->SetIndexBuffer( m_pIndexBuffer, 0 );
	m_pCommandContext->SetShaderState( m_pState->GetUsingShaderState() );
	m_pCommandContext->SetPrimitiveTopology( RHI::UPrimitiveType::Triangle );
	m_pCommandContext->DrawIndexedPrimitive( 0, 0, nTriangleCount );

	m_pState->FlushRenderList();
}


void iberbar::Renderer::CRenderer2d::DrawOneTriangles( CTrianglesCommand* pCommand )
{
	RHI::IShaderState* pShaderState = pCommand->GetMaterial()->GetShaderState();
	CShaderVariableTable* pShaderVarTable_Vertex = pCommand->GetMaterial()->GetShaderVariableTable( RHI::EShaderType::VertexShader );
	CShaderVariableTable* pShaderVarTable_Pixel = pCommand->GetMaterial()->GetShaderVariableTable( RHI::EShaderType::PixelShader );
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


void iberbar::Renderer::CRenderer2d::Flush()
{
	DrawBatchTriangles();
}


void iberbar::Renderer::CRenderer2d::ProcessGroupCommand( CRenderGroupCommand* pCommand )
{
	Flush();

	int nQueueId = pCommand->GetQueueId();

	VisitQueue( m_RenderQueue[ nQueueId ] );
}








iberbar::Renderer::CRenderer2dState::CRenderer2dState()
	: m_CommandList_Render()
	, m_pUsingShaderState( nullptr )
	, m_pUsingShaderVarTables()
	, m_nVertexSize(0)
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


bool iberbar::Renderer::CRenderer2dState::AddCommand( CTrianglesCommand* pCommand )
{
	if ( pCommand->GetMaterial() == nullptr || pCommand->GetMaterial()->GetShaderState() )
		return true;

	auto pTriangles = pCommand->GetTriangles();

	if ( m_CommandList_Render.empty() == true )
	{
		m_CommandList_Render.push_back( pCommand );
		m_pUsingShaderState = pCommand->GetMaterial()->GetShaderState();
		for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
		{
			m_pUsingShaderVarTables[ i ] = pCommand->GetMaterial()->GetShaderVariableTable( (RHI::EShaderType)i );
		}

		m_nVertexSize = pTriangles->vertexSize;
		m_nVertexCount = pTriangles->vertexCount;
		m_nVertexSizeTotal = pTriangles->vertexSizeTotal;
		m_nIndexCount = pTriangles->indexCount;
		m_nIndexSizeTotal = pTriangles->indexSizeTotal;
		m_nTriangleCount = pTriangles->nTrianglesCount;

		return true;
	}

	if ( pCommand->GetMaterial()->GetShaderState() != m_pUsingShaderState )
		return false;

	CShaderVariableTable* pTableUsing = nullptr;
	CShaderVariableTable* pTableOther = nullptr;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		pTableUsing = m_pUsingShaderVarTables[ i ];
		pTableOther = pCommand->GetMaterial()->GetShaderVariableTable( (RHI::EShaderType)i );
		if ( pTableUsing == nullptr && pTableOther == nullptr )
			continue;
		if ( pTableUsing != nullptr && pTableOther != nullptr && pTableUsing->Compare( pTableOther ) == false )
			return false;
	}

	m_CommandList_Render.push_back( pCommand );
	m_nVertexCount += pTriangles->vertexCount;
	m_nVertexSizeTotal += pTriangles->vertexSizeTotal;
	m_nIndexCount += pTriangles->indexCount;
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
	CShaderVariableTable* pShaderVariableTable;
	RHI::IShader* pShader;
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

		pShader = m_pState->GetUsingShaderState()->GetShader( nShaderType );
		if ( pShader == nullptr )
		{
			continue;
		}

		pShaderReflection = pShader->GetReflection();
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

		nTextureCount = pShaderReflection->GetTextureCountTotal();
		const std::vector<RHI::ITexture*>& Textures = pShaderVariableTable->GetTextures();
		for ( uint32 nTextureIndex = 0; nTextureIndex < nTextureCount; nTextureIndex++ )
		{
			m_pCommandContext->SetTexture( nShaderType, nTextureIndex, Textures[ nTextureIndex ] );
		}

		nSamplerStateCount = pShaderReflection->GetSamplerStateCountTotal();
		const std::vector<RHI::ISamplerState*>& SamplerStates = pShaderVariableTable->GetSamplerStates();
		for ( uint32 nSamplerIndex = 0; nSamplerIndex < nSamplerStateCount; nSamplerIndex++ )
		{
			m_pCommandContext->SetSamplerState( nShaderType, nSamplerIndex, SamplerStates[ nSamplerIndex ] );
		}
	}

}
