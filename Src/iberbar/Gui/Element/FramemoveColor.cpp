
#include "FramemoveColor.h"

#include "iberbar\Base\Math.h"



iberbar::FramemoveColor::FramemoveColor( void )
	: m_iBlendTimesMax( 0 )
	, m_iBlendTimesAlready( 0 )
	, m_bBlendOver( true )
	, m_fBlendTimeNeed( 0.0f )
	, m_fBlendTimeElapsed( 0.0f )
	, m_ColorCurrent( 0.0f, 0.0f, 0.0f, 0.0f )
{
	m_ColorBase[ 0 ] = Color4F( 0.0f, 0.0f, 0.0f, 0.0f );
	m_ColorBase[ 1 ] = Color4F( 0.0f, 0.0f, 0.0f, 0.0f );
}


void iberbar::FramemoveColor::startBlend()
{
	if ( m_fBlendTimeNeed > 0.000001f )
	{
		if ( m_iBlendTimesMax > 0 )
		{
			m_iBlendTimesAlready = 0;
			m_bBlendOver = false;
			m_fBlendTimeElapsed = 0.0f;
			m_ColorCurrent = getBaseColor0();
		}
		else
		{
			m_bBlendOver = false;
			m_ColorCurrent = getBaseColor0();
		}
	}
}


void iberbar::FramemoveColor::overBlend()
{
	m_bBlendOver = true;
}


void iberbar::FramemoveColor::update( float nElapsedTime )
{
	if ( isOverBlend() == true )
		return ;

	int lc_nDirection = m_iBlendTimesAlready & 0x1;

	const Color4F* lc_pColorSrc = NULL;
	const Color4F* lc_pColorDest = NULL;

	if ( lc_nDirection == 0 )
	{
		lc_pColorSrc  = &m_ColorBase[ 0 ];
		lc_pColorDest = &m_ColorBase[ 1 ];
	}
	else
	{
		lc_pColorSrc  = &m_ColorBase[ 1 ];
		lc_pColorDest = &m_ColorBase[ 0 ];
	}

	float lc_fTimeElapsed_Sum;
	float lc_fStep;

	lc_fTimeElapsed_Sum = m_fBlendTimeElapsed + nElapsedTime;
	if ( lc_fTimeElapsed_Sum > ( m_fBlendTimeNeed + 0.000001 ) )
	{
		m_ColorCurrent = *lc_pColorDest;
		m_iBlendTimesAlready ++;
		m_fBlendTimeElapsed = 0.0f;
		if ( m_iBlendTimesMax > 0 && m_iBlendTimesAlready == m_iBlendTimesMax )
			m_bBlendOver = true;

		update( lc_fTimeElapsed_Sum - m_fBlendTimeNeed );
	}
	else
	{
		lc_fStep = lc_fTimeElapsed_Sum / m_fBlendTimeNeed;
		Color4FLerp( &m_ColorCurrent, lc_pColorSrc, lc_pColorDest, lc_fStep );
		m_fBlendTimeElapsed += nElapsedTime;
	}
}


void iberbar::FramemoveColor::setBaseColor( const Color4B& color0, const Color4B& color1 )
{
	if ( isOverBlend() == false )
		return ;

	m_ColorBase[ 0 ] = color0;
	m_ColorBase[ 1 ] = color1;
}


void iberbar::FramemoveColor::setBlendTimesMax( int nTimesMax )
{
	if ( isOverBlend() == false )
		return;

	m_iBlendTimesMax = nTimesMax;
}


void iberbar::FramemoveColor::setBlendTimeNeed( float nSecondes )
{
	if ( isOverBlend() == false )
		return ;

	m_fBlendTimeNeed = nSecondes;
}