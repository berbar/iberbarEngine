#pragma once


#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/MeshDrawCommand.h>

namespace iberbar
{
	namespace Renderer
	{
		class CMesh;
		class CMaterial;

		class __iberbarRendererApi__ CMeshRendererComponent
		{
		public:
			CMeshRendererComponent();
			virtual ~CMeshRendererComponent();

		public:
			void SetMesh( CMesh* pMesh );
			void SetMaterialCount( int nCount );
			void SetMaterial( CMaterial* pMaterial );
			void SetMaterial( int nIndex, CMaterial* pMaterial );
			void SetZOrder( int nZOrder ) { m_RenderCommand.SetZOrder( nZOrder ); }

			virtual void Render();

		protected:
			CMesh* m_pMesh;
			std::vector<CMaterial*> m_Materials;
			Renderer::CMeshDrawCommand m_RenderCommand;
		};
	}
}
