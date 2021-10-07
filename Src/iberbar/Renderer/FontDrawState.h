#pragma once

#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Color.h>
#include <iberbar/Renderer/Font.h>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
	}

	namespace Renderer
	{
		class CRendererSprite;
		class CFontDrawBatchBuffer;

		// äÖÈ¾×´Ì¬»ú
		class CFontDrawState
		{
		public:
			CFontDrawState( CRendererSprite* pRendererSprite );
			~CFontDrawState();

			void SetZOrder( int nZOrder ) { m_nZOrder = nZOrder; }
			void BeginDrawCharBitmaps();
			void BeginDrawCharBitmaps( const TextDraw::TContextBeginDrawCharBitmaps<Renderer::CFont>* pContext );
			void EndDrawCharBitmaps();
			void DrawCharBitmap( const TextDraw::TContextDrawCharBitmap<Renderer::CFont, Renderer::UFontCharBitmap>* pContext );
			void DrawCharBitmap( CFont* pFont, const UFontCharBitmap* pBitmap, const CPoint2i& ptDst, const CColor4B& FillColor, const CRect2i* pRcViewport );

		private:
			void AddDrawCommand(
				RHI::ITexture* pTexture,
				const CRect2i& rcDst,
				const CRect2f& rcTextureCoord,
				const CColor4B& color,
				const CRect2i* pRcViewport );
			void AddCommandIntoRenderer();

		private:
			CRendererSprite* m_pRendererSprite;
			CFontDrawBatchBuffer* m_pDrawBuffer;

			RHI::ITexture* m_pTextureCurrent;

			int m_nZOrder;
			int m_nOutline;
			CColor4B m_OutlineColor;
		};
	}
}



FORCEINLINE void iberbar::Renderer::CFontDrawState::DrawCharBitmap( const TextDraw::TContextDrawCharBitmap<Renderer::CFont, Renderer::UFontCharBitmap>* pContext )
{

	DrawCharBitmap( pContext->pFont, pContext->pBitmap, *(pContext->pPointDrawAt), pContext->FillColor, pContext->pRcViewport );
}

FORCEINLINE void iberbar::Renderer::CFontDrawState::DrawCharBitmap( CFont* pFont, const UFontCharBitmap* pBitmap, const CPoint2i& ptDst, const CColor4B& FillColor, const CRect2i* pRcViewport )
{
	assert( pFont != nullptr );

	if ( pBitmap == nullptr )
		return;

	CRect2i rcDst( CPoint2i( ptDst.x + pBitmap->nDeltaX, ptDst.y + pBitmap->nDeltaY ), pBitmap->TextureSize );
	if ( pBitmap->nTextureIndex >= 0 )
	{
		AddDrawCommand(
			pFont->GetTexture( pBitmap->nTextureIndex ),
			rcDst,
			pBitmap->rcTexCoord,
			FillColor,
			pRcViewport
		);
	}
}

