


namespace iberbar
{
	inline CColor4F::CColor4F()
		: b( 0.0f ), g( 0.0f ), r( 0.0f ), a( 0.0f )
	{

	}

	inline CColor4F::CColor4F( const CColor4B& color )
		: b( (float)( color.b ) / 255.0f )
		, g( (float)( color.g ) / 255.0f )
		, r( (float)( color.r ) / 255.0f )
		, a( (float)( color.a ) / 255.0f )
	{
	}

	inline CColor4F::CColor4F( const uint32& val )
		: b( (float)( GetColorB( val ) ) / 255.0f )
		, g( (float)( GetColorG( val ) ) / 255.0f )
		, r( (float)( GetColorR( val ) ) / 255.0f )
		, a( (float)( GetColorA( val ) ) / 255.0f )
	{
	}

	inline CColor4F::CColor4F( float _a, float _r, float _g, float _b )
		: b( _b )
		, g( _g )
		, r( _r )
		, a( _a )
	{
	}

	inline CColor4F::CColor4F( float _r, float _g, float _b )
		: b( _b )
		, g( _g )
		, r( _r )
		, a( 1.0f )
	{
	}

	inline CColor4F::CColor4F( const float* pValues ) throw( )
		: b( pValues[ 0 ] )
		, g( pValues[ 1 ] )
		, r( pValues[ 2 ] )
		, a( pValues[ 3 ] )
	{
	}

	inline CColor4F::operator float* ( )
	{
		return value;
	}

	inline CColor4F::operator const float* () const
	{
		return value;
	}

	inline CColor4F::operator uint32 () const
	{
		return MakeColorARGB(
			(uint32)( a*255.0f ),
			(uint32)( r*255.0f ),
			(uint32)( g*255.0f ),
			(uint32)( b*255.0f ) );
	}




	inline CColor4B::CColor4B()
		: b(0)
		, g(0)
		, r(0)
		, a(0)
	{
	}

	inline CColor4B::CColor4B( const CColor4F& color )
		: b( (uint8)( color.b * 255.0f ) )
		, g( (uint8)( color.g * 255.0f ) )
		, r( (uint8)( color.r * 255.0f ) )
		, a( (uint8)( color.a * 255.0f ) )
	{
	}

	inline CColor4B::CColor4B( const uint32& val )
		: value32( val )
	{
	}

	inline CColor4B::CColor4B( uint8 _r, uint8 _g, uint8 _b )
		: b( _b ), g( _g ), r( _r ), a( 255 )
	{
	}

	inline CColor4B::CColor4B( uint8 _a, uint8 _r, uint8 _g, uint8 _b )
		: b( _b ), g( _g ), r( _r ), a( _a )
	{
	}

	inline CColor4B::CColor4B( const uint8* p )
		: b( p[0] ), g( p[1] ), r( p[2] ), a( p[3] )
	{
	}

	inline CColor4B::operator uint32 () const
	{
		return value32;
		//return (uint32)((((r) & 0xff) << 16) | (((g) & 0xff) << 8) | (((b) & 0xff)) | (((a) & 0xff) << 24 ) );
	}

	inline CColor4B::operator uint8* ()
	{
		return value8;
	}

	inline CColor4B::operator const uint8* () const
	{
		return value8;
	}

	inline uint32 CColor4B::toRGBA() const
	{
		return (uint32)( ( ( (r)&0xff )<<24 )|( ( (g)&0xff )<<16 )|( ( (b)&0xff )<<8 )|( (a)&0xff ) );
	}
}