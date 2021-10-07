#ifdef _WINDOWS
#include <iberbar/GameEngine/Application.h>
#include <iberbar/GameEngine/LoadingThread.h>
#include <iberbar/Network/ApiTypeDef.h>
#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/Trace.h>
#include <iberbar/RHI/Resource.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Input.h>
#include <iberbar/Utility/Timer/GlobalTimer.h>
#include <iberbar/Utility/Log/OutputDevice.h>
#include <iberbar/Utility/OS/Windows/WndMessage.h>
#include <iberbar/Utility/OS/DpiHelper.h>

#include <shellscalingapi.h>




namespace iberbar
{
	namespace Game
	{
		enum class UGameAppEvent
			: uint32
		{
			OnTime,
			OnRender,
			WakeupLoading,
			Count
		};
	}
}
















iberbar::CResult iberbar::Game::CApplication::Initial( HINSTANCE hInstance )
{
	assert( m_hWnd == NULL );

	if ( hInstance == NULL )
		hInstance = (HINSTANCE)GetModuleHandle( NULL );
	m_hInstance = hInstance;

	CResult ret;

	ret = CreateLog();
	if ( ret.IsOK() == false )
		return ret;

	// dpi ¸¨ÖúÀà
	m_pDpiHelper = new OS::CDpiHelper();

	m_pGlobalTimer = new CGlobalTimer();
	m_pGlobalTimer->SetTimeout( FPS_MS60 );
	m_pGlobalTimer->SetFunctionOnRun( std::bind( &CApplication::OnRunTimer, this, std::placeholders::_1, std::placeholders::_2 ) );
	m_pGlobalTimer->Initial( true );

	CreateHandleEvents();

	ret = CreateWnd();
	if ( ret.IsOK() == false )
		return ret;

	ret = CreateAll();
	if ( ret.IsOK() == false )
		return ret;

	m_nSysTimer = ::SetTimer( m_hWnd, 1, 10, sTimerProc );

	m_bInit = true;

	ret = OnCreated();
	if ( ret.IsOK() == false )
		return ret;

	return CResult();
}



iberbar::CResult iberbar::Game::CApplication::CreateWnd()
{
	WCHAR szExePath[MAX_PATH];
	GetModuleFileName( NULL, szExePath, MAX_PATH );
	HICON hIcon = ExtractIcon( m_hInstance, szExePath, 0 );
	DWORD nWndStyle = WS_OVERLAPPED | \
		WS_CAPTION | \
		WS_SYSMENU | \
		WS_MINIMIZEBOX;
	nWndStyle |= WS_SIZEBOX;

	// Register the windows class
	WNDCLASS wndClass;
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = sWndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = hIcon;
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT( "iberbarWindowClass" );

	if ( !RegisterClass( &wndClass ) )
	{
		DWORD dwError = GetLastError();
		if ( dwError != ERROR_CLASS_ALREADY_EXISTS )
		{
		}
		return MakeResult( ResultCode::Bad, "" );
	}

	
	m_pDpiHelper->SetAwareness( OS::EDpiAwareness::Unaware );
	int nDpi = m_pDpiHelper->GetDpi();
	int nScalingFactor = m_pDpiHelper->GetScaleFactor();

	TRect<long> RcWnd( 0, 0, m_Configuration.nWndWidth * nScalingFactor, m_Configuration.nWndHeight * nScalingFactor );
	AdjustWindowRectExForDpi( (LPRECT)&RcWnd, nWndStyle, FALSE, 0, nDpi );

	HWND hWnd = CreateWindow( wndClass.lpszClassName, m_Configuration.strAppName.c_str(), nWndStyle,
		0, 0, RcWnd.Width(), RcWnd.Height(), 0,
		nullptr, m_hInstance, 0 );
	if ( hWnd == NULL )
	{
		DWORD dwError = GetLastError();
		return MakeResult( ResultCode::Bad, "" );
	}

	m_hWnd = hWnd;
	

	ShowWindow( m_hWnd, SW_SHOWDEFAULT );
	UpdateWindow( m_hWnd );

	return CResult();
}


void iberbar::Game::CApplication::CreateHandleEvents()
{
	m_hEvents = new HANDLE[(uint32)UGameAppEvent::Count];
	m_nEventCount = (uint32)UGameAppEvent::Count;
	for ( int i = 0; i < m_nEventCount; i++ )
		m_hEvents[i] = ::CreateEventW( NULL, FALSE, FALSE, NULL );
}


void iberbar::Game::CApplication::WakeupLoadingThread()
{
	::SetEvent( m_hEvents[(int)UGameAppEvent::WakeupLoading] );
}


int iberbar::Game::CApplication::Run()
{
	if ( m_hEvents == nullptr )
		return 0;

	for ( ;; )
	{
		DWORD nWaitResult = MsgWaitForMultipleObjects( m_nEventCount, m_hEvents, FALSE, INFINITE, QS_ALLEVENTS );

		int nTriggerEventId = nWaitResult - WAIT_OBJECT_0;
		if ( nTriggerEventId == m_nEventCount )
		{
			MSG lc_msg;

			while ( PeekMessageW( &lc_msg, 0, 0, 0, PM_REMOVE ) )
			{
				if ( lc_msg.message != WM_QUIT )
				{
					TranslateMessage( &lc_msg );
					DispatchMessage( &lc_msg );
				}
				else
				{
					Destroy();
					return 0;
				}
			}

			HandleEvent( nTriggerEventId );
		}
		else if ( nTriggerEventId < m_nEventCount && nTriggerEventId >= 0 )
		{
			HandleEvent( nTriggerEventId );
		}
	} // end for

	Destroy();
	return 0;
}


void iberbar::Game::CApplication::HandleEvent( int nEventId )
{
	switch ( nEventId )
	{
		case (int)UGameAppEvent::OnTime:
		{
			::SetEvent( m_hEvents[(int)UGameAppEvent::OnRender] );
			break;
		}

		case (int)UGameAppEvent::OnRender:
		{
			m_pGlobalTimer->Run();
			break;
		}

		case (int)UGameAppEvent::Count:
		{
			::SetEvent( m_hEvents[(int)UGameAppEvent::OnRender] );
			break;
		}

		case (int)UGameAppEvent::WakeupLoading:
		{
			if ( m_pLoadingThread )
				m_pLoadingThread->Wakeup();
			break;
		}

		default:
		{
			break;
		}
	}
	OnHandleEvent( nEventId );
}


void iberbar::Game::CApplication::Quit()
{
	if ( m_hWnd != nullptr )
	{
		::PostMessageW( m_hWnd, WM_DESTROY, 0, 0 );
	}
}


LRESULT CALLBACK iberbar::Game::CApplication::sWndProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	//if ( nMsg != WM_NCMOUSEMOVE && nMsg != 32 && nMsg != 160 )
	//{
	//	const char* strMsgMacroText = OS::WndMessageGetMacroText( nMsg );
	//	if ( strMsgMacroText == nullptr )
	//	{
	//		strMsgMacroText = "";
	//	}
	//	sm_pInstance->GetLoggingOutputDevice()->Serialize(
	//		Logging::ULevel::Info,
	//		StdFormat( "msg=%d (%s)", nMsg, strMsgMacroText ).c_str(),
	//		"WndProc" );
	//}



	switch ( nMsg )
	{
		case WM_CLOSE:
			sm_pInstance->Quit();
			return 0;
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_EXITSIZEMOVE:
		{
			if ( sm_pInstance->m_bInit )
			{
				CRect2i ClientRect;
				GetClientRect( sm_pInstance->m_hWnd, (LPRECT)&ClientRect );
				sm_pInstance->m_pLoggingOutputDevice->Serialize(
					Logging::ULevel::Info,
					StdFormat( "window pos changing: width=%d, height=%d", ClientRect.Width(), ClientRect.Height() ).c_str(),
					"Rhi" );
				CResult RetReset = sm_pInstance->m_pRHIDevice->ResetDevice( ClientRect.Width(), ClientRect.Height(), true );
				if ( RetReset.IsOK() == false )
				{
					sm_pInstance->m_pLoggingOutputDevice->Serialize(
						Logging::ULevel::Error,
						StdFormat( "Failed to reset device: %s", RetReset.data.c_str() ).c_str(),
						"Rhi" );
				}
				else
				{
					sm_pInstance->m_pLoggingOutputDevice->Serialize(
						Logging::ULevel::Info,
						"Succeed to reset device",
						"Rhi" );
					sm_pInstance->Resume();
				}
			}
			break;
		}

		case WM_ENTERSIZEMOVE:
		{
			sm_pInstance->Pause();
			sm_pInstance->m_pRHIDevice->LostDevice();
			RHI::CTrace::sGetInstance()->ForeachResource( []( RHI::IResource* pResource )
				{
					if ( pResource == nullptr || pResource->IsManaged() || pResource->IsLost() == true )
						return;
					sm_pInstance->GetLoggingOutputDevice()->Serialize(
						Logging::ULevel::Info,
						StdFormat( "forgot to lost: %s", pResource->ToString().c_str() ).c_str(),
						"RhiTrace" );
				} );
			break;
		}

		case WM_CAPTURECHANGED:
		{
			if ( hWnd == sm_pInstance->GetHWnd() )
			{
				// 				if ( iberbar::CGlobalTimer::GetSharedInstance()->IsStop() )
				// 				{
				// 					iberbar::GetLogConsole()->logva(
				// 						iberbar::PRIORITY_DEBUG,
				// 						"WNDPROC",
				// 						"Got message < WM_CAPTURECHANGED, %d, %d >.Start timer.",
				// 						(int)wParam, (int)lParam );
				// 					iberbar::CGlobalTimer::GetSharedInstance()->Start();
				// 				}
				// 				else
				// 				{
				// 					iberbar::GetLogConsole()->logva(
				// 						iberbar::PRIORITY_DEBUG,
				// 						"WNDPROC",
				// 						"Got message < WM_CAPTURECHANGED, %d, %d >.Stop timer.",
				// 						(int)wParam, (int)lParam );
				// 					iberbar::CGlobalTimer::GetSharedInstance()->Stop();
				// 				}

								//iberbar::CGlobalTimer::GetSharedInstance()->Start();
			}
		}
		break;

		case WM_ACTIVATE:
		{
			if ( hWnd == sm_pInstance->GetHWnd() )
			{
				if ( wParam == WA_CLICKACTIVE || wParam == WA_ACTIVE )
				{
					sm_pInstance->Resume();
				}
				else if ( wParam == WA_INACTIVE )
				{
					sm_pInstance->Pause();
				}
			}
		}
		break;

		case WM_SETFOCUS:
		{
			break;
		}


		case WM_WTSSESSION_CHANGE:
		{
			switch ( wParam )
			{
				case WTS_SESSION_LOCK:
				{
					//ËøÆÁ
					sm_pInstance->Pause();

					break;
				}
				
				case WTS_SESSION_UNLOCK:
				{
					//½âËø
					sm_pInstance->Resume();
					sm_pInstance->GetLoggingOutputDevice()->Serialize(
						Logging::ULevel::Info,
						"SessionUnlock",
						"WndProc" );
					break;
				}
				
				default:
					break;
			}
			break;
		}



		case WM_CREATE:
		{
			sm_pInstance->Resume();
			break;
		}
		
		case WM_SYSCOMMAND:
		{
			switch ( wParam )
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
		}
		break;

		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* mminfo;
			mminfo = (PMINMAXINFO)lParam;
			break;
		}


		case WM_DISPLAYCHANGE:
		{
			sm_pInstance->m_pRHIDevice->LostDevice();
			sm_pInstance->Pause();
			break;
		}

		//case WM_WINDOWPOSCHANGING:
		//{
		//	if ( sm_pInstance->m_bInit )
		//	{
		//		CRect2i ClientRect;
		//		GetClientRect( sm_pInstance->m_hWnd, (LPRECT)&ClientRect );
		//		sm_pInstance->m_pLoggingOutputDevice->Serialize(
		//			Logging::ULevel::Info,
		//			StdFormat( "window pos changing: width=%d, height=%d", ClientRect.Width(), ClientRect.Height() ).c_str(),
		//			"Rhi" );
		//		//sm_pInstance->m_pRHIDevice->LostDevice();
		//		CResult RetReset = sm_pInstance->m_pRHIDevice->ResetDevice( ClientRect.Width(), ClientRect.Height(), true );
		//		if ( RetReset.IsOK() == false )
		//		{
		//			sm_pInstance->m_pLoggingOutputDevice->Serialize(
		//				Logging::ULevel::Error,
		//				StdFormat( "Failed to reset device: %s", RetReset.data.c_str() ).c_str(),
		//				"Rhi" );
		//		}
		//		else
		//		{
		//			sm_pInstance->Resume();
		//		}
		//	}

		//	break;
		//}


		case WM_MOUSEMOVE:
			::SetEvent( sm_pInstance->m_hEvents[(int)UGameAppEvent::OnRender] );
			//s_ptMouse.x = LOWORD( lParam );
			//s_ptMouse.y = HIWORD( lParam );
			break;

			// 	case WM_EXITSIZEMOVE:
			// 		{
			// 			if ( GetApp() && GetApp()->isInit() )
			// 			{
			// 				iberbar::CDXDevice::GetSharedInstance()->getDevice();
			// 				iberbar::CDXDevice::GetSharedInstance()->lostDevice();
			// 				iberbar::CDXDevice::GetSharedInstance()->resetDevice();
			// 			}
			// 		}
			// 		break;

		case WM_DPICHANGED:
		{
			int a = 0;
			break;
		}
			

		default:break;
	}

	//if ( GetApp() && GetApp()->isInit() )
	//{
	//	GetApp()->m_Callbacks.pOnWndProc( message, lParam, wParam );
	//}

	if ( sm_pInstance->m_bInit == true )
	{
		sm_pInstance->m_pInput->HandleWndProc( hWnd, nMsg, wParam, lParam );
		sm_pInstance->OnWndProc( nMsg, wParam, lParam );
	}

	return ::DefWindowProcW( hWnd, nMsg, wParam, lParam );
}


void CALLBACK iberbar::Game::CApplication::sTimerProc( HWND hWnd, UINT, UINT_PTR, DWORD )
{
	::SetEvent( sm_pInstance->m_hEvents[(int)UGameAppEvent::OnTime] );
}



#endif