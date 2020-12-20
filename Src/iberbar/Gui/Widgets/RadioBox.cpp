
#include <iberbar/Gui/Widgets/RadioBox.h>
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Utility/Input.h>



iberbar::Gui::CRadioBox::CRadioBox( void )
{
}


iberbar::Gui::CRadioBox::CRadioBox( const CRadioBox& radiobox )
	: CCheckBox( radiobox )
{
}


iberbar::Gui::CRadioBox* iberbar::Gui::CRadioBox::Clone() const
{
	return new CRadioBox( *this );
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CRadioBox::HandleMouse( const UMouseEventData* EventData )
{
	if ( IsEnable() == false ||
		IsVisible() == false )
		return UHandleMessageResult::Ignore;

	switch ( EventData->nMouseEvent )
	{
		case UMouseEvent::LDoubleClick:
		{
			break;
		}
		case UMouseEvent::LDown:
		{
			if ( HitTest( EventData->MousePoint ) == true )
			{
				if ( m_bFocus == false )
				{
					RequestFocus();
				}

				m_bPressed = true;

				return UHandleMessageResult::Succeed;
			}
		} // end case
		break;

		case UMouseEvent::LUp:
		{
			if ( m_bPressed == true )
			{
				m_bPressed = false;

				if ( HitTest( EventData->MousePoint ) == true )
				{
					SetCheckedInternal( UCheckState::CheckedTrue, true );
				}

				return UHandleMessageResult::Succeed;
			} // end if pressed
		} // end case
		break;

		default:break;
	}

	return UHandleMessageResult::Ignore;
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CRadioBox::HandleKeyboard( const UKeyboardEventData* EventData )
{
	if ( IsEnable() == false ||
		IsVisible() == false )
		return UHandleMessageResult::Ignore;

#if defined(WIN32)
	switch ( EventData->nEvent )
	{
		case UKeyboardEvent::KeyDown:
		{
			if ( m_nHotKey == EventData->nKeyCode )
			{
				RequestFocus();
				SetCheckedInternal( UCheckState::CheckedTrue, true );
				return UHandleMessageResult::Succeed;
			}
		}
		break;

		case UKeyboardEvent::KeyUp:
			break;

		default:
			break;
	}
#endif

	return UHandleMessageResult::Ignore;
}


void iberbar::Gui::CRadioBox::SetCheckedInternal( UCheckState nState, bool bInternal )
{
	if ( m_nCheckState == UCheckState::CheckedIndeterminate )
		return ;

	bool lc_bSendEvent = false;
	if ( m_nCheckState != nState )
		lc_bSendEvent = true;

	if ( nState == UCheckState::CheckedTrue )
	{
		if ( GetGroup() != 0 )
			ResetRadioGroupChecked();
	}

	m_nCheckState = nState;

	if ( bInternal == true && lc_bSendEvent == true )
	{
		SendEvent( BaseEvent::nValueChanged );
	} // end if
}


void iberbar::Gui::CRadioBox::ResetRadioGroupChecked()
{
	int nGroup = GetGroup();
	m_pDialog->ForeachWidgets(
		[nGroup]( CWidget* pWidget )
		{
			if ( pWidget->GetGroup() == nGroup && strcmp( pWidget->GetWidgetType(), "RadioBox" ) == 0 )
			{
				CRadioBox* pRadioBox = (CRadioBox*)pWidget;
				pRadioBox->SetCheckedInternal( UCheckState::CheckedFalse, false );
			}
		}
	);
}


// void lily::CRadioBox::setNoneGroup()
// {
// 	if ( this->getPrevRadioInGroup() )
// 		this->getPrevRadioInGroup()->m_pNextRadio = this->getNextRadioInGroup();
// 
// 	if ( this->getNextRadioInGroup() )
// 		this->getNextRadioInGroup()->m_pPrevRadio = this->getPrevRadioInGroup();
// 
// 	m_pPrevRadio = NULL;
// 	m_pNextRadio = NULL;
// }
// 
// 
// void lily::CRadioBox::setNextRadioInGroup( CRadioBox* pNext )
// {
// 	if ( pNext )
// 	{
// 		// alone
// 		pNext->setNoneGroup();
// 
// 		// connect
// 		CRadioBox* lc_pTemp = NULL;
// 		if ( m_pNextRadio )
// 			lc_pTemp = m_pNextRadio;
// 		m_pNextRadio = pNext;
// 		pNext->m_pPrevRadio = this;
// 		pNext->m_pNextRadio = lc_pTemp;
// 		if ( lc_pTemp )
// 			lc_pTemp->m_pPrevRadio = pNext;
// 	}
// }
