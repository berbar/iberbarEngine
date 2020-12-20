

#include "AnimationFrameControl.h"

#include "iberbar\Base\Math.h"

iberbar::CAnimationFrameController::CAnimationFrameController( void )
	: m_nFrameCur( 0 )
	, m_nFrameBegin( 0 )
	, m_nFrameEnd( 0 )
	, m_nRepeatTimesMax( 0 )
	, m_nRepeatTimesAlready( 0 )
	, m_fTimePerFrame( 0.0f )
	, m_fElapsedTime( 0.0f )
	, m_bHasStarted( false )
	, m_bPaused( false )
{
}


void iberbar::CAnimationFrameController::setFrameLength( int nBegin, int nEnd )
{
	if ( m_bHasStarted == true )
		return ;

	m_nFrameBegin = nBegin;
	m_nFrameEnd   = nEnd; // 当end小于begin相当于倒带
	m_nFrameCur   = nBegin;
}


void iberbar::CAnimationFrameController::setFrameCur( int nOffset, int nType /* = FrameSet_Zero */ )
{
	if ( nType == (int)FrameSet_Zero )
		m_nFrameCur = nOffset;
	else if ( nType == (int)FrameSet_Begin )
		m_nFrameCur = m_nFrameBegin + nOffset;
	else if ( nType == (int)FrameSet_Current )
		m_nFrameCur = m_nFrameCur + nOffset;
	else if ( nType = (int)FrameSet_End )
		m_nFrameCur = m_nFrameEnd - nOffset;

	m_nFrameCur = tMax( m_nFrameCur, m_nFrameBegin );
	m_nFrameCur = tMin( m_nFrameCur, m_nFrameEnd );

	m_fElapsedTime = 0.0f;
}


void iberbar::CAnimationFrameController::setRepeatTimesMax( int nTimesMax )
{
	if ( nTimesMax <= m_nRepeatTimesAlready )
	{
		stop();
	}
	else
	{
		m_nRepeatTimesMax = nTimesMax;
	}
}


void iberbar::CAnimationFrameController::setTimePerFrame( float fTime )
{
	m_fElapsedTime = m_fElapsedTime * fTime / m_fTimePerFrame;
	m_fTimePerFrame = fTime;
}


void iberbar::CAnimationFrameController::start()
{
	if ( abs( m_nFrameEnd - m_nFrameBegin ) > 0 &&
		m_fTimePerFrame > 0.0f )
	{
		m_bHasStarted = true;
		m_nFrameCur = 0;
		m_nRepeatTimesAlready = 0;
		m_fElapsedTime = 0.0f;
		m_bPaused = false;
	}
}


void iberbar::CAnimationFrameController::stop()
{
	m_bHasStarted = false;
	m_nFrameCur = 0;
	m_nRepeatTimesAlready = 0;
	m_fElapsedTime = 0.0f;
	m_bPaused = false;
}


void iberbar::CAnimationFrameController::resume()
{
	m_bPaused = false;
}


void iberbar::CAnimationFrameController::pause()
{
	m_bPaused = true;
}


void iberbar::CAnimationFrameController::update( float nElapsedTime )
{
	if ( m_bHasStarted == false || m_bPaused == true )
		return ;
	if ( nElapsedTime < 0.0f )
		return ;

	m_fElapsedTime += nElapsedTime;
	if ( m_fElapsedTime >= m_fTimePerFrame )
	{
		m_nFrameCur ++;
		if ( m_nFrameCur > m_nFrameEnd )
		{
			m_nFrameCur = m_nFrameBegin;
			m_nRepeatTimesAlready ++;
			if ( m_nRepeatTimesMax > 0 && m_nRepeatTimesAlready >= m_nRepeatTimesMax )
				stop();
		}

		m_fElapsedTime -= m_fTimePerFrame;
	}
}