
//*====================================================================
//*
//*           DirectInput
//*
//*====================================================================


#include "iberbar\Base\Input\DXInput.h"

#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )  // DXInput

#define COM_SAFE_RELEASE( com )      { if((com)) { (com)->Release(); (com)=NULL; } }
#define COM_SAFE_ADDREF( com )       { if((com)) { (com)->AddRef(); } }


LPDIRECTINPUT8 iberbar::CDXInput::s_pd3dInput = NULL;

iberbar::CDXInput::CDXInput( void )
	: m_pKeyboardDevice( NULL )
	, m_pMouseDevice( NULL )
{
	ZeroMemory( m_KeyboardBuf, sizeof( CHAR ) * 256 );
	ZeroMemory( &m_MouseState, sizeof( DIMOUSESTATE ) );
}

iberbar::CDXInput::~CDXInput()
{
	Release();
}

bool iberbar::CDXInput::CreateInput( HINSTANCE hInstance, HWND hWnd, INT iMin, INT iMax, INT iDeadZone )
{
	if ( !hInstance || !hWnd )
		return false;

	if ( CreateDirectInput( hInstance ) == false )
		return false;

	if ( CreateKeyboard( hWnd ) == false )
		return false;

	return CreateMouse( hWnd );
}

bool iberbar::CDXInput::CreateDirectInput( HINSTANCE hInstance )
{
	if ( FAILED( DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&s_pd3dInput, NULL ) ) )
	{
		MessageBox( NULL, TEXT( "建立DirectInput8接口对象失败" ), TEXT( "warning" ), MB_OK|MB_ICONINFORMATION );
		return false;
	}
	return true;
}

bool iberbar::CDXInput::CreateKeyboard( HWND hWnd )
{
	if ( FAILED( s_pd3dInput->CreateDevice( GUID_SysKeyboard, &m_pKeyboardDevice, NULL ) ) )
	{
		Release();
		return false;
	}

	if ( FAILED( m_pKeyboardDevice->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
		Release();
		return false;
	}

	if ( FAILED( m_pKeyboardDevice->SetCooperativeLevel( hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE ) ) )
	{
		Release();
		return false;
	}

	if ( FAILED( m_pKeyboardDevice->Acquire() ) )
	{
		Release();
		return false;
	}

	ZeroMemory( m_KeyboardBuf, sizeof( CHAR ) * 256 );
	return true;
}

bool iberbar::CDXInput::CreateMouse( HWND hWnd )
{
	if ( FAILED( s_pd3dInput->CreateDevice( GUID_SysMouse, &m_pMouseDevice, NULL ) ) )
	{
		Release();
		return false;
	}

	if ( FAILED( m_pMouseDevice->SetDataFormat( &c_dfDIMouse ) ) )
	{
		Release();
		return false;
	}

	if ( FAILED( m_pMouseDevice->SetCooperativeLevel( hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE ) ) )
	{
		Release();
		return false;
	}

	DIPROPDWORD lc_dipRopword;
	lc_dipRopword.diph.dwSize = sizeof( DIPROPDWORD );
	lc_dipRopword.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	lc_dipRopword.diph.dwObj = 0;
	lc_dipRopword.diph.dwHow = DIPH_DEVICE;
	lc_dipRopword.dwData = 10;

	if ( FAILED( m_pMouseDevice->SetProperty( DIPROP_BUFFERSIZE, &lc_dipRopword.diph ) ) )
	{
		Release();
		return false;
	}

	if ( FAILED( m_pMouseDevice->Acquire() ) )
	{
		Release();
		return false;
	}

	return true;
}

bool iberbar::CDXInput::ReadKeyboard()
{
	if ( !m_pKeyboardDevice )
		return false;

	if ( DIERR_INPUTLOST == m_pKeyboardDevice->GetDeviceState( sizeof( m_KeyboardBuf ), (PVOID)m_KeyboardBuf ) )
	{
		m_pKeyboardDevice->Acquire();

		if ( FAILED( m_pKeyboardDevice->GetDeviceState( sizeof( m_KeyboardBuf ), (PVOID)m_KeyboardBuf ) ) )
			return false;
	}

	return true;
}

bool iberbar::CDXInput::ReadMouse()
{
	if ( !m_pMouseDevice )
		return false;

	ZeroMemory( &m_MouseState, sizeof( DIMOUSESTATE ) );
	if ( m_pMouseDevice->GetDeviceState( sizeof( DIMOUSESTATE ), (LPVOID)&m_MouseState ) == DIERR_INPUTLOST )
	{
		m_pMouseDevice->Acquire();
		if ( FAILED( m_pMouseDevice->GetDeviceState( sizeof( DIMOUSESTATE ), (LPVOID)&m_MouseState ) ) )
			return false;
	}

	return true;
}

void iberbar::CDXInput::Release()
{
	if ( m_pKeyboardDevice )
		m_pKeyboardDevice->Unacquire();
	if ( m_pMouseDevice )
		m_pMouseDevice->Unacquire();

	COM_SAFE_RELEASE( m_pKeyboardDevice );
	COM_SAFE_RELEASE( m_pMouseDevice );
	COM_SAFE_RELEASE( s_pd3dInput );
}

