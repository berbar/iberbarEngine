

#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/Processor/DefaultRendererProcessor.h>
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





iberbar::Renderer::CRenderer* iberbar::Renderer::CRenderer::sm_pInstance = nullptr;


iberbar::Renderer::CRenderer::CRenderer()
	: m_RenderQueue()
	, m_CommandGroupStack()
	, m_bIsRendering( false )
	, m_pDevice( nullptr )
	//, m_pCommandContext( nullptr )
	, m_pRenderGroupCommandManager( new CRenderGroupCommandManager() )
{
	m_RenderQueue.push_back( CRenderQueue() );
	m_CommandGroupStack.push( 0 );

	sm_pInstance = this;
}

iberbar::Renderer::CRenderer::~CRenderer()
{
	m_RenderQueue.clear();
	SAFE_DELETE( m_pRenderGroupCommandManager );

	sm_pInstance = nullptr;
}

void iberbar::Renderer::CRenderer::Init( RHI::IDevice* pDevice )
{
	assert( pDevice );

	m_pDevice = pDevice;
	m_pDefaultRendererProcessor = new CDefaultRendererProcessor();
	m_pDefaultRendererProcessor->Initial();

}


void iberbar::Renderer::CRenderer::AddCommand( CRenderCommand* pCommand )
{
	int nQueueId = m_CommandGroupStack.top();
	AddCommand( pCommand, nQueueId );
}


void iberbar::Renderer::CRenderer::AddCommand( CRenderCommand* pCommand, int nQueueId )
{
	assert( pCommand );
	assert( nQueueId >= 0 );
	assert( m_bIsRendering == false );

	m_RenderQueue[ nQueueId ].PushBack( pCommand );
}

void iberbar::Renderer::CRenderer::CleanupCommands()
{

}


int iberbar::Renderer::CRenderer::CreateRenderQueue()
{
	CRenderQueue queue;
	m_RenderQueue.push_back( queue );
	return (int)m_RenderQueue.size() - 1;
}


void iberbar::Renderer::CRenderer::PushRenderQueue( int nQueueId )
{
	assert( m_bIsRendering == false );
	m_CommandGroupStack.push( nQueueId );
}


void iberbar::Renderer::CRenderer::PopRenderQueue()
{
	assert( m_bIsRendering == false );
	m_CommandGroupStack.pop();
}


void iberbar::Renderer::CRenderer::Clear()
{
	size_t n = m_RenderQueue.size();
	for ( size_t i = 0; i < n; i++ )
	{
		m_RenderQueue[ i ].Clear();
	}
}

void iberbar::Renderer::CRenderer::Render()
{
	m_bIsRendering = true;

	//for ( int i = 0; i < m_RenderQueue.size(); i++ )
	//{
	//	m_RenderQueue[ i ].Sort();
	//}

	m_RenderQueue[ 0 ].Sort();
	m_pDefaultRendererProcessor->VisitQueue( &m_RenderQueue[ 0 ] );

	Clear();

	m_bIsRendering = false;
}


void iberbar::Renderer::CRenderer::OnRhiLost()
{
}


iberbar::CResult iberbar::Renderer::CRenderer::OnRhiReset()
{
	CResult Ret;

	return Ret;
}


//void iberbar::Renderer::CRenderer::VisitQueue( CRenderQueue& queue )
//{
//	VisitCommandList( queue.GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Negative ) );
//	VisitCommandList( queue.GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Zero ) );
//	VisitCommandList( queue.GetQueueGroup( CRenderQueue::UQueueGroup::Zindex_Positive ) );
//}
//
//void iberbar::Renderer::CRenderer::VisitCommandList( const URenderCommandList& commandList )
//{
//	if ( commandList.empty() == true )
//		return;
//
//	size_t nLen = commandList.size();
//	for ( size_t i = 0; i < nLen; i++ )
//	{
//		VisitCommand( commandList[ i ] );
//	}
//	Flush();
//}
//
//
//void iberbar::Renderer::CRenderer::VisitCommand( CRenderCommand* pCommand )
//{
//	URenderCommandType nCommandType = pCommand->GetCommandType();
//	switch ( nCommandType )
//	{
//		case URenderCommandType::Triangles:
//		{
//			CTrianglesCommand* pTrianglesCommand = (CTrianglesCommand*)pCommand;
//
//			uint32 nVertexSizeTotal_New = m_pState->GetVertexSizeTotal() + pTrianglesCommand->GetTriangles()->vertexSizeTotal;
//			uint32 nIndexSizeTotal_New = m_pState->GetIndexSizeTotal() + pTrianglesCommand->GetTriangles()->indexSizeTotal;
//			if ( nVertexSizeTotal_New >= m_pVertexBuffer->GetInSize() ||
//				nIndexSizeTotal_New >= m_pIndexBuffer->GetInSize() ||
//				m_pState->AddCommand( pTrianglesCommand ) == false )
//			{
//				DrawBatchTriangles();
//				m_pState->AddCommand( pTrianglesCommand );
//			}
//			break;
//		}
//		
//		case URenderCommandType::Callback:
//		{
//			Flush();
//			((CRenderCallbackCommand*)pCommand)->Execute();
//			break;
//		}
//
//		case  URenderCommandType::Group:
//		{
//			ProcessGroupCommand( (CRenderGroupCommand*)pCommand );
//			break;
//		}
//
//		default:
//			break;
//	}
//}
//
//
//void iberbar::Renderer::CRenderer::DrawBatchTriangles()
//{
//	auto& CommandList = m_pState->GetRenderList();
//	if ( CommandList.empty() == true )
//		return;
//
//	CTrianglesCommand* pCommand = nullptr;
//	uint32 nVertexSize = m_pState->GetVertexSize();
//	uint32 nVertexCount = m_pState->GetVertexCount();
//	uint32 nVertexSizeTotal = m_pState->GetVertexSizeTotal();
//	uint32 nIndexCount = m_pState->GetIndexCount();
//	uint32 nIndexSizeTotal = m_pState->GetIndexSizeTotal();
//	uint32 nTriangleCount = m_pState->GetTriangleCount();
//
//	uint8* pVerticesTemp = nullptr;
//	uint16* pIndicesTemp = nullptr;
//
//	CResult retLV = m_pVertexBuffer->Lock( 0, nVertexSizeTotal, (void**)&pVerticesTemp );
//	CResult retLI = m_pIndexBuffer->Lock( 0, nIndexSizeTotal, (void**)&pIndicesTemp );
//	if ( retLV.IsOK() && retLI.IsOK() )
//	{
//		const CTrianglesCommand::UTriangles* pTriangles = nullptr;
//		uint16 nIndexOffset = 0;
//		for ( size_t i = 0, s = CommandList.size(); i < s; i++ )
//		{
//			pCommand = CommandList[ i ];
//			pTriangles = pCommand->GetTriangles();
//
//			memcpy_s( pVerticesTemp, nVertexSizeTotal, pTriangles->vertices, pTriangles->vertexSizeTotal );
//			pVerticesTemp += pTriangles->vertexSizeTotal;
//
//			// Ë÷ÒýÐèÒªÆ«ÒÆ
//			for ( uint32 n = 0, t = pTriangles->indexCount; n < t; n++ )
//			{
//				pIndicesTemp[ n ] = pTriangles->indices[ n ] + nIndexOffset;
//			}
//			pIndicesTemp += pTriangles->indexCount;
//			nIndexOffset += pTriangles->vertexCount;
//		}
//	}
//	m_pVertexBuffer->Unlock();
//	m_pIndexBuffer->Unlock();
//
//	SetShaderBindings();
//	m_pCommandContext->SetVertexBuffer( 0, m_pVertexBuffer, 0 );
//	m_pCommandContext->SetIndexBuffer( m_pIndexBuffer, 0 );
//	m_pCommandContext->SetShaderState( m_pState->GetUsingShaderState() );
//	m_pCommandContext->SetPrimitiveTopology( RHI::UPrimitiveType::Triangle );
//	m_pCommandContext->DrawIndexedPrimitive( 0, 0, nTriangleCount );
//
//	m_pState->FlushRenderList();
//}
//
//
//void iberbar::Renderer::CRenderer::DrawOneTriangles( CTrianglesCommand* pCommand )
//{
//	RHI::IShaderState* pShaderState = pCommand->GetShaderState();
//	const CShaderVariableTable* pShaderVarTable_Vertex = &pCommand->GetShaderVariableTables()[ (int) RHI::EShaderType::VertexShader ];
//	const CShaderVariableTable* pShaderVarTable_Pixel = &pCommand->GetShaderVariableTables()[ (int) RHI::EShaderType::PixelShader ];
//	if ( pShaderState == nullptr ||
//		pShaderVarTable_Vertex == nullptr ||
//		pShaderVarTable_Pixel == nullptr )
//		return;
//
//	auto pTriangles = pCommand->GetTriangles();
//	
//	CResult ret;
//	void* pDataTemp = nullptr;
//	uint32 nDataSize = 0;
//
//	nDataSize = pTriangles->vertexCount * pTriangles->vertexSize;
//	ret = m_pVertexBuffer->Lock( 0, nDataSize, &pDataTemp );
//	if ( ret.IsOK() == false )
//		return;
//	memcpy_s( pDataTemp, nDataSize, pTriangles->vertices, nDataSize );
//	m_pVertexBuffer->Unlock();
//
//	nDataSize = pTriangles->indexCount * 2;
//	ret = m_pIndexBuffer->Lock( 0, nDataSize, &pDataTemp );
//	if ( ret.IsOK() == false )
//		return;
//	memcpy_s( pDataTemp, nDataSize, pTriangles->indices, nDataSize );
//	m_pIndexBuffer->Unlock();
//
//	SetShaderBindings();
//	m_pCommandContext->SetVertexBuffer( 0, m_pVertexBuffer, 0 );
//	m_pCommandContext->SetIndexBuffer( m_pIndexBuffer, 0 );
//	m_pCommandContext->SetShaderState( pShaderState );
//	m_pCommandContext->SetPrimitiveTopology( RHI::UPrimitiveType::Triangle );
//	m_pCommandContext->DrawIndexedPrimitive( 0, 0, pTriangles->nTrianglesCount );
//}


//void iberbar::Renderer::CRenderer::Flush()
//{
//	DrawBatchTriangles();
//}


void iberbar::Renderer::CRenderer::ProcessGroupCommand( CRenderGroupCommand* pCommand )
{
	//Flush();
	assert( m_bIsRendering == true );

	CRenderGroupCommand* pGroupCommand = (CRenderGroupCommand*)pCommand;
	CRenderQueue* pQueue = GetRenderQueue( pGroupCommand->GetQueueId() );
	if ( pQueue == nullptr )
		return;
	CBaseRendererProcessor* pRendererProcessor = pGroupCommand->GetRendererProcessor();
	if ( pRendererProcessor != nullptr && m_pCurrentRendererProcessor != pRendererProcessor )
	{
		m_pCurrentRendererProcessor->Flush();
		m_pCurrentRendererProcessor = pRendererProcessor;
	}
	m_pCurrentRendererProcessor->VisitQueue( pQueue );
}







