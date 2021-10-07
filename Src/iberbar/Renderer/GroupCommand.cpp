


#include <iberbar/Renderer/GroupCommand.h>
#include <iberbar/Renderer/Renderer.h>




iberbar::Renderer::CRenderGroupCommandManager::CRenderGroupCommandManager( CRenderer2d* pRenderer )
	: m_pRenderer( pRenderer )
	, m_QueueIdMapping()
	, m_UnusedQueueId()
{
}


iberbar::Renderer::CRenderGroupCommandManager::~CRenderGroupCommandManager()
{
}


int iberbar::Renderer::CRenderGroupCommandManager::AllocQueueId()
{
	int nQueueId;

	if ( m_UnusedQueueId.empty() )
	{
		nQueueId = m_pRenderer->CreateRenderQueue();
		m_QueueIdMapping[ nQueueId ] = true;
	}
	else
	{
		nQueueId = *( m_UnusedQueueId.rbegin() );
		m_UnusedQueueId.pop_back();
		m_QueueIdMapping[nQueueId] = true;
	}

	return nQueueId;
}


void iberbar::Renderer::CRenderGroupCommandManager::ReleaseQueueId( int nQueueId )
{
	m_QueueIdMapping[nQueueId] = false;
	m_UnusedQueueId.push_back( nQueueId );
}








iberbar::Renderer::CRenderGroupCommand::CRenderGroupCommand( CRenderGroupCommandManager* pManager )
	: CRenderCommand( URenderCommandType::Group )
	, m_pGroupCommandManager( pManager )
	, m_nQueueId( 0 )
{
	assert( m_pGroupCommandManager );
	m_nQueueId = m_pGroupCommandManager->AllocQueueId();
}


iberbar::Renderer::CRenderGroupCommand::CRenderGroupCommand( const CRenderGroupCommand& Other )
	: CRenderCommand( Other )
	, m_pGroupCommandManager( Other.m_pGroupCommandManager )
	, m_nQueueId( 0 )
{
	assert( m_pGroupCommandManager );
	m_nQueueId = m_pGroupCommandManager->AllocQueueId();
}


iberbar::Renderer::CRenderGroupCommand::~CRenderGroupCommand()
{
	if ( m_pGroupCommandManager && m_nQueueId > 0 )
	{
		m_pGroupCommandManager->ReleaseQueueId( m_nQueueId );
	}
}
