

#include <iberbar/Paper2d/DrawContext.h>


iberbar::Paper2d::CDrawContext::CDrawContext( Renderer::CRendererSprite* pRendererSprite, const DirectX::XMFLOAT4X4& matViewProjection )
	: m_pRenderer( pRendererSprite->GetRenderer() )
	, m_pRendererSprite( pRendererSprite )
	, m_matViewProjection( matViewProjection )
{
}

