#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{
	template< typename T >
	class TSize2
	{
	public:
		TSize2() throw();
		TSize2( T w, T h ) throw();
		TSize2( const T* p ) throw();

		T Area() const;

		operator T* () throw();
		operator const T* () const throw();
		TSize2 operator*( int a ) const;
		TSize2 operator*=( int a );
		TSize2 operator/( int a ) const;
		TSize2 operator/=( int a );
		bool operator==( const TSize2& other ) const;
		bool operator!=( const TSize2& other ) const;

		T w;
		T h;
	};

	typedef TSize2< int >    CSize2i;
	typedef TSize2< float >  CSize2f;

	template class __iberbarUtilityApi__ TSize2<int>;
	template class __iberbarUtilityApi__ TSize2<float>;



	template< typename T >
	TSize2<T>::TSize2() throw()
		: w(0), h(0)
	{
	}

	template< typename T >
	TSize2<T>::TSize2( T w, T h ) throw()
		: w( w ), h( h )
	{
	}

	template< typename T >
	TSize2<T>::TSize2( const T* p ) throw()
		: w( p[0] ), h( p[1] )
	{
	}

	template< typename T >
	TSize2<T>::operator T* () throw()
	{
		return &w;
	}

	template< typename T >
	TSize2<T>::operator const T* () const throw()
	{
		return &w;
	}

	template< typename T >
	inline T TSize2<T>::Area() const
	{
		return w * h;
	}

	template< typename T >
	inline TSize2<T> TSize2<T>::operator*( int a ) const
	{
		return TSize2<T>( w * a, h * a );
	}

	template< typename T >
	inline TSize2<T> TSize2<T>::operator*=( int a )
	{
		return TSize2<T>( w * a, h * a );
	}

	template< typename T >
	inline TSize2<T> TSize2<T>::operator/( int a ) const
	{
		return TSize2<T>( w / a, h / a );
	}

	template< typename T >
	inline TSize2<T> TSize2<T>::operator/=( int a )
	{
		return TSize2<T>( w / a, h / a );
	}

	template< typename T >
	inline bool TSize2<T>::operator==( const TSize2<T>& other ) const
	{
		return (w == other.w && h == other.h);
	}

	template< typename T >
	inline bool TSize2<T>::operator!=( const TSize2<T>& other ) const
	{
		return (w != other.w || h != other.h);
	}

	
}
