
#include <iberbar/Utility/Input.h>
#ifdef _WINDOWS
#include <XInput.h>
#endif


iberbar::CInput::CInput()
	: m_MouseEvents()
	, m_KeyboardEvents()
#if defined ( _WIN32 )
	, m_MousePoint()
#endif
{
}


#if defined ( _WIN32 )
void iberbar::CInput::HandleWndProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( nMsg )
	{
		case WM_MOUSEMOVE:
		{
			m_MousePoint.x = LOWORD( lParam );
			m_MousePoint.y = HIWORD( lParam );
			UMouseEventData EventData;
			EventData.nMouseEvent = UMouseEvent::Move;
			EventData.nMouseWheel = 0;
			EventData.nMouseId = 0;
			EventData.MousePoint = m_MousePoint;
			DoMouseEvent( &EventData );
			break;
		}

		case WM_LBUTTONDOWN:
		{
			UMouseEventData EventData;
			EventData.nMouseEvent = UMouseEvent::LDown;
			EventData.MousePoint = m_MousePoint;
			DoMouseEvent( &EventData );
			break;
		}

		case WM_LBUTTONUP:
		{
			UMouseEventData EventData;
			EventData.nMouseEvent = UMouseEvent::LUp;
			EventData.MousePoint = m_MousePoint;
			DoMouseEvent( &EventData );
			break;
		}

		case WM_LBUTTONDBLCLK:
		{
			UMouseEventData EventData;
			EventData.nMouseEvent = UMouseEvent::LDoubleClick;
			EventData.MousePoint = m_MousePoint;
			DoMouseEvent( &EventData );
			break;
		}

		case WM_RBUTTONDOWN:
		{
			UMouseEventData EventData;
			EventData.nMouseEvent = UMouseEvent::RDown;
			EventData.MousePoint = m_MousePoint;
			DoMouseEvent( &EventData );
			break;
		}

		case WM_RBUTTONUP:
		{
			UMouseEventData EventData;
			EventData.nMouseEvent = UMouseEvent::RUp;
			EventData.MousePoint = m_MousePoint;
			DoMouseEvent( &EventData );
			break;
		}

		case WM_MOUSEWHEEL:
		{
			int nScrollAmount = int( (short)HIWORD( wParam ) ) / WHEEL_DELTA;
			UMouseEventData EventData;
			EventData.nMouseEvent = UMouseEvent::MouseWheel;
			EventData.nMouseWheel = nScrollAmount;
			EventData.MousePoint = m_MousePoint;
			DoMouseEvent( &EventData );
		}
		break;

		case WM_KEYDOWN:
		{
			UKeyboardEventData EventData;
			EventData.nEvent = UKeyboardEvent::KeyDown;
			EventData.nKeyCode = (int)wParam;
			DoKeyboardEvent( &EventData );
			break;
		}

		case WM_KEYUP:
		{
			UKeyboardEventData EventData;
			EventData.nEvent = UKeyboardEvent::KeyUp;
			EventData.nKeyCode = (int)wParam;
			DoKeyboardEvent( &EventData );
			break;
		}

		case WM_CHAR:
		{
			UKeyboardEventData EventData;
			EventData.nEvent = UKeyboardEvent::Char;
			EventData.nKeyCode = (int)wParam;
			DoKeyboardEvent( &EventData );
			break;
		}

		case WM_CAPTURECHANGED:
		{
			break;
		}

		default:
			break;
	}
}
#endif


void iberbar::CInput::DoMouseEvent( const UMouseEventData* EventData )
{
	auto iter = m_MouseEvents.begin();
	auto end = m_MouseEvents.end();
	for ( ; iter != end; iter++ )
	{
		(*iter)(EventData);
	}
}


void iberbar::CInput::DoKeyboardEvent( const UKeyboardEventData* EventData )
{
	auto iter = m_KeyboardEvents.begin();
	auto end = m_KeyboardEvents.end();
	for ( ; iter != end; iter++ )
	{
		(*iter)(EventData);
	}
}
