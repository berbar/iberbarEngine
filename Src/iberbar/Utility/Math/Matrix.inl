


namespace iberbar
{
	template < typename T >
	inline TVector2< T >::TVector2()
		: x( 0 )
		, y( 0 )
	{
	}

	template < typename T >
	inline TVector2< T >::TVector2( const TVector2< T >& v )
		: x( v.x )
		, y( v.y )
	{
	}

	template < typename T >
	inline TVector2< T >::TVector2( const T* p )
		: x( p[ 0 ] )
		, y( p[ 1 ] )
	{
	}

	template < typename T >
	inline TVector2< T >::TVector2( T x, T y )
		: x( x )
		, y( y )
	{
	}



	template < typename T >
	inline TVector3< T >::TVector3()
		: x( 0 )
		, y( 0 )
		, z( 0 )
	{
	}

	template < typename T >
	inline TVector3< T >::TVector3( const TVector3< T >& v )
		: x( v.x )
		, y( v.y )
		, z( v.z )
	{
	}

	template < typename T >
	inline TVector3< T >::TVector3( const T* p )
		: x( p[ 0 ] )
		, y( p[ 1 ] )
		, z( p[ 2 ] )
	{
	}

	template < typename T >
	inline TVector3< T >::TVector3( T x, T y, T z )
		: x( x )
		, y( y )
		, z( z )
	{
	}



	template < typename T >
	inline TVector4< T >::TVector4( void )
		: x( 0 )
		, y( 0 )
		, z( 0 )
		, w( 0 )
	{
	}

	template < typename T >
	inline TVector4< T >::TVector4( const TVector4< T >& v )
		: x( v.x )
		, y( v.y )
		, z( v.z )
		, w( v.w )
	{
	}

	template < typename T >
	inline TVector4< T >::TVector4( const T* p )
		: x( p[ 0 ] )
		, y( p[ 1 ] )
		, z( p[ 2 ] )
		, w( p[ 3 ] )
	{
	}

	template < typename T >
	inline TVector4< T >::TVector4( T x, T y, T z, T w )
		: x( x )
		, y( y )
		, z( z )
		, w( w )
	{
	}


	template < typename T >
	inline TMatrix< T >::TMatrix( void )
	{
	}


	template < typename T >
	inline TMatrix< T >::TMatrix( const TMatrix< T >& mat )
	{
		memcpy_s( value, 64, mat.value, 64 );
	}


	template < typename T >
	inline TMatrix< T >::TMatrix( const T* p )
	{
		memcpy_s( value, 64, p, 64 );
	}
}

