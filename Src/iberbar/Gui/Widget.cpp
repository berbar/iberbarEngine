
#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Utility/Command.h>
#include <iberbar/Utility/String.h>



namespace iberbar
{
	namespace Gui
	{
		class CCommand_RequestFocus final
			: public CBaseCommand
		{
			iberbarGuiOverride_OperatorNewAndDelete_1( CCommand_RequestFocus )

		public:
			CCommand_RequestFocus( CWidget* pWidget );
			virtual ~CCommand_RequestFocus();
			virtual void Execute() override;

		private:
			CWidget* m_pWidget;
		};


		class CCommand_WidgetEvent final
			: public CBaseCommand
		{
			iberbarGuiOverride_OperatorNewAndDelete_1( CCommand_WidgetEvent )
		public:
			CCommand_WidgetEvent( CWidget * pWidget, uint64 nEvent, uint64 nValueUInt, const void* pValueExt );
			virtual ~CCommand_WidgetEvent();
			virtual void Execute() override;

			void AddEventCallback( std::function<UCallbackWidgetEventProc> callback )
			{
				m_callbacks.push_back( callback );
			}

		private:
			CWidget* m_pWidget;
			uint64 m_nEvent;
			uint64 m_nValueUInt;
			const void* m_pValueExt;
			std::vector<std::function<UCallbackWidgetEventProc>> m_callbacks;
		};
	}
}




iberbar::Gui::CWidget::CWidget( void )
	: m_pDialog( nullptr )
	, m_pWidgetParent( nullptr )
	, m_bCanFocus( false )
	, m_bFocus( false )
	, m_bMouseOver( false )
	, m_bMouseInput( true )
	, m_bKeyboardInput( true )

	, m_nGroup( 0 )

	, m_bNeedClip( false )

	, m_pRenderElementDefault( nullptr )
	, m_nEventListenerHandleAlloc( 0 )
	, m_EventListeners()
{
}


iberbar::Gui::CWidget::CWidget( const CWidget& widget )
	: CObject( widget )
	, m_pDialog( nullptr )
	, m_pWidgetParent( nullptr )
	, m_bCanFocus( widget.m_bCanFocus )
	, m_bFocus( false )
	, m_bMouseOver( false )
	, m_bMouseInput( widget.m_bMouseInput )
	, m_bKeyboardInput( widget.m_bKeyboardInput )

	, m_nGroup( widget.m_nGroup )

	, m_bNeedClip( widget.m_bNeedClip )

	, m_pRenderElementDefault( nullptr )
	, m_nEventListenerHandleAlloc( 0 )
	, m_EventListeners()
{
	if ( widget.m_pRenderElementDefault != nullptr )
	{
		m_pRenderElementDefault = CRenderElement::sCloneRenderElementRecurse( widget.m_pRenderElementDefault );
		if ( m_pRenderElementDefault != nullptr )
			m_pRenderElementDefault->GetTransform()->SetParentTransform( this->GetTransform() );
	}
}


iberbar::Gui::CWidget::~CWidget()
{
	RemoveWidgetsAll();
	UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementDefault );
}


void iberbar::Gui::CWidget::SetDialog( CDialog* pDialog )
{
	m_pDialog = pDialog;
	for ( CWidget* pWidget : m_Widgets )
	{
		pWidget->SetDialog( pDialog );
	}
}


void iberbar::Gui::CWidget::RequestFocus()
{
	CEngine::sGetInstance()->AddCommand( new CCommand_RequestFocus( this ) );
}


void iberbar::Gui::CWidget::SetMouseInput( bool bEnable /* = true */ )
{
	m_bMouseInput = bEnable;
}


bool iberbar::Gui::CWidget::IsMouseInputEnabled() const
{
	if ( GetDialog() && GetDialog()->IsMouseInputEnabled() == false )
		return false;
	return m_bMouseInput;
}

void iberbar::Gui::CWidget::SetKeyBoardInput( bool bEnable /* = true */ )
{
	m_bKeyboardInput = bEnable;
}

bool iberbar::Gui::CWidget::IsKeyboardInputEnabled() const
{
	if ( GetDialog() && GetDialog()->IsKeyboardInputEnabled() == false )
		return false;
	return m_bKeyboardInput;
}

bool iberbar::Gui::CWidget::IsVisible() const
{
	if ( m_bVisible == false )
		return false;
	if ( GetDialog() && GetDialog()->IsVisible() == false )
		return false;
	if ( GetWidgetParent() && GetWidgetParent()->IsVisible() == false )
		return false;
	return m_bVisible;
}

bool iberbar::Gui::CWidget::IsEnable() const
{
	if ( m_bEnable == false )
		return false;
	if ( GetDialog() && GetDialog()->IsEnable() == false )
		return false;
	if ( GetWidgetParent() && GetWidgetParent()->IsEnable() == false )
		return false;
	return m_bEnable;
}


int iberbar::Gui::CWidget::AddEventCallback( std::function<UCallbackWidgetEventProc> callback, uint64 nEvent, bool bDefer )
{
	if ( !callback )
		return 0;
	m_nEventListenerHandleAlloc++;
	UWidgetEventListener listener { bDefer, m_nEventListenerHandleAlloc, nEvent, callback };
	m_EventListeners.push_back( listener );
	return m_nEventListenerHandleAlloc;
}


void iberbar::Gui::CWidget::RemoveEventCallback( int nHandle )
{
	auto iter = m_EventListeners.begin();
	auto end = m_EventListeners.end();
	for ( ; iter != end; iter++ )
	{
		if ( ( *iter ).nHandle == nHandle )
		{
			m_EventListeners.erase( iter );
			return;
		}
	}
}


void iberbar::Gui::CWidget::RemoveEventCallbacksAll()
{
	m_EventListeners.clear();
}


void iberbar::Gui::CWidget::SendEvent( uint64 nEvent, uint64 nValueUint, const void* pValueExt )
{
	if ( m_EventListeners.empty() == true )
		return;

	CCommand_WidgetEvent* pCommand = nullptr;
	for ( auto& listener : m_EventListeners )
	{
		if ( listener.nEvent == nEvent || listener.nEvent == 0 )
		{
			if ( listener.bDefer == true )
			{
				if ( pCommand == nullptr )
				{
					pCommand = new CCommand_WidgetEvent( this, nEvent, nValueUint, pValueExt );
					CEngine::sGetInstance()->AddCommand( pCommand );
				}
				pCommand->AddEventCallback( listener.callback );
			}
			else
			{
				listener.callback( this, nEvent, nValueUint, pValueExt );
			}
		}
	}
}


void iberbar::Gui::CWidget::SetRenderElement( CRenderElement* pRenderElement )
{
	if ( m_pRenderElementDefault == pRenderElement )
		return;

	UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementDefault );
	m_pRenderElementDefault = pRenderElement;
	if ( m_pRenderElementDefault )
	{
		m_pRenderElementDefault->AddRef();
		m_pRenderElementDefault->GetTransform()->SetParentTransform( GetTransform() );
	}
}


bool iberbar::Gui::CWidget::FindElement( const char* strName, CRenderElement** ppOutElement )
{
	if ( m_pRenderElementDefault == nullptr )
		return false;
	return m_pRenderElementDefault->FindElement( strName, ppOutElement );
}


void iberbar::Gui::CWidget::OnFocusIn()
{
	m_bFocus = true;
	// ！！！
	// 该处在gc时被调用了，在lua里面没有被gc，造成内存泄漏
	// ！！！
	SendEvent( BaseEvent::nFocusIn );
}


void iberbar::Gui::CWidget::OnFocusOut()
{
	m_bFocus = false;
	SendEvent( BaseEvent::nFocusOut );
}


void iberbar::Gui::CWidget::OnMouseEnter()
{
	m_bMouseOver = true;
	SendEvent( BaseEvent::nMouseEnter );
}


void iberbar::Gui::CWidget::OnMouseLeave()
{
	m_bMouseOver = false;
	SendEvent( BaseEvent::nMouseLeave );
}


void iberbar::Gui::CWidget::UpdateRect()
{
	if ( m_Widgets.empty() == false )
	{
		for ( CWidget* pWidget : m_Widgets )
		{
			pWidget->UpdateRect();
		}
	}
	if ( m_pRenderElementDefault )
	{
		m_pRenderElementDefault->UpdateRect();
	}
}


void iberbar::Gui::CWidget::Refresh()
{
	m_bMouseOver = false;
	m_bFocus = false;

	for ( CWidget* pWidget : m_Widgets )
	{
		pWidget->Refresh();
	}

	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->Refresh();
}


void iberbar::Gui::CWidget::Update( float nElapsedTime )
{
	if ( IsVisible() == false )
		return;

	for ( CWidget* pWidget : m_Widgets )
	{
		pWidget->Update( nElapsedTime );
	}

	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->Update( nElapsedTime );
}


void iberbar::Gui::CWidget::Render()
{
	if ( IsVisible() == false )
		return;

	bool bPopViewport = false;
	if ( m_bNeedClip == true )
	{
		CEngine::sGetInstance()->GetViewportState()->AddViewport( m_pTransform->GetBounding() );
		bPopViewport = true;
	}

	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->Render();

	for ( CWidget* pWidget : m_Widgets )
	{
		pWidget->Render();
	}

	if ( bPopViewport == true )
	{
		CEngine::sGetInstance()->GetViewportState()->PopViewport();
	}
}



int iberbar::Gui::CWidget::AddWidget( CWidget* pWidget )
{
	if ( pWidget == nullptr )
		return 0;

	if ( pWidget->GetDialog() != nullptr )
		return 0;

	for ( auto& pWidgetTemp : m_Widgets )
	{
		if ( pWidgetTemp == pWidget )
			return 0;
	}

	m_Widgets.push_back( pWidget );
	pWidget->SetWidgetParent( this );
	pWidget->AddRef();

	return 1;
}


iberbar::Gui::CWidget* iberbar::Gui::CWidget::FindWidget( const char* strId )
{
	if ( StringIsNullOrEmpty( strId ) == true )
		return nullptr;

	CWidget* pWidgetFind = nullptr;
	for ( auto& pWidget : m_Widgets )
	{
		if ( strcmp( pWidget->GetId().c_str(), strId ) == 0 )
			return pWidget;

		pWidgetFind = pWidget->FindWidget( strId );
		if ( pWidgetFind != nullptr )
			return pWidgetFind;
	}

	return nullptr;
}


int iberbar::Gui::CWidget::RemoveWidget( CWidget* pWidget )
{
	if ( pWidget == nullptr )
		return 0;

	auto iter = m_Widgets.begin();
	auto end = m_Widgets.end();
	for ( ; iter != end; iter++ )
	{
		if ( ( *iter ) == pWidget )
		{
			if ( CEngine::sGetInstance()->GetFocus() == pWidget )
			{
				CEngine::sGetInstance()->ClearFocus( true );
			}

			pWidget->SetWidgetParent( nullptr );
			pWidget->Release();

			m_Widgets.erase( iter );

			return 1;
		}
	}
	return 0;
}


void iberbar::Gui::CWidget::RemoveWidgetsAll()
{
	if ( m_Widgets.empty() == false )
	{
		auto iter = m_Widgets.begin();
		auto end = m_Widgets.end();
		for ( ; iter != end; iter++ )
		{
			if ( ( *iter ) == CEngine::sGetInstance()->GetFocus() )
			{
				CEngine::sGetInstance()->ClearFocus( false );
			}

			( *iter )->SetWidgetParent( nullptr );
			( *iter )->Release();
			( *iter ) = nullptr;
		}
		m_Widgets.clear();
	}
}


void iberbar::Gui::CWidget::ForeachWidgets( std::function<void( CWidget* )> Func, int nDepth )
{
	if ( m_Widgets.empty() )
		return;

	if ( nDepth == 0 )
		return;

	for ( CWidget* pWidget : m_Widgets )
	{
		Func( pWidget );
		pWidget->ForeachWidgets( Func, nDepth );
	}
}


void iberbar::Gui::CWidget::SetWidgetParent( CWidget* pWidgetParent )
{
	m_pWidgetParent = pWidgetParent;
	if ( m_pWidgetParent == nullptr )
	{
		SetDialog( nullptr );
		GetTransform()->SetParentTransform( nullptr );
	}
	else
	{
		SetDialog( pWidgetParent->GetDialog() );
		GetTransform()->SetParentTransform( m_pWidgetParent->GetTransform() );
	}
}




iberbar::Gui::CWidget* iberbar::Gui::CWidget::sCloneWidgetEx( CWidget* pWidgetSrc, bool bRecurse )
{
	CWidget* pWidgetNew = pWidgetSrc->Clone();
	if ( pWidgetNew == nullptr )
		return nullptr;

	if ( bRecurse == true )
	{

	}

	return pWidgetNew;
}







iberbar::Gui::CCommand_RequestFocus::CCommand_RequestFocus( CWidget* pWidget )
	: m_pWidget( pWidget )
{
	m_pWidget->AddRef();
}


iberbar::Gui::CCommand_RequestFocus::~CCommand_RequestFocus()
{
	m_pWidget->Release();
	m_pWidget = nullptr;
}


void iberbar::Gui::CCommand_RequestFocus::Execute()
{
	CEngine::sGetInstance()->RequestFocus( m_pWidget );
}






iberbar::Gui::CCommand_WidgetEvent::CCommand_WidgetEvent( CWidget* pWidget, uint64 nEvent, uint64 nValueUInt, const void* pValueExt )
	: m_pWidget( pWidget )
	, m_nEvent( nEvent )
	, m_nValueUInt( nValueUInt )
	, m_pValueExt( pValueExt )
{
	m_pWidget->AddRef();
}


iberbar::Gui::CCommand_WidgetEvent::~CCommand_WidgetEvent()
{
	m_pWidget->Release();
	m_pWidget = nullptr;
}


void iberbar::Gui::CCommand_WidgetEvent::Execute()
{
	for ( auto& callback : m_callbacks )
	{
		callback( m_pWidget, m_nEvent, m_nValueUInt, m_pValueExt );
	}
}

