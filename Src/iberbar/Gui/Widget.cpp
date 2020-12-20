
#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Utility/Command.h>



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
	, m_bCanFocus( false )
	, m_bFocus( false )
	, m_bMouseOver( false )
	, m_bMouseInput( true )
	, m_bKeyboardInput( true )

	, m_nGroup( 0 )

	, m_bNeedClip( false )

	, m_pRenderElementDefault( nullptr )
	, m_EventListeners()
{
}


iberbar::Gui::CWidget::CWidget( const CWidget& widget )
	: CObject( widget )

	, m_pDialog( nullptr )
	, m_bCanFocus( widget.m_bCanFocus )
	, m_bFocus( false )
	, m_bMouseOver( false )
	, m_bMouseInput( widget.m_bMouseInput )
	, m_bKeyboardInput( widget.m_bKeyboardInput )

	, m_nGroup( widget.m_nGroup )

	, m_bNeedClip( widget.m_bNeedClip )

	, m_pRenderElementDefault( nullptr )
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
	UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementDefault );
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
	if ( GetParent() && GetParent()->IsMouseInputEnabled() == false )
		return false;
	return m_bMouseInput;
}

void iberbar::Gui::CWidget::SetKeyBoardInput( bool bEnable /* = true */ )
{
	m_bKeyboardInput = bEnable;
}

bool iberbar::Gui::CWidget::IsKeyboardInputEnabled() const
{
	if ( GetParent() && GetParent()->IsKeyboardInputEnabled() == false )
		return false;
	return m_bKeyboardInput;
}

bool iberbar::Gui::CWidget::IsVisible() const
{
	if ( GetParent() && GetParent()->IsVisible() == false )
		return false;
	return m_bVisible;
}

bool iberbar::Gui::CWidget::IsEnable() const
{
	if ( GetParent() && GetParent()->IsEnable() == false )
		return false;
	return m_bEnable;
}


void iberbar::Gui::CWidget::AddEventCallback( std::function<UCallbackWidgetEventProc> callback, uint64 nEvent )
{
	if ( !callback )
		return;
	UWidgetEventListener listener { nEvent, callback };
	m_EventListeners.push_back( listener );
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
			if ( pCommand == nullptr )
			{
				pCommand = new CCommand_WidgetEvent( this, nEvent, nValueUint, pValueExt );
				CEngine::sGetInstance()->AddCommand( pCommand );
			}
			pCommand->AddEventCallback( listener.callback );
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


void iberbar::Gui::CWidget::SetParent( CDialog* pDialog )
{
	m_pDialog = pDialog;
	if ( m_pDialog )
		this->GetTransform()->SetParentTransform( m_pDialog->GetTransform() );
	else
		this->GetTransform()->SetParentTransform( nullptr );
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
}


void iberbar::Gui::CWidget::Refresh()
{
	m_bMouseOver = false;
	m_bFocus = false;

	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->Refresh();
}


void iberbar::Gui::CWidget::Update( float nElapsedTime )
{
	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->Update( nElapsedTime );
}


void iberbar::Gui::CWidget::Render()
{
	bool bPopViewport = false;
	if ( m_bNeedClip == true )
	{
		CEngine::sGetInstance()->GetViewportState()->AddViewport( m_pTransform->GetBounding() );
		bPopViewport = true;
	}

	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->Render();

	if ( bPopViewport == true )
	{
		CEngine::sGetInstance()->GetViewportState()->PopViewport();
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

