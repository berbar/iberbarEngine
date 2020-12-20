

#pragma once

#include <iberbar/Utility/Rect.h>

namespace iberbar
{
	template < typename T >
	void RectClip2d( TRect<T>* dest, const TRect<T>* viewport );

	template < typename T >
	void RectClip2d( CRect2f* rectTexCoord, TRect<T>* rectDst, const TRect<T>* viewport );
}


template < typename T >
inline void iberbar::RectClip2d( TRect<T>* dest, const TRect<T>* viewport )
{
	if ( dest->l < viewport->l )
		dest->l = viewport->l;

	if ( dest->t < viewport->t )
		dest->t = viewport->t;

	if ( dest->r > viewport->r )
		dest->r = viewport->r;

	if ( dest->b > viewport->b )
		dest->b = viewport->b;
}

template < typename T >
inline void iberbar::RectClip2d( CRect2f* rectTexCoord, TRect<T>* rectDst, const TRect<T>* viewport )
{
	if ( rectDst->l < viewport->l )
	{
		rectTexCoord->l = rectTexCoord->l + rectTexCoord->Width() * (viewport->l - rectDst->l) / rectDst->Width();
		rectDst->l = viewport->l;
	}

	if ( rectDst->t < viewport->t )
	{
		rectTexCoord->t = rectTexCoord->t + rectTexCoord->Height() * (viewport->t - rectDst->t) / rectDst->Height();
		rectDst->t = viewport->t;
	}

	if ( rectDst->r > viewport->r )
	{
		rectTexCoord->r = rectTexCoord->r - rectTexCoord->Width() * (rectDst->r - viewport->r) / rectDst->Width();
		rectDst->r = viewport->r;
	}

	if ( rectDst->b > viewport->b )
	{
		rectTexCoord->b = rectTexCoord->b - rectTexCoord->Height() * (rectDst->b - viewport->b) / rectDst->Height();
		rectDst->b = viewport->b;
	}
}
