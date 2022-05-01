#pragma once


#include <iberbar/Renderer/RenderCommand.h>

namespace iberbar
{
	namespace Renderer
	{
		class CShaderVariableTable;
		class CMesh;
		class CMaterial;

		class __iberbarRendererApi__ CMeshDrawCommand
			: public CRenderCommand
		{
		public:
			CMeshDrawCommand();

		public:
			inline void SetMesh( const CMesh* pMesh ) { m_pMesh = pMesh; }
			inline void SetMaterial( const CMaterial* pMaterial ) { m_pMaterial = pMaterial; }

			inline const CMesh* GetMesh() const { return m_pMesh; }
			inline const CMaterial* GetMaterial() const { return m_pMaterial; }

		protected:
			const CMesh* m_pMesh;
			const CMaterial* m_pMaterial;
		};
	}
}
