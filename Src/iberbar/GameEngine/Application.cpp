
#include <iberbar/GameEngine/Application.h>

// Game
//#include <iberbar/GameEngine/Paper2dLoader.h>
#include <iberbar/GameEngine/ResourcePreloader.h>
#include <iberbar/GameEngine/TextureManager.h>
#include <iberbar/GameEngine/ShaderManager.h>
#include <iberbar/GameEngine/FontManager.h>
#include <iberbar/GameEngine/Timer.h>
#include <iberbar/GameEngine/LoadingThread.h>
#include <iberbar/GameEngine/BaseResourceManager.h>

// Renderer
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/Vertex.h>
//#include <iberbar/Renderer/Font.h>

// RHI
#include <iberbar/RHI/Device.h>
//#include <iberbar/RHI/OpenGL/Device.h>
//#ifdef _WIN32
//#include <iberbar/RHI/D3D9/Device.h>
//#endif
#include <iberbar/RHI/VertexDeclaration.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/RenderState.h>
#include <iberbar/RHI/CommandContext.h>

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
#include <iberbar/Network/ApiTypedef.h>
#include <iberbar/Network/SocketClient.h>


// Lua
#include <iberbar/Lua/LuaDevice.h>
#include <iberbar/Lua/LoggingHelper.h>
#include <iberbar/LuaCppApis/Utility.h>
#include <iberbar/LuaCppApis/Rhi.h>
#include <iberbar/LuaCppApis/Gui.h>
#include <iberbar/LuaCppApis/Paper2d.h>
#include <iberbar/LuaCppApis/Game.h>
#include <iberbar/LuaCppApis/MsgPack.h>
#include <iberbar/LuaCppApis/Json.h>
#include <iberbar/LuaCppApis/Xml.h>

// Logging
#include <iberbar/Utility/Log/OutputDeviceFile.h>

// utility
#include <iberbar/Utility/Timer/GlobalTimer.h>
#include <iberbar/Utility/Input.h>
#include <iberbar/Utility/RefStatistics.h>
#include <iberbar/Utility/FileHelper.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Command.h>
#include <iberbar/Utility/OS/MultiEventWaiter.h>
#include <iberbar/Utility/OS/DynamicLibrary.h>




#ifdef __ANDROID__
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TestRenderer", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "TestRenderer", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "TestRenderer", __VA_ARGS__))
#endif



namespace iberbar
{
	namespace Game
	{

		void OnPreloadTexture( const CResourcePreloader::ULoadTextureContext& Context );
		void OnPreloadFont( const CResourcePreloader::ULoadFontContext& Context );

		struct UNetworkProcAddressInfo
		{
			IO::PFunctionInitial pInitial;
			IO::PFunctionDestroy pDestroy;
			IO::PFunctionRead pRead;
			IO::PFunctionLuaCppRegister pLuaCppRegister;
		};
	}
}





iberbar::Game::CApplication* iberbar::Game::CApplication::sm_pInstance = nullptr;


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
	, m_bWndActive( false )
	, m_Configuration()

	, m_pLoggingOutputDevice( nullptr )

	, m_pGuiEngine( nullptr )
	//, m_pGuiXmlParser( nullptr )
	, m_pRHIDevice( nullptr )

	, m_ResourcePreloader( nullptr )
	, m_pTextureManager( nullptr )
	, m_pShaderLoader( nullptr )
	, m_pShaderManager( nullptr )
	, m_pFontManager( nullptr )
	//, m_pPaper2dLoader( nullptr )

	, m_pRenderer( nullptr )

	//, m_pPaper2dDirector( nullptr )

	, m_pLuaDevice( nullptr )
	, m_pGlobalTimer( nullptr )
	, m_pTimerEasySystem( nullptr )
	, m_pInput( nullptr )

	, m_pLoadingThread( nullptr )

	, m_pDynamicLib_Rhi( nullptr )
	, m_pDynamicLib_Network( nullptr )
	, m_pNetworkProcAddressInfo( nullptr )
	, m_pDpiHelper( nullptr )

	, m_pCommandQueue( nullptr )

	, m_pMemoryRes( new std::pmr::unsynchronized_pool_resource() )
{
	sm_pInstance = this;
}


iberbar::Game::CApplication::~CApplication()
{
#ifdef __ANDROID__
	Destroy();
#endif
	sm_pInstance = nullptr;
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

	SAFE_DELETE( m_pDpiHelper );

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
	//SAFE_DELETE( m_pPaper2dDirector );
	SAFE_DELETE( m_pGuiEngine );
	//SAFE_DELETE( m_pGuiXmlParser );

	// 然后释放渲染器
	SAFE_DELETE( m_pRenderer );

	// 最后释放资源管理
	SAFE_DELETE( m_ResourcePreloader );
	SAFE_DELETE( m_pTextureManager );
	SAFE_DELETE( m_pShaderManager );
	SAFE_DELETE( m_pShaderLoader );
	SAFE_DELETE( m_pFontManager );
	//SAFE_DELETE( m_pPaper2dLoader );

	if ( m_pRHIDevice )
	{
		m_pRHIDevice->Shutdown();
		delete m_pRHIDevice;
		m_pRHIDevice = nullptr;
	}

	SAFE_DELETE( m_pGlobalTimer );
	SAFE_DELETE( m_pInput );

	// 释放日志
	UNKNOWN_SAFE_RELEASE_NULL( m_pLoggingOutputDevice );

	// 释放 IO
	if ( m_pNetworkProcAddressInfo && m_pNetworkProcAddressInfo->pDestroy )
		m_pNetworkProcAddressInfo->pDestroy();
	SAFE_DELETE( m_pNetworkProcAddressInfo );

	// 释放内存池
	SAFE_DELETE( m_pMemoryRes );

	// 先打印僵尸对象
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

	// 释放动态库
	SAFE_DELETE( m_pDynamicLib_Rhi );
	SAFE_DELETE( m_pDynamicLib_Network );
}


void iberbar::Game::CApplication::Resume()
{
	m_pLoggingOutputDevice->Serialize( Logging::ULevel::Info, "active", "Application" );
	m_bWndActive = true;
	OnResume();
}


void iberbar::Game::CApplication::Pause()
{
	m_pLoggingOutputDevice->Serialize( Logging::ULevel::Info, "inactive", "Application" );
	m_bWndActive = false;
	OnPause();
}


iberbar::CResult iberbar::Game::CApplication::RhiDeviceCreate()
{
	return CResult();
}


void iberbar::Game::CApplication::RhiDeviceLost()
{
	if ( m_pRenderer )
	{
		m_pRenderer->OnRhiLost();
	}

	OnRhiDeviceLost();
}


iberbar::CResult iberbar::Game::CApplication::RhiDeviceReset()
{
	CResult Ret;
	if ( m_pRenderer )
	{
		Ret = m_pRenderer->OnRhiReset();
		if ( Ret.IsOK() == false )
			return Ret;
	}

	Ret = OnRhiDeviceReset();
	if ( Ret.IsOK() == false )
		return Ret;

	return CResult();
}


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
	ret = CreateNetwork();
	if ( ret.IsOK() == false )
		return ret;



	// 创建底层RHI
	ret = CreateRHI();
	if ( ret.IsOK() == false )
		return ret;

	// 创建Renderer相关渲染工具
	m_pRenderer = new Renderer::CRenderer();
	m_pRenderer->Init( m_pRHIDevice );

	// 创建资源管理
	m_pShaderLoader = new CShaderLoader( m_pRHIDevice );
	m_pShaderManager = new CShaderManager();
	m_pTextureManager = new CTextureManager( m_pRHIDevice );
	m_pFontManager = new CFontManager( m_pRHIDevice );
	ret = m_pFontManager->Initial();
	if ( ret.IsOK() == false )
		return ret;
	//m_pPaper2dLoader = new CPaper2dLoader();

	// 初始化默认的渲染状态
	ret = InitDefaultRenderState();
	if ( ret.IsOK() == false )
		return ret;

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
	m_pGuiEngine = new Gui::CEngine( m_pRenderer, m_pCommandQueue, m_pMemoryRes );
	ret = m_pGuiEngine->Initial();
	if ( ret.IsOK() == false )
		return ret;
	m_pGuiEngine->SetCanvasResolution( m_pRHIDevice->GetContextSize() );

	// 创建Gui的XML解释器
	//m_pGuiXmlParser = new Gui::CXmlParser();
	//m_pGuiXmlParser->SetLogOutputDevice( m_pLoggingOutputDevice );
	//m_pGuiXmlParser->RegisterGetTexture( std::bind( &CTextureManager::GetOrCreateTextureA, m_pTextureManager, std::placeholders::_1, std::placeholders::_2 ) );
	//m_pGuiXmlParser->RegisterGetFont( [=]( Renderer::CFont** ppOutFont, const UFontDesc& FontDesc )
	//	{
	//		if ( m_pFontManager->GetFont( ppOutFont, FontDesc ) == true )
	//			return CResult();
	//		if ( m_pFontManager->GetFontDefault( ppOutFont ) == true )
	//			return CResult();
	//		return MakeResult( ResultCode::Bad, "Can't find font" );
	//	} );

	//m_pGuiXmlParser->RegisterCreateProc_Widget( "Default", std::bind( &Gui::XmlCreateProc_Widget, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Widget( "Button", std::bind( &Gui::XmlCreateProc_Widget_Button, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Widget( "CheckBox", std::bind( &Gui::XmlCreateProc_Widget_CheckBox, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Widget( "RadioBox", std::bind( &Gui::XmlCreateProc_Widget_RadioBox, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Widget( "ListBox", std::bind( &Gui::XmlCreateProc_Widget_ListBox, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Widget( "EditBox", std::bind( &Gui::XmlCreateProc_Widget_EditBox, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Widget( "ProgressBar", std::bind( &Gui::XmlCreateProc_Widget_ProgressBar, std::placeholders::_1 ) );

	//m_pGuiXmlParser->RegisterCreateProc_Element( "Default", std::bind( &Gui::XmlCreateProc_Element, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Element( "ColorRect", std::bind( &Gui::XmlCreateProc_Element_ColorRect, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Element( "StateTexture", std::bind( &Gui::XmlCreateProc_Element_StateTexture, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Element( "StateLabel", std::bind( &Gui::XmlCreateProc_Element_StateLabel, std::placeholders::_1 ) );
	//m_pGuiXmlParser->RegisterCreateProc_Element( "TextField", std::bind( &Gui::XmlCreateProc_Element_EditBoxText, std::placeholders::_1 ) );

	//m_pGuiXmlParser->RegisterReadProc_Widget( "Default", std::bind( &Gui::XmlReadProc_Widget,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	//m_pGuiXmlParser->RegisterReadProc_Widget( "Button", std::bind( &Gui::XmlReadProc_Widget_Button,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	//m_pGuiXmlParser->RegisterReadProc_Widget( "CheckBox", std::bind( &Gui::XmlReadProc_Widget_CheckBox,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	//m_pGuiXmlParser->RegisterReadProc_Widget( "RadioBox", std::bind( &Gui::XmlReadProc_Widget_RadioBox,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	//m_pGuiXmlParser->RegisterReadProc_Widget( "ListBox", std::bind( &Gui::XmlReadProc_Widget_ListBox,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	//m_pGuiXmlParser->RegisterReadProc_Widget( "EditBox", std::bind( &Gui::XmlReadProc_Widget_EditBox,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );
	//m_pGuiXmlParser->RegisterReadProc_Widget( "ProgressBar", std::bind( &Gui::XmlReadProc_Widget_ProgressBar,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 ) );

	//m_pGuiXmlParser->RegisterReadProc_Element( "Default", std::bind( &Gui::XmlReadProc_Element,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	//m_pGuiXmlParser->RegisterReadProc_Element( "ColorRect", std::bind( &Gui::XmlReadProc_Element_ColorRect,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	//m_pGuiXmlParser->RegisterReadProc_Element( "StateTexture", std::bind( &Gui::XmlReadProc_Element_StateTexture,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	//m_pGuiXmlParser->RegisterReadProc_Element( "StateLabel", std::bind( &Gui::XmlReadProc_Element_StateLabel,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	//m_pGuiXmlParser->RegisterReadProc_Element( "TextField", std::bind( &Gui::XmlReadProc_Element_EditBoxText,
	//	std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );


	// 创建Paper2d
	//m_pPaper2dDirector = new Paper2d::CDirector( m_pRendererSprite );


	// 创建设备输入控制器
	m_pInput = new CInput();
	m_pInput->AddMouseEvent( std::bind( &Gui::CEngine::HandleMouse, m_pGuiEngine, std::placeholders::_1 ) );
	//m_pInput->AddMouseEvent( std::bind( &Paper2d::CDirector::HandleMouse, m_pPaper2dDirector, std::placeholders::_1 ) );
	m_pInput->AddKeyboardEvent( std::bind( &Gui::CEngine::HandleKeyboard, m_pGuiEngine, std::placeholders::_1 ) );
	//m_pInput->AddKeyboardEvent( std::bind( &Paper2d::CDirector::HandleKeyboard, m_pPaper2dDirector, std::placeholders::_1 ) );

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
	RHI::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	Gui::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	//Paper2d::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	Game::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	MsgPack::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	Json::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	Xml::RegisterLuaCpp( m_pLuaDevice->GetLuaState() );
	if ( m_pNetworkProcAddressInfo && m_pNetworkProcAddressInfo->pLuaCppRegister )
		m_pNetworkProcAddressInfo->pLuaCppRegister( m_pLuaDevice->GetLuaState() );

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


iberbar::CResult iberbar::Game::CApplication::CreateLog()
{
	// 初始化Logging
	m_pLoggingOutputDevice = new Logging::COutputDeviceFile( CResourceFileSystem::GetResoucePath( "Game.log" ).c_str() );

	return CResult();
}


iberbar::CResult iberbar::Game::CApplication::CreateRHI()
{
	CResult Ret;

	RECT ClientRect;
	::GetClientRect( m_hWnd, &ClientRect );

	std::string strRhiApiName = GetRhiApiName( m_Configuration.nRHIApi );
	if ( StringIsNullOrEmpty( strRhiApiName.c_str() ) )
		return MakeResult( ResultCode::Bad, "Unknown rhi api" );

	std::string strLibName = "iberbarRHI." + strRhiApiName;
	m_pDynamicLib_Rhi = new OS::CDynamicLibrary();
	Ret = m_pDynamicLib_Rhi->Load( strLibName.c_str() );
	if ( Ret.IsOK() == false )
		return Ret;

	typedef RHI::IDevice* ( *PCallbackRhiDeviceCreate )( );
	PCallbackRhiDeviceCreate pCallbackRhiDeviceCreate = ( PCallbackRhiDeviceCreate )m_pDynamicLib_Rhi->GetProcAddress( "iberbarRhiDeviceCreate" );
	if ( pCallbackRhiDeviceCreate == nullptr )
		return MakeResult( ResultCode::Bad, "Can't find the function to create dynamic rhi device" );

	m_pRHIDevice = pCallbackRhiDeviceCreate();
	if ( m_pRHIDevice == nullptr )
		return MakeResult( ResultCode::Bad, "Failed to create dynamic rhi device" );

	m_pRHIDevice->SetCallbackOnCreated( std::bind( &CApplication::RhiDeviceCreate, this ) );
	m_pRHIDevice->SetCallbackOnLost( std::bind( &CApplication::RhiDeviceLost, this ) );
	m_pRHIDevice->SetCallbackOnReset( std::bind( &CApplication::RhiDeviceReset, this ) );

#ifdef _WINDOWS
	Ret = m_pRHIDevice->CreateDevice(
		m_hWnd,
		m_Configuration.bWindow,
		ClientRect.right - ClientRect.left,
		ClientRect.bottom - ClientRect.top );
#endif
	if ( Ret.IsOK() == false )
		return Ret;



	m_pRHIDevice->SetClearColor( CColor4B( 255, 255, 0, 0 ) );

	return CResult();
}


iberbar::CResult iberbar::Game::CApplication::CreateNetwork()
{
	CResult Ret;

	if ( m_Configuration.bUseNetwork == false )
		return CResult();

	m_pDynamicLib_Network = new OS::CDynamicLibrary();
	Ret = m_pDynamicLib_Network->Load( "iberbarNetwork" );
	if ( Ret.IsOK() == false )
		return Ret;

	m_pNetworkProcAddressInfo = new UNetworkProcAddressInfo();
	m_pNetworkProcAddressInfo->pInitial = (IO::PFunctionInitial)m_pDynamicLib_Network->GetProcAddress( "iberbarNetworkInitial" );
	m_pNetworkProcAddressInfo->pDestroy = (IO::PFunctionDestroy)m_pDynamicLib_Network->GetProcAddress( "iberbarNetworkDestroy" );
	m_pNetworkProcAddressInfo->pRead = (IO::PFunctionRead)m_pDynamicLib_Network->GetProcAddress( "iberbarNetworkRead" );
	m_pNetworkProcAddressInfo->pLuaCppRegister = (IO::PFunctionLuaCppRegister)m_pDynamicLib_Network->GetProcAddress( "iberbarNetworkRegisterLuaCpp" );

	return CResult();
}


iberbar::CResult iberbar::Game::CApplication::InitDefaultRenderState()
{
	CResult Ret;

	TSmartRefPtr<RHI::IBlendState> pBlendState = nullptr;
	RHI::UBlendDesc BlendDesc;
	memset( &BlendDesc, 0, sizeof( RHI::UBlendDesc ) );
	BlendDesc.RenderTargets[ 0 ].BlendEnable = true;
	BlendDesc.RenderTargets[ 0 ].SrcBlend = RHI::EBlend::SrcAlpha;
	BlendDesc.RenderTargets[ 0 ].DestBlend = RHI::EBlend::InvSrcAlpha;
	BlendDesc.RenderTargets[ 0 ].BlendOp = RHI::EBlendOP::Add;
	BlendDesc.RenderTargets[ 0 ].SrcBlendAlpha = RHI::EBlend::One;
	BlendDesc.RenderTargets[ 0 ].DestBlendAlpha = RHI::EBlend::Zero;
	BlendDesc.RenderTargets[ 0 ].BlendOpAlpha = RHI::EBlendOP::Add;
	Ret = m_pRHIDevice->CreateBlendState( &pBlendState, BlendDesc );
	if ( Ret.IsOK() == false )
		return Ret;

	//m_pRenderer->GetRHIContext()->SetBlendStateDefault( pBlendState );

	return CResult();
}


iberbar::CResult iberbar::Game::CApplication::LoadDefaultShaders()
{
	CResult ret;

	std::string strShaderRootDir = "Shaders/";
	const char* strRhiApiName = GetRhiApiName( m_Configuration.nRHIApi );
	if ( StringIsNullOrEmpty( strRhiApiName ) )
		return MakeResult( ResultCode::Bad, "Unknown rhi api" );
	strShaderRootDir += strRhiApiName;

	m_pShaderLoader->SetRootDir( strShaderRootDir.c_str() );

	{
		//RHI::UVertexElement VertexElements[] =
		//{
		//	{ 0, RHI::UVertexDeclareUsage::Position, 0, RHI::UVertexFormat::FLOAT3, offsetof( Renderer::UVertex_V3F_C4B_T2F, position ) },
		//	{ 0, RHI::UVertexDeclareUsage::Color, 0, RHI::UVertexFormat::FLOAT4, offsetof( Renderer::UVertex_V3F_C4B_T2F, color ) },
		//	{ 0, RHI::UVertexDeclareUsage::TexCoord, 0, RHI::UVertexFormat::FLOAT2, offsetof( Renderer::UVertex_V3F_C4B_T2F, texcoord ) }
		//};
		TSmartRefPtr<RHI::IShaderProgram> pShaderProgram;
		ret = m_pShaderLoader->LoadShaderProgram( "PositionColorTexture2d", &pShaderProgram );
		if ( ret.IsOK() == false )
			return ret;

		m_pShaderManager->AddShaderProgram( "PositionColorTexture2d" , pShaderProgram );

		ret = m_pShaderLoader->LoadShaderProgram( "PositionColor2d", &pShaderProgram );
		if ( ret.IsOK() == false )
			return ret;
		m_pShaderManager->AddShaderProgram( "PositionColor2d", pShaderProgram );

		//TSmartRefPtr<RHI::IVertexDeclaration> pVertexDeclaration;
		//uint32 nStrides[1] = { sizeof( Renderer::UVertex_V3F_C4B_T2F ) };
		//ret = m_pRHIDevice->CreateVertexDeclaration( &pVertexDeclaration, VertexElements, 3, nStrides, 1 );
		//if ( ret.IsOK() == false )
		//	return ret;

		//TSmartRefPtr<RHI::IShaderState> pShaderState;
		//ret = m_pRHIDevice->CreateShaderState( &pShaderState, pVertexDeclaration, pVertexShader, pPixelShader, nullptr, nullptr, nullptr );
		//if ( ret.IsOK() == false )
		//	return ret;
	}

	return CResult();
}


void iberbar::Game::CApplication::OnRunTimer( int64 nElapsedTimeMilliSecond, float nElapsedTimeSecond )
{
	// Network IO 读取
	if ( m_pNetworkProcAddressInfo && m_pNetworkProcAddressInfo->pRead )
		m_pNetworkProcAddressInfo->pRead();

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


	//m_pPaper2dDirector->UpdateScene( nElapsedTimeSecond );
	m_pGuiEngine->Update( nElapsedTimeMilliSecond, nElapsedTimeSecond );
	OnUpdate( nElapsedTimeMilliSecond, nElapsedTimeSecond );

	// loading 解锁
	//if ( m_pLoadingThread->IsWakeup() )
	//	m_pLoadingThread->Unlock();

	if ( m_bWndActive == true )
	{
		//OnRender();
		//m_pPaper2dDirector->DrawScene();
		m_pGuiEngine->Render();

		CResult RetRhiBegin = m_pRHIDevice->Begin();
		if ( RetRhiBegin.IsOK() )
		{
			OnRender();
			m_pRenderer->Render();
			m_pRHIDevice->End();
		}
		else
		{
			std::string strErr = StdFormat( "Failed to begin draw: %s", RetRhiBegin.data.c_str() );
			m_pLoggingOutputDevice->Serialize(
				Logging::ULevel::Error,
				strErr.c_str(),
				"Rhi" );
		}
	}

}


void iberbar::Game::CApplication::AddLoadingTask( CLoadingTask* pTask )
{
	if ( m_pLoadingThread )
		m_pLoadingThread->AddTask( pTask );
}


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



//iberbar::Game::CApplication* iberbar::Game::GetApp()
//{
//	return s_pApplication;
//}








inline const char* iberbar::Game::GetRhiApiName( RHI::UApiType nApiType )
{
	switch ( nApiType )
	{
		case RHI::UApiType::OpenGL:
			return "OpenGL";
#ifdef _WINDOWS
		case RHI::UApiType::D3D9:
			return "D3D9";
		case RHI::UApiType::D3D11:
			return "D3D11";
		case RHI::UApiType::D3D12:
			return "D3D12";
#endif
		default:break;
	}
	return "";
}


void iberbar::Game::OnPreloadTexture( const CResourcePreloader::ULoadTextureContext& Context )
{
	CResult ret = CApplication::sGetApp()->GetTextureManager()->GetOrCreateTextureA( Context.strFile, nullptr );
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
	CResult ret = CApplication::sGetApp()->GetFontManager()->GetOrCreateFont( nullptr, FontDesc, Renderer::UFontCharVocabularyType::Unknown );
	if ( ret.IsOK() == false )
	{
	}
}

