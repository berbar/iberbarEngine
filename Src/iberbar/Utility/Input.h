#pragma once

#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Point.h>
#include <functional>

namespace iberbar
{
	enum class UMouseEvent
		: uint32
	{
		Move,
		LDown,
		LUp,
		LDoubleClick,
#if defined ( _WIN32 )
		RDown,
		RUp,
		RDoubleClick,
		MDown,
		MUp,
		MouseWheel
#endif
	};

	enum UKeyboardEvent
	{
		KeyDown,
		KeyUp,
		Char,
	};

	struct UMouseEventData
	{
		UMouseEvent nMouseEvent;
		int nMouseId;
		int nMouseWheel;
		CPoint2i MousePoint;
	};

	struct UKeyboardEventData
	{
		UKeyboardEvent nEvent;
		int nKeyCode;
	};

	class __iberbarUtilityApi__ CInput
	{
	public:
		typedef void UCallackMouseEvent( const UMouseEventData* EventData );
		typedef void UCallbackKeyboardEvent( const UKeyboardEventData* EventData );

	public:
		CInput();

		void AddMouseEvent( std::function<UCallackMouseEvent> func ) { m_MouseEvents.push_back( func ); }
		void AddKeyboardEvent( std::function<UCallbackKeyboardEvent> func ) { m_KeyboardEvents.push_back( func ); }

#if defined ( _WIN32 )
		void HandleWndProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef __ANDROID__
#endif

	private:
		void DoMouseEvent( const UMouseEventData* EventData );
		void DoKeyboardEvent( const UKeyboardEventData* EventData );

	private:
		std::vector< std::function<UCallackMouseEvent> > m_MouseEvents;
		std::vector< std::function< UCallbackKeyboardEvent> > m_KeyboardEvents;

#if defined (_WIN32)
		CPoint2i m_MousePoint;
#endif

#if defined (__ANDROID__)
#endif
	};
}

