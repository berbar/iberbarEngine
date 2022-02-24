#pragma once


#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/MeshDrawCommand.h>

namespace iberbar
{
	namespace RHI
	{
		class IShaderState;
	}

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
			//void UpdateBoundShaderStates( int nIndex );

		protected:
			CMesh* m_pMesh;
			std::vector<CMaterial*> m_Materials;
			std::vector<RHI::IShaderState*> m_BoundShaderStates;
			Renderer::CMeshDrawCommand m_RenderCommand;
			//bool m_DirtyBoundShaderStates;
		};
	}
}
