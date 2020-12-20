

#include <iberbar/Gui/Widgets/CheckBox.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Utility/Input.h>



iberbar::Gui::CCheckBox::CCheckBox( void )
	: m_nCheckState( UCheckState::CheckedFalse )
	, m_pRenderElementCheckFalse( nullptr )
	, m_pRenderElementCheckTrue( nullptr )
	, m_pRenderElementCheckIndeterminate( nullptr )
{
}

iberbar::Gui::CCheckBox::CCheckBox( const CCheckBox& checkbox )
	: CButton( checkbox )
	, m_nCheckState( checkbox.m_nCheckState )
	, m_pRenderElementCheckFalse( nullptr )
	, m_pRenderElementCheckTrue( nullptr )
	, m_pRenderElementCheckIndeterminate( nullptr )
{
	if ( checkbox.m_pRenderElementCheckFalse )
	{
		this->SetRenderElementFalse( checkbox.m_pRenderElementCheckFalse->GetId().c_str() );
	}

	if ( checkbox.m_pRenderElementCheckTrue )
	{
		this->SetRenderElementTrue( checkbox.m_pRenderElementCheckTrue->GetId().c_str() );
	}

	if ( checkbox.m_pRenderElementCheckIndeterminate )
	{
		this->SetRenderElementIndeterminate( checkbox.m_pRenderElementCheckIndeterminate->GetId().c_str() );
	}
}


iberbar::Gui::CCheckBox* iberbar::Gui::CCheckBox::Clone() const
{
	return new CCheckBox( *this );
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CCheckBox::HandleMouse( const UMouseEventData* EventData )
{
	if ( IsEnable() == false ||
		IsVisible() == false )
		return UHandleMessageResult::Ignore;

	switch ( EventData->nMouseEvent )
	{
		case UMouseEvent::LDoubleClick:
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
			break;
		} // end case
		

		case UMouseEvent::LUp:
		{
			if ( m_bPressed == true )
			{
				m_bPressed = false;

				if ( HitTest( EventData->MousePoint ) == true )
				{
					if ( m_nCheckState == UCheckState::CheckedFalse || m_nCheckState == UCheckState::CheckedIndeterminate )
						SetCheckedInternal( UCheckState::CheckedTrue, true );
					else
						SetCheckedInternal( UCheckState::CheckedFalse, true );
				}

				return UHandleMessageResult::Succeed;
			} // end if pressed
			break;
		} // end case
	
		default:break;
	}

	return UHandleMessageResult::Ignore;
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CCheckBox::HandleKeyboard( const UKeyboardEventData* EventData )
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
				if ( m_nCheckState == UCheckState::CheckedFalse || m_nCheckState == UCheckState::CheckedIndeterminate )
					SetCheckedInternal( UCheckState::CheckedTrue, true );
				else
					SetCheckedInternal( UCheckState::CheckedFalse, true );
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


void iberbar::Gui::CCheckBox::SetCheckedInternal( UCheckState nState, bool bInternal )
{
	if ( nState != m_nCheckState )
	{
		m_nCheckState = nState;

		if ( bInternal )
			SendEvent( BaseEvent::nValueChanged );
	}
}


//-----------------------------------------------------------------------------------------
void iberbar::Gui::CCheckBox::Update( float nElapsedTime )
{
	UWidgetState lc_stateOrdi = UWidgetState::Normal;
	if ( IsVisible() == false )
		lc_stateOrdi = UWidgetState::Hidden;
	else if ( IsEnable() == false )
		lc_stateOrdi = UWidgetState::Disabled;

	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->SetState( (int)lc_stateOrdi );

	UWidgetState lc_state = UWidgetState::Normal;
	if ( IsVisible() == false )
		lc_state = UWidgetState::Hidden;
	else if ( IsEnable() == false )
		lc_state = UWidgetState::Disabled;
	else if ( IsPressed() == true )
		lc_state = UWidgetState::Pressed;
	else if ( GetFocus() == true )
		lc_state = UWidgetState::Focus;
	else if ( GetMouseOver() == true )
		lc_state = UWidgetState::MouseOver;

	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->SetState( (int)lc_state );

	

	UWidgetState nStateCheck;

	if ( m_pRenderElementCheckFalse != nullptr )
	{
		nStateCheck = lc_state;
		if ( GetChecked() != UCheckState::CheckedFalse )
			nStateCheck = UWidgetState::Hidden;
		m_pRenderElementCheckFalse->SetState( (int)nStateCheck );
	}
		
	if ( m_pRenderElementCheckTrue != nullptr )
	{
		nStateCheck = lc_state;
		if ( GetChecked() != UCheckState::CheckedTrue )
			nStateCheck = UWidgetState::Hidden;
		else
			int s = 0;
		m_pRenderElementCheckTrue->SetState( (int)nStateCheck );
	}

	if ( m_pRenderElementCheckIndeterminate != nullptr )
	{
		nStateCheck = lc_state;
		if ( GetChecked() != UCheckState::CheckedIndeterminate )
			nStateCheck = UWidgetState::Hidden;
		m_pRenderElementCheckIndeterminate->SetState( (int)nStateCheck );
	}

	CWidget::Update( nElapsedTime );
}


void iberbar::Gui::CCheckBox::SetRenderElementFalse( const char* strElementId )
{
	if ( m_pRenderElementDefault == nullptr )
	{
		return;
	}

	if ( m_pRenderElementDefault->FindElement( strElementId, &m_pRenderElementCheckFalse ) == false )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementCheckFalse );
	}
	else
	{
		m_pRenderElementCheckFalse->GetTransform()->SetParentTransform( this->GetTransform() );
	}		
}


void iberbar::Gui::CCheckBox::SetRenderElementTrue( const char* strElementId )
{
	if ( m_pRenderElementDefault == nullptr )
	{
		return;
	}

	if ( m_pRenderElementDefault->FindElement( strElementId, &m_pRenderElementCheckTrue ) == false )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementCheckTrue );
	}
	else
	{
		m_pRenderElementCheckTrue->GetTransform()->SetParentTransform( this->GetTransform() );
	}
}


void iberbar::Gui::CCheckBox::SetRenderElementIndeterminate( const char* strElementId )
{
	if ( m_pRenderElementDefault == nullptr )
	{
		return;
	}

	if ( m_pRenderElementDefault->FindElement( strElementId, &m_pRenderElementCheckIndeterminate ) == false )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementCheckIndeterminate );
	}
	else
	{
		m_pRenderElementCheckIndeterminate->GetTransform()->SetParentTransform( this->GetTransform() );
	}
}
