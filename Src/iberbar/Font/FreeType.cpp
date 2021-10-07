

#include <iberbar/Font/FreeType.h>
#include <iberbar/Utility/Math.h>
#include <iberbar/Utility/Buffer.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H
#include <freetype/fterrors.h>



namespace iberbar
{
	class CBitsBuffer
	{
	public:
		CBitsBuffer( void );
		~CBitsBuffer();

		void PaintChar( const CSize2i& BitmapSize, const CSize2i& CopySize, const CPoint2i& CopyOffset, const uint8* pBytes );
		void PaintChar( const CSize2i& BitmapSize, const uint8* pBytes );
		void* GetBitsFill() { return m_pBits.GetPointer(); }

	private:
		TBuffer<uint8> m_pBits;
		CSize2i m_BitmapSize;
	};

	class CFontDeviceFtCore
	{
	public:
		CFontDeviceFtCore();
		~CFontDeviceFtCore();

	public:
		CResult Initialize();
		FORCEINLINE FT_Library GetFTLibrary() { return m_Library; }

	private:
		FT_Library m_Library;
		FT_Stroker m_Stroker;
	};

	class CFontFaceFtCore
	{
	public:
		CFontFaceFtCore();
		~CFontFaceFtCore();
		CResult Initialize( FT_Library pLibrary, const char* strFile );
		FORCEINLINE FT_Face GetFtFace() { return m_pFtFace; }

	private:
		FT_Face m_pFtFace;
	};
}




iberbar::CFontDeviceFtCore::CFontDeviceFtCore()
	: m_Library( nullptr )
	, m_Stroker( nullptr )
{
}


iberbar::CFontDeviceFtCore::~CFontDeviceFtCore()
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


iberbar::CResult iberbar::CFontDeviceFtCore::Initialize()
{
	FT_Error err = FT_Init_FreeType( &m_Library );
	if ( err )
		return MakeResult( ResultCode::Bad, FT_Error_String( err ) );

	err = FT_Stroker_New( m_Library, &m_Stroker );
	if ( err )
		return MakeResult( ResultCode::Bad, FT_Error_String( err ) );

	return CResult();
}






iberbar::CFontFaceFtCore::CFontFaceFtCore()
	: m_pFtFace( nullptr )
{
}


iberbar::CFontFaceFtCore::~CFontFaceFtCore()
{
	if ( m_pFtFace )
	{
		FT_Done_Face( m_pFtFace );
		m_pFtFace = nullptr;
	}
}


iberbar::CResult iberbar::CFontFaceFtCore::Initialize( FT_Library pLibrary, const char* strFile )
{
	FT_Error err = FT_New_Face( pLibrary, strFile, 0, &m_pFtFace );
	if ( err == FT_Err_Unknown_File_Format )
		return MakeResult( ResultCode::Bad, FT_Error_String( err ) );
	else if ( err )
		return MakeResult( ResultCode::Bad, FT_Error_String( err ) );
	return CResult();
}






iberbar::CFontDeviceFt::CFontDeviceFt()
	: m_pCore( new CFontDeviceFtCore() )
{
}


iberbar::CFontDeviceFt::~CFontDeviceFt()
{
	SAFE_DELETE( m_pCore );
}


iberbar::CResult iberbar::CFontDeviceFt::Initialize()
{
	return m_pCore->Initialize();
}


iberbar::CResult iberbar::CFontDeviceFt::CreateFace( CFontFaceFt** ppOutFace, const char* strFile )
{
	assert( ppOutFace );

	CFontFaceFtCore* pFaceCore = new CFontFaceFtCore();
	CResult ret = pFaceCore->Initialize( m_pCore->GetFTLibrary(), strFile );
	if ( ret.IsOK() == false )
	{
		delete pFaceCore;
		return ret;
	}
	if ( *ppOutFace )
		(*ppOutFace)->Release();
	(*ppOutFace) = new CFontFaceFt( pFaceCore );

	return CResult();
}







iberbar::CFontFaceFt::CFontFaceFt( CFontFaceFtCore* pFaceCore )
	: m_pCore( pFaceCore )
	, m_nFontSizeNow( 0 )
	, m_nFontWeightNow( 0 )
	, m_pByteBuffer( new CBitsBuffer() )
{
}


iberbar::CFontFaceFt::~CFontFaceFt()
{
	SAFE_DELETE( m_pCore );
	SAFE_DELETE( m_pByteBuffer );
}


iberbar::CResult iberbar::CFontFaceFt::SetFontSize( int nSize )
{
	FT_Error err;
	FT_Face pFtFace = m_pCore->GetFtFace();

	if ( m_nFontSizeNow == nSize )
		return CResult();

	m_nFontSizeNow = nSize;
	if ( m_nFontSizeNow <= 0 )
		return MakeResult( ResultCode::Bad, "Invalid font size" );

	//err = FT_Set_Char_Size( pFtFace, 0, nSize * 64, 72, 72 );
	//if ( err )
	//	return MakeResult( ResultCode::Bad, FT_Error_String( err ) );

	// 先获取该字体的逻辑尺寸和实际尺寸的比例
	err = FT_Set_Pixel_Sizes( pFtFace, nSize, nSize );
	if ( err )
		return MakeResult( ResultCode::Bad, FT_Error_String( err ) );
	int lc_RealSize = pFtFace->size->metrics.height >> 6;

	// 重新计算真实需要的逻辑尺寸
	int FontWidth = nSize * nSize / lc_RealSize;
	int FontHeight = nSize * nSize / lc_RealSize;
	err = FT_Set_Pixel_Sizes( pFtFace, FontWidth, FontHeight );
	if ( err )
		return MakeResult( ResultCode::Bad, FT_Error_String( err ) );

	return CResult();
}


iberbar::CResult iberbar::CFontFaceFt::SetFontWeight( int nWeight )
{
	
	return CResult();
}


iberbar::CResult iberbar::CFontFaceFt::CreateCharBitmap( wchar_t nChar, UFontCharBitmapDesc* pDesc, UFontBitsFormat nBitsFormat )
{
	FT_Error error = FT_Err_Ok;

	FT_Face pFtFace = m_pCore->GetFtFace();
	assert( pFtFace );

	FT_UInt nGlyphIndex = FT_Get_Char_Index( pFtFace, nChar );
	//FT_Int32 nLoadGlyphFlags = FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER;
	FT_Int32 nLoadGlyphFlags = FT_LOAD_FORCE_AUTOHINT;
	error = FT_Load_Glyph( pFtFace, nGlyphIndex, nLoadGlyphFlags );
	if ( error )
		return MakeResult( ResultCode::Bad, FT_Error_String( error ) );

	////得到字模 
	FT_GlyphSlot pGlyphSlot = pFtFace->glyph;
	FT_Glyph pGlyph;
	error = FT_Get_Glyph( pGlyphSlot, &pGlyph );
	if ( error )
		return MakeResult( ResultCode::Bad, FT_Error_String( error ) );

	//转化成位图 
	error = FT_Render_Glyph( pGlyphSlot, FT_RENDER_MODE_NORMAL );
	if ( error )
		return MakeResult( ResultCode::Bad, FT_Error_String( error ) );
	error = FT_Glyph_To_Bitmap( &pGlyph, FT_RENDER_MODE_NORMAL, 0, true );
	if ( error )
		return MakeResult( ResultCode::Bad, FT_Error_String( error ) );

	FT_BitmapGlyph pBitmapGlyph = (FT_BitmapGlyph)pGlyph;

	//取道位图数据
	FT_Bitmap& lc_bitmap = pBitmapGlyph->bitmap;

	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	int nBitmapWidth = lc_bitmap.width;
	int nBitmapHeight = lc_bitmap.rows;
	int nAscender = (int)(pFtFace->size->metrics.ascender / 64.0f);
	int nAdvX = (int)(pFtFace->glyph->metrics.horiAdvance / 64.0f);  //步进宽度
	int nAdvY = (int)/*( pFtFace->pGlyph->metrics.vertAdvance / 64.0f )*/pFtFace->size->metrics.y_ppem;
	int nDeltaX = (int)pBitmapGlyph->left;			//left:字形原点(0,0)到字形位图最左边象素的水平距离.它以整数象素的形式表示。 
	int nDeltaY = (int)(nAscender - pFtFace->glyph->bitmap_top);
	nAdvY = tMax( nDeltaY + nBitmapHeight, nAdvY );

	if ( nBitmapWidth > 0 && nBitmapHeight > 0 )
	{
		m_pByteBuffer->PaintChar( CSize2i( nBitmapWidth, nBitmapHeight ), lc_bitmap.buffer );
		pDesc->pBitsFill = m_pByteBuffer->GetBitsFill();
	}
	else
	{
		pDesc->pBitsFill = nullptr;
	}
	
	pDesc->bOutline = false;
	pDesc->nBmpWidth = nBitmapWidth;
	pDesc->nBmpHeight = nBitmapHeight;
	pDesc->nCharWidth = nAdvX;
	pDesc->nCharHeight = m_nFontSizeNow;
	pDesc->nDeltaX = nDeltaX;
	pDesc->nDeltaY = nDeltaY;

	FT_Done_Glyph( pGlyph );

	return CResult();
}





iberbar::CBitsBuffer::CBitsBuffer( void )
	: m_pBits()
{
}


iberbar::CBitsBuffer::~CBitsBuffer()
{
	m_pBits.Clear();
}


//void iberbar::CBitsBuffer::StartPaint( CSize2i BitmapSize, UFontBitsFormat format )
//{
//	assert( BitmapSize.w > 0 && BitmapSize.h );
//	m_bStartDraw = true;
//	m_nPaintOffset = 0;
//	m_nPaintSize = 0;
//
//	uint32 lc_bitsSize = BitmapSize.w * BitmapSize.h * 4;
//	if ( m_nBitsSize == 0 || lc_bitsSize > m_nBitsSize )
//	{
//		m_pBits[ 0 ].Resize( lc_bitsSize, false );
//		m_pBits[ 1 ].Resize( lc_bitsSize, false );
//		m_nBitsSize = lc_bitsSize;
//	}
//	memset( m_pBits[ 0 ].GetPointer(), 0, lc_bitsSize );
//	memset( m_pBits[ 1 ].GetPointer(), 0, lc_bitsSize );
//	m_BitmapSize = BitmapSize;
//	m_format = format;
//}
//
//
//void iberbar::CBitsBuffer::EndPaint()
//{
//	m_bStartDraw = false;
//}


void iberbar::CBitsBuffer::PaintChar( const CSize2i& BitmapSize, const CSize2i& CopySize, const CPoint2i& ptDelta, const uint8* pBytes )
{
	int nByteCount = BitmapSize.w * BitmapSize.h * 4;
	if ( nByteCount > (int)m_pBits.GetDataSize() )
	{
		m_pBits.Resize( nByteCount );
	}
	memset( m_pBits.GetPointer(), 0xff, nByteCount );

	int nAlphaIndex = 3;

	uint8* pDst = m_pBits.GetPointer();
	const uint8* pSrc = pBytes;

	int nSrcStartX = (ptDelta.x < 0) ? 0 : ptDelta.x;
	int nSrcStartY = (ptDelta.y < 0) ? 0 : ptDelta.y;
	int nSrcEndX = nSrcStartX + CopySize.w;
	int nSrcEndY = nSrcStartY + CopySize.h;
	int nDstStride = BitmapSize.w * 4;
	int nSrcStride = CopySize.w;

	for ( int y = 0; y < BitmapSize.h; y++ )
	{
		if ( y >= nSrcStartY && y < nSrcEndY )
		{
			for ( int x = 0; x < BitmapSize.w; x++ )
			{
				if ( x >= nSrcStartX && x < nSrcEndX )
				{
					pDst[ nAlphaIndex ] = pSrc[ x - nSrcStartX ];
				}
				else
				{
					pDst[ nAlphaIndex ] = 0;
				}
				pDst += 4;
			}
			pSrc += nSrcStride;
		}
		else
		{
			for ( int x = 0; x < BitmapSize.w; x++ )
			{
				pDst[ nAlphaIndex ] = 0;
				pDst += 4;
			}
		}
			
	}

	//for ( int y = nSrcStartY; y < nSrcEndY; y++ )
	//{
	//	for ( int x = nSrcStartX; x < nSrcEndX; x++ )
	//	{
	//		pDstTemp[ nAlphaIndex ] = pSrc[ 0 ];
	//		pDstTemp += 4;
	//		pSrc += 1;
	//	}
	//	pDst += nStride;
	//	pDstTemp = pDst;
	//}

	//byte* lc_pBitsFill = m_pBits.GetPointer();

	//int nCopyHeight = tMin( ChBitmapSize.h, m_BitmapSize.h - ChBitmapDelta.y );
	//int lc_nCopyWidth = tMin( ChBitmapSize.w, m_BitmapSize.w - m_nPaintOffset - ChBitmapDelta.x );
	//int lc_src_start_x = (ChBitmapDelta.x < 0) ? (-ChBitmapDelta.x) : 0;
	//int lc_src_start_y = (ChBitmapDelta.y < 0) ? (-ChBitmapDelta.y) : 0;
	//int lc_dest_start_x = ((ChBitmapDelta.x < 0) ? 0 : ChBitmapDelta.x) + m_nPaintOffset;
	//int lc_dest_start_y = ((ChBitmapDelta.y < 0) ? 0 : ChBitmapDelta.y);

	//int rowByteSize = m_BitmapSize.w * 4;
	//int rowByteSizeCopy = lc_nCopyWidth * 4;
	//int dest_offset = (lc_dest_start_x + lc_dest_start_y * m_BitmapSize.w) * 4;
	//int dest_offset_x = 0;
	//int src_offset = lc_src_start_y * ChBitmapSize.w + lc_src_start_x;
	//int src_offset_last = src_offset;
	//byte* ptr = nullptr;

	//int alphaIndex = 3;

	//if ( m_format == UFontBitsFormat::BGRA || m_format == UFontBitsFormat::RGBA )
	//{
	//	alphaIndex = 0;
	//}

	//for ( int lc_j = lc_src_start_y; lc_j < nCopyHeight; lc_j++ )
	//{
	//	ptr = lc_pBitsFill + dest_offset;
	//	memset( ptr, 0xff, rowByteSizeCopy );
	//	for ( int lc_i = lc_src_start_x; lc_i < lc_nCopyWidth; lc_i++ )
	//	{
	//		ptr[ alphaIndex ] = pBitsFill[ src_offset ];
	//		ptr += 4;
	//		src_offset++;
	//	}

	//	dest_offset += rowByteSize;

	//	src_offset = src_offset_last + ChBitmapSize.w;
	//	src_offset_last = src_offset;
	//}

	//m_nPaintOffset += nCharWidth;
	//m_nPaintSize += ChBitmapSize.w;
}


void iberbar::CBitsBuffer::PaintChar( const CSize2i& BitmapSize, const uint8* pBytes )
{
	int nByteCount = BitmapSize.w * BitmapSize.h * 4;
	if ( nByteCount > (int)m_pBits.GetDataSize() )
	{
		m_pBits.Resize( nByteCount );
	}
	memset( m_pBits.GetPointer(), 0xff, nByteCount );

	int nAlphaIndex = 3;

	uint8* pDst = m_pBits.GetPointer();
	const uint8* pSrc = pBytes;

	//int nSrcStartX = ( ptDelta.x < 0 ) ? 0 : ptDelta.x;
	//int nSrcStartY = ( ptDelta.y < 0 ) ? 0 : ptDelta.y;
	//int nSrcEndX = nSrcStartX + CopySize.w;
	//int nSrcEndY = nSrcStartY + CopySize.h;
	int nDstStride = BitmapSize.w * 4;
	int nSrcStride = BitmapSize.w;

	for ( int y = 0; y < BitmapSize.h; y++ )
	{
		for ( int x = 0; x < BitmapSize.w; x++ )
		{
			pDst[nAlphaIndex] = pSrc[0];
			pDst += 4;
			pSrc += 1;
		}
	}
}



