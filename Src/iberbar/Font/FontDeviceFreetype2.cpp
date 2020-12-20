

#include "FontBase.h"
#include "FontCreate.h"
#include <iberbar/Utility/Math.h>
//#include <iberbar/Utility/String.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H

#include <vector>


namespace iberbar
{
	class CFontDeviceFT2;
	class CFontToolFT2;


	class FontConstLabelBitmapPaintHelperFT2
	{
	public:
		FontConstLabelBitmapPaintHelperFT2( void );
		~FontConstLabelBitmapPaintHelperFT2();

		void start_paint( CSize2i BitmapSize, UFontBitsFormat format );
		void end_paint();
		void paint_char( int nCharWidth, CSize2i BitmapSize, CPoint2i BitmapDelta, uint8* pBitsFill ) throw();
		void paint_char(
			int nCharWidth,
			CSize2i BitmapSizeFill,    CPoint2i BitmapDeltaFill,    uint8* pBitsFill,
			CSize2i BitmapSizeOutline, CPoint2i BitmapDeltaOutline, uint8* pBitsOutline ) throw();

		int   get_paint_size();
		void* get_bits_fill() { return m_pBits[ 0 ]; }
		void* get_bits_outline() { return m_pBits[ 1 ]; }


	private:
		uint8* m_pBits[ 2 ];
		uint32 m_nBitsSize;
		bool   m_bStartDraw;
		int    m_nPaintOffset;
		int    m_nPaintSize;
		CSize2i  m_BitmapSize;
		UFontBitsFormat m_format;
	};




	class CFontDeviceFT2
		: public CxFontDevice
	{
	public:
		CFontDeviceFT2( void );
		~CFontDeviceFT2() override;

	public:
		virtual bool begin( CxFontTool* pInFontTool ) override;
		virtual void end() override;
		virtual bool createFontToolA( const FontDescA* pFontDesc, CxFontTool** ppOutFontTool ) override;
		virtual bool createFontToolW( const FontDescW* pFontDesc, CxFontTool** ppOutFontTool ) override;
		virtual bool createConstantLabelA( CxFontTool* pFontTool, FontConstLabelBitmapDescA* pLabelDesc ) override;     // 
		virtual bool createConstantLabelW( CxFontTool* pFontTool, FontConstLabelBitmapDescW* pLabelDesc ) override;
		virtual bool createChBitmap( wchar_t ch, __out FontCharBitmapDesc* pBmpDesc, UFontBitsFormat format ) override;
		virtual int  getCharWidth( wchar_t ch ) override;
		virtual void setChBitmapFormat( FontChBitmapFormat Format ) override;


	public:
		bool initFreeType();
		FT_Library getFTLibrary() { return m_Library; }


	private:
		bool createConstantLabelInternal( FontConstLabelBitmapDescW* pLabelDesc );

		static void _RasterCallback( int y, int count, const FT_Span* spans, void* user );

	private:
		FT_Library m_Library;
		FT_Stroker m_Stroker; 
		CFontToolFT2* m_pCurFontTool;
		int           m_nFontHeight;

		FontConstLabelBitmapPaintHelperFT2 m_LabelBitmapBuffer;
	};





	class CFontToolFT2
		: public CxFontTool
	{
	public:
		CFontToolFT2( void );
		~CFontToolFT2() override;


	public:
		bool onCreateW( CFontDeviceFT2* pDeviceFT2, const FontDescW* pFontDesc );
		bool onCreateA( CFontDeviceFT2* pDeviceFT2, const FontDescA* pFontDesc );

		FT_Face getFTFace() { return m_face; }

	private:
		FT_Face m_face;
	};
}












iberbar::FontConstLabelBitmapPaintHelperFT2::FontConstLabelBitmapPaintHelperFT2( void )
	: m_nBitsSize( 0 )
	, m_bStartDraw( false )
	, m_nPaintOffset( 0 )
	, m_nPaintSize( 0 )
	, m_BitmapSize( 0, 0 )
	, m_format( UFontBitsFormat::ARGB )
{
	m_pBits[ 0 ] = NULL;
	m_pBits[ 1 ] = NULL;
}


iberbar::FontConstLabelBitmapPaintHelperFT2::~FontConstLabelBitmapPaintHelperFT2()
{
	SAFE_DELETE_ARRAY( m_pBits[ 0 ] );
	SAFE_DELETE_ARRAY( m_pBits[ 1 ] );
}


void iberbar::FontConstLabelBitmapPaintHelperFT2::start_paint( CSize2i BitmapSize, UFontBitsFormat format )
{
	assert( BitmapSize.w > 0 && BitmapSize.h );
	m_bStartDraw = true;
	m_nPaintOffset = 0;
	m_nPaintSize = 0;

	uint32 lc_bitsSize = BitmapSize.w * BitmapSize.h * 4;
	if ( m_nBitsSize == NULL || lc_bitsSize > m_nBitsSize )
	{
		SAFE_DELETE_ARRAY( m_pBits[ 0 ] );
		SAFE_DELETE_ARRAY( m_pBits[ 1 ] );
		m_pBits[ 0 ] = new uint8[ lc_bitsSize ];
		m_pBits[ 1 ] = new uint8[ lc_bitsSize ];
		m_nBitsSize = lc_bitsSize;
	}
	memset( m_pBits[ 0 ], 0, lc_bitsSize );
	memset( m_pBits[ 1 ], 0, lc_bitsSize );
	m_BitmapSize = BitmapSize;
	m_format = format;
}


void iberbar::FontConstLabelBitmapPaintHelperFT2::end_paint()
{
	m_bStartDraw = false;
}


void iberbar::FontConstLabelBitmapPaintHelperFT2::paint_char( int nCharWidth, CSize2i ChBitmapSize, CPoint2i ChBitmapDelta, uint8* pBitsFill ) throw()
{
	byte* lc_pBitsFill = m_pBits[ 0 ];

	int nCopyHeight = tMin( ChBitmapSize.h, m_BitmapSize.h-ChBitmapDelta.y );
	int lc_nCopyWidth  = tMin( ChBitmapSize.w, m_BitmapSize.w-m_nPaintOffset-ChBitmapDelta.x );
	int lc_src_start_x = ( ChBitmapDelta.x < 0 ) ? ( -ChBitmapDelta.x ) : 0;
	int lc_src_start_y = ( ChBitmapDelta.y < 0 ) ? ( -ChBitmapDelta.y ) : 0;
	int lc_dest_start_x = ( ( ChBitmapDelta.x < 0 ) ? 0 : ChBitmapDelta.x ) +  m_nPaintOffset;
	int lc_dest_start_y = ( ( ChBitmapDelta.y < 0 ) ? 0 : ChBitmapDelta.y );

	int rowByteSize = m_BitmapSize.w * 4;
	int rowByteSizeCopy = lc_nCopyWidth * 4;
	int dest_offset = ( lc_dest_start_x + lc_dest_start_y * m_BitmapSize.w ) * 4;
	int dest_offset_x = 0;
	int src_offset = lc_src_start_y * ChBitmapSize.w + lc_src_start_x;
	int src_offset_last = src_offset;
	byte* ptr = nullptr;

	int alphaIndex = 0;

	if ( m_format == UFontBitsFormat::BGRA || m_format == UFontBitsFormat::RGBA )
	{
		alphaIndex = 3;
	}

	for ( int lc_j = lc_src_start_y; lc_j < nCopyHeight; lc_j++ )
	{
		ptr = lc_pBitsFill + dest_offset;
		memset( ptr, 0xff, rowByteSizeCopy );
		for ( int lc_i = lc_src_start_x; lc_i < lc_nCopyWidth; lc_i++ )
		{
			ptr[alphaIndex] = pBitsFill[src_offset];
			ptr += 4;
			src_offset++;
		}

		dest_offset += rowByteSize;

		src_offset = src_offset_last + ChBitmapSize.w;
		src_offset_last = src_offset;
	}

	m_nPaintOffset += nCharWidth;
	m_nPaintSize += ChBitmapSize.w;
}


void iberbar::FontConstLabelBitmapPaintHelperFT2::paint_char( int nCharWidth, CSize2i ChBitmapSize, CPoint2i ChBitmapDelta, uint8* pBitsFill, CSize2i BitmapSizeOutline, CPoint2i ChBitmapDeltaOutline, uint8* pBitsOutline ) throw()
{
	//uint32* lc_pBitsFill    = (uint32*)m_pBits[ 0 ];
	//uint32* lc_pBitsOutline = (uint32*)m_pBits[ 1 ];

	//int lc_nCopyHeight = tMin( ChBitmapSize.h, m_BitmapSize.h-ChBitmapDelta.y );
	//int lc_nCopyWidth  = tMin( ChBitmapSize.w, m_BitmapSize.w-m_nPaintOffset-ChBitmapDelta.x );
	//int lc_src_start_x = ( ChBitmapDelta.x < 0 ) ? ( -ChBitmapDelta.x ) : 0;
	//int lc_src_start_y = ( ChBitmapDelta.y < 0 ) ? ( -ChBitmapDelta.y ) : 0;
	//int lc_dest_start_x = ( ( ChBitmapDelta.x < 0 ) ? 0 : ChBitmapDelta.x ) +  m_nPaintOffset;
	//int lc_dest_start_y = ( ( ChBitmapDelta.y < 0 ) ? 0 : ChBitmapDelta.y );

	//int dest_offset = lc_dest_start_x + lc_dest_start_y * m_BitmapSize.w;
	//int dest_offset_x = 0;
	//int src_offset = lc_src_start_y * ChBitmapSize.w + lc_src_start_x;
	//int src_offset_last = src_offset;
	//for ( int lc_j = lc_src_start_y; lc_j < lc_nCopyHeight; lc_j ++ )
	//{
	//	for ( int lc_i = lc_src_start_x; lc_i < lc_nCopyWidth; lc_i ++ )
	//	{
	//		lc_pBitsFill   [ dest_offset + dest_offset_x ] = CColor4B( pBitsFill[ src_offset ], 0xff, 0xff, 0xff );

	//		dest_offset_x ++;
	//		src_offset ++;
	//	}

	//	dest_offset += m_BitmapSize.w;
	//	dest_offset_x = 0;

	//	src_offset = src_offset_last + ChBitmapSize.w;
	//	src_offset_last = src_offset;
	//}


	//lc_nCopyHeight = tMin( BitmapSizeOutline.h, m_BitmapSize.h-ChBitmapDeltaOutline.y );
	//lc_nCopyWidth  = tMin( BitmapSizeOutline.w, m_BitmapSize.w-m_nPaintOffset-ChBitmapDeltaOutline.x );
	//lc_src_start_x = ( ChBitmapDeltaOutline.x < 0 ) ? ( -ChBitmapDeltaOutline.x ) : 0;
	//lc_src_start_y = ( ChBitmapDeltaOutline.y < 0 ) ? ( -ChBitmapDeltaOutline.y ) : 0;
	//lc_dest_start_x = ( ( ChBitmapDeltaOutline.x < 0 ) ? 0 : ChBitmapDeltaOutline.x ) +  m_nPaintOffset;
	//lc_dest_start_y = ( ( ChBitmapDeltaOutline.y < 0 ) ? 0 : ChBitmapDeltaOutline.y );

	//dest_offset = lc_dest_start_x + lc_dest_start_y * m_BitmapSize.w;
	//dest_offset_x = 0;
	//src_offset = lc_src_start_y * BitmapSizeOutline.w + lc_src_start_x;
	//src_offset_last = src_offset;
	//for ( int lc_j = lc_src_start_y; lc_j < lc_nCopyHeight; lc_j ++ )
	//{
	//	for ( int lc_i = lc_src_start_x; lc_i < lc_nCopyWidth; lc_i ++ )
	//	{
	//		lc_pBitsOutline[ dest_offset + dest_offset_x ] = CColor4B( pBitsOutline[ src_offset ], 0xff, 0xff, 0xff );

	//		dest_offset_x ++;
	//		src_offset ++;
	//	}

	//	dest_offset += m_BitmapSize.w;
	//	dest_offset_x = 0;

	//	src_offset = src_offset_last + BitmapSizeOutline.w;
	//	src_offset_last = src_offset;
	//}


	//m_nPaintOffset += nCharWidth;
	//m_nPaintSize += ChBitmapSize.w;
}










iberbar::CFontDeviceFT2::CFontDeviceFT2( void )
	: m_Library( NULL )
	, m_Stroker( NULL )
	, m_pCurFontTool( NULL )
{
}


iberbar::CFontDeviceFT2::~CFontDeviceFT2()
{
	// 需要用end来设置为NULL
	assert( m_pCurFontTool == NULL );

	if ( m_Stroker )
	{
		FT_Stroker_Done( m_Stroker );
		m_Stroker = NULL;
	}

	if ( m_Library )
	{
		FT_Done_FreeType( m_Library );
		m_Library = NULL;
	}
}


bool iberbar::CFontDeviceFT2::initFreeType()
{
	FT_Error lc_error = FT_Init_FreeType( &m_Library );
	if ( lc_error )
	{
		return false;
	}

	lc_error = FT_Stroker_New( m_Library, &m_Stroker );
	if ( lc_error )
		return false;

	return true;
}


bool iberbar::CFontDeviceFT2::begin( CxFontTool* pInFontTool )
{
	assert( pInFontTool );
	m_pCurFontTool = (CFontToolFT2*)pInFontTool;
	assert( m_pCurFontTool->getFTFace() );
	m_pCurFontTool->addRef();

	FontDescW lc_FontDesc;
	m_pCurFontTool->getFontDescW( &lc_FontDesc );
	m_nFontHeight = lc_FontDesc.Height;

	return true;
}


void iberbar::CFontDeviceFT2::end()
{
	assert( m_pCurFontTool );

	m_pCurFontTool->release();
	m_pCurFontTool = NULL;
}


bool iberbar::CFontDeviceFT2::createFontToolW( const FontDescW* pFontDesc, CxFontTool** ppOutFontTool )
{
	if ( pFontDesc == NULL )
		return false;
	if ( ppOutFontTool == NULL )
		return false;
	if ( *ppOutFontTool )
		return false;

	CFontToolFT2* lc_pFontTool = new CFontToolFT2;
	if ( lc_pFontTool == NULL )
		return false;

	if ( lc_pFontTool->onCreateW( this, pFontDesc ) == false )
	{
		lc_pFontTool->release();
		lc_pFontTool = NULL;

		return false;
	}

	*ppOutFontTool = lc_pFontTool;

	return true;
}


bool iberbar::CFontDeviceFT2::createFontToolA( const FontDescA* pFontDesc, CxFontTool** ppOutFontTool )
{
	if ( pFontDesc == NULL )
		return false;
	if ( pFontDesc->FaceName.c_str()[ 0 ] == 0 )
		return false;
	if ( ppOutFontTool == NULL )
		return false;
	if ( *ppOutFontTool )
		return false;

	CFontToolFT2* lc_pFontTool = new CFontToolFT2;
	if ( lc_pFontTool == NULL )
		return false;

	if ( lc_pFontTool->onCreateA( this, pFontDesc ) == false )
	{
		lc_pFontTool->release();
		lc_pFontTool = NULL;

		return false;
	}

	*ppOutFontTool = lc_pFontTool;

	return true;
}


bool iberbar::CFontDeviceFT2::createConstantLabelA( CxFontTool* pFontTool, FontConstLabelBitmapDescA* pLabelDesc )
{
	if ( pFontTool == NULL )
		return false;
	if ( pLabelDesc == NULL )
		return false;
	if ( pLabelDesc->text.c_str()[ 0 ] == 0 )
		return false;

	FontConstLabelBitmapDescW lc_LabelStyleDesc;
	lc_LabelStyleDesc.text = Utf8ToUnicode( pLabelDesc->text.c_str() );
	lc_LabelStyleDesc.nOutlineSize = pLabelDesc->nOutlineSize;
	lc_LabelStyleDesc.nFormat = pLabelDesc->nFormat;
	lc_LabelStyleDesc.pBitsFill = NULL;
	lc_LabelStyleDesc.pBitsOutline = NULL;
	lc_LabelStyleDesc.nBitsWidth = 0;
	lc_LabelStyleDesc.nBitsHeight = 0;
	if ( createConstantLabelW( pFontTool, &lc_LabelStyleDesc ) == false )
		return false;

	pLabelDesc->nBitsWidth = lc_LabelStyleDesc.nBitsWidth;
	pLabelDesc->nBitsHeight = lc_LabelStyleDesc.nBitsHeight;
	pLabelDesc->pBitsFill = lc_LabelStyleDesc.pBitsFill;
	pLabelDesc->pBitsOutline = lc_LabelStyleDesc.pBitsOutline;

	return true;
}


bool iberbar::CFontDeviceFT2::createConstantLabelW( CxFontTool* pFontTool, FontConstLabelBitmapDescW* pLabelDesc )
{
	if ( pFontTool == NULL )
		return false;
	if ( pLabelDesc == NULL )
		return false;
	if ( pLabelDesc->text.c_str()[ 0 ] == 0 )
		return false;

	if ( begin( pFontTool ) == false )
	{
		end();
		return false;
	}

	createConstantLabelInternal( pLabelDesc );

	end();

	return true;
}


bool iberbar::CFontDeviceFT2::createChBitmap( wchar_t ch, __out FontCharBitmapDesc* pBmpDesc, UFontBitsFormat format )
{
	assert( m_pCurFontTool );
	FT_Error lc_error = FT_Err_Ok;

	FT_Face lc_Face = m_pCurFontTool->getFTFace();
	assert( lc_Face );

	FT_UInt lc_glyph_index = FT_Get_Char_Index( lc_Face, ch );
	lc_error = FT_Load_Glyph( lc_Face, lc_glyph_index, FT_LOAD_FORCE_AUTOHINT );
	if ( lc_error )
		return false;

	//得到字模 
	FT_GlyphSlot lc_slot = lc_Face->glyph;
	FT_Glyph lc_glyph;
	if ( FT_Get_Glyph( lc_slot, &lc_glyph ) )
		return false;

	//转化成位图 
	FT_Render_Glyph( lc_slot, FT_RENDER_MODE_NORMAL );
	FT_Glyph_To_Bitmap( &lc_glyph, ft_render_mode_normal, 0, true );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)lc_glyph;

	//取道位图数据
	FT_Bitmap& lc_bitmap = bitmap_glyph->bitmap;

	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	int lc_copy_width =  lc_bitmap.width;
	int lc_copy_height =  lc_bitmap.rows;
	int _ascender = (int)( lc_Face->size->metrics.ascender / 64.0f );
	int _adv_x = (int)( lc_Face->glyph->metrics.horiAdvance / 64.0f );  //步进宽度
	int _adv_y = (int)/*( lc_Face->glyph->metrics.vertAdvance / 64.0f )*/lc_Face->size->metrics.y_ppem;
	int _delta_x = (int)bitmap_glyph->left;			//left:字形原点(0,0)到字形位图最左边象素的水平距离.它以整数象素的形式表示。 
	int _delta_y = (int)( _ascender-lc_Face->glyph->bitmap_top );
	_adv_y = tMax( _delta_y+lc_copy_height, _adv_y);

	m_LabelBitmapBuffer.start_paint( CSize2i( _adv_x, _adv_y ), format );
	if ( m_LabelBitmapBuffer.get_bits_fill() == NULL )
	{
		FT_Done_Glyph( lc_glyph );
		return false;
	}
	m_LabelBitmapBuffer.paint_char(
		_adv_x,
		CSize2i( lc_copy_width, lc_copy_height ),
		CPoint2i( _delta_x, _delta_y ),
		lc_bitmap.buffer );
	m_LabelBitmapBuffer.end_paint();

	pBmpDesc->pBitsFill = m_LabelBitmapBuffer.get_bits_fill();
	pBmpDesc->bOutline = false;
	pBmpDesc->nBmpWidth = _adv_x;
	pBmpDesc->nBmpHeight = _adv_y;
	pBmpDesc->nCh = ch;
	pBmpDesc->nCharWidth = _adv_x;
	pBmpDesc->nCharHeight = m_nFontHeight;

	FT_Done_Glyph( lc_glyph );

	return true;



// 	int lc_offset_x;
// 	int lc_offset_y;
// 	for ( int lc_j=0; lc_j < lc_copy_height; lc_j++ )
// 	{
// 		for ( int lc_i=0; lc_i < lc_copy_width; lc_i++ )
// 		{
// 			lc_offset_x = lc_i+_delta_x;
// 			lc_offset_y = lc_j+_delta_y;
// 			if ( lc_offset_x >= 0 &&
// 				lc_offset_y >= 0 &&
// 				lc_offset_x < _adv_x &&
// 				lc_offset_y < _adv_y )
// 				lc_pBits[ lc_offset_x + lc_offset_y * _adv_x ] = Color4B( lc_bitmap.buffer[ lc_i + lc_bitmap.width*lc_j ], 255, 255, 255 );
// 		}
// 	}
}


int iberbar::CFontDeviceFT2::getCharWidth( wchar_t ch )
{
	return 0;
}


void iberbar::CFontDeviceFT2::setChBitmapFormat( FontChBitmapFormat Format )
{
}


bool iberbar::CFontDeviceFT2::createConstantLabelInternal( FontConstLabelBitmapDescW* pLabelDesc )
{
	struct Glyph_Temp 
	{
		int      nCharWidth;
		FT_Glyph pFtGlyph;
	};

	assert( pLabelDesc );
	assert( m_pCurFontTool );

	FT_Error lc_error = FT_Err_Ok;
	FT_Face lc_Face = m_pCurFontTool->getFTFace();
	FT_GlyphSlot lc_slot = NULL;
	FT_Glyph lc_glyph = NULL;
	

	int lc_nPaintSize = 0;

	Glyph_Temp lc_temp;
	std::vector< Glyph_Temp > lc_glyphArray;




	const wchar_t* lc_strptr = pLabelDesc->text.c_str();
	while( lc_strptr[ 0 ] != 0 )
	{
		lc_temp.pFtGlyph = NULL;
		lc_temp.nCharWidth = 0;

		lc_error = FT_Load_Glyph( lc_Face, FT_Get_Char_Index( lc_Face, lc_strptr[ 0 ] ), FT_LOAD_FORCE_AUTOHINT );
		if ( lc_error == 0 )
		{
			lc_error = FT_Get_Glyph( lc_Face->glyph, &lc_glyph );
			if ( lc_error == 0 )
			{
				lc_nPaintSize += ( (int)lc_Face->glyph->metrics.horiAdvance >> 6 );  //步进宽度

				lc_temp.pFtGlyph = lc_glyph;
				lc_temp.nCharWidth = ( (int)lc_Face->glyph->metrics.horiAdvance >> 6 );
			}
		}

		lc_glyphArray.push_back( lc_temp );
		lc_strptr ++;
	}


	//
	//  将字符位图数据拷贝到大位图中
	//
	FT_Stroker_Set( m_Stroker, (int)( ( pLabelDesc->nOutlineSize << 6 ) >> 1 ), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0 );


	m_LabelBitmapBuffer.start_paint( CSize2i( lc_nPaintSize, m_nFontHeight ), UFontBitsFormat::ARGB );

	int _ascender;
	int _delta_x_fill;
	int _delta_y_fill;
	int _delta_x_outline;
	int _delta_y_outline;
	int _bitmap_cx_fill;
	int _bitmap_cy_fill;
	int _bitmap_cx_outline;
	int _bitmap_cy_outline;
	FT_Glyph lc_glyph_fill;
	FT_Glyph lc_glyph_outline;
	FT_BitmapGlyph lc_bitmap_glyph_fill = NULL;
	FT_BitmapGlyph lc_bitmap_glyph_outline = NULL;

	_ascender = (int)( lc_Face->size->metrics.ascender / 64.0f );

	for ( size_t lc_i = 0; lc_i < lc_glyphArray.size(); lc_i ++ )
	{
		if ( lc_glyphArray[ lc_i ].pFtGlyph )
		{
			FT_Error lc_ret;

			//
			// paint fill
			//
			lc_glyph_fill = lc_glyphArray[ lc_i ].pFtGlyph;
			lc_ret = FT_Glyph_Copy( lc_glyph_fill, &lc_glyph_outline );

			FT_Render_Glyph( lc_slot, FT_RENDER_MODE_NORMAL );
			FT_Glyph_To_Bitmap( &lc_glyph_fill, ft_render_mode_normal, 0, true );
			lc_bitmap_glyph_fill = (FT_BitmapGlyph)lc_glyph_fill;

			_delta_x_fill = (int)lc_bitmap_glyph_fill->left;
			_delta_y_fill = (int)( _ascender - lc_bitmap_glyph_fill->top );
			_bitmap_cx_fill = lc_bitmap_glyph_fill->bitmap.width;
			_bitmap_cy_fill = lc_bitmap_glyph_fill->bitmap.rows;
			
			//
			//  paint outline
			//
			if ( pLabelDesc->nOutlineSize > 0 )
			{
				lc_ret = FT_Glyph_StrokeBorder( &lc_glyph_outline, m_Stroker, 0, true );
				FT_Outline* lc_outline = &( reinterpret_cast<FT_OutlineGlyph>( lc_glyph_outline )->outline );
				FT_Raster_Params params;
				memset( &params, 0, sizeof( params ) );
				params.flags = FT_RASTER_FLAG_AA;
				lc_ret = FT_Outline_Render( m_Library, lc_outline, &params );
				FT_Glyph_To_Bitmap( &lc_glyph_outline, ft_render_mode_normal, 0, true );
				lc_bitmap_glyph_outline = (FT_BitmapGlyph)lc_glyph_outline;

				_delta_x_outline = (int)lc_bitmap_glyph_outline->left;
				_delta_y_outline = (int)( _ascender - lc_bitmap_glyph_outline->top );
				_bitmap_cx_outline = lc_bitmap_glyph_outline->bitmap.width;
				_bitmap_cy_outline = lc_bitmap_glyph_outline->bitmap.rows;
			}


			//
			//  copy data
			//
			if ( pLabelDesc->nOutlineSize > 0 )
			{
				m_LabelBitmapBuffer.paint_char(
					lc_glyphArray[ lc_i ].nCharWidth,
					CSize2i( _bitmap_cx_fill, _bitmap_cy_fill ),
					CPoint2i( _delta_x_fill, _delta_y_fill ),
					lc_bitmap_glyph_fill->bitmap.buffer,
					CSize2i( _bitmap_cx_outline, _bitmap_cy_outline ),
					CPoint2i( _delta_x_outline, _delta_y_outline ),
					lc_bitmap_glyph_outline->bitmap.buffer );
			}
			else
			{
				m_LabelBitmapBuffer.paint_char(
					lc_glyphArray[ lc_i ].nCharWidth,
					CSize2i( _bitmap_cx_fill, _bitmap_cy_fill ),
					CPoint2i( _delta_x_fill, _delta_y_fill ),
					lc_bitmap_glyph_fill->bitmap.buffer );
			}


			//
			//  release
			//
			FT_Done_Glyph( lc_glyph_fill );
			if ( lc_glyph_outline )
				FT_Done_Glyph( lc_glyph_outline );
			lc_glyph_fill = NULL;
			lc_glyph_outline = NULL;
		}
	}

	m_LabelBitmapBuffer.end_paint();

	pLabelDesc->nBitsWidth = lc_nPaintSize;
	pLabelDesc->nBitsHeight = m_nFontHeight;
	pLabelDesc->pBitsFill = m_LabelBitmapBuffer.get_bits_fill();
	if ( pLabelDesc->nOutlineSize > 0 )
		pLabelDesc->pBitsOutline = m_LabelBitmapBuffer.get_bits_outline();

	return true;
}










iberbar::CFontToolFT2::CFontToolFT2( void )
	: m_face( NULL )
{
}


iberbar::CFontToolFT2::~CFontToolFT2()
{
	if ( m_face )
	{
		FT_Done_Face( m_face );
		m_face = NULL;
	}
}


bool iberbar::CFontToolFT2::onCreateA( CFontDeviceFT2* pDeviceFT2, const FontDescA* pFontDesc )
{
	FT_Error lc_error = FT_Err_Ok;

	lc_error = FT_New_Face( pDeviceFT2->getFTLibrary(), pFontDesc->FaceName.c_str(), 0, &m_face );
	if ( lc_error == FT_Err_Unknown_File_Format )
		return false;
	else if ( lc_error )
		return false;

	// 先获取该字体的逻辑尺寸和实际尺寸的比例
	lc_error = FT_Set_Pixel_Sizes( m_face, pFontDesc->Width/**2/3*/, pFontDesc->Height/**2/3*/ );
	if ( lc_error )
		return false;
	int lc_RealSize = m_face->size->metrics.height >> 6;

	// 重新计算真实需要的逻辑尺寸
	int FontWidth  = pFontDesc->Height * pFontDesc->Width / lc_RealSize;
	int FontHeight = pFontDesc->Height * pFontDesc->Height / lc_RealSize;
	lc_error = FT_Set_Pixel_Sizes( m_face, FontWidth, FontHeight );
	if ( lc_error )
		return false;

	m_FontDesc.Height          = pFontDesc->Height;
	m_FontDesc.Width           = pFontDesc->Width;
	m_FontDesc.Weight          = pFontDesc->Weight;
	m_FontDesc.Italic          = pFontDesc->Italic;
	m_FontDesc.CharSet         = pFontDesc->CharSet;
	m_FontDesc.OutputPrecision = pFontDesc->OutputPrecision;
	m_FontDesc.Quality         = pFontDesc->Quality;
	m_FontDesc.PitchAndFamily  = pFontDesc->PitchAndFamily;
	m_FontDesc.FaceName = Utf8ToUnicode( pFontDesc->FaceName.c_str() );

	return true;
}


bool iberbar::CFontToolFT2::onCreateW( CFontDeviceFT2* pDeviceFT2, const FontDescW* pFontDesc )
{
	char* lc_pCharBuffer = NULL;
	int lc_nCharCount = unicode_to_utf8_calculate( pFontDesc->FaceName.c_str(), -1 );
	lc_pCharBuffer = new char[ lc_nCharCount ];
	if ( lc_pCharBuffer == NULL )
		return false;
	unicode_to_utf8( pFontDesc->FaceName.c_str(), -1, lc_pCharBuffer, lc_nCharCount );

	std::string lc_strFaceName( lc_pCharBuffer );
	delete[] lc_pCharBuffer;
	lc_pCharBuffer = NULL;


	FT_Error lc_error = FT_Err_Ok;

	lc_error = FT_New_Face( pDeviceFT2->getFTLibrary(), lc_strFaceName.c_str(), 0, &m_face );
	if ( lc_error == FT_Err_Unknown_File_Format )
		return false;
	else if ( lc_error )
		return false;

	// 先获取该字体的逻辑尺寸和实际尺寸的比例
	lc_error = FT_Set_Pixel_Sizes( m_face, pFontDesc->Width/**2/3*/, pFontDesc->Height/**2/3*/ );
	if ( lc_error )
		return false;
	int lc_RealSize = m_face->size->metrics.height >> 6;

	// 重新计算真实需要的逻辑尺寸
	int FontWidth  = pFontDesc->Height * pFontDesc->Width / lc_RealSize;
	int FontHeight = pFontDesc->Height * pFontDesc->Height / lc_RealSize;
	lc_error = FT_Set_Pixel_Sizes( m_face, FontWidth, FontHeight );
	if ( lc_error )
		return false;

	m_FontDesc = *pFontDesc;

	return true;
}












//--------------------------------------------------------------------------
bool iberbar::CreateXFontDeviceFromFT2( CxFontDevice** ppDevice )
{
	if ( ppDevice == NULL || *ppDevice )
		return false;

	CFontDeviceFT2* lc_pDevice = new CFontDeviceFT2();
	if ( lc_pDevice == NULL )
		return false;

	if ( lc_pDevice->initFreeType() == false )
	{
		lc_pDevice->release();
		lc_pDevice = NULL;
		return false;
	}

	*ppDevice = lc_pDevice;

	return true;
}
