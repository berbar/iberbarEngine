
#ifdef __ANDROID__

#include <iberbar/GameEngine/Application.h>


iberbar::CResult iberbar::Game::CApplication::Initial()
{
	CResult ret;

	ret = CreateLog();
	if ( ret.IsOK() == false )
		return ret;

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


void iberbar::Game::CApplication::Quit()
{
	jclass pClass = m_pJNIEnv->FindClass( "com/iberbar/lib/MyHelper" );
	if ( pClass == nullptr )
		return;

	jmethodID pMethodId = nullptr;

	pMethodId = m_pJNIEnv->GetStaticMethodID( pClass, "tryQuit", "()V" );
	if ( pMethodId != nullptr )
	{
		m_pJNIEnv->CallStaticObjectMethod( pClass, pMethodId );
	}

}

#endif