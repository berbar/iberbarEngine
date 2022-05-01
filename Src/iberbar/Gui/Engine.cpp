
#include <iberbar/Gui/Engine.h>
#include <iberbar/Gui/BaseTypes.h>
#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/CallbackCommand.h>
#include <iberbar/Renderer/GroupCommand.h>
#include <iberbar/Renderer/Effects/EffectMatrices.h>
#include <iberbar/Utility/Command.h>





iberbar::Gui::CEngine* iberbar::Gui::CEngine::sm_pInstance = nullptr;


iberbar::Gui::CEngine::CEngine( Renderer::CRenderer* pRenderer, CCommandQueue* pCommandQueue )
	: m_pRenderer( pRenderer )
	, m_pRenderGroupCommandManager( pRenderer->GetRenderGroupCommandManager() )
	, m_pRenderCommand_Callback( new Renderer::CRenderCallbackCommand() )
	, m_pCommandQueue( pCommandQueue )
	, m_bMemoryResAuto( true )
	, m_pMemoryRes( new std::pmr::unsynchronized_pool_resource() )
	, m_pWidgetFocus( nullptr )
	, m_Dialogs()
	, m_RenderGroupCommandList()
	, m_ViewportState()
	, m_CanvasResolution( 0, 0 )
	, m_pEffectMatrices( new Renderer::CEffectMatrices() )
{
	sm_pInstance = this;
	m_pRenderCommand_Callback->SetProc( std::bind( &CEngine::OnRenderCallbackCommand, this ) );
}


iberbar::Gui::CEngine::CEngine( Renderer::CRenderer* pRenderer, CCommandQueue* pCommandQueue, std::pmr::memory_resource* pMemoryRes )
	: m_pRenderer( pRenderer )
	, m_pRenderGroupCommandManager( pRenderer->GetRenderGroupCommandManager() )
	, m_pRenderCommand_Callback( new Renderer::CRenderCallbackCommand() )
	, m_pCommandQueue( pCommandQueue )
	, m_bMemoryResAuto( pMemoryRes == nullptr ? true : false )
	, m_pMemoryRes( pMemoryRes == nullptr ? new std::pmr::unsynchronized_pool_resource() : pMemoryRes )
	, m_pWidgetFocus( nullptr )
	, m_Dialogs()
	, m_RenderGroupCommandList()
	, m_ViewportState()
	, m_CanvasResolution( 0, 0 )
	, m_pEffectMatrices( new Renderer::CEffectMatrices() )
{
	sm_pInstance = this;
	m_pRenderCommand_Callback->SetProc( std::bind( &CEngine::OnRenderCallbackCommand, this ) );
}


iberbar::Gui::CEngine::~CEngine()
{
	if ( m_RenderGroupCommandList.empty() == false )
	{
		auto iter = m_RenderGroupCommandList.begin();
		auto end = m_RenderGroupCommandList.end();
		for ( ; iter != end; iter++ )
		{
			SAFE_DELETE( *iter );
		}
		m_RenderGroupCommandList.clear();
	}

	if ( m_Dialogs.empty() == false )
	{
		auto iter = m_Dialogs.begin();
		auto end = m_Dialogs.end();
		for ( ; iter != end; iter++ )
		{
			UNKNOWN_SAFE_RELEASE_NULL( *iter );
		}
		m_Dialogs.clear();
	}

	if ( m_bMemoryResAuto == true )
	{
		SAFE_DELETE( m_pMemoryRes );
	}

	SAFE_DELETE( m_pRenderCommand_Callback );

	UNKNOWN_SAFE_RELEASE_NULL( m_pEffectMatrices );

	sm_pInstance = nullptr;
}


iberbar::CResult iberbar::Gui::CEngine::Initial()
{
	m_pEffectMatrices->Initial();

	return CResult();
}


void iberbar::Gui::CEngine::AddDialog( CDialog* pDialog )
{
	assert( pDialog );
	for ( auto& pDialogTemp : m_Dialogs )
	{
		if ( pDialogTemp == pDialog )
			return;
	}
	m_Dialogs.push_back( pDialog );
	pDialog->AddRef();

	if ( m_RenderGroupCommandList.size() < m_Dialogs.size() )
	{
		Renderer::CRenderGroupCommand* pGroupCommand = new Renderer::CRenderGroupCommand( m_pRenderGroupCommandManager );
		m_RenderGroupCommandList.push_back( pGroupCommand );
	}
}


iberbar::Gui::CDialog* iberbar::Gui::CEngine::GetDialog( const char* strId )
{
	for ( auto& pDialogTemp : m_Dialogs )
	{
		if ( strcmp( pDialogTemp->GetId().c_str(), strId ) == 0 )
			return pDialogTemp;
	}
	return nullptr;
}


void iberbar::Gui::CEngine::RemoveDialog( CDialog* pDialog )
{
	if ( pDialog == nullptr )
		return;

	if ( m_Dialogs.empty() == false )
	{
		auto iter = m_Dialogs.begin();
		auto end = m_Dialogs.end();
		for ( ; iter != end; iter++ )
		{
			if ( (*iter) == pDialog )
			{
				UNKNOWN_SAFE_RELEASE_NULL( *iter );
				m_Dialogs.erase( iter );
				return;
			}
		}
	}
}


void iberbar::Gui::CEngine::RequestFocus( CWidget* pWidget )
{
	if ( pWidget == nullptr )
		return;

	if ( m_pWidgetFocus == pWidget )
		return;

	if ( m_pWidgetFocus != nullptr )
		m_pWidgetFocus->OnFocusOut();

	m_pWidgetFocus = pWidget;
	m_pWidgetFocus->OnFocusIn();

	RequestTop( m_pWidgetFocus->GetDialog() );
}


void iberbar::Gui::CEngine::ClearFocus( bool bSendEvent )
{
	if ( m_pWidgetFocus != nullptr )
	{
		if ( bSendEvent == true )
		{
			m_pWidgetFocus->OnFocusOut();
		}
		m_pWidgetFocus = nullptr;
	}
}


void iberbar::Gui::CEngine::RequestTop( CDialog* pDialog )
{
	if ( pDialog == nullptr )
		return;

	if ( pDialog->GetParentDialog() != nullptr )
	{
		RequestTop( pDialog->GetParentDialog() );
	}

	auto iter = m_Dialogs.begin();
	auto end = m_Dialogs.end();
	for ( ; iter != end; iter++ )
	{
		if ( (*iter) == pDialog )
		{
			m_Dialogs.erase( iter );
			m_Dialogs.push_back( pDialog );
			break;
		}
	}
}


void iberbar::Gui::CEngine::AddCommand( CBaseCommand* pCommand )
{
	m_pCommandQueue->AddCommand( pCommand );
}


void iberbar::Gui::CEngine::Update( int64 nElapsedTimeMs, float nElapsedTimeSecond )
{
	for ( auto& pDialog : m_Dialogs )
	{
		if ( pDialog->IsEnable() == false )
			continue;
		pDialog->Update( nElapsedTimeSecond );
	}
}


void iberbar::Gui::CEngine::Render()
{
	m_pRenderer->AddCommand( m_pRenderCommand_Callback );

	int nIndex = 0;
	Renderer::CRenderGroupCommand* pGroupCommand = nullptr;
	for ( auto& pDialog : m_Dialogs )
	{
		if ( pDialog->IsVisible() == false )
			continue;

		// 开启队列
		pGroupCommand = m_RenderGroupCommandList[nIndex];
		m_pRenderer->AddCommand( pGroupCommand, 0 );
		m_pRenderer->PushRenderQueue( pGroupCommand->GetQueueId() );

		// 渲染对话框
		pDialog->Render();

		// 结束队列
		m_pRenderer->PopRenderQueue();

		nIndex++;
	}
}


void iberbar::Gui::CEngine::HandleMouse( const UMouseEventData* EventData )
{
	auto iter = m_Dialogs.rbegin();
	auto end = m_Dialogs.rend();
	for ( ; iter != end; iter++ )
	{
		if ( (*iter)->HandleMouse( EventData ) == UHandleMessageResult::Succeed )
		{
			return;
		}
		if ( ( *iter )->IsModal() )
		{
			return;
		}
	}
}


void iberbar::Gui::CEngine::HandleKeyboard( const UKeyboardEventData* pEvent )
{
	auto iter = m_Dialogs.rbegin();
	auto end = m_Dialogs.rend();
	for ( ; iter != end; iter++ )
	{
		if ( (*iter)->HandleKeyboard( pEvent ) == UHandleMessageResult::Succeed )
		{
			return;
		}
		if ( ( *iter )->IsModal() )
		{
			return;
		}
	}
}


void iberbar::Gui::CEngine::SetCanvasResolution( const CSize2i& Size )
{
	assert( m_pEffectMatrices );
	m_CanvasResolution = Size;

	DirectX::XMFLOAT4 vecEye( Size.w/2, -Size.h/2, 0.0f, 1.0f );
	DirectX::XMFLOAT4 vecEyeDirection( 0, 0, 1.0f, 0.0f );
	DirectX::XMFLOAT4 vecUpDirection( 0.0f, 1.0f, 0.0f, 0.0f );
	DirectX::XMMATRIX MatView = DirectX::XMMatrixLookToLH(
		DirectX::XMLoadFloat4( &vecEye ),
		DirectX::XMLoadFloat4( &vecEyeDirection ),
		DirectX::XMLoadFloat4( &vecUpDirection ) );
	DirectX::XMFLOAT4X4 MatView_4x4;
	DirectX::XMStoreFloat4x4( &MatView_4x4, MatView );

	DirectX::XMMATRIX MatProjection = DirectX::XMMatrixOrthographicLH(
		(float)m_CanvasResolution.w,
		(float)m_CanvasResolution.h,
		-100.0f,
		100.0f );
	DirectX::XMFLOAT4X4 MatProjection_4x4;
	DirectX::XMStoreFloat4x4( &MatProjection_4x4, MatProjection );

	m_pEffectMatrices->SetViewMatrix( MatView_4x4 );
	m_pEffectMatrices->SetProjectionMatrix( MatProjection_4x4 );
}


void iberbar::Gui::CEngine::OnRenderCallbackCommand()
{
	assert( m_pEffectMatrices );
	m_pEffectMatrices->Apply();
}



