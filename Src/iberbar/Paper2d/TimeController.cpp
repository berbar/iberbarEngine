
#include <iberbar/Paper2d/TimeController.h>



iberbar::Paper2d::CTimeController::CTimeController()
	: m_nTimeScale( 1.0f )
{
}


void iberbar::Paper2d::CTimeController::Update( float nDelta )
{
	CNode::Update( nDelta * m_nTimeScale );
}