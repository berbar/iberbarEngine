
#include <iberbar/GameEngine/Timer.h>





iberbar::Game::CTimer::CTimer()
	: m_bEnabled( false )
	, m_bLoop( false )
	, m_bPause( true )
	, m_bRemoveSelf( false )
	, m_nTimeout( 0.0f )
	, m_nCountdown( 0.0f )
	, m_nScale( 1.0f )
	, m_strId()
	, m_CallbackExecute()
{
}


iberbar::Game::CTimer::~CTimer()
{
}


bool iberbar::Game::CTimer::Start( float fTimeout, bool bLoop )
{
	// 需要先停止，或者刚刚创建的timer
	if ( m_bEnabled == true )
		return false;

	if ( fTimeout < 0.0001f )
		return false;

	m_bEnabled      = true;
	m_nTimeout      = fTimeout;
	m_nCountdown    = fTimeout;
	m_bLoop         = bLoop;
	m_nScale        = 1.0f;

	Resume();

	return true;
}


void iberbar::Game::CTimer::Stop()
{
	m_bEnabled = false;
}


void iberbar::Game::CTimer::Pause()
{
	m_bPause = true;
}


void iberbar::Game::CTimer::Resume()
{
	m_bPause = false;
}


void iberbar::Game::CTimer::Scale( float nScaleParams )
{
	m_nScale = nScaleParams;
}


void iberbar::Game::CTimer::Run( float nDelta )
{
	if ( m_bEnabled == false || m_bPause == true )
		return;

	float nDeltaFinal = nDelta * m_nScale;
	m_nCountdown -= nDeltaFinal;

	if ( m_nCountdown < 0.0f )
	{
		if ( m_bLoop )
			m_nCountdown += m_nTimeout;
		else
			m_bEnabled = false;

		if ( m_CallbackExecute )
			m_CallbackExecute( this );
	}
}






iberbar::Game::CTimerEasySystem::CTimerEasySystem()
	: m_Timers()
{
}


iberbar::Game::CTimerEasySystem::~CTimerEasySystem()
{
	auto iter = m_Timers.begin();
	auto end = m_Timers.end();
	for ( ; iter != end; iter++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( iter->pTimer );
	}
	m_Timers.clear();
}


void iberbar::Game::CTimerEasySystem::Run( float nDelta )
{
	auto iter = m_Timers.begin();
	auto end = m_Timers.end();
	for ( ; iter != end; )
	{
		if ( iter->bRemove == true )
		{
			iter->pTimer->Release();
			iter->pTimer = nullptr;

			iter = m_Timers.erase( iter );
		}
		else
		{
			iter->pTimer->Run( nDelta );

			iter++;
		}

	}
}


void iberbar::Game::CTimerEasySystem::AddTimer( CTimer* pTimer )
{
	if ( pTimer == nullptr )
		return;

	_TimerNode node;
	node.bRemove = false;
	node.pTimer = pTimer;
	m_Timers.push_back( node );

	pTimer->AddRef();
}


bool iberbar::Game::CTimerEasySystem::FindTimer( const std::string& strId, CTimer** ppOutTimer )
{
	auto iter = m_Timers.begin();
	auto end = m_Timers.end();
	for ( ; iter != end; iter++ )
	{
		if ( iter->pTimer->GetId() == strId )
		{
			if ( ppOutTimer )
			{
				UNKNOWN_SAFE_RELEASE_NULL( *ppOutTimer );
				(*ppOutTimer) = iter->pTimer;
				(*ppOutTimer)->Release();
			}
			return true;
		}
	}

	return false;
}


void iberbar::Game::CTimerEasySystem::RemoveTimer( CTimer* pTimer )
{
	auto iter = m_Timers.begin();
	auto end = m_Timers.end();
	for ( ; iter != end; iter++ )
	{
		if ( iter->pTimer == pTimer )
		{
			iter->bRemove = true;
			break;
		}
	}
}



