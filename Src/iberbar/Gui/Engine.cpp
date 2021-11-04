
#include <iberbar/Gui/Engine.h>
#include <iberbar/Gui/BaseTypes.h>
#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/CallbackCommand.h>
#include <iberbar/Renderer/GroupCommand.h>
#include <iberbar/Utility/Command.h>





iberbar::Gui::CEngine* iberbar::Gui::CEngine::sm_pInstance = nullptr;


iberbar::Gui::CEngine::CEngine( Renderer::CRenderer2d* pRenderer, CCommandQueue* pCommandQueue )
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
{
	sm_pInstance = this;
}


iberbar::Gui::CEngine::CEngine( Renderer::CRenderer2d* pRenderer, CCommandQueue* pCommandQueue, std::pmr::memory_resource* pMemoryRes )
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
{
	sm_pInstance = this;
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

	sm_pInstance = nullptr;
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






