

#include <Windows.h>
#include <iberbar/GameEngine/Application.h>

//#include <iberbar/GameEngine/Paper2dLoader.h>
#include <iberbar/GameEngine/ResourcePreloader.h>
#include <iberbar/GameEngine/FontManager.h>
#include <iberbar/GameEngine/LoadingThread.h>

#include <iberbar/GameEngine/ShaderManager.h>
#include <iberbar/GameEngine/TextureManager.h>

//#include <iberbar/Paper2d/Director.h>
//#include <iberbar/Paper2d/Scene.h>
//#include <iberbar/Paper2d/Camera.h>
//#include <iberbar/Paper2d/Image.h>
//#include <iberbar/Paper2d/Animation.h>
//#include <iberbar/Paper2d/Terrain.h>

//#include <iberbar/Gui/Widgets/EditBox.h>
//#include <iberbar/Gui/Element/ElemColorRect.h>
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/Element/ElemStateTexture.h>

//#include <iberbar/Renderer/RendererSprite.h>

#include <iberbar/Network/IO.h>

#include <iberbar/Lua/LuaDevice.h>
#include <iberbar/Lua/FunctionHelper.h>

#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/RenderState.h>
#include <iberbar/RHI/Shader.h>
#include <iberbar/RHI/TestDraw.h>

#include <iberbar/Utility/RefStatistics.h>


#pragma comment(lib,"winmm.lib")

float GetFPS()
{
	static float  fps = 0;
	static int    frameCount = 0;
	static float  currentTime = 0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;//每调用一次Get_FPS()函数，帧数+1
	currentTime = timeGetTime() * 0.001f;//获取系统时间,并转化成秒为单位
	//经过了一秒，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if ( currentTime - lastTime > 1.0f ) //将时间控制在1秒钟
	{
		fps = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime作为下一秒的基准时间
		frameCount = 0;//将本次帧数frameCount值清零
	}
	return fps;
}


class CTestApplication
	: public iberbar::Game::CApplication
{
public:
	CTestApplication();

protected:
	virtual iberbar::CResult OnCreated() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate( int64 nElapsedTimeMilliSecond, float nElapsedTimeSecond ) override;
	virtual void OnRender() override;

public:
	iberbar::RHI::ITestDraw* m_pTestDraw;
};



CTestApplication::CTestApplication()
	: m_pTestDraw( nullptr )
{
}


iberbar::CResult CTestApplication::OnCreated()
{
	iberbar::CResult ret;

	//ret = m_ResourcePreloader->ReadFile( "Scripts/Preload.xml" );

	//iberbar::TSmartRefPtr<iberbar::Renderer::CFont> pFont = nullptr;
	//if ( m_pFontManager->GetFontDefault( &pFont ) )
	//{
	//	pFont->LoadText( L"fps: 0123456798." );
	//}
	m_pLuaDevice->AddLuaPath( "Scripts/Game2/?.lua" );
	m_pLuaDevice->ExecuteFile( "Scripts/Game2/main.lua" );
	
	iberbar::Lua::CFunctionHelper::sExecuteGlobalFunction( m_pLuaDevice->GetLuaState(), "Main" );

	iberbar::TSmartRefPtr<iberbar::Gui::CDialog> dlg = nullptr;
	iberbar::Gui::CDialog::sCreateDialog( &dlg );
	dlg->SetId( "MainMenu" );
	dlg->SetPosition( 0, 0 );
	dlg->SetSize( 800, 600 );

	iberbar::TSmartRefPtr<iberbar::Gui::CWidget> bg = iberbar::TSmartRefPtr<iberbar::Gui::CWidget>::_sNew();
	bg->SetSize( 800, 600 );

	iberbar::TSmartRefPtr<iberbar::RHI::IShaderProgram> pShaderProgram;
	m_pShaderManager->GetShaderProgram( "PositionColorTexture2d", &pShaderProgram );

	iberbar::TSmartRefPtr<iberbar::RHI::ITexture> pTexture;
	m_pTextureManager->GetOrCreateTextureA( "Images/Background.png", &pTexture );

	iberbar::TSmartRefPtr<iberbar::RHI::ISamplerState> pSamplerState;
	iberbar::RHI::UTextureSamplerState SamplerStateDesc;
	m_pRHIDevice->CreateSamplerState( &pSamplerState, SamplerStateDesc );

	float Viewport[ 2 ] = { 800, 600 };
	iberbar::TSmartRefPtr<iberbar::Renderer::CMaterial> mat = iberbar::TSmartRefPtr<iberbar::Renderer::CMaterial>::_sNew();
	mat->Initial( pShaderProgram );
	mat->SetTexture( "g_texture", pTexture );
	mat->SetSamplerState( "g_textureSampler", pSamplerState );
	mat->SetInt( "g_rhw", 1 );
	mat->SetFloat( "g_viewport_w", Viewport[0] );
	mat->SetFloat( "g_viewport_h", Viewport[1] );

	iberbar::TSmartRefPtr<iberbar::Gui::CElementStateTexture> element_texture = iberbar::TSmartRefPtr<iberbar::Gui::CElementStateTexture>::_sNew();
	bg->SetRenderElement( element_texture );
	element_texture->SetPosition( 0, 0 );
	element_texture->SetSize( 800, 600 );
	element_texture->SetAlignHorizental( iberbar::UAlignHorizental::Left );
	element_texture->SetAlignVertical( iberbar::UAlignVertical::Top );
	//element_texture->SetSize( 100, 100 );
	//element_texture->SetPercentW( true );
	//element_texture->SetPercentH( true );
	element_texture->SetUV( iberbar::CRect2f( 0.0, 0.0, 1.0, 1.0 ) );
	element_texture->SetMaterial( mat );

	dlg->GetWidgetRoot()->AddWidget( bg );

	m_pTestDraw = m_pRHIDevice->CreateTestDraw();
	m_pTestDraw->SetShaderProgram( pShaderProgram );
	m_pTestDraw->SetTexture( pTexture );

	//auto pEditBox = iberbar::TSmartRefPtr<iberbar::Gui::CEditBox>::_sNew();
	//auto pEditBoxTextElement = iberbar::TSmartRefPtr<iberbar::Gui::CEditBoxTextElement>::_sNew();
	//auto pEditBoxBgElement = iberbar::TSmartRefPtr<iberbar::Gui::CElementColorRect>::_sNew();

	//pEditBox->SetRenderElement( pEditBoxBgElement );
	//pEditBox->SetTextElementRef( pEditBoxTextElement );
	//pEditBox->SetSize( iberbar::CSize2i( 200, 100 ) );
	//pEditBox->SetPosition( 100, 100 );

	//pEditBoxBgElement->Init();
	//pEditBoxBgElement->SetColor( -1, iberbar::CColor4B( 0xffffffff ) );
	//pEditBoxBgElement->SetSize( iberbar::CSize2i( 200, 100 ) );
	//pEditBoxBgElement->SetZOrder( 99997 );
	//pEditBoxBgElement->AddChildElement( pEditBoxTextElement );

	//pEditBoxTextElement->SetFont( pFont );
	//pEditBoxTextElement->SetTextColor( iberbar::CColor4B( 0xff000000 ) );
	//pEditBoxTextElement->SetSelTextColor( iberbar::CColor4B( 0xffffffff ) );
	//pEditBoxTextElement->SetSelBgColor( iberbar::CColor4B( 0xff3390ff ) );
	//pEditBoxTextElement->SetCaretColor( iberbar::CColor4B( 0xff000000 ) );
	//pEditBoxTextElement->SetZOrder( 99999 );
	//pEditBoxTextElement->SetBgZOrder( 99998 );
	//pEditBoxTextElement->SetSize( iberbar::CSize2i( 200, 100 ) );
	//pEditBoxTextElement->SetTextAlignHorizental( iberbar::UAlignHorizental::Left );

	//auto pDialog = iberbar::Gui::CEngine::sGetInstance()->GetDialog( "MainMenu" );

	//pDialog->GetWidgetRoot()->AddWidget( pEditBox );

#ifdef _DEBUG
	FILE* f = nullptr;
	fopen_s( &f, "SnapShot.txt", "wt" );
	if ( f != nullptr )
	{
		iberbar::CRefStatistics::sGetShared()->ForEach(
			[=]( iberbar::CRef* pRefZombie )
			{
#ifdef _WINDOWS
				auto n = typeid( *pRefZombie ).name();
				auto r = pRefZombie->Refcount();
				std::string strText = iberbar::StdFormat( "0x%016llx: TypeName=<%s> Ref=%d\n", (uint64)pRefZombie, n, r );
#else
				std::string strText = iberbar::StdFormat( "0x%016llx: Ref=%d\n", (uint64)pRefZombie, pRefZombie->Refcount() );
#endif
				fwrite( strText.c_str(), 1, strlen( strText.c_str() ), f );
			} );
		fclose( f );
		f = nullptr;
	}
#endif

	return iberbar::CResult();
}


void CTestApplication::OnDestroy()
{
	SAFE_DELETE( m_pTestDraw );
}


void CTestApplication::OnUpdate( int64 nElapsedTimeMilliSecond, float nElapsedTimeSecond )
{
}


void CTestApplication::OnRender()
{
	//if ( m_pTestDraw )
		//m_pTestDraw->Draw();
	//iberbar::TSmartRefPtr<iberbar::Renderer::CFont> pFont = nullptr;
	//if ( m_pFontManager->GetFontDefault( &pFont ) )
	//{
	//	iberbar::CRect2i rect( 0, 100, 200, 150 );
	//	wchar_t strText[ 256 ];
	//	swprintf_s( strText, L"fps: %4.4f", GetFPS() );
	//	m_pRendererSprite->DrawText( INT_MAX, pFont, nullptr, strText, -1, &rect, iberbar::CColor4B( 0xffffffff ), iberbar::UFontDrawTextOptions() );
	//}
}


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
{
	//std::regex regex( "rgb\\\(([0-9a-fA-F].*),([0-9a-fA-F].*),([0-9a-fA-F].*)\\\)" );
	//std::match_results<const char*> match;
	//if ( std::regex_match( "rgb(255,0x11,33)", match, regex ) )
	//{
	//	for ( const auto& m : match )
	//	{
	//		std::string s = m.str();
	//		m.first;
	//	}
	//}

	const wchar_t* pStr = L"sfuon\nsofnxg\nsoo\r\n";
	std::wregex Regex( L"(\\\n)|(\\\r\\\n)" );
	std::wstring strFmt;
	std::wstring strInsert = std::regex_replace( pStr, Regex, strFmt );

	int n = sizeof( wchar_t );

	iberbar::Game::UApplicationCreateParams appInitData;
	appInitData.strAppName = TEXT( "TestRenderer2" );
	appInitData.nWndWidth = 800;
	appInitData.nWndHeight = 600;
	appInitData.bFullScreen = false;
	appInitData.nRHIApi = iberbar::RHI::UApiType::D3D11;
	appInitData.bUseLoadingThread = true;
	CTestApplication app;
	app.SetConfiguration( appInitData );
	iberbar::CResult Ret = app.Initial( hInstance );
	if ( Ret.IsOK() == false )
	{
		::MessageBoxA( nullptr, Ret.data.c_str(), "", MB_ICONERROR );
		return 0;
	}
	app.Run();
	return 0;
}