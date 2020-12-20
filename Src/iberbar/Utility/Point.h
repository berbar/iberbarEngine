#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{
	template< typename T >
	class TPoint2
	{
	public:
		TPoint2() throw();
		TPoint2( T x, T y ) throw();
		TPoint2( const T* p ) throw();

		operator T* () throw() { return &x; }

		TPoint2 operator*( int a ) const;
		TPoint2 operator*=( int a );
		TPoint2 operator/( int a ) const;
		TPoint2 operator/=( int a );
		bool operator==( const TPoint2& other ) const;
		bool operator!=( const TPoint2& other ) const;

	public:
		T x;
		T y;
	};

	typedef TPoint2< int >   CPoint2i;
	typedef TPoint2< float > CPoint2f;

	template class __iberbarUtilityApi__ TPoint2<int>;
	template class __iberbarUtilityApi__ TPoint2<float>;


	template< typename T >
	inline TPoint2< T >::TPoint2() throw()
		: x( 0 ), y( 0 )
	{
	}

	template< typename T >
	inline TPoint2< T >::TPoint2( T x, T y ) throw()
		: x( x ), y( y )
	{
	}

	template< typename T >
	inline TPoint2< T >::TPoint2( const T* p ) throw()
		: x( p[0] ), y( p[0] )
	{
	}

	template< typename T >
	inline TPoint2< T > TPoint2< T >::operator*( int a ) const
	{
		return TPoint2< T >( x * a, y * a );
	}

	template< typename T >
	inline TPoint2< T > TPoint2< T >::operator*=( int a )
	{
		return TPoint2< T >( x * a, y * a );
	}

	template< typename T >
	inline TPoint2< T > TPoint2< T >::operator/( int a ) const
	{
		return TPoint2< T >( x / a, y / a );
	}

	template< typename T >
	inline TPoint2< T > TPoint2< T >::operator/=( int a )
	{
		return TPoint2< T >( x / a, y / a );
	}

	template< typename T >
	inline bool TPoint2< T >::operator==( const TPoint2& other ) const
	{
		return (x == other.x && y == other.y);
	}

	template< typename T >
	inline bool TPoint2< T >::operator!=( const TPoint2& other ) const
	{
		return (x != other.x || y != other.y);
	}
}





