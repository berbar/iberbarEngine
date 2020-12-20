
#include <iberbar/Paper2d/Director.h>
#include <iberbar/Paper2d/Scene.h>
#include <iberbar/Paper2d/DrawContext.h>
#include <iberbar/Paper2d/PhysicsWorld.h>
#include <iberbar/Paper2d/ComponentSystem.h>
#include <iberbar/Paper2d/Component_Updatable.h>
#include <iberbar/Paper2d/Component_HandleMouseInput.h>
#include <iberbar/Paper2d/Component_HandleKeyboardInput.h>
#include <iberbar/Paper2d/Camera.h>
#include <iberbar/RHI/Device.h>
#include <iberbar/Utility/Input.h>



iberbar::Paper2d::CDirector* iberbar::Paper2d::CDirector::sm_pSharedInstance = nullptr;


iberbar::Paper2d::CDirector::CDirector( Renderer::CRendererSprite* pRendererSprite )
	: m_pRendererSprite( pRendererSprite )
	, m_pRunningScene( nullptr )
	, m_pPhysicsWorld( new CPhysicsWorld() )
	, m_pComponentSystem_HandleMouseInput( new CComponentSystem_HandleMouseInput() )
	, m_pComponentSystem_HandleKeyboardInput( new CComponentSystem_HandleKeyboardInput() )
{
	sm_pSharedInstance = this;
}


iberbar::Paper2d::CDirector::~CDirector()
{
	SAFE_DELETE( m_pPhysicsWorld );
	UNKNOWN_SAFE_RELEASE_NULL( m_pRunningScene );
	SAFE_DELETE( m_pComponentSystem_HandleMouseInput );
	SAFE_DELETE( m_pComponentSystem_HandleKeyboardInput );

	sm_pSharedInstance = nullptr;
}


void iberbar::Paper2d::CDirector::RunScene( CScene* pScene )
{
	if ( m_pRunningScene != pScene )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pRunningScene );
		m_pRunningScene = pScene;
		UNKNOWN_SAFE_ADDREF( m_pRunningScene );
	}
}


void iberbar::Paper2d::CDirector::UpdateScene( float nDelta )
{
	m_pPhysicsWorld->Update( nDelta );

	if ( m_pRunningScene != nullptr )
	{
		m_pRunningScene->Update( nDelta );
	}
}


void iberbar::Paper2d::CDirector::DrawScene()
{
	if ( m_pRunningScene != nullptr )
	{
		CDrawContext Context = CDrawContext( m_pRendererSprite, DirectX::XMFLOAT4X4() );
		m_pRunningScene->Draw( &Context );
	}
}


void iberbar::Paper2d::CDirector::OnResizeViewport( int nWidth, int nHeight )
{

}


void iberbar::Paper2d::CDirector::HandleMouse( const UMouseEventData* pEventData )
{
	if ( m_pRunningScene != nullptr )
	{
		const auto& ViewportSize = m_pRendererSprite->GetRenderer()->GetRHIDevice()->GetContextSize();
		CCamera* pCamera = m_pRunningScene->GetCamera();

		DirectX::XMFLOAT2 f2_MousePoint = DirectX::XMFLOAT2( (float)pEventData->MousePoint.x, (float)pEventData->MousePoint.y );
		DirectX::XMVECTOR vec_MousePoint = DirectX::XMLoadFloat2( &f2_MousePoint );
		DirectX::XMVECTOR vec_PointAtWorld = DirectX::XMVector3Unproject(
			vec_MousePoint,
			0.0f, 0.0f, (float)ViewportSize.w, (float)ViewportSize.h, 0.0f, 1.0f,
			pCamera->GetMatrix_Projection(),
			pCamera->GetMatrix_View(),
			DirectX::XMMatrixIdentity() );
		DirectX::XMFLOAT2 f2_PointAtWorld;
		DirectX::XMStoreFloat2( &f2_PointAtWorld, vec_PointAtWorld );
		UComponentMouseEventData EventDataNew;
		EventDataNew.nEvent = pEventData->nMouseEvent;
		EventDataNew.nMouseId = pEventData->nMouseId;
		EventDataNew.nMouseWheel = pEventData->nMouseWheel;
		EventDataNew.MousePoint = DirectX::XMFLOAT2( (float)pEventData->MousePoint.x, (float)pEventData->MousePoint.y );
		EventDataNew.WorldPoint = f2_PointAtWorld;
		m_pComponentSystem_HandleMouseInput->HandleMouse( &EventDataNew );
	}
}


void iberbar::Paper2d::CDirector::HandleKeyboard( const UKeyboardEventData* pEventData )
{
	if ( m_pRunningScene != nullptr )
	{
		UComponentKeyboardEventData EventDataNew;
		EventDataNew.nEvent = pEventData->nEvent;
		EventDataNew.nKeyCode = pEventData->nKeyCode;
		m_pComponentSystem_HandleKeyboardInput->HandleKeyboard( &EventDataNew );
	}
}

