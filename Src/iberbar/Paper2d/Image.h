#pragma once

#include <iberbar/Paper2d/Node.h>
#include <iberbar/RHI/Vertex.h>

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
		class __iberbarPaper2dApis__ CImage
			: public CNode
		{
		public:
			CImage();
			virtual ~CImage();
		protected:
			CImage( const CImage& image );

		public:
			virtual CImage* Clone() const override { return new CImage( *this ); }
			virtual void Init() override;
			virtual void UpdateTransform() override;
			virtual void DrawSelf( CDrawContext* pContext ) override;

		public:
			void SetTexture( RHI::ITexture* pTexture );
			void SetUV( const CRect2f& rcUV );
			void SetColor( const CColor4B& Color );
			void SetContentSizeAccordingToTexture();

		private:
			RHI::ITexture* m_pTexture;
			CColor4B m_Color;
			CRect2f m_rcUV;

			RHI::UVertex_V3F_C4B_T2F m_pVertices[ 4 ];
			uint16 m_pIndices[ 6 ];
			Renderer::CTrianglesCommand* m_pRenderCommand;
		};
	}
}
