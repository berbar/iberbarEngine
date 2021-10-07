
#include <iberbar/Font/FontDeviceFreeType.h>
#include <iberbar/Utility/Math.h>
#include <iberbar/Utility/Buffer.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H
#include <freetype/fterrors.h>



namespace iberbar
{

	class CFreeTypeCore
	{
	public:
		CFreeTypeCore();
		~CFreeTypeCore();

	public:
		inline CResult Initialize();
		inline FT_Library GetFTLibrary() { return m_Library; }

	private:
		FT_Library m_Library;
		FT_Stroker m_Stroker;
	};

	class CFreeTypeFace
	{
	public:
		CFreeTypeFace();
		~CFreeTypeFace();
		FT_Face pFT_Face;
	};

	class CFontBitsBufferFreeType
	{
	public:
		CFontBitsBufferFreeType( void );
		~CFontBitsBufferFreeType();

		void start_paint( CSize2i BitmapSize, UFontBitsFormat format );
		void end_paint();
		void paint_char( int nCharWidth, CSize2i BitmapSize, CPoint2i BitmapDelta, uint8* pBitsFill ) throw();
		void paint_char(
			int nCharWidth,
			CSize2i BitmapSizeFill, CPoint2i BitmapDeltaFill, uint8* pBitsFill,
			CSize2i BitmapSizeOutline, CPoint2i BitmapDeltaOutline, uint8* pBitsOutline ) throw();

		int   get_paint_size();
		void* get_bits_fill() { return m_pBits[0].GetPointer(); }
		void* get_bits_outline() { return m_pBits[1].GetPointer(); }


	private:
		TBuffer<uint8> m_pBits[2];
		uint32 m_nBitsSize;
		bool   m_bStartDraw;
		int    m_nPaintOffset;
		int    m_nPaintSize;
		CSize2i  m_BitmapSize;
		UFontBitsFormat m_format;
	};
}




iberbar::CFreeTypeCore::CFreeTypeCore()
	: m_Library( nullptr )
	, m_Stroker( nullptr )
{
}


iberbar::CFreeTypeCore::~CFreeTypeCore()
{
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


iberbar::CResult iberbar::CFreeTypeCore::Initialize()
{
	FT_Error lc_error = FT_Init_FreeType( &m_Library );
	if ( lc_error )
		return MakeResult( ResultCode::Bad, FT_Error_String( lc_error ) );

	lc_error = FT_Stroker_New( m_Library, &m_Stroker );
	if ( lc_error )
		return MakeResult( ResultCode::Bad, FT_Error_String( lc_error ) );

	return CResult();
}



iberbar::CFreeTypeFace::CFreeTypeFace()
	: pFT_Face( nullptr )
{
}

iberbar::CFreeTypeFace::~CFreeTypeFace()
{
	if ( pFT_Face )
	{
		FT_Done_Face( pFT_Face );
		pFT_Face = nullptr;
	}
}



iberbar::CFontDeviceFreeType::CFontDeviceFreeType()
	: CRef()
	, m_pFreeTypeCore( new CFreeTypeCore() )
{
}

iberbar::CFontDeviceFreeType::~CFontDeviceFreeType()
{
	SAFE_DELETE( m_pFreeTypeCore );
}

iberbar::CResult iberbar::CFontDeviceFreeType::Initialize()
{
	return m_pFreeTypeCore->Initialize();
}

iberbar::CResult iberbar::CFontDeviceFreeType::CreateFontFace( const iberbar::UFontDesc& fontDesc, iberbar::CFontFaceFreeType** ppFontFace )
{
	assert( ppFontFace );
	CFontFaceFreeType* fontFace = new CFontFaceFreeType();
	CResult ret = fontFace->Create( m_pFreeTypeCore, fontDesc );
	if ( ret.IsOK() )
	{
		(*ppFontFace) = fontFace;
		(*ppFontFace)->AddRef();
	}
	fontFace->Release();
	return ret;
}





iberbar::CFontFaceFreeType::CFontFaceFreeType()
	: CRef()
	, m_face( nullptr )
	, m_buffer( nullptr )
{

}

iberbar::CFontFaceFreeType::~CFontFaceFreeType()
{
	SAFE_DELETE( m_face );
	SAFE_DELETE( m_buffer );
}

iberbar::CResult iberbar::CFontFaceFreeType::Create( CFreeTypeCore* pFreeTypeCore, const UFontDesc& fontDesc )
{
	FT_Error lc_error = FT_Err_Ok;

	m_face = new CFreeTypeFace();

	lc_error = FT_New_Face( pFreeTypeCore->GetFTLibrary(), fontDesc.FamilyName.c_str(), 0, &m_face->pFT_Face );
	if ( lc_error == FT_Err_Unknown_File_Format )
		return MakeResult( ResultCode::Bad, FT_Error_String( lc_error ) );
	else if ( lc_error )
		return MakeResult( ResultCode::Bad, FT_Error_String( lc_error ) );

	// 先获取该字体的逻辑尺寸和实际尺寸的比例
	lc_error = FT_Set_Pixel_Sizes( m_face->pFT_Face, fontDesc.Size/**2/3*/, fontDesc.Size/**2/3*/ );
	if ( lc_error )
		return MakeResult( ResultCode::Bad, FT_Error_String( lc_error ) );
	int lc_RealSize = m_face->pFT_Face->size->metrics.height >> 6;

	// 重新计算真实需要的逻辑尺寸
	int FontWidth = fontDesc.Size * fontDesc.Size / lc_RealSize;
	int FontHeight = fontDesc.Size * fontDesc.Size / lc_RealSize;
	lc_error = FT_Set_Pixel_Sizes( m_face->pFT_Face, FontWidth, FontHeight );
	if ( lc_error )
		return MakeResult( ResultCode::Bad, FT_Error_String( lc_error ) );

	m_fontDesc = fontDesc;

	m_buffer = new CFontBitsBufferFreeType();

	return CResult();
}

iberbar::CResult iberbar::CFontFaceFreeType::CreateCharBitmap( wchar_t ch, iberbar::UFontCharBitmapDesc* pDesc, UFontBitsFormat bitsFormat )
{
	FT_Error error = FT_Err_Ok;

	FT_Face lc_Face = m_face->pFT_Face;
	assert( lc_Face );

	FT_UInt lc_glyph_index = FT_Get_Char_Index( lc_Face, ch );
	error = FT_Load_Glyph( lc_Face, lc_glyph_index, FT_LOAD_FORCE_AUTOHINT );
	if ( error )
		return MakeResult( ResultCode::Bad, FT_Error_String( error ) );

	//得到字模 
	FT_GlyphSlot slot = lc_Face->glyph;
	FT_Glyph glyph;
	error = FT_Get_Glyph( slot, &glyph );
	if ( error )
		return MakeResult( ResultCode::Bad, FT_Error_String( error ) );

	//转化成位图 
	FT_Render_Glyph( slot, FT_RENDER_MODE_NORMAL );
	FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, true );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//取道位图数据
	FT_Bitmap& lc_bitmap = bitmap_glyph->bitmap;

	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	int lc_copy_width = lc_bitmap.width;
	int lc_copy_height = lc_bitmap.rows;
	int _ascender = (int)(lc_Face->size->metrics.ascender / 64.0f);
	int _adv_x = (int)(lc_Face->glyph->metrics.horiAdvance / 64.0f);  //步进宽度
	int _adv_y = (int)/*( lc_Face->glyph->metrics.vertAdvance / 64.0f )*/lc_Face->size->metrics.y_ppem;
	int _delta_x = (int)bitmap_glyph->left;			//left:字形原点(0,0)到字形位图最左边象素的水平距离.它以整数象素的形式表示。 
	int _delta_y = (int)(_ascender - lc_Face->glyph->bitmap_top);
	_adv_y = tMax( _delta_y + lc_copy_height, _adv_y );

	if ( ch == L'j' )
	{
		int aaa = 0;
	}

	m_buffer->start_paint( CSize2i( _adv_x, _adv_y ), bitsFormat );
	if ( m_buffer->get_bits_fill() == NULL )
	{
		FT_Done_Glyph( glyph );
		return MakeResult( ResultCode::Bad, "out of memory" );
	}
	m_buffer->paint_char(
		_adv_x,
		CSize2i( lc_copy_width, lc_copy_height ),
		CPoint2i( _delta_x, _delta_y ),
		lc_bitmap.buffer );
	m_buffer->end_paint();

	pDesc->pBitsFill = m_buffer->get_bits_fill();
	pDesc->bOutline = false;
	pDesc->nBmpWidth = _adv_x;
	pDesc->nBmpHeight = _adv_y;
	pDesc->nCharWidth = _adv_x;
	pDesc->nCharHeight = m_fontDesc.Size;
	pDesc->nDeltaX = 0;

	FT_Done_Glyph( glyph );

	return CResult();
}

bool iberbar::CFontFaceFreeType::CreateConstLabelBitmap( const wchar_t* str, UFontCharBitmapDesc* pDesc )
{
	return false;
}

int iberbar::CFontFaceFreeType::MeasureCharWidth( wchar_t ch )
{
	return 0;
}








iberbar::CFontBitsBufferFreeType::CFontBitsBufferFreeType( void )
	: m_nBitsSize( 0 )
	, m_bStartDraw( false )
	, m_nPaintOffset( 0 )
	, m_nPaintSize( 0 )
	, m_BitmapSize( 0, 0 )
	, m_format( UFontBitsFormat::ARGB )
{
	//m_pBits[0] = NULL;
	//m_pBits[1] = NULL;
}


iberbar::CFontBitsBufferFreeType::~CFontBitsBufferFreeType()
{
	//SAFE_DELETE_ARRAY( m_pBits[0] );
	//SAFE_DELETE_ARRAY( m_pBits[1] );
	m_pBits[ 0 ].Clear();
	m_pBits[ 1 ].Clear();
}


void iberbar::CFontBitsBufferFreeType::start_paint( CSize2i BitmapSize, UFontBitsFormat format )
{
	assert( BitmapSize.w > 0 && BitmapSize.h );
	m_bStartDraw = true;
	m_nPaintOffset = 0;
	m_nPaintSize = 0;

	uint32 lc_bitsSize = BitmapSize.w * BitmapSize.h * 4;
	if ( m_nBitsSize == 0 || lc_bitsSize > m_nBitsSize )
	{
		//SAFE_DELETE_ARRAY( m_pBits[0] );
		//SAFE_DELETE_ARRAY( m_pBits[1] );
		//m_pBits[0] = new uint8[lc_bitsSize];
		//m_pBits[1] = new uint8[lc_bitsSize];
		m_pBits[ 0 ].Resize( lc_bitsSize, false );
		m_pBits[ 1 ].Resize( lc_bitsSize, false );
		m_nBitsSize = lc_bitsSize;
	}
	memset( m_pBits[0].GetPointer(), 0, lc_bitsSize );
	memset( m_pBits[1].GetPointer(), 0, lc_bitsSize );
	m_BitmapSize = BitmapSize;
	m_format = format;
}


void iberbar::CFontBitsBufferFreeType::end_paint()
{
	m_bStartDraw = false;
}


void iberbar::CFontBitsBufferFreeType::paint_char( int nCharWidth, CSize2i ChBitmapSize, CPoint2i ChBitmapDelta, uint8* pBitsFill ) throw()
{
	byte* lc_pBitsFill = m_pBits[0].GetPointer();

	int nCopyHeight = tMin( ChBitmapSize.h, m_BitmapSize.h - ChBitmapDelta.y );
	int lc_nCopyWidth = tMin( ChBitmapSize.w, m_BitmapSize.w - m_nPaintOffset - ChBitmapDelta.x );
	int lc_src_start_x = (ChBitmapDelta.x < 0) ? (-ChBitmapDelta.x) : 0;
	int lc_src_start_y = (ChBitmapDelta.y < 0) ? (-ChBitmapDelta.y) : 0;
	int lc_dest_start_x = ((ChBitmapDelta.x < 0) ? 0 : ChBitmapDelta.x) + m_nPaintOffset;
	int lc_dest_start_y = ((ChBitmapDelta.y < 0) ? 0 : ChBitmapDelta.y);

	int rowByteSize = m_BitmapSize.w * 4;
	int rowByteSizeCopy = lc_nCopyWidth * 4;
	int dest_offset = (lc_dest_start_x + lc_dest_start_y * m_BitmapSize.w) * 4;
	int dest_offset_x = 0;
	int src_offset = lc_src_start_y * ChBitmapSize.w + lc_src_start_x;
	int src_offset_last = src_offset;
	byte* ptr = nullptr;

	int alphaIndex = 3;

	if ( m_format == UFontBitsFormat::BGRA || m_format == UFontBitsFormat::RGBA )
	{
		alphaIndex = 0;
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


void iberbar::CFontBitsBufferFreeType::paint_char( int nCharWidth, CSize2i ChBitmapSize, CPoint2i ChBitmapDelta, uint8* pBitsFill, CSize2i BitmapSizeOutline, CPoint2i ChBitmapDeltaOutline, uint8* pBitsOutline ) throw()
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


