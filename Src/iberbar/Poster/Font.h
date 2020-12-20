#pragma once


#include <iberbar/Font/FontDeviceFreeType.h>
#include <iberbar/Font/FontDrawText.h>
#include <iberbar/Poster/Surface.h>
#include <list>

namespace iberbar
{
	namespace Poster
	{
		class CFont;
		class CFontCharBitmap;

		IBERBAR_UNKNOWN_PTR_DECLARE( CFont );
		IBERBAR_UNKNOWN_PTR_DECLARE( CFontCharBitmap );

		class __iberbarExports__ CFont
			: public CRef
		{
		private:
			struct __Node
			{
			public:
				__Node( CFontCharBitmap* p );
				~__Node();

			public:
				CFontCharBitmap* bitmap;
			};

		public:
			CFont( CFontFaceFreeType* fontFace );
			virtual ~CFont() override;

		public:
			const UFontDesc& GetFontDesc() { return m_fontFace->GetFontDesc(); }

			CFontCharBitmap* GetCharBitmap( char32_t ch );

		public:
			int PreloadCharaters( const char32_t* str );

		private:
			CFontCharBitmap* CreateChBitmap( char32_t ch );

		private:
			PTR_CFontFaceFreeType m_fontFace;
			std::list<__Node*> m_bitmaps[ 256 ];
		};

		class __iberbarExports__ CFontCharBitmap
		{
		public:
			CFontCharBitmap();
			~CFontCharBitmap();

		public:
			void Create( char32_t ch, const UFontCharBitmapDesc& bitsDesc );

		public:
			void Draw( CSurface* target, const CPoint2i& ptDest, const CColor4B& color, const CRect2i* pRectViewport );
			inline char32_t GetChar() { return m_char; }
			inline int GetCharWidth() { return m_charWidth; }
			inline const CSize2i& GetBitmapSize() { return m_bitmapSize; }

		public:
			PTR_CSurface m_surface;
			char32_t m_char;
			int m_charWidth;
			CSize2i m_bitmapSize;
		};


		int RenderText(
			CSurface* target,
			CFont* font,
			const char32_t* text,
			int nCount,
			CRect2i* pRect,
			const CColor4B& nFillColor );

		int RenderText_MultiLines_WithAdjustment(
			CSurface* target,
			CFont* font,
			const char32_t* text,
			int nCount,
			CRect2i* pRect,
			const CColor4B& color,
			const UFontDrawTextOptions& options,
			const UTextAdjustResult& adjustment );

		int RenderText_SingleLine(
			CSurface* target,
			CFont* font,
			const char32_t* text,
			int nCount,
			CRect2i* pRect,
			const CColor4B& nFillColor,
			const UFontDrawTextOptions& options );
	}
}

inline iberbar::Poster::CFont::__Node::__Node( CFontCharBitmap* p )
	: bitmap( p )
{
}

inline iberbar::Poster::CFont::__Node::~__Node()
{
	SAFE_DELETE( bitmap );
}

inline int iberbar::Poster::RenderText(
	CSurface* target,
	CFont* font,
	const char32_t* text,
	int nCount,
	CRect2i* pRect,
	const CColor4B& color )
{
	UFontDrawTextOptions options;
	return XFontDrawText<CFont, CFontCharBitmap, CSurface>( target, font, nullptr, text, -1, pRect, color, options );
}

inline int iberbar::Poster::RenderText_MultiLines_WithAdjustment(
	CSurface* target,
	CFont* font,
	const char32_t* text,
	int nCount,
	CRect2i* pRect,
	const CColor4B& color,
	const UFontDrawTextOptions& options,
	const UTextAdjustResult& adjustment )
{
	return XFontDrawText_MultiLines_WithAdjustment<CFont, CFontCharBitmap, CSurface>( target, font, nullptr, text, -1, pRect, color, options, adjustment );
}

inline int iberbar::Poster::RenderText_SingleLine(
	CSurface* target,
	CFont* font,
	const char32_t* text,
	int nCount,
	CRect2i* pRect,
	const CColor4B& color,
	const UFontDrawTextOptions& options )
{
	return XFontDrawText_SingleLine<CFont, CFontCharBitmap, CSurface>( target, font, nullptr, text, -1, pRect, color, options );
}
