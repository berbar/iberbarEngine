#pragma once

#include <iberbar/Paper2d/Node.h>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
	}

	namespace Renderer
	{
		class CTrianglesCommand;
	}

	namespace Paper2d
	{
		class __iberbarPaper2dApis__ CGridTerrain
			: public CNode
		{
		public:
			struct UGridPaletteNode
			{
				CColor4B Color;
				CRect2f rcTexCoord;
			};

		public:
			CGridTerrain();
			virtual ~CGridTerrain();
		protected:
			CGridTerrain( const CGridTerrain& terrain );

		public:
			virtual CGridTerrain* Clone() const override;
			virtual void Init() override;
			virtual void UpdateTransform() override;
			virtual void DrawSelf( CDrawContext* pContext ) override;

		public:
			void SetGridSize( float nWidth, float nHeight );
			void SetGrids( int nGridRow, int nGridCol, const UGridPaletteNode* pPalette, int nPaletteSize, const int* pGrids );
			void SetTexture( RHI::ITexture* pTexture );
			int GetGridRow() { return m_nGridRow; }
			int GetGridCol() { return m_nGridCol; }

		protected:
			void BuildGrids( const UGridPaletteNode* pPalette, int nPaletteSize, const int* pGrids );
			void BuildGrids();

		protected:
			int m_nGridRow;
			int m_nGridCol;
			CSize2f m_GridSize;
			RHI::ITexture* m_pTexture;
			void* m_pVertices;
			uint16* m_pIndices;
			Renderer::CTrianglesCommand* m_pRenderCommand;
		};
	}
}

