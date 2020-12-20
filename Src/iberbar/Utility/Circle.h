#pragma once

#include <iberbar/Utility/Point.h>

namespace iberbar
{
	template < typename T >
	class TCircle
	{
	public:
		T x;
		T y;
		T r;

	public:
		TCircle();
		TCircle( T r );
		TCircle( T x, T y, T r );
		TCircle( const TPoint2<T>& pt, T r );

		T Diameter() const { return r * 2; }

		TRect<T> GetBounding() const;
	};

	template < typename T >
	inline TCircle<T>::TCircle()
		: x( 0 ), y( 0 ), r( 0 )
	{
	}

	template < typename T >
	inline TCircle<T>::TCircle( T r )
		: x( 0 ), y( 0 ), r( r )
	{
	}

	template < typename T >
	inline TCircle<T>::TCircle( T x, T y, T r )
		: x( x ), y( y ), r( r )
	{
	}

	template < typename T >
	inline TCircle<T>::TCircle( const TPoint2<T>& pt, T r )
		: x( pt.x ), y( pt.y ), r( r )
	{
	}

	template < typename T >
	inline TRect<T> TCircle<T>::GetBounding() const
	{
		return TRect<T>( x - r, y - r, x + r, y + r );
	}

	typedef TCircle<int> CCircle;
}
