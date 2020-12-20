

#ifndef __iberbar_Base_wtypes_inl__
#define __iberbar_Base_wtypes_inl__

template< typename T >
inline iberbar::TPoint< T >::TPoint( void ) throw()
{
}


template< typename T >
inline iberbar::TPoint< T >::TPoint( T _x, T _y ) throw( )
	: x( _x )
	, y( _y )
{
}


template< typename T >
inline iberbar::TSize< T >::TSize( void ) throw()
{
}


template< typename T >
inline iberbar::TSize< T >::TSize( T x, T y ) throw( )
	: cx( x )
	, cy( y )
{
}


template< typename T >
inline iberbar::TRect< T >::TRect( void ) throw()
{
}


template< typename T >
inline iberbar::TRect< T >::TRect( T l, T t, T r, T b ) throw( )
	: left( l )
	, top( t )
	, right( r )
	, bottom( b )
{
}


template< typename T >
inline iberbar::TRect< T >::TRect( const T* p ) throw( )
	: left( p[ 0 ] )
	, top( p[ 1 ] )
	, right( p[ 2 ] )
	, bottom( p[ 3 ] )
{
}


template< typename T >
inline bool iberbar::TRect< T >::isEmpty() const
{
	if ( left >= right )
		return true;
	if ( top >= bottom )
		return true;
	return false;
}


template< typename T >
inline bool iberbar::TRect< T >::isNull() const
{
}



inline iberbar::CRect*
iberbar::RectSetEmpty( CRect* pOut )
{
	pOut->left = 0;
	pOut->top = 0;
	pOut->right = 0;
	pOut->bottom = 0;
	return pOut;
}


inline iberbar::CRect*
iberbar::RectSetLocSize( iberbar::CRect* pOut, int x, int y, int w, int h )
{
	pOut->left = x;
	pOut->top = y;
	pOut->right = pOut->left + w;
	pOut->bottom = pOut->top + h;
	return pOut;
}


inline iberbar::CRect*
iberbar::RectSetWithCenter( iberbar::CRect* pOut, const iberbar::CPoint* center, const iberbar::CSize* size )
{
	pOut->left   = center->x - size->cx / 2;
	pOut->top    = center->y - size->cy / 2;
	pOut->right  = pOut->left + size->cx;
	pOut->bottom = pOut->top + size->cy;
	return pOut;
}

inline iberbar::CRect*
iberbar::RectMoveX( iberbar::CRect* pOut, const iberbar::CRect* pRect, int n )
{
	pOut->left = pRect->left + n;
	pOut->right = pRect->right + n;
	return pOut;
}

inline iberbar::CRect*
iberbar::RectMoveY( iberbar::CRect* pOut, const iberbar::CRect* pRect, int n )
{
	pOut->top = pRect->top + n;
	pOut->bottom = pRect->bottom + n;
	return pOut;
}


inline iberbar::CRect* iberbar::RectAlignWithX( CRect* pOut, int x, int width, UAlignHorizental hAlign, const iberbar::CRect* rcParent )
{
	if ( hAlign == Right )
	{
		pOut->right = rcParent->right - x;
		pOut->left = pOut->right - width;
	}
	else if ( hAlign == Center )
	{
		pOut->left = ( rcParent->right + rcParent->left - width ) / 2 + x;
		pOut->right = pOut->left + width;
	}
	else
	{
		pOut->left = rcParent->left + x;
		pOut->right = pOut->left + width;
	}

	return pOut;
}


inline iberbar::CRect* iberbar::RectAlignWithY( CRect* pOut, int y, int height, UAlignVertical vAlign, const iberbar::CRect* rcParent )
{
	if ( vAlign == Bottom )
	{
		pOut->bottom = rcParent->bottom - y;
		pOut->top = pOut->bottom - height;
	}
	else if ( vAlign == Center )
	{
		pOut->top = ( rcParent->bottom+rcParent->top - height ) / 2 + y;
		pOut->bottom = pOut->top + height;
	}
	else
	{
		// 不是靠右就全是靠左
		pOut->top = rcParent->top + y;
		pOut->bottom = pOut->top + height;
	}

	return pOut;
}


inline iberbar::CRect*
iberbar::RectAlignWith( iberbar::CRect* pOut,
int x, int y, int width, int height,
iberbar::UAlignHorizental hAlign, iberbar::UAlignVertical vAlign,
const iberbar::CRect* rcParent )
{
	RectAlignWithX( pOut, x, width, hAlign, rcParent );
	RectAlignWithY( pOut, y, height, vAlign, rcParent );

	return pOut;
}

inline bool iberbar::RectPointHitTest( const iberbar::CRect* pRect, const iberbar::CPoint* point )
{
	if ( pRect->width() <= 0 || pRect->height() <= 0 )
		return false;
	if ( point->x >= pRect->left && point->x <= pRect->right &&
		point->y >= pRect->top && point->y <= pRect->bottom )
		return true;
	return false;
}





template < typename T >
inline void iberbar::tRectClip(
	iberbar::TRect< T >* pOut,
	const iberbar::TRect< T >* pRectClip )
{
	if ( pOut->left < pRectClip->left )
		pOut->left = pRectClip->left;

	if ( pOut->top < pRectClip->top )
		pOut->top = pRectClip->top;

	if ( pOut->right > pRectClip->right )
		pOut->right = pRectClip->right;

	if ( pOut->bottom > pRectClip->bottom )
		pOut->bottom = pRectClip->bottom;
}




// inline long GetRectCenterX( const RECT& rect )
// {
// 	return ( rect.left + rect.right ) / 2;
// }
// 
// 
// inline long GetRectWidth( const RECT& rect )
// {
// 	return rect.right - rect.left;
// }
// 
// inline long GetRectHeight( const RECT& rect )
// {
// 	return rect.bottom - rect.top;
// }
// 
// inline void SetRect_LocSize( RECT* pRc, long xLoc, long yLoc, long wSize, long hSize )
// {
// 	pRc->left = xLoc;
// 	pRc->top = yLoc;
// 	pRc->right = pRc->left + wSize;
// 	pRc->bottom = pRc->top + hSize;
// }
// 
// inline void SetRect_CenterSize( RECT* pRc, long xCenter, long yCenter, long wSize, long hSize )
// {
// 	pRc->left   = xCenter - wSize / 2;
// 	pRc->top    = yCenter - hSize / 2;
// 	pRc->right  = pRc->left + wSize;
// 	pRc->bottom = pRc->top + hSize;
// }





namespace iberbar
{

	inline void SetRectAlian( CRect* pRect,
		int x, int y, int width, int height,
		UAlignHorizental hAlign, UAlignVertical vAlign,
		const CRect& rcParent )
	{
		if ( hAlign == Right )
		{
			pRect->right = rcParent.right - x;
			pRect->left = pRect->right - width;
		}
		else if ( hAlign == Center )
		{
			pRect->left = ( rcParent.right+rcParent.left - width )/2;
			pRect->right = pRect->left + width;
		}
		else
		{
			// 不是靠右就全是靠左
			pRect->left = rcParent.left + x;
			pRect->right = pRect->left + width;
		}

		if ( vAlign == Bottom )
		{
			pRect->bottom = rcParent.bottom - y;
			pRect->top = pRect->bottom - height;
		}
		else if ( vAlign == Center )
		{
			pRect->top = ( rcParent.bottom+rcParent.top - height )/2;
			pRect->bottom = pRect->top + height;
		}
		else
		{
			// 不是靠右就全是靠左
			pRect->top = rcParent.top + y;
			pRect->bottom = pRect->top + height;
		}
	}


	inline bool IsRectMixedRect( const CRect& rc1, const CRect& rc2 )
	{
		if ( rc1.right <= rc2.left )
			return false;
		else if ( rc1.left >= rc2.right )
			return false;
		else if ( rc1.bottom <= rc2.top )
			return false;
		else if ( rc1.top >= rc2.bottom )
			return false;
		return false;
	}


}

#endif