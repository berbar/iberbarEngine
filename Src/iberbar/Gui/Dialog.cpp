
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


iberbar::Gui::CDialog::CDialog( CDialog* pDlgParent )
	: m_pDialogParent( pDlgParent )
	, m_pContainer( nullptr )
	, m_bMouseInput( true )
	, m_bKeyboardInput( true )
	, m_bNeedClip( false )
	, m_bModal( false )
{
	if ( m_pDialogParent )
		m_pTransform->SetParentTransform( m_pDialogParent->GetTransform() );
	PTR_CWidget pContainer = PTR_CWidget::_sNew();
	pContainer->SetSize( 100, 100 );
	pContainer->SetPercentW( true );
	pContainer->SetPercentH( true );
	SetWidgetRoot( pContainer );
}


iberbar::Gui::CDialog::CDialog(const CDialog& dialog )
	: CObject( dialog )
	, m_pDialogParent( dialog.m_pDialogParent )
	, m_pContainer( nullptr )
	, m_bMouseInput( dialog.m_bMouseInput )
	, m_bKeyboardInput( dialog.m_bKeyboardInput )
	, m_bNeedClip( dialog.m_bNeedClip )
	, m_bModal( dialog.m_bModal )
{
	if ( dialog.m_pDialogParent )
		m_pTransform->SetParentTransform( dialog.m_pDialogParent->GetTransform() );
	PTR_CWidget pContainerClone = PTR_CWidget::_sClone( dialog.m_pContainer );
	SetWidgetRoot( pContainerClone );
}



iberbar::Gui::CDialog::~CDialog()
{
	RemoveContainer();
}



void iberbar::Gui::CDialog::UpdateRect()
{
	m_pContainer->UpdateRect();
}



iberbar::Gui::UHandleMessageResult iberbar::Gui::CDialog::HandleMouse( const UMouseEventData* EventData )
{
	if (IsVisible() == false ||
		IsEnable() == false ||
		IsMouseInputEnabled() == false )
		return UHandleMessageResult::Ignore;

	if ( EventData->nMouseEvent == UMouseEvent::Move )
	{
		HandleMouseMoveInContainer( m_pContainer, EventData );
	}

	// 优先处理焦点控件
	CWidget* pWidgetFocus = CEngine::sGetInstance()->GetFocus();
	if ( pWidgetFocus != NULL &&
		pWidgetFocus->IsEnable() &&
		pWidgetFocus->IsVisible() &&
		pWidgetFocus->GetDialog() == this )
	{
		if ( pWidgetFocus->HandleMouse( EventData ) == UHandleMessageResult::Succeed )
			return UHandleMessageResult::Succeed;
	}

	if ( EventData->nMouseEvent != UMouseEvent::Move )
	{
		if ( HandleMouseInContainer( m_pContainer, EventData ) == true )
			return UHandleMessageResult::Succeed;
	}

	if ( ( EventData->nMouseEvent == UMouseEvent::LDown ||
			EventData->nMouseEvent == UMouseEvent::LDoubleClick ||
			EventData->nMouseEvent == UMouseEvent::RDown ||
			EventData->nMouseEvent == UMouseEvent::RDoubleClick ) &&
		HitTest( EventData->MousePoint ) )
	{
		if ( pWidgetFocus != NULL &&
			pWidgetFocus->GetDialog() == this )
		{
			CEngine::sGetInstance()->ClearFocus( true );
		}
		RequestTop();
		return UHandleMessageResult::Succeed;
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
				pWidgetFocus->GetDialog() == this &&
				pWidgetFocus->IsEnable() &&
				pWidgetFocus->IsMouseInputEnabled() )
			{
				if ( pWidgetFocus->HandleKeyboard( pEvent ) == UHandleMessageResult::Succeed )
					return UHandleMessageResult::Succeed;
			}

			// Not yet handled, see if this matches a control's hotkey
			// Activate the hotkey if the focus doesn't belong to an
			// edit box.
			if ( pEvent->nEvent == UKeyboardEvent::KeyDown || pEvent->nEvent == UKeyboardEvent::Char )
			{
				HandleKeyboardInContainer( m_pContainer, pEvent );
			}
		}
		break;
	}

	return UHandleMessageResult::Ignore;
}


void iberbar::Gui::CDialog::Refresh()
{
	m_pContainer->Refresh();
}



void iberbar::Gui::CDialog::Update( float nElapsedTime )
{
	m_pContainer->Update( nElapsedTime );
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

	m_pContainer->Render();

	if ( bPopViewport == true )
	{
		CEngine::sGetInstance()->GetViewportState()->PopViewport();
	}
}


void iberbar::Gui::CDialog::RequestTop()
{
	CEngine::sGetInstance()->AddCommand( new CCommand_RequestTop( this ) );
}


void iberbar::Gui::CDialog::SetWidgetRoot( CWidget* pContainer )
{
	assert( pContainer );
	assert( pContainer->GetDialog() == nullptr );

	if ( pContainer == m_pContainer )
		return;

	RemoveContainer();

	m_pContainer = pContainer;
	m_pContainer->SetDialog( this );
	m_pContainer->GetTransform()->SetParentTransform( this->GetTransform() );
	m_pContainer->AddRef();
}


void iberbar::Gui::CDialog::HandleMouseMoveInContainer( CWidget* pContainer, const UMouseEventData* pEventData )
{
	if ( pContainer->IsVisible() == false ||
		pContainer->IsEnable() == false ||
		pContainer->IsMouseInputEnabled() == false )
		return;

	int nWidgetCount = pContainer->GetWidgetCount();
	if ( nWidgetCount == 0 )
		return;

	CWidget* pWidgetTemp = nullptr;
	for ( int i = nWidgetCount-1; i >= 0; i -- )
	{
		pWidgetTemp = pContainer->GetWidgetAt( i );

		HandleMouseMoveInContainer( pWidgetTemp, pEventData );

		if ( pWidgetTemp->IsVisible() &&
			pWidgetTemp->IsEnable() &&
			pWidgetTemp->IsMouseInputEnabled() &&
			pWidgetTemp->GetCanFocus() )
		{
			if ( pWidgetTemp->HitTest( pEventData->MousePoint ) )
			{
				if ( pWidgetTemp->GetMouseOver() == false )
					pWidgetTemp->OnMouseEnter();

				pWidgetTemp->HandleMouse( pEventData );
			}
			else
			{
				if ( pWidgetTemp->GetMouseOver() == true )
					pWidgetTemp->OnMouseLeave();
			}
		}
	}
}


bool iberbar::Gui::CDialog::HandleMouseInContainer( CWidget* pContainer, const UMouseEventData* pEventData )
{
	if ( pContainer->IsVisible() == false ||
		pContainer->IsEnable() == false ||
		pContainer->IsMouseInputEnabled() == false )
		return false;

	int nWidgetCount = pContainer->GetWidgetCount();
	if ( nWidgetCount == 0 )
		return false;

	CWidget* pWidgetTemp = nullptr;
	for ( int i = nWidgetCount - 1; i >= 0; i-- )
	{
		pWidgetTemp = pContainer->GetWidgetAt( i );

		// 先访问子控件
		if ( HandleMouseInContainer( pWidgetTemp, pEventData ) == true )
			return true;

		// 再访问自己
		if ( pWidgetTemp->IsVisible() &&
			pWidgetTemp->IsEnable() &&
			pWidgetTemp->IsMouseInputEnabled() &&
			pWidgetTemp->GetCanFocus() )
		{
			if ( pWidgetTemp->GetMouseOver() == true &&
				pWidgetTemp->HandleMouse( pEventData ) == UHandleMessageResult::Succeed )
				return true;
		}
	}

	return false;
}


bool iberbar::Gui::CDialog::HandleKeyboardInContainer( CWidget* pContainer, const UKeyboardEventData* pEventData )
{
	if ( pContainer->IsVisible() == false ||
		pContainer->IsEnable() == false ||
		pContainer->IsKeyboardInputEnabled() == false )
		return false;

	int nWidgetCount = pContainer->GetWidgetCount();
	if ( nWidgetCount == 0 )
		return false;

	CWidget* pWidgetTemp = nullptr;
	for ( int i = nWidgetCount - 1; i >= 0; i-- )
	{
		pWidgetTemp = pContainer->GetWidgetAt( i );

		// 先访问子控件
		if ( HandleKeyboardInContainer( pWidgetTemp, pEventData ) == true )
			return true;

		// 再访问自己
		if ( pWidgetTemp->IsVisible() &&
			pWidgetTemp->IsEnable() &&
			pWidgetTemp->IsKeyboardInputEnabled() &&
			pWidgetTemp->GetCanFocus() )
		{
			if ( pWidgetTemp->HandleKeyboard( pEventData ) == UHandleMessageResult::Succeed )
				return true;
		}
	}

	return false;
}

//
//void iberbar::Gui::CDialog::AddDialog( CDialog* pDialog )
//{
//	assert( pDialog );
//	m_Dialogs.push_back( pDialog );
//	pDialog->AddRef();
//}
//
//
//void iberbar::Gui::CDialog::RemoveDialogs()
//{
//	//if ( m_Dialogs.empty() == false )
//	//{
//	//	for ( auto& pDialogTemp : m_Dialogs )
//	//	{
//	//		pDialogTemp->m_pDialogParent = nullptr;
//	//		pDialogTemp->RemoveDialogs();
//	//		pDialogTemp->Release();
//	//	}
//	//	m_Dialogs.clear();
//	//}
//
//	if ( m_Dialogs.empty() == false )
//	{
//		for ( CDialog* pDialogTemp : m_Dialogs )
//		{
//			CEngine::sGetInstance()->RemoveDialog( pDialogTemp );
//			pDialogTemp->m_pDialogParent = nullptr;
//			pDialogTemp->RemoveDialogs();
//			pDialogTemp->Release();
//		}
//		m_Dialogs.clear();
//	}
//}


void iberbar::Gui::CDialog::RemoveContainer()
{
	if ( m_pContainer == nullptr )
		return;

	m_pContainer->SetDialog( nullptr );
	m_pContainer->GetTransform()->SetParentTransform( nullptr );
	m_pContainer->Release();
	m_pContainer = nullptr;
}


iberbar::CResult iberbar::Gui::CDialog::sCreateDialog( CDialog** ppOutDlg, CDialog* pDlgParent )
{
	CDialog* pDlg = new CDialog( pDlgParent );

	CEngine::sGetInstance()->AddDialog( pDlg );

	(*ppOutDlg) = pDlg;

	return CResult();
}


void iberbar::Gui::CDialog::sDestroyDialog( CDialog* pDialog )
{
	if ( pDialog == nullptr )
		return;

	pDialog->m_pDialogParent = nullptr;

	CEngine::sGetInstance()->RemoveDialog( pDialog );
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






