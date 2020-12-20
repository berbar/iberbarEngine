
#include <iberbar/Poster/Font.h>
#include <iberbar/Poster/Sprite.h>




iberbar::Poster::CFont::CFont( CFontFaceFreeType* fontFace )
	: CRef()
	, m_fontFace( fontFace )
{
}

iberbar::Poster::CFont::~CFont()
{
	std::list<__Node*>::iterator iter;
	std::list<__Node*>::iterator end;
	for ( int i = 0; i < 256; i++ )
	{
		iter = m_bitmaps[i].begin();
		end = m_bitmaps[i].end();
		for ( ; iter != end; iter++ )
		{
			SAFE_DELETE( (*iter) );
		}
		m_bitmaps[i].Clear();
	}
	m_fontFace = nullptr;
}

iberbar::Poster::CFontCharBitmap* iberbar::Poster::CFont::GetCharBitmap( char32_t ch )
{
	int group = ch & 0x00ff;
	auto iter = m_bitmaps[group].begin();
	auto end = m_bitmaps[group].end();
	for ( ; iter != end; iter++ )
	{
		if ( (*iter)->bitmap->GetChar() == ch )
			return (*iter)->bitmap;
	}
	CFontCharBitmap* bitmapNew = CreateChBitmap( ch );
	if ( bitmapNew == nullptr )
		return nullptr;
	__Node* node = new __Node( bitmapNew );
	m_bitmaps[group].push_back( node );
	return bitmapNew;
}

int iberbar::Poster::CFont::PreloadCharaters( const char32_t* str )
{
	int count = 0;
	int group = -1;
	CFontCharBitmap* bitmapNew = nullptr;
	const char32_t* strPtr = str;
	while ( *strPtr != 0 )
	{
		bitmapNew = CreateChBitmap( *strPtr );
		if ( bitmapNew == nullptr )
			continue;
		group = *strPtr & 0x00ff;
		__Node* node = new __Node( bitmapNew );
		m_bitmaps[group].push_back( node );
		strPtr++;
		count++;
	}
	return count;
}

iberbar::Poster::CFontCharBitmap* iberbar::Poster::CFont::CreateChBitmap( char32_t ch )
{
	UFontCharBitmapDesc bitsDesc;
	if ( m_fontFace->CreateCharBitmap( ch, &bitsDesc, UFontBitsFormat::BGRA ) == false )
		return nullptr;
	CFontCharBitmap* bitmapNew = new CFontCharBitmap();
	bitmapNew->Create( ch, bitsDesc );
	return bitmapNew;
}



iberbar::Poster::CFontCharBitmap::CFontCharBitmap()
	: m_surface( nullptr )
	, m_char( 0 )
	, m_charWidth( 0 )
	, m_bitmapSize( 0, 0 )
{
}

iberbar::Poster::CFontCharBitmap::~CFontCharBitmap()
{
	m_surface = nullptr;
}

void iberbar::Poster::CFontCharBitmap::Create( char32_t ch, const UFontCharBitmapDesc& bitsDesc )
{
	m_surface = PTR_CSurface::_sNew();
	m_surface->LoadFromPixels( (const byte*)bitsDesc.pBitsFill, bitsDesc.nBmpWidth, bitsDesc.nBmpHeight );
	m_char = ch;
	m_charWidth = bitsDesc.nCharWidth;
	m_bitmapSize = CSize2i( bitsDesc.nBmpWidth, bitsDesc.nBmpHeight );
}

void iberbar::Poster::CFontCharBitmap::Draw( CSurface* target, const CPoint2i& ptDest, const CColor4B& color, const CRect2i* pRectViewport )
{
	CSize2i surfaceSize = m_surface->GetSize();
	DrawTexture( target, CRect2i( ptDest, surfaceSize ), m_surface, color, nullptr );
}

