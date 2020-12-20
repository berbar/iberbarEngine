
#include <iberbar/Renderer/FontDrawState.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/Utility/RectClip2d.h>



namespace iberbar
{
	namespace Renderer
	{
		class CFontDrawBatchBuffer
		{
		public:
			struct URects {
				CColor4B color;
				CRect2f rcDst;
				CRect2f rcTextureCoord;
			};

			static const int sm_nRects_Stride = sizeof( URects );
			static const int sm_nRects_Offset_Color = offsetof( URects, color );
			static const int sm_nRects_Offset_RcDst = offsetof( URects, rcDst );
			static const int sm_nRects_Offset_RcTextureCoord = offsetof( URects, rcTextureCoord );

		public:
			CFontDrawBatchBuffer();
			~CFontDrawBatchBuffer();

			void AddRect( const CRect2f& rectDrawDst, const CRect2f& rectTexCoord, const CColor4B& color );
			FORCEINLINE void Reset() { m_nRectCount = 0; }
			FORCEINLINE uint32 GetRectCount() const { return m_nRectCount; }
			FORCEINLINE const URects* GetRectsPointer() const { return m_pRectDstList.data(); }

		private:
			void Reallocate();

		private:
			std::vector<URects> m_pRectDstList;
			uint32 m_nRectCount;

			static const uint32 sm_nRectStep = 32; // 1个矩形，4个顶点
		};
	}
}





iberbar::Renderer::CFontDrawBatchBuffer::CFontDrawBatchBuffer()
	: m_pRectDstList()
	, m_nRectCount( 0 )
{
	m_pRectDstList.resize( sm_nRectStep );
}


iberbar::Renderer::CFontDrawBatchBuffer::~CFontDrawBatchBuffer()
{
	m_pRectDstList.clear();
}


FORCEINLINE void iberbar::Renderer::CFontDrawBatchBuffer::AddRect( const CRect2f& rectDrawDst, const CRect2f& rectTexCoord, const CColor4B& color )
{
	if ( m_nRectCount == m_pRectDstList.size() )
	{
		Reallocate();
	}

	URects* ptr = &m_pRectDstList[ m_nRectCount ];
	ptr->color = color;
	ptr->rcDst = rectDrawDst;
	ptr->rcTextureCoord = rectTexCoord;
	m_nRectCount++;
}


FORCEINLINE void iberbar::Renderer::CFontDrawBatchBuffer::Reallocate()
{
	m_pRectDstList.resize( m_pRectDstList.size() + sm_nRectStep );
}





iberbar::Renderer::CFontDrawState::CFontDrawState( CRendererSprite* pRendererSprite )
	: m_pRendererSprite( pRendererSprite )
	, m_pDrawBuffer( new CFontDrawBatchBuffer() )
	, m_pTextureCurrent( nullptr )
	, m_nZOrder( 0 )
	, m_nOutline( 0 )
	, m_OutlineColor( 0xffffffff )
{
}


iberbar::Renderer::CFontDrawState::~CFontDrawState()
{
	SAFE_DELETE( m_pDrawBuffer );
}


void iberbar::Renderer::CFontDrawState::BeginDrawCharBitmaps()
{
	m_pDrawBuffer->Reset();
}


void iberbar::Renderer::CFontDrawState::BeginDrawCharBitmaps( const TextDraw::TContextBeginDrawCharBitmaps<Renderer::CFont>* pContext )
{
	m_pDrawBuffer->Reset();
}


void iberbar::Renderer::CFontDrawState::EndDrawCharBitmaps()
{
	AddCommandIntoRenderer();
}


void iberbar::Renderer::CFontDrawState::AddDrawCommand(
	RHI::ITexture* pTexture,
	const CRect2i& rcDst,
	const CRect2f& rcTextureCoord,
	const CColor4B& color,
	const CRect2i* pRcViewport )
{
	if ( m_pTextureCurrent == nullptr )
	{
		m_pTextureCurrent = pTexture;
	}
	else
	{
		if ( m_pTextureCurrent != pTexture )
		{
			AddCommandIntoRenderer();

			m_pTextureCurrent = pTexture;
		}
	}

	CRect2f rcTextureCoordNew = rcTextureCoord;
	CRect2i rcDstNew = rcDst;
	if ( pRcViewport != nullptr )
	{
		if ( RectTestIntersection( &rcDstNew, pRcViewport ) == false )
			return;
		RectClip2d( &rcTextureCoordNew, &rcDstNew, pRcViewport );
	}

	m_pDrawBuffer->AddRect(
		CRect2f( (float)rcDstNew.l, (float)rcDstNew.t, (float)rcDstNew.r, (float)rcDstNew.b ),
		rcTextureCoordNew,
		color );
}


void iberbar::Renderer::CFontDrawState::AddCommandIntoRenderer()
{
	m_pRendererSprite->DrawRectsBatch(
		m_nZOrder,
		m_pDrawBuffer->GetRectCount(),
		m_pDrawBuffer->GetRectsPointer(),
		m_pDrawBuffer->sm_nRects_Stride,
		m_pDrawBuffer->sm_nRects_Offset_RcDst,
		m_pDrawBuffer->sm_nRects_Offset_Color,
		m_pDrawBuffer->sm_nRects_Offset_RcTextureCoord,
		m_pTextureCurrent
	);
	m_pDrawBuffer->Reset();
}

