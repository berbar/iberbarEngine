

#include "iberbar\Base\GlobalTimer.h"
#include <time.h>



long JNIGetCurrentTime()
{
	struct timeval tv;
	gettimeofday( &tv, NULL );
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


iberbar::PTR_CGlobalTimer iberbar::CGlobalTimer::sm_pShared( nullptr );

//---------------------------------------------------------------------------------------------------------------------------------
iberbar::CGlobalTimer::CGlobalTimer( void )
	: m_nOneSecondTicks( 0 )
	, m_dwTimeTickStartCounts( 0 )
	, m_nTimeStart( 0 )
	
    , m_iFrameCount( 0 )
	, m_fFPS( 0.0f )
	, m_fFpsTimePre( 0.0f )
	, m_fFpsTimeSlice( 0.0f )

	, m_fMinSliceRun( 1.0f )
	, m_fTimePrevRun( 0.0f )
	, m_pCallBackRunHandler( NULL )
	, m_pCallBackRun( NULL )
	, m_bUserLargeTime( false )
	, m_bStop( false )
{
	InitialTime();
}

//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CGlobalTimer::InitialTime()
{
	m_nTimeStart = JNIGetCurrentTime();
}

//---------------------------------------------------------------------------------------------------------------------------------
void  iberbar::CGlobalTimer::SetRunningTimeout( float timeout )
{
	m_fMinSliceRun = timeout;
}

//---------------------------------------------------------------------------------------------------------------------------------
void  iberbar::CGlobalTimer::SetRunningProc( CUnknown* pHandler, PTIMER_RUNPROC proc )
{ 
	m_pCallBackRunHandler = pHandler;
	m_pCallBackRun = proc;
}

//---------------------------------------------------------------------------------------------------------------------------------
void  iberbar::CGlobalTimer::Start()
{
	m_fTimePrevRun = GetElapseTimeS();
	m_bStop = false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void  iberbar::CGlobalTimer::Stop()
{
	m_bStop = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
float iberbar::CGlobalTimer::GetElapseTimeMS()
{
	return ( JNIGetCurrentTime() - m_nTimeStart );
}

//---------------------------------------------------------------------------------------------------------------------------------
float iberbar::CGlobalTimer::GetElapseTimeS()
{
	return ( (float)( JNIGetCurrentTime() - m_nTimeStart ) / 1000.0f );
}

//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CGlobalTimer::Run()
{
	if ( m_bStop == false )
	{
		// 获取时间差
		float lc_fTimeSlice = 0.0f;
		float lc_fTimeCur = GetElapseTimeS();
		lc_fTimeSlice = lc_fTimeCur - m_fTimePrevRun;
		if ( lc_fTimeSlice < m_fMinSliceRun )
			return;

		//UpdateFps();
		if ( m_pCallBackRunHandler && m_pCallBackRun )
			(m_pCallBackRunHandler->*m_pCallBackRun)( lc_fTimeSlice );

		// 更新时间
		m_fTimePrevRun = lc_fTimeCur;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CGlobalTimer::UpdateFps( float nElapsedTime )
{
	m_iFrameCount ++;
	m_fFpsTimeSlice += nElapsedTime;

// 	if ( m_iFrameCount == 5 )
// 	{
// 		float lc_fFpsTimeCur = GetElapseTimeS();
// 		m_fFpsTimeSlice = lc_fFpsTimeCur - m_fFpsTimePre;
// 		m_fFPS = (float)m_iFrameCount / m_fFpsTimeSlice;
// 
// 		m_fFpsTimePre = lc_fFpsTimeCur;
// 		m_iFrameCount = 0;
// 	}

	if ( m_fFpsTimeSlice > 1.0f )
	{
		m_fFPS = (float)m_iFrameCount / m_fFpsTimeSlice;

		m_iFrameCount = 0;
		m_fFpsTimeSlice = 0.0f;
	}
}


