


#include <iberbar/Utility/ViewportState.h>
#include <iberbar/Utility/Math.h>



bool iberbar::CViewportState::IsEmpty()
{
	return m_ClipViewportStack.IsEmpty();
}

void iberbar::CViewportState::AddViewport( const CRect2i& rect )
{
	CRect2i lc_new = rect;
	if ( rect.IsEmpty() == true )
	{
		lc_new = CRect2i(0,0,0,0);
	}
	else if ( m_ClipViewportStack.IsEmpty() == false )
	{
		if ( GetClipViewport()->IsEmpty() == true )
		{
			lc_new = CRect2i(0,0,0,0);
		}
		else
		{
			CRect2i lc_clip = *GetClipViewport();
			lc_new.l = tMax( rect.l, lc_clip.l );
			lc_new.r = tMin( rect.r, lc_clip.r );
			lc_new.t = tMax( rect.t, lc_clip.t );
			lc_new.b = tMin( rect.b, lc_clip.b );
			if ( lc_new.IsEmpty() == true )
				lc_new = CRect2i( 0, 0, 0, 0 );
		}
	}

	m_ClipViewportStack.Push( lc_new );
}

void iberbar::CViewportState::PopViewport()
{
	assert( m_ClipViewportStack.IsEmpty() == false );

	m_ClipViewportStack.Pop();
}

const iberbar::CRect2i* iberbar::CViewportState::GetClipViewport()
{
	if ( m_ClipViewportStack.IsEmpty() )
		return nullptr;

	return &(m_ClipViewportStack.Top());
}


