
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Utility/Input.h>
#include <iberbar/Utility/Command.h>
#include <iberbar/Utility/String.h>




namespace iberbar
{
	namespace Gui
	{
		class CCommand_RequestTop final
			: public CBaseCommand
		{
			iberbarGuiOverride_OperatorNewAndDelete_1( CCommand_RequestTop )

		public:
			CCommand_RequestTop( Gui::CDialog* pDialog );
			virtual ~CCommand_RequestTop();
			virtual void Execute() override;

		private:
			Gui::CDialog* m_pDialog;
		};
	}
}






//iberbar::Gui::CDialog::CDialog()
//	: m_pWidgetMouseOver( nullptr )
//	, m_bMouseInput( true )
//	, m_bKeyboardInput( true )
//	, m_bNeedClip( true )
//	, m_Widgets()
//{
//}


iberbar::Gui::CDialog::CDialog( UDialogStyle nStyle, CDialog* pDlgParent )
	: m_nDialogStyle( nStyle )
	, m_pDialogParent( pDlgParent )
	, m_pWidgetMouseOver( nullptr )
	, m_bMouseInput( true )
	, m_bKeyboardInput( true )
	, m_bNeedClip( true )
	, m_Widgets()
	, m_Dialogs()
{
}


iberbar::Gui::CDialog::CDialog(const CDialog& dialog )
	: CObject( dialog )
	, m_nDialogStyle( dialog.m_nDialogStyle )
	, m_pDialogParent( dialog.m_pDialogParent )
	, m_pWidgetMouseOver( nullptr )
	, m_bMouseInput( dialog.m_bMouseInput )
	, m_bKeyboardInput( dialog.m_bKeyboardInput )
	, m_bNeedClip( dialog.m_bNeedClip )
	, m_Widgets()
	, m_Dialogs()
{
}



iberbar::Gui::CDialog::~CDialog()
{
	RemoveWidgetsAll();
	RemoveDialogs();
}



void iberbar::Gui::CDialog::UpdateRect()
{
	for ( auto& pWidget : m_Widgets )
	{
		pWidget->UpdateRect();
	}
}



iberbar::Gui::UHandleMessageResult iberbar::Gui::CDialog::HandleMouse( const UMouseEventData* EventData )
{
	if (IsVisible() == false ||
		IsEnable() == false ||
		IsMouseInputEnabled() == false )
		return UHandleMessageResult::Ignore;

	//--------------------------------------
	// ·ÃÎÊº¢×Ó¿Ø¼þ
	if ( m_Widgets.empty() == false )
	{
		// 1. Handle focus control first
		CWidget* pWidgetFocus = CEngine::sGetInstance()->GetFocus();
		if ( pWidgetFocus != NULL &&
			pWidgetFocus->IsEnable() &&
			pWidgetFocus->IsVisible() &&
			pWidgetFocus->GetParent() == this )
		{
			if ( pWidgetFocus->HandleMouse( EventData ) == UHandleMessageResult::Succeed )
				return UHandleMessageResult::Succeed;
		}

		//  hit test
		CWidget* pWidgetActive = GetActiveWidgetAtPoint( EventData->MousePoint );
		if ( pWidgetActive )
		{
			if ( pWidgetActive->IsMouseInputEnabled() && pWidgetActive->HandleMouse( EventData ) == UHandleMessageResult::Succeed )
				return UHandleMessageResult::Succeed;
		}
		else
		{
			// Mouse not over any controls in this dialog, if there was a control
			// which had focus it just lost it
			if ( EventData->nMouseEvent == UMouseEvent::LDown )
			{
				if ( pWidgetFocus != nullptr &&
					pWidgetFocus->GetParent() == this )
				{
					CEngine::sGetInstance()->ClearFocus( true );
				}
			}
		}

		if ( EventData->nMouseEvent == UMouseEvent::Move )
		{
			// If the mouse is still over the same control, nothing needs to be done
			if ( pWidgetActive != m_pWidgetMouseOver )
			{
				// Handle mouse leaving the old control
				if ( m_pWidgetMouseOver )
					m_pWidgetMouseOver->OnMouseLeave();

				// Handle mouse entering the new control
				m_pWidgetMouseOver = pWidgetActive;
				if ( m_pWidgetMouseOver != NULL )
					m_pWidgetMouseOver->OnMouseEnter();
			}
		}
	}

	return UHandleMessageResult::Ignore;
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CDialog::HandleKeyboard( const UKeyboardEventData* pEvent )
{
	if ( IsVisible() == false ||
		IsEnable() == false ||
		IsMouseInputEnabled() == false )
		return UHandleMessageResult::Ignore;

	switch( pEvent->nEvent )
	{
	//case WM_KEYDOWN:
	//case WM_SYSKEYDOWN:
	//case WM_KEYUP:
	//case WM_SYSKEYUP:
		case UKeyboardEvent::KeyDown:
		case UKeyboardEvent::KeyUp:
		case UKeyboardEvent::Char:
		{
			// If a control is in focus, it belongs to this dialog, and it's enabled, then give
			// it the first chance at handling the message.
			CWidget* pWidgetFocus = CEngine::sGetInstance()->GetFocus();
			if ( pWidgetFocus &&
				pWidgetFocus->GetParent() == this &&
				pWidgetFocus->GetEnable() )
			{
				if ( pWidgetFocus->HandleKeyboard( pEvent ) == UHandleMessageResult::Succeed )
					return UHandleMessageResult::Succeed;
			}

			// Not yet handled, see if this matches a control's hotkey
			// Activate the hotkey if the focus doesn't belong to an
			// edit box.
			if ( pEvent->nEvent == UKeyboardEvent::KeyDown || pEvent->nEvent == UKeyboardEvent::Char )
			{
				auto iter = m_Widgets.rbegin();
				auto end = m_Widgets.rend();
				for ( ; iter != end; iter++ )
				{
					if ( (*iter)->HandleKeyboard( pEvent ) == UHandleMessageResult::Succeed )
						return UHandleMessageResult::Succeed;
				}
				//GuiWidgetVector lc_vector( this );
				//GuiWidgetVector::reserve_iterator lc_iter = lc_vector.rbegin();
				//GuiWidgetVector::reserve_iterator lc_end = lc_vector.rend();
				//for ( ; lc_iter != lc_end; ++ lc_iter )
				//{
				//	if ( (*lc_iter)->HandleKeyboard( input ) )
				//	{
				//		return GuiHandlMessageRet_Succeed;
				//	}
				//}
// 				for ( int i = 0; i < m_Controls.GetSize(); i++ )
// 				{
// 					CDXUTControl* pControl = m_Controls.GetAt( i );
// 					if ( pControl->GetHotkey() == wParam )
// 					{
// 						pControl->OnHotkey();
// 						return true;
// 					}
// 				}
			}
		}
		break;
	}

	return UHandleMessageResult::Ignore;
}



void iberbar::Gui::CDialog::Refresh()
{
	for ( auto& pWidget : m_Widgets )
	{
		pWidget->Refresh();
	}
}



void iberbar::Gui::CDialog::Update( float nElapsedTime )
{
	for ( auto& pWidget : m_Widgets )
	{
		pWidget->Update( nElapsedTime );
	}
}



void iberbar::Gui::CDialog::Render()
{
	if ( IsVisible() == false )
		return ;

	bool bPopViewport = false;
	if ( m_bNeedClip == true )
	{
		CEngine::sGetInstance()->GetViewportState()->AddViewport( m_pTransform->GetBounding() );
		bPopViewport = true;
	}

	for ( auto& pWidget : m_Widgets )
	{
		pWidget->Render();
	}

	if ( bPopViewport == true )
	{
		CEngine::sGetInstance()->GetViewportState()->PopViewport();
	}
}


void iberbar::Gui::CDialog::AddWidget( CWidget* pWidget )
{
	if ( pWidget == nullptr )
		return;

	if ( pWidget->GetParent() != nullptr )
		return;

	for ( auto& pWidgetTemp : m_Widgets )
	{
		if ( pWidgetTemp == pWidget )
			return;
	}

	m_Widgets.push_back( pWidget );
	pWidget->SetParent( this );
	pWidget->AddRef();
}


iberbar::Gui::CWidget* iberbar::Gui::CDialog::FindWidget( const char* strId )
{
	if ( StringIsNullOrEmpty( strId ) == true )
		return nullptr;

	for ( auto& pWidget : m_Widgets )
	{
		if ( strcmp( pWidget->GetId().c_str(), strId ) == 0 )
			return pWidget;
	}

	return nullptr;
}


void iberbar::Gui::CDialog::RemoveWidget( CWidget* pWidget )
{
	if ( pWidget == nullptr )
		return;

	auto iter = m_Widgets.begin();
	auto end = m_Widgets.end();
	for ( ; iter != end; iter++ )
	{
		if ( (*iter) == pWidget )
		{
			if ( CEngine::sGetInstance()->GetFocus() == pWidget )
			{
				CEngine::sGetInstance()->ClearFocus( true );
			}
			if ( m_pWidgetMouseOver == pWidget )
			{
				m_pWidgetMouseOver = nullptr;
			}

			pWidget->SetParent( nullptr );
			pWidget->Release();

			m_Widgets.erase( iter );

			return;
		}
	}
}


void iberbar::Gui::CDialog::RemoveWidgetsAll()
{
	if ( m_Widgets.empty() == false )
	{
		auto iter = m_Widgets.begin();
		auto end = m_Widgets.end();
		for ( ; iter != end; iter++ )
		{
			if ( (*iter) == CEngine::sGetInstance()->GetFocus() )
			{
				CEngine::sGetInstance()->ClearFocus( false );
			}

			if ( (*iter) == m_pWidgetMouseOver )
			{
				m_pWidgetMouseOver = nullptr;
			}

			(*iter)->SetParent( nullptr );

			UNKNOWN_SAFE_RELEASE_NULL( *iter );
		}
		m_Widgets.clear();
	}
}


void iberbar::Gui::CDialog::RequestTop()
{
	CEngine::sGetInstance()->AddCommand( new CCommand_RequestTop( this ) );
}


void iberbar::Gui::CDialog::ForeachWidgets( std::function<void( CWidget* )> Func )
{
	for ( auto& pWidget : m_Widgets )
	{
		Func( pWidget );
	}
}


iberbar::Gui::CWidget* iberbar::Gui::CDialog::GetActiveWidgetAtPoint( const CPoint2i& point )
{
	auto iter = m_Widgets.rbegin();
	auto end = m_Widgets.rend();
	CWidget* pWidget = nullptr;
	for ( ; iter != end; iter++ )
	{
		pWidget = (*iter);
		if ( pWidget->IsEnable() == true &&
			pWidget->IsVisible() == true &&
			pWidget->GetCanFocus() == true &&
			pWidget->HitTest( point ) == true )
		{
			return pWidget;
		}
	}

	return nullptr;
}


void iberbar::Gui::CDialog::AddDialog( CDialog* pDialog )
{
	if ( pDialog->m_nDialogStyle == UDialogStyle::Overlapped )
	{
		m_DialogsOverlapped.push_back( pDialog );
	}
	else
	{
		m_Dialogs.push_back( pDialog );
	}
	pDialog->AddRef();
}


void iberbar::Gui::CDialog::RemoveDialogs()
{
	if ( m_Dialogs.empty() == false )
	{
		for ( auto& pDialogTemp : m_Dialogs )
		{
			pDialogTemp->m_pDialogParent = nullptr;
			pDialogTemp->RemoveDialogs();
			pDialogTemp->Release();
		}
		m_Dialogs.clear();
	}

	if ( m_DialogsOverlapped.empty() == false )
	{
		for ( auto& pDialogTemp : m_DialogsOverlapped )
		{
			CEngine::sGetInstance()->RemoveDialog( pDialogTemp );
			pDialogTemp->m_pDialogParent = nullptr;
			pDialogTemp->RemoveDialogs();
			pDialogTemp->Release();
		}
		m_DialogsOverlapped.clear();
	}
}


iberbar::CResult iberbar::Gui::CDialog::sCreateDialog( CDialog** ppOutDlg, UDialogStyle nStyle, CDialog* pDlgParent )
{
	if ( nStyle == UDialogStyle::Child && pDlgParent == nullptr )
		return MakeResult( ResultCode::Bad, "the style(Child) of dialog " );

	CDialog* pDlg = new CDialog( nStyle, pDlgParent );

	if ( nStyle == UDialogStyle::Overlapped )
	{
		CEngine::sGetInstance()->AddDialog( pDlg );
	}

	if ( pDlgParent != nullptr )
	{
		pDlgParent->AddDialog( pDlg );
	}

	(*ppOutDlg) = pDlg;

	return CResult();
}


void iberbar::Gui::CDialog::sDestroyDialog( CDialog* pDialog )
{
	if ( pDialog == nullptr )
		return;

	pDialog->RemoveDialogs();

	if ( pDialog->m_nDialogStyle == UDialogStyle::Overlapped )
	{
		pDialog->m_pDialogParent = nullptr;

		CEngine::sGetInstance()->RemoveDialog( pDialog );
	}
	else if ( pDialog->m_nDialogStyle == UDialogStyle::Child )
	{
		if ( pDialog->m_pDialogParent->m_Dialogs.empty() == false )
		{
			auto iter = pDialog->m_pDialogParent->m_Dialogs.begin();
			auto end = pDialog->m_pDialogParent->m_Dialogs.end();
			for ( ; iter != end; iter++ )
			{
				if ( (*iter) == pDialog )
				{
					pDialog->m_pDialogParent->m_Dialogs.erase( iter );
					break;
				}
			}
		}

		pDialog->m_pDialogParent = nullptr;
	}
}










iberbar::Gui::CCommand_RequestTop::CCommand_RequestTop( CDialog* pDialog )
	: m_pDialog( pDialog )
{
	UNKNOWN_SAFE_ADDREF( m_pDialog );
}


iberbar::Gui::CCommand_RequestTop::~CCommand_RequestTop()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDialog );
}


void iberbar::Gui::CCommand_RequestTop::Execute()
{
	if ( m_pDialog == nullptr )
		return;

	CEngine::sGetInstance()->RequestTop( m_pDialog );
}






