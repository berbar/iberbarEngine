
#pragma once



#include <iberbar/Utility/Point.h>
#include <iberbar/Utility/Size.h>
#include <iberbar/Utility/Math.h>

namespace iberbar
{

	enum class UAlignStyle : uint8
	{
		Inner = 0,
		Outter = 1
	};

	enum class UAlignHorizental : uint8
	{
		Left   = 0,
		Center = 1,
		Right  = 2,
	};

	enum class UAlignVertical : uint8
	{
		Top    = 0,
		Center = 1,
		Bottom = 2,
	};

	enum class USizeMode : uint8
	{
		Default = 0, // 自定义
		Fill = 1, // 填充
		Stretching = 2 //拉伸

	};


	inline uint32 AlignMakeDword( UAlignHorizental h, UAlignVertical v )
	{
		return (uint32)( static_cast<uint32>( h ) | ( static_cast<uint32>( v ) << 2 ) );
	}
	inline UAlignHorizental AlignHorizentalFromDword( uint32 val )
	{
		return (UAlignHorizental)( val&0x2 );
	}
	inline UAlignVertical AlignVerticalFromDword( uint32 val )
	{
		return (UAlignVertical)( ( val >> 0x2 ) & 0x2 );
	}



	template< typename T >
	class TRect
	{
	public:
		TRect();
		TRect( T l, T t, T r, T b );
		TRect( const TPoint2<T>& pt, const TSize2<T>& size );
		TRect( const T* p ) noexcept;

	public:
		T Width() const;
		T Height() const;
		T CenterX() const;
		T CenterY() const;
		bool IsEmpty() const;
		//bool IsNull() const;

		TSize2< T > Size() const;

		operator T* ();
		operator const T* () const;
		TRect operator*( T a ) const;
		TRect operator*=( T a );
		TRect operator/( T a ) const;
		TRect operator/=( T a );

	public:
		T l;
		T t;
		T r;
		T b;
	};

	typedef TRect< int >      CRect2i;
	typedef TRect< float >    CRect2f;

	template class __iberbarUtilityApi__ TRect<int>;
	template class __iberbarUtilityApi__ TRect<float>;



	//static IBERBAR_DLL CRect* RectSetEmpty( CRect* pOut );
	//static IBERBAR_DLL CRect* RectSetLocSize( CRect* pOut, const CPoint* lt, const CSize* size );
	//static CRect2i* RectSetLocSize( CRect2i* pOut, int x, int y, int w, int h );
	//static CRect* RectSetWithCenter( CRect* pOut, const CPoint* center, const CSize* size );
	//static CRect* RectMoveX( CRect* pOut, const CRect* pRect, int n );
	//static CRect* RectMoveY( CRect* pOut, const CRect* pRect, int n );

	template < typename T >
	void RectOffsetX( TRect<T>* pRect, T offset );

	template < typename T >
	void RectOffsetY( TRect<T>* pRect, T offset );

	template < typename T >
	void RectOffset( TRect<T>* pRect, T offsetX, T offsetY );

	template < typename T >
	TRect<T>* RectAlignWithX( TRect<T>* pOut, T x, T width, UAlignHorizental hAlign, const TRect<T>* rcParent );

	template < typename T >
	TRect<T>* RectAlignWithY( TRect<T>* pOut, T y, T height, UAlignVertical vAlign, const TRect<T>* rcParent );

	template < typename T >
	TRect<T>* RectAlignWith( TRect<T>* pOut, const TPoint2<T>& position, const TSize2<T>& size, UAlignHorizental hAlign, UAlignVertical vAlign, const TRect<T>* rcParent );

	template < typename T >
	TRect<T>* RectAlign_OutterX( TRect<T>* pOut, T x, T w, UAlignHorizental hAlign, const TRect<T>* rcParent );

	template < typename T >
	TRect<T>* RectAlign_OutterY( TRect<T>* pOut, T y, T h, UAlignVertical vAlign, const TRect<T>* rcParent );

	template < typename T >
	TRect<T>* RectAlign_Outter( TRect<T>* pOut, const TPoint2<T>& position, const TSize2<T>& size, UAlignHorizental hAlign, UAlignVertical vAlign, const TRect<T>* rcParent );

	//static void   RectGetLoc( int* px, int* py, int width, int height, const CRect* rcBounding, const CRect* rcParent, UAlignHorizental hAlign, UAlignVertical vAlign );
	static bool   RectPointHitTest( const CRect2i* pRect, const CPoint2i* point );

	template < typename T >
	bool RectTestIntersection( const TRect<T>* rect1, const TRect<T>* rect2 )
	{
		if ( rect1->l >= rect2->r )
			return false;
		if ( rect1->t >= rect2->b )
			return false;
		if ( rect1->r <= rect2->l )
			return false;
		if ( rect1->b <= rect2->t )
			return false;
		return true;
	}

	template < typename T >
	void RectFindIntersection( TRect<T>* pOutRect, const TRect<T>* pRect1, const TRect<T>* pRect2 )
	{
		pOutRect->l = tMax( pRect1->l, pRect2->l );
		pOutRect->t = tMax( pRect1->t, pRect2->t );
		pOutRect->r = tMin( pRect1->r, pRect2->r );
		pOutRect->b = tMin( pRect1->b, pRect2->b );
	}


	template < typename T >
	TRect<float> RectToFloat( const TRect<T>& Rect )
	{
		return TRect<float>( (float)Rect.l, (float)Rect.t, (float)Rect.r, (float)Rect.b );
	}
}


namespace iberbar
{
	template< typename T >
	inline TRect<T>::TRect()
		: l( 0 ), t( 0 ), r( 0 ), b( 0 )
	{
	}

	template< typename T >
	inline TRect<T>::TRect( T l, T t, T r, T b )
		: l( l ), t( t ), r( r ), b( b )
	{
	}

	template< typename T >
	inline TRect<T>::TRect( const T* p ) noexcept
		: l( p[0] ), t( p[1] ), r( p[2] ), b( p[3] )
	{
	}

	template< typename T >
	inline TRect<T>::TRect( const TPoint2<T>& point, const TSize2<T>& size )
		: l( point.x ), t( point.y ), r( point.x + size.w ), b( point.y + size.h )
	{
	}

	template< typename T >
	inline T TRect<T>::Width() const
	{
		return r - l;
	}

	template< typename T >
	inline T TRect<T>::Height() const
	{
		return b - t;
	}

	template< typename T >
	T TRect<T>::CenterX() const
	{
		return (l + r) / 2;
	}

	template< typename T >
	T TRect<T>::CenterY() const
	{
		return (t + b) / 2;
	}

	template< typename T >
	inline bool TRect<T>::IsEmpty() const
	{
		if ( l >= r )
			return true;
		if ( t >= b )
			return true;
		return false;
	}

	template< typename T >
	inline TSize2<T> TRect<T>::Size() const
	{
		return TSize2< T >( Width(), Height() );
	}

	template< typename T >
	inline TRect<T> TRect<T>::operator*( T a ) const
	{
		return TRect<T>( l * a, t * a, r * a, b * a );
	}

	template< typename T >
	inline TRect<T> TRect<T>::operator*=( T a )
	{
		return TRect<T>( l * a, t * a, r * a, b * a );
	}

	template< typename T >
	inline TRect<T> TRect<T>::operator/( T a ) const
	{
		return TRect<T>( l / a, t / a, r / a, b / a );
	}

	template< typename T >
	inline TRect<T> TRect<T>::operator/=( T a )
	{
		return TRect<T>( l / a, t / a, r / a, b / a );
	}

	template< typename T >
	inline TRect<T>::operator T* ()
	{
		return &l;
	}

	template< typename T >
	inline TRect<T>::operator const T* () const
	{
		return &l;
	}
}


//inline iberbar::CRect* iberbar::RectSetLocSize( iberbar::CRect* pOut, int x, int y, int w, int h )
//{
//	pOut->left = x;
//	pOut->top = y;
//	pOut->right = pOut->left + w;
//	pOut->bottom = pOut->top + h;
//	return pOut;
//}


template < typename T >
inline void iberbar::RectOffsetX( TRect<T>* pRect, T offset )
{
	pRect->l += offset;
	pRect->r += offset;
}

template < typename T >
inline void iberbar::RectOffsetY( TRect<T>* pRect, T offset )
{
	pRect->t += offset;
	pRect->b += offset;
}

template < typename T >
inline void iberbar::RectOffset( TRect<T>* pRect, T offsetX, T offsetY )
{
	RectOffsetX( pRect, offsetX );
	RectOffsetY( pRect, offsetY );
}

template < typename T >
inline iberbar::TRect<T>* iberbar::RectAlignWithX( TRect<T>* pOut, T x, T width, UAlignHorizental hAlign, const TRect<T>* rcParent )
{
	if ( hAlign == UAlignHorizental::Right )
	{
		pOut->r = rcParent->r - x;
		pOut->l = pOut->r - width;
	}
	else if ( hAlign == UAlignHorizental::Center )
	{
		pOut->l = (rcParent->r + rcParent->l - width) / 2 + x;
		pOut->r = pOut->l + width;
	}
	else
	{
		pOut->l = rcParent->l + x;
		pOut->r = pOut->l + width;
	}

	return pOut;
}

template < typename T >
inline iberbar::TRect<T>* iberbar::RectAlignWithY( TRect<T>* pOut, T y, T height, UAlignVertical vAlign, const TRect<T>* rcParent )
{
	if ( vAlign == UAlignVertical::Bottom )
	{
		pOut->b = rcParent->b - y;
		pOut->t = pOut->b - height;
	}
	else if ( vAlign == UAlignVertical::Center )
	{
		pOut->t = (rcParent->b + rcParent->t - height) / 2 + y;
		pOut->b = pOut->t + height;
	}
	else
	{
		pOut->t = rcParent->t + y;
		pOut->b = pOut->t + height;
	}

	return pOut;
}


template < typename T >
inline iberbar::TRect<T>* iberbar::RectAlignWith( TRect<T>* pOut, const TPoint2<T>& position, const TSize2<T>& size, UAlignHorizental hAlign, UAlignVertical vAlign, const TRect<T>* rcParent )
{
	RectAlignWithX( pOut, position.x, size.w, hAlign, rcParent );
	RectAlignWithY( pOut, position.y, size.h, vAlign, rcParent );
	return pOut;
}


inline bool iberbar::RectPointHitTest( const iberbar::CRect2i* pRect, const iberbar::CPoint2i* point )
{
	if ( pRect->Width() <= 0 || pRect->Height() <= 0 )
		return false;
	if ( point->x >= pRect->l && point->x <= pRect->r &&
		point->y >= pRect->t && point->y <= pRect->b )
		return true;
	return false;
}
