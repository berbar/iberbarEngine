
#pragma once

//#include <iberbar/Gui/Element/BlendColor.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Renderer/TrianglesCommand.h>
#include <iberbar/Renderer/Vertex.h>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
	}

	namespace Renderer
	{
		class CMeshRendererComponent;
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
			virtual void SetZOrder( int nZOrder ) override;
			//virtual void Refresh() override;
			//virtual void Update( float nElapsedTime ) override;
			virtual void UpdateRect() override;
			virtual void Render() override;


		public:
			void SetColor( const CColor4F& Color );
			void SetUV( const CRect2f& UV );
			void SetMaterial( Renderer::CMaterial* pMaterial );

		private:
			CColor4F m_Color;
			CRect2f m_UV;
			bool m_bDirtyMeshPositionOrUV;
			bool m_bEmptyBoundingFinal;
			Renderer::UVertex_V3F_C4B_T2F m_MeshVertices[ 4 ];
			uint16 m_MeshIndices[ 6 ];
			Renderer::CMeshRendererComponent* m_pMeshRendererComponent;
			Renderer::CMaterial* m_pMaterial;
			Renderer::CTrianglesCommand m_RenderCommand;
		};


		IBERBAR_UNKNOWN_PTR_DECLARE( CElementStateTexture );
	}
	

}
