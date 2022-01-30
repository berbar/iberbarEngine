
#include <iberbar/Renderer/Processor/BaseRendererProcessor.h>


iberbar::Renderer::CBaseRendererProcessor::CBaseRendererProcessor( IRenderer* pRenderer, PCallbackRendererVisitQueue pRendererVisitQueue )
	: m_pRenderer( pRenderer )
	, m_pRendererVisitQueue( pRendererVisitQueue )
{
	assert( m_pRenderer );
	assert( m_pRendererVisitQueue );
}
