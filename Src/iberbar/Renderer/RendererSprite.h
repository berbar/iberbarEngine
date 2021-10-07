#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Color.h>
#include <iberbar/RHI/Vertex.h>
#include <iberbar/RHI/Types.h>
#include <iberbar/Font/Types.h>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
		class IDevice;
		class IShaderState;
		class IShaderVariableTable;
		class CShaderVariableTableUnion;
	}

	namespace Renderer
	{
		class CFont;
		class CRenderer2d;

		class CRendererSpriteState;


		static const char s_strShaderVarName_RHW[] = "g_rhw";
		static const char s_strShaderVarName_UseTexture[] = "g_useTexture";
		static const char s_strShaderVarName_MatViewProjection[] = "g_matViewProjection";
		static const char s_strShaderVarName_Texture[] = "g_texture";
		static const char s_strShaderVarName_TextureSampler[] = "g_textureSampler";
		static const char s_strShaderVarName_OutlineSize[] = "g_outlineSize";
		static const char s_strShaderVarName_OutlineColor[] = "g_outlineColor";


		struct USpriteBatchRectNode
		{
			CColor4B color;
			CRect2f rcDst;
			CRect2f rcTexCoord;
		};


		//struct USpriteBatchParameters
		//{
		//	USpriteBatchParameters()
		//		: nZOrder( 0 )
		//		, nOutlineSize( 0 )
		//		, OutlineColor()
		//	{
		//	}
		//	USpriteBatchParameters( int nZOrder )
		//		: nZOrder( nZOrder )
		//		, nOutlineSize( 0 )
		//		, OutlineColor()
		//	{
		//	}
		//	int nZOrder;
		//	int nOutlineSize;
		//	CColor4B OutlineColor;
		//};


		class __iberbarRendererApi__ CRendererSprite
		{
		public:
			CRendererSprite( CRenderer2d* pRenderer );
			~CRendererSprite();

		public:
			void BuildVertices_V3F_C4B_T2F( RHI::UVertex_V3F_C4B_T2F* pVertices, const CRect2f& rectDst, const CRect2f& rectTexCoord, const CColor4B& color );

			void Clean();
			void SetDefaultShaderState( RHI::IShaderState* pShaderState );

			int DrawRect(
				int nZOrder,
				const CRect2f& rcDst,
				const CColor4B& color );

			int DrawRect(
				int nZOrder,
				const CRect2f& rcDst,
				const CColor4B& color,
				RHI::ITexture* pTexture, const CRect2f& rcTextureCoord );

			int DrawRectsBatch(
				int nZOrder,
				int nRectCount,
				const void* pData,
				int nStride,
				int nOffset_RcDst,
				int nOffset_Color );

			int DrawRectsBatch(
				int nZOrder,
				int nRectCount,
				const void* pData,
				int nStride,
				int nOffset_RcDst,
				int nOffset_Color,
				int nOffset_RcTextureCoord,
				RHI::ITexture* pTexture );

			int DrawText(
				int nZOrder,
				CFont* pFont,
				const CRect2i* pRcViewport,
				const wchar_t* strText,
				int nCount,
				CRect2i* pRect,
				const CColor4B& nFillColor,
				const UFontDrawTextOptions& options
			);

			int DrawTextWithUserData(
				int nZOrder,
				CFont* pFont,
				const CRect2i* pRcViewport,
				const void* pData,
				int nDataElementSize,
				int nDataElementCount,
				int nDataOffsetBitmap,
				int nDataOffsetDrawPoint,
				int nDataOffsetColor
			);


			int CalculateTextRectAtCharIndex(
				CRect2i* pOutRect,
				CFont* pFont,
				const wchar_t* strText,
				const CRect2i& RectDraw,
				UAlignHorizental nTextAlign,
				int nCharIndex );

			// 单个点拾取字符所在的Rect包围体
			int CalculateTextRectAtPoint(
				CRect2i* pOutRect,
				int* pOutCharIndex,
				CFont* pFont,
				const wchar_t* strText,
				const CRect2i& RectDraw,
				UAlignHorizental nTextAlign,
				const CPoint2i& Point );

			// 两个点拾取字符串，用于文字选择
			int CalculateTextRectAtPoints(
				CRect2i* pOutRect,
				int* pOutCharIndex1,
				int* pOutCharIndex2,
				CFont* pFont,
				const wchar_t* strText,
				const CRect2i& RectDraw,
				UAlignHorizental nTextAlign,
				const CPoint2i& Point1,
				const CPoint2i& Point2 );

			int DrawRectRhwEx(
				int nZOrder,
				const CRect2f& rcDst,
				const CColor4B& color,
				const CRect2f& rcTextureCoord,
				RHI::IShaderState* pShaderState,
				RHI::IShaderVariableTable* pShaderVarTable_Vertex,
				RHI::IShaderVariableTable* pShaderVarTable_Pixel);

			int DrawRectEx(
				int nZOrder,
				const CRect2f& rcDst,
				const CColor4B& color,
				const CRect2f& rcTextureCoord,
				RHI::IShaderState* pShaderState,
				RHI::IShaderVariableTable* pShaderVarTable_Vertex,
				RHI::IShaderVariableTable* pShaderVarTable_Pixel );


			FORCEINLINE void SetTextureFilterType( RHI::UTextureFilterType nFilterType, RHI::UTextureFilterType nMipmapFilterType )
			{
				m_DefaultSamplerState.nMinFilter = nFilterType;
				m_DefaultSamplerState.nMagFilter = nFilterType;
				m_DefaultSamplerState.nMipFilter = nMipmapFilterType;
			}

			FORCEINLINE const RHI::UTextureSamplerState& GetDefaultSamplerState() const { return m_DefaultSamplerState; }

			FORCEINLINE CRenderer2d* GetRenderer() const { return m_pRenderer; }

			void GetDefaultShaderState( RHI::IShaderState** ppOutShaderState );
			void CreateDefaultShaderVariableTable( RHI::IShaderVariableTable** ppOutShaderVariableTable, RHI::EShaderType eShaderType );
			void CreateDefaultShaderVariableTableUnion( RHI::CShaderVariableTableUnion* pShaderVariableTableUnion );

		protected:
			CRenderer2d* m_pRenderer;
			RHI::IDevice* m_pDevice;
			RHI::IShaderState* m_pDefaultShaderState;
			CRendererSpriteState* m_pState;

			RHI::UTextureSamplerState m_DefaultSamplerState;

		};
	}
}
