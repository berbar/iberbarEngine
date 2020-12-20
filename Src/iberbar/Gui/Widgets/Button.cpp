
#include <iberbar/Gui/Widgets/Button.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Utility/Input.h>




iberbar::Gui::CButton::CButton()
	: m_nHotKey( 0 )
	, m_bPressed( false )
{
	m_bCanFocus = true;
}



iberbar::Gui::CButton::CButton( const CButton& btn )
	: CWidget( btn )
	, m_nHotKey( btn.m_nHotKey )
	, m_bPressed( false )
{
	m_bCanFocus = true;
}


iberbar::Gui::CButton* iberbar::Gui::CButton::Clone() const
{
	return new CButton( *this );
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CButton::HandleMouse( const UMouseEventData* EventData )
{
	if (IsEnable() == false ||
		IsVisible() == false )
		return UHandleMessageResult::Ignore;

	switch ( EventData->nMouseEvent )
	{
		//case UMouseEvent::LDoubleClick:
		//{
		//	break;
		//}

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
		} // end case
		break;

		case UMouseEvent::LUp:
		{
			if ( m_bPressed == true )
			{
				m_bPressed = false;
				if ( HitTest( EventData->MousePoint ) == true )
				{
					SendEvent( BaseEvent::nClicked );
				}

				return UHandleMessageResult::Succeed;
			} // end if pressed

			m_bPressed = false;
		} // end case
		break;

		default:break;
	}

	return UHandleMessageResult::Ignore;
}



iberbar::Gui::UHandleMessageResult iberbar::Gui::CButton::HandleKeyboard( const UKeyboardEventData* EventData )
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
				SendEvent( BaseEvent::nClicked );
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


void iberbar::Gui::CButton::Update( float nElapsedTime )
{
	UWidgetState nState = UWidgetState::Normal;
	if ( IsVisible() == false )
		nState = UWidgetState::Hidden;
	else if ( IsEnable() == false )
		nState = UWidgetState::Disabled;
	else if ( IsPressed() == true )
		nState = UWidgetState::Pressed;
	else if ( GetFocus() == true )
		nState = UWidgetState::Focus;
	else if ( GetMouseOver() == true )
		nState = UWidgetState::MouseOver;

	m_pRenderElementDefault->SetState( (int)nState );

	CWidget::Update( nElapsedTime );
}





