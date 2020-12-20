
#pragma once

#include <iberbar/Gui/Element/BlendColor.h>
#include <iberbar/Gui/RenderElement.h>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
	}

	namespace Gui
	{
		class __iberbarGuiApi__ CElementStateTexture
			: public CRenderElement
		{
		public:
			CElementStateTexture();
			virtual ~CElementStateTexture();
		protected:
			CElementStateTexture( const CElementStateTexture& element );

		public:
			virtual CElementStateTexture* Clone() const override;
			virtual void Init() override;
			virtual void Refresh() override;
			virtual void Update( float nElapsedTime ) override;
			virtual void Render() override;


		public:
			void SetTexture( int state, RHI::ITexture* pTexture );
			void SetUV( int state, const CRect2f& uv );
			void SetColor( int state, const CColor4B& color ) { m_BlendColor.SetColor( state, color ); }
			void SetColorBlendRate( int state, float nRate );
			//void SetShaderState( RHI::IShaderState* pShaderState );

		private:
			float               m_BlendColorRate[ uWidgetStateCount ];
			BlendColor          m_BlendColor;
			RHI::ITexture* m_ppTextures[ uWidgetStateCount ];
			CRect2f              m_nRenderUV[ uWidgetStateCount ];

			//RHI::TTriangles<RHI::UVertex_V3F_C4B_T2F, 4, 6> m_Triangles;
			//Renderer::CTrianglesCommand m_RenderCommand;
		};


		IBERBAR_UNKNOWN_PTR_DECLARE( CElementStateTexture );
	}
	

}
