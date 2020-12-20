
#include <iberbar/Paper2d/Scene.h>
#include <iberbar/Paper2d/Camera.h>
#include <iberbar/Paper2d/DrawContext.h>



iberbar::Paper2d::CScene::CScene()
	: m_pCamera( new CCamera() )
{
}


iberbar::Paper2d::CScene::CScene( const CScene& scene )
	: CNode( scene )
	, m_pCamera( new CCamera() )
{
}


iberbar::Paper2d::CScene::~CScene()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pCamera );
}


iberbar::Paper2d::CScene* iberbar::Paper2d::CScene::Clone() const
{
	return new CScene( *this );
}


void iberbar::Paper2d::CScene::UpdateTransform()
{
	CNode::UpdateTransform();
}


void iberbar::Paper2d::CScene::Draw( CDrawContext* pContext )
{
	CDrawContext DrawContext( pContext->GetRendererSprite(), m_pCamera->GetFloat4x4_ViewProjection() );
	CNode::Draw( &DrawContext );
}


void iberbar::Paper2d::CScene::DrawSelf( CDrawContext* pContext )
{

}

