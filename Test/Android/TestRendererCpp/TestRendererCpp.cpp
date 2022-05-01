

#include "TestRendererCpp.h"
#include <iberbar/Utility/Platform/Android/JniHelper.h>
#include <iberbar/GameEngine/BaseResourceManager.h>



#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "TestRendererCpp", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "TestRendererCpp", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "TestRendererCpp", __VA_ARGS__))


CTestApplication g_app;

JNIEXPORT jint JNICALL JNI_OnLoad( JavaVM* vm, void* )
{
	JNIEnv* env = nullptr;
	if ( vm->GetEnv( (void**)&env, JNI_VERSION_1_6 ) != JNI_OK ) {
		LOGE( "ERROR: GetEnv failed" );
		return -1;
	}

	assert( env != nullptr );

	iberbar::Game::UApplicationCreateParams appInitData;
	appInitData.strAppName = TEXT( "TestRenderer" );
	appInitData.nRHIApi = iberbar::RHI::UApiType::OpenGL;

	g_app.SetConfiguration( appInitData );
	
	iberbar::CResult ret = g_app.SetJNIEnv( env );
	if ( ret.IsOK() == false )
		return -1;

	return JNI_VERSION_1_6;
}





void CTestApplication::OnCreated()
{
	iberbar::CResult ret;

	ret = m_ResourcePreloader.ReadFile( "Scripts/Preload.xml" );

	//iberbar::UFontDesc FontDesc;
	//FontDesc.FamilyName = "./Fonts/arial.ttf";
	//FontDesc.Size = 21;
	//FontDesc.Weight = 1000;
	//m_pFontEngine->GetFont( nullptr, FontDesc );

	iberbar::TSmartRefPtr<CMainMenu> pMainMenu = iberbar::TSmartRefPtr<CMainMenu>::_sNew();
	pMainMenu->SetPosition( iberbar::CPoint2i( 0, 0 ) );
	pMainMenu->SetSize( iberbar::CSize2i( 800, 600 ) );
	m_pGuiXmlParser->PushWidget( pMainMenu );
	m_pGuiXmlParser->ReadFile( iberbar::Game::CResourceFileSystem::GetResoucePath( "Scripts/MainMenu.xml" ).c_str() );
	m_pGuiXmlParser->PopWidget();
	m_pGuiEngine->AddUI( pMainMenu, true );
}


void CTestApplication::OnDestroy()
{
}


void OnExportAssets( const std::string& strAssetFile, const std::string& strExportFile, const iberbar::CResult& exportResult )
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


iberbar::CResult CTestApplication::ExportAssetsPreInitial()
{
	CApplication::ExportAssetsPreInitial();

	iberbar::Android::CAssetsExportUtil util;
	auto f = std::bind( &OnExportAssets, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
	util.ExportDir( "Images", "", std::bind( &OnExportAssets, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	util.ExportDir( "Fonts", "", std::bind( &OnExportAssets, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
	util.ExportDir( "Scripts", "", std::bind( &OnExportAssets, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );

	return iberbar::CResult();
}

