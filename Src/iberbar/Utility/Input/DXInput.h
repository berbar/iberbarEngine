

#pragma once



//====================================================================
//  DirectInput
//====================================================================

#ifndef _BDIRECT_INPUT
#define _BDIRECT_INPUT

#include <dinput.h>

namespace iberbar
{

#define BINPUT_LBTN 0
#define BINPUT_RBTN 1
#define BINPUT_MBTN 2

	typedef void ( CALLBACK*PCallBackInputEvent )( bool bClicked, void* pParam, void* pUserContext );

	class CDXInput
	{
	protected:
		IDirectInputDevice8* m_pKeyboardDevice;
		IDirectInputDevice8* m_pMouseDevice;
		CHAR                 m_KeyboardBuf[ 256 ];
		DIMOUSESTATE         m_MouseState;
		LPVOID               m_pAnotherData;

	public:
		bool CreateInput( HINSTANCE hInstance, HWND hWnd, INT iMin=-100, INT iMax=100, INT iDeadZone=20 );
		bool ReadKeyboard();
		bool ReadMouse();

	public:
		inline const CHAR* GetKeyboardBufPtr()              const  { return m_KeyboardBuf; }

		inline bool    IsKeyPressed( INT iKey )  const { return ( ( m_KeyboardBuf[ iKey ] & 0x80 ) ? true : false ); }
		inline LONG    GetMouseMoveX()           const { return m_MouseState.lX; }
		inline LONG    GetMouseMoveY()           const { return m_MouseState.lY; }
		inline LONG    GetMouseMoveZ()           const { return m_MouseState.lZ; }

		bool IsLButtonPressed()                const { return ( ( m_MouseState.rgbButtons[ 0 ] & 0x80 ) ? true : false ); }
		bool IsRButtonPressed()                const { return ( ( m_MouseState.rgbButtons[ 1 ] & 0x80 ) ? true : false ); }
		bool IsMButtonPressed()                const { return ( ( m_MouseState.rgbButtons[ 2 ] & 0x80 ) ? true : false ); }
		bool IsButtonPressed( BYTE btIndex )   const { if ( btIndex > 2 ) return false; return ( ( m_MouseState.rgbButtons[ btIndex ] & 0x80 ) ? true : false ); }

	public:
		void    Release();

	protected:
		bool CreateDirectInput( HINSTANCE hInstance );
		bool CreateKeyboard( HWND hWnd );
		bool CreateMouse( HWND hWnd );

	private:
		static LPDIRECTINPUT8  s_pd3dInput;

	public:
		CDXInput( void );
		~CDXInput();
	};

	typedef CDXInput *LPBDIRECTINPUT;
}

#endif