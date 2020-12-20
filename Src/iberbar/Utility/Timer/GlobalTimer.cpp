

#include <iberbar/Utility/Timer/GlobalTimer.h>

#ifdef _WIN32
#include <Windows.h>
#include <Mmsystem.h>

#pragma comment( lib, "winmm.lib" )
#endif







iberbar::CGlobalTimer::CGlobalTimer( void )
	: m_bInit( false )
	, m_nOneSecondTicks( 0 )
	, m_nTimeStart( 0 )

	, m_nTimePrevRun( 0 )
	, m_nMinSliceRun( 1000 )
	, m_nLastSliceRun( 0 )
	, m_funcOnRun()

	, m_bUserLargeTime( false )
{
}


iberbar::CGlobalTimer::~CGlobalTimer()
{
}


void iberbar::CGlobalTimer::Initial( bool accurate )
{
#ifdef _WIN32
	if ( accurate == true && QueryPerformanceFrequency( (LARGE_INTEGER*)&m_nOneSecondTicks ) == TRUE )
	{
		m_bUserLargeTime = true;
		QueryPerformanceCounter( (LARGE_INTEGER*)&m_nTimeStart );
	}
	else
	{
		m_bUserLargeTime = false;
		m_nTimeStart = timeGetTime();
	}
#endif

#ifdef __ANDROID__
	struct timeval tv;
	gettimeofday( &tv, NULL );
	m_nTimeStart = (int64)tv.tv_sec * 1000 + (int64)tv.tv_usec / 1000;
#endif

	m_bInit = true;
}


int64 iberbar::CGlobalTimer::GetElapsedTime()
{
#ifdef _WIN32
	if ( m_bUserLargeTime == true )
	{
		LARGE_INTEGER lc_dwTimeCurrentCounts;
		QueryPerformanceCounter( &lc_dwTimeCurrentCounts );

		return (((int64)lc_dwTimeCurrentCounts.QuadPart - m_nTimeStart) * 1000 / m_nOneSecondTicks);
	}

	return ((int64)timeGetTime() - m_nTimeStart);
#endif

#ifdef __ANDROID__
	struct timeval tv;
	gettimeofday( &tv, NULL );

	int64 ti = (int64)tv.tv_sec * 1000 + (int64)tv.tv_usec / 1000;
	return ti - m_nTimeStart;
#endif
}


void iberbar::CGlobalTimer::Run()
{
	assert( m_bInit == true );

	//assert( m_pCallBackRunHandler );
	//assert( m_pCallBackRun );

	int64 nTimeSlice = 0;
	int64 nTimeCur = GetElapsedTime();
	nTimeSlice = nTimeCur - m_nTimePrevRun;
	if ( m_nLastSliceRun <= m_nMinSliceRun )
	{
		if ( nTimeSlice < m_nMinSliceRun )
			return;
	}
	else
	{
		// 如果上一次间隔时间太长，补回差距
		if ( ( nTimeSlice + m_nLastSliceRun ) < ( m_nMinSliceRun << 1 ) )
			return;
	}


	// 执行回调
	m_funcOnRun( nTimeSlice, (float)nTimeSlice / 1000.0f );

	// 更新时间
	m_nLastSliceRun = nTimeSlice;
	m_nTimePrevRun = nTimeCur;
}



