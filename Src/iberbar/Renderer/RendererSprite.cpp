
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/TrianglesCommand.h>
#include <iberbar/Renderer/FontDrawState.h>
#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/VertexBuilder.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/Utility/Buffer.h>
#include <iberbar/Font/FontDrawText.h>
#include <iberbar/Font/CalculateTextRect.h>



namespace iberbar
{
	namespace Renderer
	{
		class CRendererSpriteState
		{
		public:
			CRendererSpriteState( CRendererSprite* pRendererSprite );
			~CRendererSpriteState();

		public:
			void Clean();
			void* AllocVertices( uint32 nByteSize );
			void* AllocIndices( uint32 nByteSize );
			uint32 CalculateVertexCountMax( uint32 nVertexSize ) { return sm_nVertexBufferResizeStep / nVertexSize; }
			uint32 CalculateIndexCountMax() { return sm_nIndexBufferResizeStep >> 1; }
			CTrianglesCommand* AllocTrianglesCommand();
			RHI::CShaderVariableTableUnion* AllocShaderVariableTable();
			CFontDrawState* GetFontDrawState() { return m_pFontDrawState; }

		private:
			CRendererSprite* m_pRendererSprite;
			TBufferList<uint8> m_VertexBufferList;
			TBufferList<uint8> m_IndexBufferList;
			uint32 m_nTrianglesCommandUsed;
			uint32 m_nShaderVariableTableUsed;
			std::vector<CTrianglesCommand*> m_TrianglesCommands;
			std::vector<RHI::CShaderVariableTableUnion*> m_ShaderVariableTableCache;
			CFontDrawState* m_pFontDrawState;

			static const uint32 sm_nVertexBufferResizeStep = 65536;
			static const uint32 sm_nIndexBufferResizeStep = 65536;

		public:
			static const uint32 sm_nRectVerticsByteSize = sizeof( RHI::UVertex_V3F_C4B_T2F ) * 4;
			static const uint32 sm_nRectIndicesByteSize = sizeof( uint16 ) * 6;
			static const CRect2f sm_rcTextureCoord;
		};
	}
}







const iberbar::CRect2f iberbar::Renderer::CRendererSpriteState::sm_rcTextureCoord = CRect2f( 0, 0, 0, 0 );


iberbar::Renderer::CRendererSpriteState::CRendererSpriteState( CRendererSprite* pRendererSprite )
	: m_pRendererSprite( pRendererSprite )
	, m_VertexBufferList( sm_nVertexBufferResizeStep )
	, m_IndexBufferList( sm_nIndexBufferResizeStep )
	, m_nTrianglesCommandUsed( 0 )
	, m_nShaderVariableTableUsed( 0 )
	, m_TrianglesCommands()
	, m_pFontDrawState( new CFontDrawState( pRendererSprite ) )
{
}


iberbar::Renderer::CRendererSpriteState::~CRendererSpriteState()
{
	m_VertexBufferList.Clear();
	m_VertexBufferList.Clear();
	for ( size_t i = 0, s = m_TrianglesCommands.size(); i < s; i++ )
	{
		SAFE_DELETE( m_TrianglesCommands[ i ] );
	}
	m_TrianglesCommands.clear();
	for ( size_t i = 0, s = m_ShaderVariableTableCache.size(); i < s; i++ )
	{
		SAFE_DELETE( m_ShaderVariableTableCache[ i ] );
	}
	m_ShaderVariableTableCache.clear();
	SAFE_DELETE( m_pFontDrawState );
}


void iberbar::Renderer::CRendererSpriteState::Clean()
{
	m_VertexBufferList.ResetUsed();
	m_IndexBufferList.ResetUsed();
	m_nTrianglesCommandUsed = 0;
	m_nShaderVariableTableUsed = 0;
}


FORCEINLINE void* iberbar::Renderer::CRendererSpriteState::AllocVertices( uint32 nByteSize )
{
	return m_VertexBufferList.AllocElements( nByteSize );
}


FORCEINLINE void* iberbar::Renderer::CRendererSpriteState::AllocIndices( uint32 nByteSize )
{
	return m_IndexBufferList.AllocElements( nByteSize );
}


FORCEINLINE iberbar::Renderer::CTrianglesCommand* iberbar::Renderer::CRendererSpriteState::AllocTrianglesCommand()
{
	CTrianglesCommand* pCommand = nullptr;
	if ( m_nTrianglesCommandUsed == m_TrianglesCommands.size() )
	{
		m_TrianglesCommands.push_back( pCommand = new CTrianglesCommand() );
	}
	else
	{
		pCommand = m_TrianglesCommands[ m_nTrianglesCommandUsed ];
	}
	m_nTrianglesCommandUsed++;
	return pCommand;
}


FORCEINLINE iberbar::RHI::CShaderVariableTableUnion* iberbar::Renderer::CRendererSpriteState::AllocShaderVariableTable()
{
	RHI::CShaderVariableTableUnion* pShaderVarTableUnion = nullptr;
	if ( m_nShaderVariableTableUsed == m_ShaderVariableTableCache.size() )
	{
		pShaderVarTableUnion = new RHI::CShaderVariableTableUnion();

		TSmartRefPtr<RHI::IShaderVariableTable> pShaderVarTable;
		for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
		{
			m_pRendererSprite->CreateDefaultShaderVariableTable( &pShaderVarTable, (RHI::EShaderType)i );
			pShaderVarTableUnion->SetVariableTable( (RHI::EShaderType)i, pShaderVarTable );
		}
		
		m_ShaderVariableTableCache.push_back( pShaderVarTableUnion );
	}
	else
	{
		pShaderVarTableUnion = m_ShaderVariableTableCache[ m_nShaderVariableTableUsed ];
	}
	m_nShaderVariableTableUsed++;
	return pShaderVarTableUnion;
}





iberbar::Renderer::CRendererSprite::CRendererSprite( CRenderer2d* pRenderer )
	: m_pRenderer( pRenderer )
	, m_pDevice( pRenderer->GetRHIDevice() )
	, m_pDefaultShaderState( nullptr )
	, m_pState( new CRendererSpriteState( this ) )
	, m_DefaultSamplerState()
{
	SetTextureFilterType( RHI::UTextureFilterType::Linear, RHI::UTextureFilterType::Linear );
	UNKNOWN_SAFE_ADDREF( m_pDevice );
}

iberbar::Renderer::CRendererSprite::~CRendererSprite()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDefaultShaderState );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
	SAFE_DELETE( m_pState );
}

//
//void iberbar::Renderer::CRendererSprite::BuildVertices_V3F_C4B( RHI::UVertex_V3F_C4B* pVertices, const CRect2f& rectDst, const CColor4B& color )
//{
//	assert( pVertices != nullptr );
//
//	if ( RHI::IsOpenGLApi( m_pDevice->GetApiType() ) )
//	{
//		const CSize2i& contextSize = m_pDevice->GetContextSize();
//		CRect2f rectDstNew = CRect2f(
//			rectDst.l / contextSize.w * 2.0f - 1.0f,
//			1.0f - rectDst.t / contextSize.h * 2.0f,
//			rectDst.r / contextSize.w * 2.0f - 1.0f,
//			1.0f - rectDst.b / contextSize.h * 2.0f
//		);
//		CColor4B colorNew( color.r, color.g, color.b, color.a );
//		RHI::RectBuildVertices(
//			pVertices,
//			0.0f,
//			rectDstNew,
//			colorNew );
//	}
//#ifdef _WIN32
//	else if ( RHI::IsD3DApi( m_pDevice->GetApiType() ) )
//	{
//		const CSize2i& contextSize = m_pDevice->GetContextSize();
//		CRect2f rectDstNew = CRect2f(
//			rectDst.l / contextSize.w * 2.0f - 1.0f,
//			1.0f - rectDst.t / contextSize.h * 2.0f,
//			rectDst.r / contextSize.w * 2.0f - 1.0f,
//			1.0f - rectDst.b / contextSize.h * 2.0f
//		);
//		RHI::RectBuildVertices(
//			pVertices,
//			0.0f,
//			rectDstNew,
//			color );
//	}
//#endif
//}


void iberbar::Renderer::CRendererSprite::BuildVertices_V3F_C4B_T2F( RHI::UVertex_V3F_C4B_T2F* pVertices, const CRect2f& rectDst, const CRect2f& rectTexCoord, const CColor4B& color )
{
	assert( pVertices != nullptr );

	const CSize2i& contextSize = m_pDevice->GetContextSize();
	CRect2f rectDstNew = CRect2f(
		rectDst.l / contextSize.w * 2.0f - 1.0f,
		1.0f - rectDst.t / contextSize.h * 2.0f,
		rectDst.r / contextSize.w * 2.0f - 1.0f,
		1.0f - rectDst.b / contextSize.h * 2.0f
	);
	if ( RHI::IsOpenGLApi( m_pDevice->GetApiType() ) )
	{
		CColor4B colorNew( color.a, color.b, color.g, color.r );
		RHI::RectBuildVertices(
			pVertices,
			0.0f,
			rectDstNew,
			colorNew,
			rectTexCoord );
	}
	else
	{
		RHI::RectBuildVertices(
			pVertices,
			0.0f,
			rectDstNew,
			color,
			rectTexCoord );
	}
}


void iberbar::Renderer::CRendererSprite::Clean()
{
	m_pState->Clean();
}


void iberbar::Renderer::CRendererSprite::SetDefaultShaderState( RHI::IShaderState* pShaderState )
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDefaultShaderState );
	m_pDefaultShaderState = pShaderState;
	UNKNOWN_SAFE_ADDREF( m_pDefaultShaderState );
}


int iberbar::Renderer::CRendererSprite::DrawRect( int nZOrder,
	const CRect2f& rcDst,
	const CColor4B& color )
{
	assert( m_pDefaultShaderState );

	if ( color.a == 0 )
		return 0;

	RHI::UVertex_V3F_C4B_T2F* pVertices = (RHI::UVertex_V3F_C4B_T2F*)m_pState->AllocVertices( CRendererSpriteState::sm_nRectVerticsByteSize );
	uint16* pIndices = (uint16*)m_pState->AllocIndices( CRendererSpriteState::sm_nRectIndicesByteSize );
	CTrianglesCommand* pCommand = m_pState->AllocTrianglesCommand();
	RHI::CShaderVariableTableUnion* pShaderVarTableUnion = m_pState->AllocShaderVariableTable();
	RHI::IShaderVariableTable* pShaderVarTable_Vertex = pShaderVarTableUnion->GetVariableTable( RHI::EShaderType::VertexShader );
	RHI::IShaderVariableTable* pShaderVarTable_Pixel = pShaderVarTableUnion->GetVariableTable( RHI::EShaderType::PixelShader );

	BuildVertices_V3F_C4B_T2F(
		pVertices,
		rcDst,
		CRendererSpriteState::sm_rcTextureCoord,
		color
	);
	RHI::BuildRectVertexIndices( pIndices, 0 );

	pShaderVarTable_Vertex->SetBool( s_strShaderVarName_RHW, true );
	pShaderVarTable_Pixel->SetBool( s_strShaderVarName_UseTexture, false );

	pCommand->SetShaderState( m_pDefaultShaderState );
	pCommand->SetShaderVariableTable( RHI::EShaderType::VertexShader, pShaderVarTable_Vertex );
	pCommand->SetShaderVariableTable( RHI::EShaderType::PixelShader, pShaderVarTable_Pixel );
	pCommand->SetTriangles( CTrianglesCommand::UTriangles(
		pVertices,
		pIndices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ), 4, 6, 2
	) );
	pCommand->SetZOrder( nZOrder );
	m_pRenderer->AddCommand( pCommand );

	return 1;	
}


int iberbar::Renderer::CRendererSprite::DrawRect( int nZOrder,
	const CRect2f& rcDst,
	const CColor4B& color,
	RHI::ITexture* pTexture, const CRect2f& rcTextureCoord )
{
	assert( m_pDefaultShaderState );

	if ( color.a == 0 )
		return 0;
	if ( pTexture == nullptr )
		return 0;
	
	RHI::UVertex_V3F_C4B_T2F* pVertices = (RHI::UVertex_V3F_C4B_T2F*)m_pState->AllocVertices( CRendererSpriteState::sm_nRectVerticsByteSize );
	uint16* pIndices = (uint16*)m_pState->AllocIndices( CRendererSpriteState::sm_nRectIndicesByteSize );
	CTrianglesCommand* pCommand = m_pState->AllocTrianglesCommand();
	RHI::CShaderVariableTableUnion* pShaderVarTableUnion = m_pState->AllocShaderVariableTable();
	RHI::IShaderVariableTable* pShaderVarTable_Vertex = pShaderVarTableUnion->GetVariableTable( RHI::EShaderType::VertexShader );
	RHI::IShaderVariableTable* pShaderVarTable_Pixel = pShaderVarTableUnion->GetVariableTable( RHI::EShaderType::PixelShader );

	BuildVertices_V3F_C4B_T2F(
		pVertices,
		rcDst,
		rcTextureCoord,
		color
	);
	RHI::BuildRectVertexIndices( pIndices, 0 );

	pShaderVarTable_Vertex->SetBool( s_strShaderVarName_RHW, true );
	pShaderVarTable_Pixel->SetBool( s_strShaderVarName_UseTexture, false );
	pShaderVarTable_Pixel->SetTexture( s_strShaderVarName_Texture, pTexture );
	pShaderVarTable_Pixel->SetSamplerState( s_strShaderVarName_TextureSampler, m_DefaultSamplerState );

	pCommand->SetShaderState( m_pDefaultShaderState );
	pCommand->SetShaderVariableTable( RHI::EShaderType::VertexShader, pShaderVarTable_Vertex );
	pCommand->SetShaderVariableTable( RHI::EShaderType::PixelShader, pShaderVarTable_Pixel );
	pCommand->SetTriangles( CTrianglesCommand::UTriangles(
		pVertices,
		pIndices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ), 4, 6, 2
	) );
	pCommand->SetZOrder( nZOrder );
	m_pRenderer->AddCommand( pCommand );

	return 1;
}


int iberbar::Renderer::CRendererSprite::DrawRectsBatch( int nZOrder,
	int nRectCount,
	const void* pData,
	int nStride,
	int nOffset_RcDst,
	int nOffset_Color )
{
	assert( m_pDefaultShaderState );

	RHI::UVertex_V3F_C4B_T2F* pVertices = (RHI::UVertex_V3F_C4B_T2F*)m_pState->AllocVertices( CRendererSpriteState::sm_nRectVerticsByteSize * nRectCount );
	RHI::UVertex_V3F_C4B_T2F* pVerticesPointer = pVertices;
	uint16* pIndices = (uint16*)m_pState->AllocIndices( CRendererSpriteState::sm_nRectIndicesByteSize * nRectCount );
	uint16* pIndicesPointer = pIndices;
	CTrianglesCommand* pCommand = m_pState->AllocTrianglesCommand();
	RHI::CShaderVariableTableUnion* pShaderVarTableUnion = m_pState->AllocShaderVariableTable();
	RHI::IShaderVariableTable* pShaderVarTable_Vertex = pShaderVarTableUnion->GetVariableTable( RHI::EShaderType::VertexShader );
	RHI::IShaderVariableTable* pShaderVarTable_Pixel = pShaderVarTableUnion->GetVariableTable( RHI::EShaderType::PixelShader );

	const uint8* pDataPointer = (const uint8*)pData;
	for ( int i = 0; i < nRectCount; i++ )
	{
		BuildVertices_V3F_C4B_T2F(
			pVerticesPointer,
			*(const CRect2f*)(pDataPointer + nOffset_RcDst),
			CRendererSpriteState::sm_rcTextureCoord,
			*(const CColor4B*)(pDataPointer + nOffset_Color)
		);
		RHI::BuildRectVertexIndices( pIndicesPointer, i );

		pVerticesPointer += 4;
		pIndicesPointer += 6;
		pDataPointer += nStride;
	}

	pShaderVarTable_Vertex->SetBool( s_strShaderVarName_RHW, true );
	pShaderVarTable_Pixel->SetBool( s_strShaderVarName_UseTexture, false );

	pCommand->SetShaderState( m_pDefaultShaderState );
	pCommand->SetShaderVariableTable( RHI::EShaderType::VertexShader, pShaderVarTable_Vertex );
	pCommand->SetShaderVariableTable( RHI::EShaderType::PixelShader, pShaderVarTable_Pixel );
	pCommand->SetTriangles( CTrianglesCommand::UTriangles(
		pVertices,
		pIndices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ), 4 * nRectCount, 6 * nRectCount, 2 * nRectCount
	) );
	pCommand->SetZOrder( nZOrder );
	m_pRenderer->AddCommand( pCommand );

	return nRectCount;
}


int iberbar::Renderer::CRendererSprite::DrawRectsBatch(
	int nZOrder,
	int nRectCount,
	const void* pData,
	int nStride,
	int nOffset_RcDst,
	int nOffset_Color,
	int nOffset_RcTextureCoord,
	RHI::ITexture* pTexture )
{
	assert( m_pDefaultShaderState );

	if ( pTexture == nullptr )
		return 0;
	if ( nRectCount <= 0 )
		return 0;

	// 暂时超出顶点数量的就跳过，有时间再将顶点划分为几段去渲染
	uint32 nVertexCountMax = m_pState->CalculateVertexCountMax( CRendererSpriteState::sm_nRectVerticsByteSize );
	uint32 nVertexCount = nRectCount * 4;
	if ( nVertexCount > nVertexCountMax )
		return 0;
	uint32 nIndexCountMax = m_pState->CalculateIndexCountMax();
	uint32 nIndexCount = nRectCount * 6;
	if ( nIndexCount > nIndexCountMax )
		return 0;

	RHI::UVertex_V3F_C4B_T2F* pVertices = (RHI::UVertex_V3F_C4B_T2F*)m_pState->AllocVertices( CRendererSpriteState::sm_nRectVerticsByteSize * nRectCount );
	RHI::UVertex_V3F_C4B_T2F* pVerticesPointer = pVertices;
	uint16* pIndices = (uint16*)m_pState->AllocIndices( CRendererSpriteState::sm_nRectIndicesByteSize * nRectCount );
	uint16* pIndicesPointer = pIndices;
	CTrianglesCommand* pCommand = m_pState->AllocTrianglesCommand();
	RHI::CShaderVariableTableUnion* pShaderVarTableUnion = m_pState->AllocShaderVariableTable();
	RHI::IShaderVariableTable* pShaderVarTable_Vertex = pShaderVarTableUnion->GetVariableTable( RHI::EShaderType::VertexShader );
	RHI::IShaderVariableTable* pShaderVarTable_Pixel = pShaderVarTableUnion->GetVariableTable( RHI::EShaderType::PixelShader );

	const uint8* pDataPointer = (const uint8*)pData;
	for ( int i = 0; i < nRectCount; i++ )
	{
		BuildVertices_V3F_C4B_T2F(
			pVerticesPointer,
			*(const CRect2f*)(pDataPointer + nOffset_RcDst),
			*(const CRect2f*)(pDataPointer + nOffset_RcTextureCoord),
			*(const CColor4B*)(pDataPointer + nOffset_Color)
		);
		RHI::BuildRectVertexIndices( pIndicesPointer, i );

		pVerticesPointer += 4;
		pIndicesPointer += 6;
		pDataPointer += nStride;
	}

	pShaderVarTable_Vertex->SetBool( s_strShaderVarName_RHW, true );
	pShaderVarTable_Pixel->SetBool( s_strShaderVarName_UseTexture, true );
	pShaderVarTable_Pixel->SetTexture( s_strShaderVarName_Texture, pTexture );
	pShaderVarTable_Pixel->SetSamplerState( s_strShaderVarName_TextureSampler, m_DefaultSamplerState );

	pCommand->SetZOrder( nZOrder );
	pCommand->SetShaderState( m_pDefaultShaderState );
	pCommand->SetShaderVariableTable( RHI::EShaderType::VertexShader, pShaderVarTable_Vertex );
	pCommand->SetShaderVariableTable( RHI::EShaderType::PixelShader, pShaderVarTable_Pixel );
	pCommand->SetTriangles( CTrianglesCommand::UTriangles(
		pVertices,
		pIndices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ), nVertexCount, nIndexCount, 2 * nRectCount
	) );
	
	m_pRenderer->AddCommand( pCommand );

	return nRectCount;
}


int iberbar::Renderer::CRendererSprite::DrawText(
	int nZOrder,
	CFont* pFont,
	const CRect2i* pRcViewport,
	const wchar_t* strText,
	int nCount,
	CRect2i* pRect,
	const CColor4B& nFillColor,
	const UFontDrawTextOptions& options
)
{
	if ( pFont == nullptr )
		return 0;

	CFontDrawState* pFontDrawState = m_pState->GetFontDrawState();
	pFontDrawState->SetZOrder( nZOrder );
	int retDraw = XFontDrawText<CFont, UFontCharBitmap, CFontDrawState>( pFontDrawState, pFont, pRcViewport, strText, nCount, pRect, nFillColor, options );
	return retDraw;
}


int iberbar::Renderer::CRendererSprite::DrawTextWithUserData(
	int nZOrder,
	CFont* pFont,
	const CRect2i* pRcViewport,
	const void* pData,
	int nDataElementSize,
	int nDataElementCount,
	int nDataOffsetBitmap,
	int nDataOffsetDrawPoint,
	int nDataOffsetColor
)
{
	assert( nDataElementSize > 0 );

	if ( pFont == nullptr )
		return 0;

	if ( nDataElementCount == 0 )
		return 0;

	int nRetDraw = 0;
	CFontDrawState* pFontDrawState = m_pState->GetFontDrawState();
	pFontDrawState->SetZOrder( nZOrder );
	pFontDrawState->BeginDrawCharBitmaps();
	const uint8* pDataTemp = (const uint8*)pData;
	for ( int i = 0; i < nDataElementCount; i++ )
	{
		pFontDrawState->DrawCharBitmap(
			pFont,
			*((const UFontCharBitmap**)(pDataTemp+nDataOffsetBitmap)),
			*((const CPoint2i*)(pDataTemp + nDataOffsetDrawPoint)),
			*((const CColor4B*)(pDataTemp + nDataOffsetColor)),
			pRcViewport );
		pDataTemp += nDataElementSize;
		nRetDraw++;
	}
	pFontDrawState->EndDrawCharBitmaps();
	return nRetDraw;
}


int iberbar::Renderer::CRendererSprite::CalculateTextRectAtCharIndex(
	CRect2i* pOutRect,
	CFont* pFont,
	const wchar_t* strText,
	const CRect2i& RectDraw,
	UAlignHorizental nTextAlign,
	int nCharIndex
)
{
	if ( pFont == nullptr )
		return 0;

	return iberbar::CalculateTextRectAtCharIndex< CFont, UFontCharBitmap >( pOutRect, pFont, strText, RectDraw, nTextAlign, nCharIndex );
}


// 单个点拾取字符所在的Rect包围体
int iberbar::Renderer::CRendererSprite::CalculateTextRectAtPoint(
	CRect2i* pOutRect,
	int* pOutCharIndex,
	CFont* pFont,
	const wchar_t* strText,
	const CRect2i& RectDraw,
	UAlignHorizental nTextAlign,
	const CPoint2i& Point )
{
	if ( pFont == nullptr )
		return 0;

	return iberbar::CalculateTextRectAtPoint< CFont, UFontCharBitmap >( pOutRect, pOutCharIndex, pFont, strText, RectDraw, nTextAlign, Point );
}

// 两个点拾取字符串，用于文字选择
int iberbar::Renderer::CRendererSprite::CalculateTextRectAtPoints(
	CRect2i* pOutRect,
	int* pOutCharIndex1,
	int* pOutCharIndex2,
	CFont* pFont,
	const wchar_t* strText,
	const CRect2i& RectDraw,
	UAlignHorizental nTextAlign,
	const CPoint2i& Point1,
	const CPoint2i& Point2 )
{
	if ( pFont == nullptr )
		return 0;

	return iberbar::CalculateTextRectAtPoints< CFont, UFontCharBitmap >( pOutRect, pOutCharIndex1, pOutCharIndex2, pFont, strText, RectDraw, nTextAlign, Point1, Point2 );
}


int iberbar::Renderer::CRendererSprite::DrawRectRhwEx(
	int nZOrder,
	const CRect2f& rcDst,
	const CColor4B& color,
	const CRect2f& rcTextureCoord,
	RHI::IShaderState* pShaderState,
	RHI::IShaderVariableTable* pShaderVarTable_Vertex,
	RHI::IShaderVariableTable* pShaderVarTable_Pixel )
{

	if ( color.a == 0 )
		return 0;

	RHI::UVertex_V3F_C4B_T2F* pVertices = (RHI::UVertex_V3F_C4B_T2F*)m_pState->AllocVertices( CRendererSpriteState::sm_nRectVerticsByteSize );
	uint16* pIndices = (uint16*)m_pState->AllocIndices( CRendererSpriteState::sm_nRectIndicesByteSize );
	CTrianglesCommand* pCommand = m_pState->AllocTrianglesCommand();

	const CSize2i& contextSize = m_pDevice->GetContextSize();
	CRect2f rcDstNew = CRect2f(
		rcDst.l / contextSize.w * 2.0f - 1.0f,
		1.0f - rcDst.t / contextSize.h * 2.0f,
		rcDst.r / contextSize.w * 2.0f - 1.0f,
		1.0f - rcDst.b / contextSize.h * 2.0f
	);
	RHI::RectBuildVertices(
		pVertices,
		0.0f,
		rcDstNew,
		color,
		rcTextureCoord );
	RHI::BuildRectVertexIndices( pIndices, 0 );

	pCommand->SetZOrder( nZOrder );
	pCommand->SetShaderState( pShaderState );
	pCommand->SetShaderVariableTable( RHI::EShaderType::VertexShader, pShaderVarTable_Vertex );
	pCommand->SetShaderVariableTable( RHI::EShaderType::PixelShader, pShaderVarTable_Pixel );
	pCommand->SetTriangles( CTrianglesCommand::UTriangles(
		pVertices,
		pIndices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ), 4, 6, 2
	) );
	m_pRenderer->AddCommand( pCommand );

	return 1;
}


int iberbar::Renderer::CRendererSprite::DrawRectEx(
	int nZOrder,
	const CRect2f& rcDst,
	const CColor4B& color,
	const CRect2f& rcTextureCoord,
	RHI::IShaderState* pShaderState,
	RHI::IShaderVariableTable* pShaderVarTable_Vertex,
	RHI::IShaderVariableTable* pShaderVarTable_Pixel )
{

	if ( color.a == 0 )
		return 0;

	RHI::UVertex_V3F_C4B_T2F* pVertices = (RHI::UVertex_V3F_C4B_T2F*)m_pState->AllocVertices( CRendererSpriteState::sm_nRectVerticsByteSize );
	uint16* pIndices = (uint16*)m_pState->AllocIndices( CRendererSpriteState::sm_nRectIndicesByteSize );
	CTrianglesCommand* pCommand = m_pState->AllocTrianglesCommand();

	RHI::RectBuildVertices(
		pVertices,
		0.0f,
		rcDst,
		color,
		rcTextureCoord );
	RHI::BuildRectVertexIndices( pIndices, 0 );

	pCommand->SetZOrder( nZOrder );
	pCommand->SetShaderState( pShaderState );
	pCommand->SetShaderVariableTable( RHI::EShaderType::VertexShader, pShaderVarTable_Vertex );
	pCommand->SetShaderVariableTable( RHI::EShaderType::PixelShader, pShaderVarTable_Pixel );
	pCommand->SetTriangles( CTrianglesCommand::UTriangles(
		pVertices,
		pIndices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ), 4, 6, 2
	) );
	m_pRenderer->AddCommand( pCommand );

	return 1;
}


void iberbar::Renderer::CRendererSprite::GetDefaultShaderState( RHI::IShaderState** ppOutShaderState )
{
	assert( ppOutShaderState );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderState );
	(*ppOutShaderState) = m_pDefaultShaderState;
	UNKNOWN_SAFE_ADDREF( *ppOutShaderState );
}


void iberbar::Renderer::CRendererSprite::CreateDefaultShaderVariableTable( RHI::IShaderVariableTable** ppOutShaderVariableTable, RHI::EShaderType eShaderType )
{
	assert( ppOutShaderVariableTable );

	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderVariableTable );

	RHI::IShader* pShader = m_pDefaultShaderState->GetShader( eShaderType );
	if ( pShader == nullptr )
		return;

	TSmartRefPtr<RHI::IShaderVariableTable> pTable;
	m_pDevice->CreateShaderVariableTable( &pTable );
	pTable->SetShader( pShader );
	( *ppOutShaderVariableTable ) = pTable;
	( *ppOutShaderVariableTable )->AddRef();
}


void iberbar::Renderer::CRendererSprite::CreateDefaultShaderVariableTableUnion( RHI::CShaderVariableTableUnion* pShaderVariableTableUnion )
{
	assert( pShaderVariableTableUnion );

	TSmartRefPtr<RHI::IShaderVariableTable> pTable;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		m_pDevice->CreateShaderVariableTable( &pTable );
		pTable->SetShader( m_pDefaultShaderState->GetShader( (RHI::EShaderType)i ) );
		pShaderVariableTableUnion->SetVariableTable( (RHI::EShaderType)i, pTable );
	}
}
