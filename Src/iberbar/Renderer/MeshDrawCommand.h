#pragma once


#include <iberbar/Renderer/RenderCommand.h>

namespace iberbar
{
	namespace Renderer
	{
		class CShaderVariableTable;
		class IMesh;

		class __iberbarRendererApi__ CMeshDrawCommand
			: public CRenderCommand
		{
		public:
			CMeshDrawCommand();

		public:
			inline void SetMesh( const IMesh* pMesh ) { m_pMesh = pMesh; }
			inline void SetShaderState( RHI::IShaderState* pShaderState ) { m_pShaderState = pShaderState; }
			inline void SetShaderVariableTables( const CShaderVariableTable* pTables ) { m_pShaderVariableTables = pTables; }

			inline RHI::IShaderState* GetShaderState() { return m_pShaderState; }
			inline const CShaderVariableTable* GetShaderVariableTables() const { return m_pShaderVariableTables; }
			inline const IMesh* GetMeshData() const { return m_pMesh; }

		protected:
			RHI::IShaderState* m_pShaderState;
			const CShaderVariableTable* m_pShaderVariableTables;
			const IMesh* m_pMesh;
			//UMeshData m_MeshData;
		};
	}
}
