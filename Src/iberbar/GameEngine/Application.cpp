
#include <iberbar/GameEngine/Application.h>

// Game
#include <iberbar/GameEngine/Paper2dLoader.h>
#include <iberbar/GameEngine/ResourcePreloader.h>
#include <iberbar/GameEngine/TextureManager.h>
#include <iberbar/GameEngine/ShaderManager.h>
#include <iberbar/GameEngine/FontManager.h>
#include <iberbar/GameEngine/Timer.h>
#include <iberbar/GameEngine/LoadingThread.h>
#include <iberbar/GameEngine/BaseResourceManager.h>

// Renderer
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/Renderer/Font.h>

// RHI
#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/OpenGL/Device.h>
#ifdef _WIN32
#include <iberbar/RHI/D3D9/Device.h>
#endif
#include <iberbar/RHI/VertexDeclaration.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/Gui/Xml/XmlProc.h>
#ifdef __ANDROID__
#include <iberbar/Utility/Platform/Android/JniHelper.h>
#include <android/asset_manager_jni.h>
#endif

// Gui
#include <iberbar/Gui/Engine.h>
#include <iberbar/Gui/Xml/XmlState.h>

// Paper2d
#include <iberbar/Paper2d/Director.h>

// Network
#include <iberbar/Network/IO.h>

// Lua
#include <iberbar/Lua/LuaDevice.h>
#include <iberbar/Lua/LoggingHelper.h>
#include <iberbar/LuaCppApis/Utility.h>
#include <iberbar/LuaCppApis/Utility/Xml.h>
#include <iberbar/LuaCppApis/Rhi.h>
#include <iberbar/LuaCppApis/Gui.h>
#include <iberbar/LuaCppApis/Paper2d.h>
#include <iberbar/LuaCppApis/Game.h>
#include <iberbar/LuaCppApis/MsgPack.h>
#include <iberbar/LuaCppApis/Json.h>
#include <iberbar/LuaCppApis/Network.h>

// Logging
#include <iberbar/Utility/Log/OutputDeviceFile.h>

// utility
#include <iberbar/Utility/Timer/GlobalTimer.h>
#include <iberbar/Utility/Input.h>
#include <iberbar/Utility/RefStatistics.h>
#include <iberbar/Utility/FileHelper.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Command.h>




#ifdef __ANDROID__
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TestRenderer", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "TestRenderer", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "TestRenderer", __VA_ARGS__))
#endif





enum class UGameAppEvent
	: uint32
{
	OnTime,
	OnRender,
	WakeupLoading,
	Count
};


namespace iberbar
{
	namespace Game
	{
		void OnPreloadTexture( const CResourcePreloader::ULoadTextureContext& Context );
		void OnPreloadFont( const CResourcePreloader::ULoadFontContext& Context );
	}
}



iberbar::Game::CApplication* s_pApplication = nullptr;


iberbar::Game::CApplication::CApplication()
	: m_bInit( false )
#ifdef _WIN32
	, m_hInstance( nullptr )
	, m_hWnd( nullptr )
	, m_hEvents( nullptr )
	, m_nEventCount( 0 )
	, m_nSysTimer( 0 )
#endif
#ifdef __ANDROID__
	, m_pJNIEnv( nullptr )
#endif
	, m_bPause( false )
	, m_Configuration()

	, m_pLoggingOutputDevice( nullptr )

	, m_pGuiEngine( nullptr )
	, m_pGuiXmlParser( nullptr )
	, m_pRHIDevice( nullptr )

	, m_ResourcePreloader( nullptr )
	, m_pTextureManager( nullptr )
	, m_pShaderManager( nullptr )
	, m_pFontManager( nullptr )
	, m_pPaper2dLoader( nullptr )

	, m_pRenderer( nullptr )
	, m_pRendererSprite( nullptr )

	, m_pPaper2dDirector( nullptr )

	, m_pLuaDevice( nullptr )
	, m_pGlobalTimer( nullptr )
	, m_pTimerEasySystem( nullptr )
	, m_pInput( nullptr )

	, m_pLoadingThread( nullptr )

	, m_pCommandQueue( nullptr )

	, m_pMemoryRes( new std::pmr::unsynchronized_pool_resource() )
{
	s_pApplication = this;
}


iberbar::Game::CApplication::~CApplication()
{
#ifdef __ANDROID__
	Destroy();
#endif
	s_pApplication = nullptr;
}


void iberbar::Game::CApplication::Destroy()
{
	this->OnDestroy();

#ifdef _WIN32
	if ( m_nSysTimer > 0 )
	{
		::KillTimer( m_hWnd, m_nSysTimer );
	}

	if ( m_hEvents != nullptr )
	{
		for ( uint32 i = 0; i < m_nEventCount; i++ )
		{
			CloseHandle( m_hEvents[ i ] );
			m_hEvents[ i ] = nullptr;
		}
		m_hEvents = nullptr;
		m_nEventCount = 0;
	}
#endif

	// 停止并销毁loading线程
	if ( m_pLoadingThread )
	{
		m_pLoadingThread->Destroy();
		delete m_pLoadingThread;
		m_pLoadingThread = nullptr;
	}

	// 释放Lua占用的东西
	UNKNOWN_SAFE_RELEASE_NULL( m_pLuaDevice );
	Lua::CLoggingHelper::sDestroy();

	// 删除命令队列
	if ( m_pCommandQueue )
	{
		DestroyCommandQueue( m_pCommandQueue );
		m_pCommandQueue = nullptr;
	}

	// 删除timer
	SAFE_DELETE( m_pTimerEasySystem );

	// 先释放场景内容
	SAFE_DELETE( m_pPaper2dDirector );
	SAFE_DELETE( m_pGuiEngine );
	SAFE_DELETE( m_pGuiXmlParser );

	// 然后释放渲染器
	SAFE_DELETE( m_pRendererSprite );
	SAFE_DELETE( m_pRenderer );

	// 最后释放资源管理
	SAFE_DELETE( m_ResourcePreloader );
	SAFE_DELETE( m_pTextureManager );
	SAFE_DELETE( m_pShaderManager );
	SAFE_DELETE( m_pFontManager );
	SAFE_DELETE( m_pPaper2dLoader );

	UNKNOWN_SAFE_RELEASE_NULL( m_pRHIDevice );

	SAFE_DELETE( m_pGlobalTimer );
	SAFE_DELETE( m_pInput );

	// 释放日志
	UNKNOWN_SAFE_RELEASE_NULL( m_pLoggingOutputDevice );

	// 释放 IO
	IO::Destroy();

	// 释放内存池
	SAFE_DELETE( m_pMemoryRes );

#ifdef _DEBUG
	FILE* f = nullptr;
	fopen_s( &f, "RefZombies.txt", "wt" );
	if ( f != nullptr )
	{
		CRefStatistics::sGetShared()->ForEach(
			[=]( CRef* pRefZombie )
			{
#ifdef _WINDOWS
				auto n = typeid(*pRefZombie).name();
				auto r = pRefZombie->Refcount();
				std::string strText = StdFormat( "0x%016llx: TypeName=<%s> Ref=%d\n", (uint64)pRefZombie, n, r );
#else
				std::string strText = StdFormat( "0x%016llx: Ref=%d\n", (uint64)pRefZombie, pRefZombie->Refcount() );
#endif
				fwrite( strText.c_str(), 1, strlen( strText.c_str() ), f );
			} );
		fclose( f );
		f = nullptr;
	}
#endif
}


void iberbar::Game::CApplication::Resume()
{
	m_bPause = false;
	OnResume();
}


void iberbar::Game::CApplication::Pause()
{
	m_bPause = true;
	OnPause();
}


#ifdef _WINDOWS
iberbar::CResult iberbar::Game::CApplication::Initial( HINSTANCE hInstance )
{
	assert( m_hWnd == NULL );

	if ( hInstance == NULL )
		hInstance = (HINSTANCE)GetModuleHandle( NULL );
	m_hInstance = hInstance;

	CResult ret;

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
	if (ret.IsOK() == false)
		return ret;

	return CResult();
}



iberbar::CResult iberbar::Game::CApplication::CreateWnd()
{
	WCHAR szExePath[ MAX_PATH ];
	GetModuleFileName( NULL, szExePath, MAX_PATH );
	HICON hIcon = ExtractIcon( m_hInstance, szExePath, 0 );
	DWORD nWndStyle = WS_OVERLAPPED | \
		WS_CAPTION | \
		WS_SYSMENU | \
		WS_MINIMIZEBOX;

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

	RECT rc;
	SetRect( &rc, 0, 0, m_Configuration.nWndWidth, m_Configuration.nWndHeight );
	AdjustWindowRect( &rc, nWndStyle, FALSE );

	HWND hWnd = CreateWindow( wndClass.lpszClassName, m_Configuration.strAppName.c_str(), nWndStyle,
		0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0,
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
	m_hEvents = new HANDLE[ (uint32)UGameAppEvent::Count ];
	m_nEventCount = (uint32)UGameAppEvent::Count;
	for ( int i = 0; i < m_nEventCount; i++ )
		m_hEvents[ i ] = ::CreateEventW( NULL, FALSE, FALSE, NULL );
}


iberbar::CResult iberbar::Game::CApplication::CreateRHI()
{
	CResult ret;

	RECT ClientRect;
	::GetClientRect( m_hWnd, &ClientRect );

	switch ( m_Configuration.nRHIApi )
	{
		case RHI::UApiType::OpenGL:
		{
			m_pRHIDevice = new RHI::OpenGL::CDevice();
			RHI::OpenGL::CDevice* pOpenGLDevice = (RHI::OpenGL::CDevice*)m_pRHIDevice;
			ret = pOpenGLDevice->CreateDevice(
				m_hWnd,
				m_Configuration.bWindow,
				ClientRect.right - ClientRect.left,
				ClientRect.bottom - ClientRect.top );
			if ( ret.IsOK() == false )
				return ret;

			break;
		}

		case RHI::UApiType::D3D9:
		{
			m_pRHIDevice = new RHI::D3D9::CDevice();
			RHI::D3D9::CDevice* pRHIDeviceTemp = (RHI::D3D9::CDevice*)m_pRHIDevice;
			ret = pRHIDeviceTemp->CreateDevice(
				m_hWnd,
				m_Configuration.bWindow,
				ClientRect.right - ClientRect.left,
				ClientRect.bottom - ClientRect.top );
			if ( ret.IsOK() == false )
				return ret;
			break;
		}

		case RHI::UApiType::D3D11:
		{
			ret = MakeResult( ResultCode::Bad, "Not support D3D11" );
			break;
		}

		case RHI::UApiType::D3D12:
		{
			ret = MakeResult( ResultCode::Bad, "Not support D3D11" );
			break;
		}

		default:
			ret = MakeResult( ResultCode::Bad, "Unknown RHI api" );
			break;
	}

	if ( m_pRHIDevice != nullptr )
	{
		m_pRHIDevice->SetClearColor( CColor4B( 255, 0, 0, 0 ) );
	}

	return ret;
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
			::SetEvent( m_hEvents[ (int)UGameAppEvent::OnRender ] );
			break;
		}

		case (int)UGameAppEvent::OnRender:
		{
			m_pGlobalTimer->Run();
			break;
		}

		case (int)UGameAppEvent::Count:
		{
			::SetEvent( m_hEvents[ (int)UGameAppEvent::OnRender ] );
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
#endif


#ifdef __ANDROID__
iberbar::CResult iberbar::Game::CApplication::SetJNIEnv( JNIEnv* pJNIEnv )
{
	m_pJNIEnv = pJNIEnv;

	jclass pJClass = nullptr;
	int methodCount = 0;

	pJClass = m_pJNIEnv->FindClass( "com/iberbar/lib/MyRenderer" );
	if ( pJClass == nullptr )
		return MakeResult( ResultCode::Bad, "" );
	static JNINativeMethod methodsRenderer[] = {
		{ "nativeInit", "(II)V", reinterpret_cast<void*>(CApplication::sJNI_nativeInit) },
		{ "nativeRender", "()V", reinterpret_cast<void*>(CApplication::sJNI_nativeRender) },
		{ "nativeOnSurfaceChanged", "(II)V", reinterpret_cast<void*>(CApplication::sJNI_nativeOnSurfaceChanged) },
		{ "nativeOnPause", "()V", reinterpret_cast<void*>(CApplication::sJNI_nativeOnPause) },
		{ "nativeOnResume", "()V", reinterpret_cast<void*>(CApplication::sJNI_nativeOnResume) },
		{ "nativeTouchesBegin", "(IFF)V", reinterpret_cast<void*>(CApplication::sJNI_nativeTouchesBegin) },
		{ "nativeTouchesEnd", "(IFF)V", reinterpret_cast<void*>(CApplication::sJNI_nativeTouchesEnd) },
		{ "nativeTouchesMove", "([I[F[F)V", reinterpret_cast<void*>(CApplication::sJNI_nativeTouchesMove) },
		{ "nativeTouchesCancel", "([I[F[F)V", reinterpret_cast<void*>(CApplication::sJNI_nativeTouchesCancel) },
		{ "nativeKeyEvent", "(IZ)Z", reinterpret_cast<void*>(CApplication::sJNI_nativeKeyEvent) }
	};
	methodCount = sizeof( methodsRenderer ) / sizeof( JNINativeMethod );
	if ( m_pJNIEnv->RegisterNatives( pJClass, methodsRenderer, methodCount ) < 0 )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	pJClass = m_pJNIEnv->FindClass( "com/iberbar/lib/MyHelper" );
	if ( pJClass == nullptr )
		return MakeResult( ResultCode::Bad, "" );
	static JNINativeMethod methodsHelper[] = {
		{ "nativeSetContext", "(Landroid/content/Context;Landroid/content/res/AssetManager;)V", reinterpret_cast<void*>(CApplication::sJNI_nativeSetContext) },
		{ "nativeOnDestroy", "()V", reinterpret_cast<void*>(CApplication::sJNI_nativeOnDestroy) },
	};
	methodCount = sizeof( methodsHelper ) / sizeof( JNINativeMethod );
	if ( m_pJNIEnv->RegisterNatives( pJClass, methodsHelper, methodCount ) < 0 )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	return CResult();
}


iberbar::CResult iberbar::Game::CApplication::ExportAssetsPreInitial()
{
	iberbar::Android::CAssetsExportUtil util;
	util.ExportDir(
		"Shaders/OpenGL", "",
		[]( const std::string& strAssetFile, const std::string& strExportFile, const iberbar::CResult& exportResult )
		{
			if ( exportResult.IsOK() == true )
			{
				LOGI( "export %s -> %s: success", strAssetFile.c_str(), strExportFile.c_str() );
			}
			else
			{
				LOGW( "export %s -> %s: %s", strAssetFile.c_str(), strExportFile.c_str(), exportResult.data.c_str() );
			}
		}
	);

	return CResult();
}


iberbar::CResult iberbar::Game::CApplication::Initial()
{
	CResult ret;

	m_pGlobalTimer = new CGlobalTimer();
	m_pGlobalTimer->SetTimeout( FPS_MS60 );
	m_pGlobalTimer->SetFunctionOnRun( std::bind( &CApplication::OnRunTimer, this, std::placeholders::_1, std::placeholders::_2 ) );
	m_pGlobalTimer->Initial( false );

	ret = CreateAll();
	if ( ret.IsOK() == false )
		return ret;

	m_bInit = true;

	OnCreated();

	return CResult();
}


iberbar::CResult iberbar::Game::CApplication::CreateRHI()
{
	CResult ret;

	m_pRHIDevice = new RHI::OpenGL::CDevice();
	RHI::OpenGL::CDevice* pOpenGLDevice = (RHI::OpenGL::CDevice*)m_pRHIDevice;
	ret = pOpenGLDevice->CreateDevice( m_Configuration.nWndWidth, m_Configuration.nWndHeight );
	if ( ret.IsOK() == false )
		return ret;

	m_pRHIDevice->SetClearColor( CColor4B( 255, 0, 0, 0 ) );

	return CResult();
}
#endif


iberbar::CResult iberbar::Game::CApplication::CreateAll()
{
	CResult ret;

	// 初始化IO
	IO::Initial();

	// 初始化Logging
	m_pLoggingOutputDevice = new Logging::COutputDeviceFile( CResourceFileSystem::GetResoucePath( "Game.log" ).c_str() );

	// 创建底层RHI
	ret = CreateRHI();
	if ( ret.IsOK() == false )
		return ret;

	// 创建Renderer相关渲染工具
	m_pRenderer = new Renderer::CRenderer2d();
	m_pRenderer->Init( m_pRHIDevice );
	m_pRendererSprite = new Renderer::CRendererSprite( m_pRenderer );

	// 创建资源管理
	m_pShaderManager = new CShaderManager( m_pRHIDevice );
	m_pTextureManager = new CTextureManager( m_pRHIDevice );
	m_pFontManager = new CFontManager( m_pRHIDevice );
	ret = m_pFontManager->Initial();
	if ( ret.IsOK() == false )
		return ret;
	m_pPaper2dLoader = new CPaper2dLoader();

	// 加载默认shader
	ret = LoadDefaultShaders();
	if ( ret.IsOK() == false )
		return ret;

	// 创建命令队列
	UCommandQueueOptions CommandQueueOptions;
	CommandQueueOptions.bSync = false;
	m_pCommandQueue = CreateCommandQueue( CommandQueueOptions, m_pMemoryRes );
	if ( m_pCommandQueue == nullptr )
		return MakeResult( ResultCode::Bad, "failed to create queue of commands" );

	// 创建Gui引擎
	m_pGuiEngine = new Gui::CEngine( m_pRendererSprite, m_pCommandQueue, m_pMemoryRes );


	// 创建Gui的XML解释器
	m_pGuiXmlParser = new Gui::CXmlParser();
	//m_pGuiXmlParser->SetBaseDir( iberbar::Android::CFileUtil::GetShared()->GetBestFilesDir() + "/" );
	m_pGuiXmlParser->RegisterGetTexture( std::bind( &CTextureManager::GetOrCreateTextureA, m_pTextureManager, std::placeholders::_1, std::placeholders::_2 ) );
	m_pGuiXmlParser->RegisterGetFont( std::bind( &CFontManager::GetFont, m_pFontManager, std::placeholders::_1, std::placeholders::_2 ) );

	m_pGuiXmlParser->RegisterCreateProc_Widget( "Default", std::bind( &Gui::XmlCreateProc_Widget, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Widget( "Button", std::bind( &Gui::XmlCreateProc_Widget_Button, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Widget( "CheckBox", std::bind( &Gui::XmlCreateProc_Widget_CheckBox, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Widget( "RadioBox", std::bind( &Gui::XmlCreateProc_Widget_RadioBox, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Widget( "ListBox", std::bind( &Gui::XmlCreateProc_Widget_ListBox, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Widget( "EditBox", std::bind( &Gui::XmlCreateProc_Widget_EditBox, std::placeholders::_1 ) );

	m_pGuiXmlParser->RegisterCreateProc_Element( "Default", std::bind( &Gui::XmlCreateProc_Element, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Element( "ColorRect", std::bind( &Gui::XmlCreateProc_Element_ColorRect, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Element( "StateTexture", std::bind( &Gui::XmlCreateProc_Element_StateTexture, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Element( "StateLabel", std::bind( &Gui::XmlCreateProc_Element_StateLabel, std::placeholders::_1 ) );
	m_pGuiXmlParser->RegisterCreateProc_Element( "TextField", std::bind( &Gui::XmlCreateProc_Element_EditBoxText, std::placeholders::_1 ) );

	m_pGuiXmlParser->RegisterReadProc_Widget( "Default", std::bind( &Gui::XmlReadProc_Widget,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	m_pGuiXmlParser->RegisterReadProc_Widget( "Button", std::bind( &Gui::XmlReadProc_Widget_Button,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	m_pGuiXmlParser->RegisterReadProc_Widget( "CheckBox", std::bind( &Gui::XmlReadProc_Widget_CheckBox,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	m_pGuiXmlParser->RegisterReadProc_Widget( "RadioBox", std::bind( &Gui::XmlReadProc_Widget_RadioBox,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	m_pGuiXmlParser->RegisterReadProc_Widget( "ListBox", std::bind( &Gui::XmlReadProc_Widget_ListBox,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	m_pGuiXmlParser->RegisterReadProc_Widget( "EditBox", std::bind( &Gui::XmlReadProc_Widget_EditBox,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );

	m_pGuiXmlParser->RegisterReadProc_Element( "Default", std::bind( &Gui::XmlReadProc_Element,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	m_pGuiXmlParser->RegisterReadProc_Element( "ColorRect", std::bind( &Gui::XmlReadProc_Element_ColorRect,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	m_pGuiXmlParser->RegisterReadProc_Element( "StateTexture", std::bind( &Gui::XmlReadProc_Element_StateTexture,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	m_pGuiXmlParser->RegisterReadProc_Element( "StateLabel", std::bind( &Gui::XmlReadProc_Element_StateLabel,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	m_pGuiXmlParser->RegisterReadProc_Element( "TextField", std::bind( &Gui::XmlReadProc_Element_EditBoxText,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );


	// 创建Paper2d
	m_pPaper2dDirector = new Paper2d::CDirector( m_pRendererSprite );


	// 创建设备输入控制器
	m_pInput = new CInput();
	m_pInput->AddMouseEvent( std::bind( &Gui::CEngine::HandleMouse, m_pGuiEngine, std::placeholders::_1 ) );
	m_pInput->AddMouseEvent( std::bind( &Paper2d::CDirector::HandleMouse, m_pPaper2dDirector, std::placeholders::_1 ) );
	m_pInput->AddKeyboardEvent( std::bind( &Gui::CEngine::HandleKeyboard, m_pGuiEngine, std::placeholders::_1 ) );
	m_pInput->AddKeyboardEvent( std::bind( &Paper2d::CDirector::HandleKeyboard, m_pPaper2dDirector, std::placeholders::_1 ) );

	// 绑定预加载的回调方法
	m_ResourcePreloader = new CResourcePreloader();
	m_ResourcePreloader->m_LoadTexture = std::bind( &OnPreloadTexture, std::placeholders::_1 );
	m_ResourcePreloader->m_LoadFont = std::bind( &OnPreloadFont, std::placeholders::_1 );

	// 初始化Lua
	Lua::CLoggingHelper::sInitial( m_pLoggingOutputDevice );
	m_pLuaDevice = new CLuaDevice();
	m_pLuaDevice->Initial();
	// 注册Lua方法
	RegisterLuaCpp_ForUtility( m_pLuaDevice->GetLuaState() );
	Xml::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	RHI::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	Gui::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	Paper2d::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	Game::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	MsgPack::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	Json::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	IO::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );

	// Timer系统
	m_pTimerEasySystem = new CTimerEasySystem();

	// 创建LoadingThread
	if ( m_Configuration.bUseLoadingThread == true )
	{
		m_pLoadingThread = new CLoadingThread();
		m_pLoadingThread->Initial();
	}



	return CResult();
}


iberbar::CResult iberbar::Game::CApplication::LoadDefaultShaders()
{
	CResult ret;

	std::string strShaderRootDir = "";
	if ( iberbar::RHI::IsOpenGLApi( m_pRHIDevice->GetApiType() ) )
	{
		strShaderRootDir += "Shaders/OpenGL";
	}
#ifdef _WIN32
	else if ( iberbar::RHI::IsD3DApi( m_pRHIDevice->GetApiType() ) )
	{
		strShaderRootDir += "Shaders/D3D";
	}
#endif
	m_pShaderManager->SetRootDir( strShaderRootDir.c_str() );

	{
		RHI::UVertexElement VertexElements[] =
		{
			{ RHI::UVertexDeclareUsage::Position, 0, RHI::UVertexFormat::FLOAT3, 0 },
			{ RHI::UVertexDeclareUsage::Color, 0, RHI::UVertexFormat::COLOR, 12 },
			{ RHI::UVertexDeclareUsage::TexCoord, 0, RHI::UVertexFormat::FLOAT2, 16 }
		};
		TSmartRefPtr<RHI::IShader> pShader;
		ret = m_pShaderManager->GetOrCreateShader( "PositionColorTexture2d", &pShader );
		if ( ret.IsOK() == false )
			return ret;

		TSmartRefPtr<RHI::IVertexDeclaration> pVertexDeclaration;
		m_pRHIDevice->CreateVertexDeclaration( &pVertexDeclaration, VertexElements, 3, 24 );

		TSmartRefPtr<RHI::IShaderState> pShaderState;
		m_pRHIDevice->CreateShaderState( &pShaderState, pShader, pVertexDeclaration );

		m_pRendererSprite->SetDefaultShaderState( pShaderState );
	}

	return CResult();
}


void iberbar::Game::CApplication::OnRunTimer( int64 nElapsedTimeMilliSecond, float nElapsedTimeSecond )
{
	// Network IO 读取
	IO::Read();

	if ( m_pLoadingThread )
	{
		m_pLoadingThread->LoopStepResults();
	}

	// loading 加锁
	//if ( m_pLoadingThread->IsWakeup() )
	//	m_pLoadingThread->Lock();

	// 执行命令队列
	m_pCommandQueue->Execute();

	// easy timer system
	m_pTimerEasySystem->Run( nElapsedTimeSecond );

	m_pPaper2dDirector->UpdateScene( nElapsedTimeSecond );
	m_pGuiEngine->Update( nElapsedTimeMilliSecond, nElapsedTimeSecond );
	OnUpdate( nElapsedTimeMilliSecond, nElapsedTimeSecond );

	// loading 解锁
	//if ( m_pLoadingThread->IsWakeup() )
	//	m_pLoadingThread->Unlock();

	m_pRendererSprite->Clean();
	OnRender();
	m_pPaper2dDirector->DrawScene();
	m_pGuiEngine->Render();

	m_pRHIDevice->Begin();
	m_pRenderer->Render();
	m_pRHIDevice->End();
}


void iberbar::Game::CApplication::Quit()
{
#ifdef _WINDOWS
	if ( m_hWnd != nullptr )
	{
		::PostMessageW( m_hWnd, WM_DESTROY, 0, 0 );
	}
#endif

#ifdef __ANDROID__
	jclass pClass = m_pJNIEnv->FindClass( "com/iberbar/lib/MyHelper" );
	if ( pClass == nullptr )
		return;

	jmethodID pMethodId = nullptr;

	pMethodId = m_pJNIEnv->GetStaticMethodID( pClass, "tryQuit", "()V" );
	if ( pMethodId != nullptr )
	{
		m_pJNIEnv->CallStaticObjectMethod( pClass, pMethodId );
	}
#endif
}


void iberbar::Game::CApplication::WakeupLoadingThread()
{
#ifdef _WINDOWS
	::SetEvent( m_hEvents[ (int)UGameAppEvent::WakeupLoading ] );
#endif
}


void iberbar::Game::CApplication::AddLoadingTask( CLoadingTask* pTask )
{
	if ( m_pLoadingThread )
		m_pLoadingThread->AddTask( pTask );
}


#ifdef _WIN32
LRESULT CALLBACK iberbar::Game::CApplication::sWndProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( nMsg )
	{
		case WM_CLOSE:
			s_pApplication->Quit();
			return 0;
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_CAPTURECHANGED:
		{
			if ( hWnd == s_pApplication->GetHWnd() )
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
			if ( hWnd == s_pApplication->GetHWnd() )
			{
				if ( wParam == WA_CLICKACTIVE || wParam == WA_ACTIVE )
				{
					s_pApplication->Resume();
				}
				else if ( wParam == WA_INACTIVE )
				{
					s_pApplication->Pause();
				}
			}
		}
		break;


		case WM_CREATE:
		{
		}
		break;



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


		case WM_MOUSEMOVE:
			::SetEvent( s_pApplication->m_hEvents[ (int)UGameAppEvent::OnRender ] );
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

		default:break;
	}

	//if ( GetApp() && GetApp()->isInit() )
	//{
	//	GetApp()->m_Callbacks.pOnWndProc( message, lParam, wParam );
	//}

	if ( s_pApplication->m_bInit == true )
	{
		s_pApplication->m_pInput->HandleWndProc( hWnd, nMsg, wParam, lParam );
		s_pApplication->OnWndProc( nMsg, wParam, lParam );
	}

	return ::DefWindowProcW( hWnd, nMsg, wParam, lParam );
}


void CALLBACK iberbar::Game::CApplication::sTimerProc( HWND hWnd, UINT, UINT_PTR, DWORD )
{
	::SetEvent( s_pApplication->m_hEvents[ (int)UGameAppEvent::OnTime ] );
}

#endif


#ifdef __ANDROID__

JNIEXPORT void iberbar::Game::CApplication::sJNI_nativeInit( JNIEnv* env, jobject obj, jint width, jint height )
{
	LOGI( "nativeInit" );

	// 先导出Shaders目录
	s_pApplication->ExportAssetsPreInitial();

	// 初始化
	s_pApplication->m_Configuration.nWndWidth = width;
	s_pApplication->m_Configuration.nWndHeight = height;
	CResult ret = s_pApplication->Initial();
	if ( ret.IsOK() == false )
	{
		LOGE( ret.data.c_str() );
	}
}


void iberbar::Game::CApplication::sJNI_nativeRender( JNIEnv* env, jobject obj )
{
	LOGI( "nativeRender" );
	s_pApplication->m_pGlobalTimer->Run();
}


void iberbar::Game::CApplication::sJNI_nativeOnSurfaceChanged( JNIEnv* env, jobject obj, jint width, jint height )
{
	LOGI( "nativeOnSurfaceChanged" );
	RHI::IDevice* pRHIDevice = s_pApplication->GetRHIDevice();
	pRHIDevice->LostDevice();
	pRHIDevice->ResetDevice( width, height, true );
}


void iberbar::Game::CApplication::sJNI_nativeOnPause( JNIEnv* env, jobject obj )
{
	LOGI( "nativeOnPause" );
	s_pApplication->Pause();
}


void iberbar::Game::CApplication::sJNI_nativeOnResume( JNIEnv* env, jobject obj )
{
	LOGI( "nativeOnResume" );
	s_pApplication->Resume();
}


void iberbar::Game::CApplication::sJNI_nativeOnDestroy( JNIEnv* env, jobject obj )
{
	LOGI( "nativeOnDestroy" );
	s_pApplication->Destroy();
}


void iberbar::Game::CApplication::sJNI_nativeTouchesBegin( JNIEnv* env, jobject obj, jint id, jfloat x, jfloat y )
{
	LOGI( "nativeTouchesBegin" );
}


void iberbar::Game::CApplication::sJNI_nativeTouchesEnd( JNIEnv* env, jobject obj, jint id, jfloat x, jfloat y )
{
	LOGI( "nativeTouchesEnd" );
}


void iberbar::Game::CApplication::sJNI_nativeTouchesMove( JNIEnv* env, jobject obj, jintArray ids, jfloatArray xs, jfloatArray ys )
{
	LOGI( "nativeTouchesMove" );
}


void iberbar::Game::CApplication::sJNI_nativeTouchesCancel( JNIEnv* env, jobject obj, jintArray ids, jfloatArray xs, jfloatArray ys )
{
	LOGI( "nativeTouchesCance" );
}


jboolean iberbar::Game::CApplication::sJNI_nativeKeyEvent( JNIEnv* env, jobject obj, jint keyCode, jboolean isPressed )
{
	LOGI( "nativeKeyEvent" );
	return JNI_TRUE;
}


JNIEXPORT void iberbar::Game::CApplication::sJNI_nativeSetContext( JNIEnv* env, jobject obj, jobject context, jobject assetManager )
{
	LOGI( "nativeSetContext" );
	iberbar::Android::CFileUtil::GetShared()->Init( env );
	iberbar::Android::CAssetsUtil::GetShared()->SetAssets( AAssetManager_fromJava( env, assetManager ) );
}
#endif



iberbar::Game::CApplication* iberbar::Game::GetApp()
{
	return s_pApplication;
}








void iberbar::Game::OnPreloadTexture( const CResourcePreloader::ULoadTextureContext& Context )
{
	CResult ret = GetApp()->GetTextureManager()->GetOrCreateTextureA( Context.strFile, nullptr );
	if ( ret.IsOK() == false )
	{

	}
}


void iberbar::Game::OnPreloadFont( const CResourcePreloader::ULoadFontContext& Context )
{
	UFontDesc FontDesc;
	FontDesc.FamilyName = Context.strFamily;
	FontDesc.Size = Context.nSize;
	FontDesc.Weight = (Context.bIsBold == true) ? 1000 : 500;
	FontDesc.Italic = 0;
	CResult ret = GetApp()->GetFontManager()->GetOrCreateFont( nullptr, FontDesc, Renderer::UFontCharVocabularyType::Unknown );
	if ( ret.IsOK() == false )
	{
	}
}

