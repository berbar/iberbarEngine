
#pragma once

namespace iberbar
{
	template < typename T >
	struct TVector2
	{
		TVector2( void );
		TVector2( const TVector2< T >& v );
		TVector2( const T* p );
		TVector2( T x, T y );

		operator const T*()
		{
			return value;
		}

		union
		{
			struct
			{
				T x, y;
			};
			T value[ 2 ];
		};
	};

	template < typename T >
	struct TVector3
	{
		TVector3( void );
		TVector3( const T* p );
		TVector3( const TVector3< T >& v );
		TVector3( T x, T y, T z );

		operator const T*( )
		{
			return value;
		}

		union
		{
			struct
			{
				T x, y, z;
			};
			T value[ 3 ];
		};
	};


	template < typename T >
	struct TVector4
	{
		TVector4( void );
		TVector4( const TVector4< T >& v );
		TVector4( const T* p );
		TVector4( T x, T y, T z, T w );

		operator const T*( )
		{
			return value;
		}

		union
		{
			struct  
			{
				T x, y, z, w;
			};
			T value[ 4 ];
		};
	};


	template < typename T >
	struct TMatrix
	{
		TMatrix( void );
		TMatrix( const TMatrix< T >& mat );
		TMatrix( const T* p );

		operator const T*( )
		{
			return (const T*)( &_11 );
		}

		union {
			struct {
				float        _11, _12, _13, _14;
				float        _21, _22, _23, _24;
				float        _31, _32, _33, _34;
				float        _41, _42, _43, _44;

			};
			float value[ 4 ][ 4 ];
		};
	};



	typedef TVector2< float >  UVector2f;
	typedef TVector3< float >  UVector3f;
	typedef TVector4< float >  UVector4f;
	typedef TMatrix< float >   UMatrix;
}

#include <iberbar\Utility\Math\Matrix.inl>
