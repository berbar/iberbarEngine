
#include "Timer.h"



//---------------------------------------------------------------------------------------------------
iberbar::CTimeline::CTimeline( void )
	: m_bStarted( false )
	, m_bEnable( true )
	, m_nScaleParam( 1.0f )
	, m_timers()
	, m_timelines()
{
}


//---------------------------------------------------------------------------------------------------
iberbar::CTimeline::~CTimeline()
{
	m_timers.Clear();
	m_timersTemp.Clear();

	m_timelines.Clear();
	m_timelinesTemp.Clear();
}


//---------------------------------------------------------------------------------------------------
void iberbar::CTimeline::run( float nElapsedTime )
{
	if ( m_bStarted == false )
		return;
	if ( m_bEnable == false )
		return;

	float lc_nNewElapsedTime = nElapsedTime * m_nScaleParam;

	//
	// 把新增加的timer添加到更新队列中，然后清空新增队列
	//
	if ( m_timersTemp.IsEmpty() == false )
	{
		// 将Temp中的timer移动到正式队列中
		TimerGroup::iterator lc_iter = m_timersTemp.begin();
		TimerGroup::iterator lc_end  = m_timersTemp.end();
		for ( ; lc_iter != lc_end; lc_iter++ )
			m_timers.push_back( *lc_iter );

		// 清空Temp队列
		m_timersTemp.Clear();
	}


	//
	// 把Temp中的timeline添加到更新队列中，然后清空Temp队列
	//
	if ( m_timelinesTemp.IsEmpty() == false )
	{
		// 将Temp中的timeline移动到正式队列中
		TimelineGroup::iterator lc_iter = m_timelinesTemp.begin();
		TimelineGroup::iterator lc_end  = m_timelinesTemp.end();
		for ( ; lc_iter != lc_end; lc_iter++ )
			m_timelines.push_back( *lc_iter );

		// 清空Temp队列
		m_timelinesTemp.Clear();
	}

	//
	// 更新队列进行更新
	//
	if ( m_timers.IsEmpty() == false )
	{
		TimerGroup::iterator lc_node = m_timers.begin();
		TimerGroup::iterator lc_end  = m_timers.end();
		Timer lc_timer( NULL );
		for ( ; lc_node != lc_end; )
		{
			lc_timer = ( *lc_node );

			if ( lc_timer->IsEnable() == false )
			{
				lc_timer->onDestroy();
				lc_node = m_timers.erase( lc_node );
				lc_end  = m_timers.end();
			}
			else
			{
				lc_timer->onRun( lc_nNewElapsedTime );

				lc_node ++;
			}

			lc_timer = NULL;
		}
	}

	//
	// 更新子时间线
	//
	if ( m_timelines.IsEmpty() == false )
	{
		TimelineGroup::iterator lc_node = m_timelines.begin();
		TimelineGroup::iterator lc_end  = m_timelines.end();
		Timeline lc_timeline( NULL );
		for ( ; lc_node != lc_end; )
		{
			lc_timeline = ( *lc_node );

			if ( lc_timeline->IsEnable() == false )
			{
				lc_node = m_timelines.erase( lc_node );
				lc_end  = m_timelines.end();
			}
			else
			{
				lc_timeline->run( lc_nNewElapsedTime );

				lc_node ++;
			}

			lc_timeline = NULL;
		}
	}
}


//---------------------------------------------------------------------------------------------------
iberbar::Timer iberbar::CTimeline::addTimer()
{
	// 创建timer对象
	Timer lc_timer = NULL;
	lc_timer.attach( new CTimer() );

	// 将timer附属到该时间轴上
	// 先加到Temp组中，等到run的时候将Temp中的timer移到更新队列中
	m_timersTemp.push_back( lc_timer );

	return lc_timer;
}


//---------------------------------------------------------------------------------------------------
iberbar::Timer iberbar::CTimeline::getTimer( const std::string& name )
{
	return nullptr;
}


//---------------------------------------------------------------------------------------------------
void iberbar::CTimeline::removeTimer( Timer timer )
{
	if ( timer )
	{
		timer->pause();
		timer->stop();
	}
}


//---------------------------------------------------------------------------------------------------
iberbar::Timeline iberbar::CTimeline::addTimeline()
{
	// 创建timeline对象
	Timeline lc_timeline = NULL;
	lc_timeline.attach( new CTimeline() );

	// 将timeline附属到该时间轴上
	// 先加到Temp组中，等到run的时候将Temp中的timeline移到更新队列中
	m_timelinesTemp.push_back( lc_timeline );

	return lc_timeline;
}








//---------------------------------------------------------------------------------------------------
iberbar::CTimer::CTimer( void )
	: m_fTimeout( 0.0f )
	, m_fCountdown( 0.0f )
	, m_bEnabled( false )
	, m_bLoop( false )
	, m_bPause( true )
	, m_pTimerHandler( NULL )
	, m_pTimerFunc( NULL )
	, m_bEnableScale( false )
	, m_nScale( 1.0f )
{
}


//---------------------------------------------------------------------------------------------------
iberbar::CTimer::~CTimer()
{
}


//---------------------------------------------------------------------------------------------------
bool iberbar::CTimer::start( float fTimeout, bool bLoop, PTR_CUnknown pTimerHandler, PTimerProc pTimerProc )
{
	// 需要先停止，或者刚刚创建的timer
	if ( m_bEnabled == true )
		return false;

	if ( pTimerHandler == NULL || (CRef*)pTimerHandler == this )
		return false;
	if ( pTimerProc == NULL )
		return false;
	if ( fTimeout < 0.0001f )
		return false;

	m_bEnabled      = true;
	m_fTimeout      = fTimeout;
	m_fCountdown    = fTimeout;
	m_bLoop         = bLoop;
	m_pTimerFunc    = pTimerProc;
	m_pTimerHandler = pTimerHandler;

	m_nScale        = 1.0f;
	m_bEnableScale  = false;

	resume();

	return true;
}


//---------------------------------------------------------------------------------------------------
void iberbar::CTimer::stop()
{
	m_bEnabled = false;
}


//---------------------------------------------------------------------------------------------------
void iberbar::CTimer::pause()
{
	m_bPause = true;
}


//---------------------------------------------------------------------------------------------------
void iberbar::CTimer::resume()
{
	m_bPause = false;
}


//---------------------------------------------------------------------------------------------------
void iberbar::CTimer::scale( bool bEnable, float nScaleParams )
{
	m_bEnableScale = bEnable;

	if ( bEnable == true )
	{
		m_nScale = nScaleParams;

		if ( m_nScale < 0.0001f )
			m_nScale = 0.0001f;
	}
}


//---------------------------------------------------------------------------------------------------
void iberbar::CTimer::onRun( float fElapseTime )
{
	if ( m_bEnabled && m_bPause == false )
	{
		if ( m_bEnableScale == true )
			m_fCountdown -= fElapseTime * m_nScale;
		else
			m_fCountdown -= fElapseTime;
		if ( m_fCountdown < 0.0f )
		{
			if ( m_bLoop )
				m_fCountdown += m_fTimeout;
			else
				m_bEnabled = false;  // destroy

			( m_pTimerHandler->*m_pTimerFunc )( this );
		}
	}
}


//---------------------------------------------------------------------------------------------------
void iberbar::CTimer::onDestroy()
{
	m_pTimerHandler = NULL;
	m_pTimerFunc = NULL;
}







// bool iberbar::CTimer::InitialSystem()
// {
// 	return true;
// }
// 
// 
// void iberbar::CTimer::DestroySystem()
// {
// 	if ( g_Timers.empty() == false )
// 	{
// 		TimerList::iterator lc_node = g_Timers.begin();
// 		TimerList::iterator lc_end  = g_Timers.end();
// 		for ( ; lc_node != lc_end; lc_node ++ )
// 		{
// 			( *lc_node )->pause();
// 			( *lc_node )->stop();
// 			( *lc_node )->onDestroy();
// 			( *lc_node ) = NULL;
// 		}
// 		g_Timers.clear();
// 	}
// 
// 	if ( g_TimersCreate.empty() == false )
// 	{
// 		g_TimersCreate.clear();
// 	}
// }
// 
// 
// void iberbar::CTimer::RunSystem( float nElapsedTime )
// {
// 	// 把新增加的timer添加到更新队列中，然后清空新增队列
// 	if ( g_TimersCreate.empty() == false )
// 	{
// 		TimerList::iterator lc_iter = g_TimersCreate.begin();
// 		TimerList::iterator lc_end  = g_TimersCreate.end();
// 		for ( ; lc_iter != lc_end; lc_iter++ )
// 			g_Timers.push_back( *lc_iter );
// 		g_TimersCreate.clear();
// 	}
// 
// 	// 更新队列进行更新
// 	if ( g_Timers.empty() == false )
// 	{
// 		TimerList::iterator lc_node = g_Timers.begin();
// 		TimerList::iterator lc_end  = g_Timers.end();
// 		PTR_CTimer lc_timer( NULL );
// 		for ( ; lc_node != lc_end; )
// 		{
// 			lc_timer = ( *lc_node );
// 
// 			if ( lc_timer->isEnable() == false )
// 			{
// 				lc_timer->onDestroy();
// 				lc_node = g_Timers.erase( lc_node );
// 				lc_end  = g_Timers.end();
// 			}
// 			else
// 			{
// 				lc_timer->onRun( nElapsedTime );
// 
// 				lc_node ++;
// 			}
// 
// 			lc_timer = NULL;
// 		}
// 	}
// }
// 
// 
// iberbar::Timer iberbar::CTimer::CreateTimer()
// {
// 	Timer lc_timer;
// 	lc_timer.attach( new Timer::_InterfaceT() );
// 	g_TimersCreate.push_back( lc_timer );
// 	return lc_timer;
// }
// 
// 
// void iberbar::CTimer::DestroyTimer( Timer timer )
// {
// 	if ( timer )
// 	{
// 		timer->pause();
// 		timer->stop();
// 	}
// }



