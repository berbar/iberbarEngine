
#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{

	inline uint32 MakeColorARGB( uint32 a, uint32 r, uint32 g, uint32 b )
	{
		return (uint32)( ( ( (a)&0xff )<<24 )|( ( (r)&0xff )<<16 )|( ( (g)&0xff )<<8 )|( (b)&0xff ) );
	}

	inline uint32 MakeColorXRGB( uint32 a, uint32 r, uint32 g, uint32 b )
	{
		return (uint32)( ( ( 0xff )<<24 )|( ( (r)&0xff )<<16 )|( ( (g)&0xff )<<8 )|( (b)&0xff ) );
	}

	inline uint32 GetColorA( uint32 color )
	{
		return ( ( color&0xff000000 )>>24 );
	}

	inline uint32 GetColorR( uint32 color )
	{
		return ( ( color&0x00ff0000 )>>16 );
	}

	inline uint32 GetColorG( uint32 color )
	{
		return ( ( color&0x0000ff00 )>>8 );
	}

	inline uint32 GetColorB( uint32 color )
	{
		return ( color&0x000000ff );
	}



	class CColor4B;
	class CColor4F;


	class __iberbarUtilityApi__ CColor4F
	{
	public:
		CColor4F();
		CColor4F( const CColor4B& color );
		CColor4F( const uint32& val );
		CColor4F( float _a, float _r, float _g, float _b );
		CColor4F( float _r, float _g, float _b );
		CColor4F( const float* pValues ) throw();

		operator float* ();
		operator const float* () const;
		explicit operator uint32 () const;

		union 
		{
			struct
			{
				float b, g, r, a;
			};
			float value[ 4 ];
		};
	};

	//template < typename T >
	//class TColor4B
	//{
	//public:
	//	union
	//	{
	//		T;
	//		uint8 value8[4];
	//	};
	//};


	//struct UColorPixels_ARGB
	//{
	//	uint8 a, r, g, b;
	//};

	//typedef TColor4B<UColorPixels_ARGB> CColor4B_ARGB;


	class __iberbarUtilityApi__ CColor4B
	{
	public:
		CColor4B();
		CColor4B( const CColor4F& color );
		CColor4B( const uint32& val );
		CColor4B( uint8 _r, uint8 _g, uint8 _b );
		CColor4B( uint8 _a, uint8 _r, uint8 _g,  uint8 _b );
		CColor4B( const uint8* p );

		explicit operator uint32 () const;
		operator uint8* ();
		operator const uint8*() const;
		uint32 toRGBA() const;

		union
		{
			struct
			{
				uint8 b, g, r, a;
			};
			uint8 value8[ 4 ];
			uint32 value32;
		};
	};


	//class CColor4B_BGRA
	//{
	//public:
	//	CColor4B_BGRA();
	//	CColor4B_BGRA( const CColor4F& color );
	//	CColor4B_BGRA( uint8 _a, uint8 _r, uint8 _g, uint8 _b );
	//	CColor4B_BGRA( const uint8* p );

	//	operator uint8* ();
	//	operator const uint8* () const;

	//	union
	//	{
	//		struct
	//		{
	//			uint8 b, g, r, a;
	//		};
	//		uint8 value8[4];
	//	};
	//};



	inline CColor4F* Color4FLerp(
		CColor4F *pOut,
		const CColor4F *pC1, const CColor4F *pC2,
		float s )
	{
		pOut->r = pC1->r + s * ( pC2->r - pC1->r );
		pOut->g = pC1->g + s * ( pC2->g - pC1->g );
		pOut->b = pC1->b + s * ( pC2->b - pC1->b );
		pOut->a = pC1->a + s * ( pC2->a - pC1->a );
		return pOut;
	}


	CColor4B __iberbarUtilityApi__ Color4BParse( const char* text, CColor4B def = CColor4B(0,0,0,0xff) );


	/*
	 * RGB-888 像素数据转化为 ARGB-8888 像素数据， mask 为透明色的键值，当 mask 的 alpha 为 0x00 时键值无效，新数据为不透明
	 * pNewData_ARGB 必须已经被分配好的数据块，尺寸为 width * height * 4 (byte)
	 */
	void ColorMap_8bit_RGB_to_ARGB( int width, int height, const uint8* pOldData_RGB, uint8* pNewData_ARGB, uint32 mask );

	/*
	 * RGB-888 像素数据转化为 RGBA-8888 像素数据， mask 为透明色的键值，当 mask 的 alpha 为 0x00 时键值无效，新数据为不透明
	 * pNewData_ARGB 必须已经被分配好的数据块，尺寸为 width * height * 4 (byte)
	 */
	void ColorMap_8bit_RGB_to_RGBA( int width, int height, const uint8* pOldData_RGB, uint8* pNewData_RGBA, uint32 mask );

	/*
	 * RGB-888 像素数据转化为 RGBA-8888 像素数据， mask 为透明色的键值，当 mask 的 alpha 为 0x00 时键值无效，新数据为不透明
	 * pNewData_ARGB 必须已经被分配好的数据块，尺寸为 width * height * 4 (byte)
	 */
	void ColorMap_8bit_RGBA_to_RGBA( int width, int height, const uint8* pOldData_RGB, uint8* pNewData_RGBA, uint32 mask );



#define IBERBAR_COLOR_CONST_VALUE_EXTERN
#ifdef IBERBAR_COLOR_CONST_VALUE_EXTERN

	static const CColor4B COLOR_CONST_RED(0xffff0000);
	static const CColor4B COLOR_CONST_GREEN(0xff00ff00);
	static const CColor4B COLOR_CONST_BLEN(0xff0000ff);
	static const CColor4B COLOR_CONST_BLACK(0xff000000);
	static const CColor4B COLOR_CONST_WHITE(0xffffffff);
	static const CColor4B COLOR_CONST_GREY(0xff8a8a8a);
#endif

}


#include <iberbar\Utility\Color.inl>




