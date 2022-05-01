#pragma once

#include <iberbar/Renderer/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		class ICommandContext;
		class IShaderState;
	}


	namespace Renderer
	{
		class CMeshDrawCommand;
		class CShaderVariableTable;
		class CMaterial;


		class CRendererState
		{

		public:
			CRendererState();
			~CRendererState();

		public:
			//void SetVertexDeclaration( RHI::IVertexDeclaration* pVertexDeclaration );
			bool AddCommand( CMeshDrawCommand* pCommand );
			void FlushRenderList();
			void SetShaderBindings();
			FORCEINLINE const std::vector<CMeshDrawCommand*>& GetRenderList() { return m_CommandList_Render; }
			FORCEINLINE uint32 GetVertexSize() const { return m_nVertexSize; }
			FORCEINLINE uint32 GetVertexCount() const { return m_nVertexCount; }
			FORCEINLINE uint32 GetVertexSizeTotal() const { return m_nVertexSizeTotal; }
			FORCEINLINE uint32 GetIndexCount() const { return m_nIndexCount; }
			FORCEINLINE uint32 GetIndexSizeTotal() const { return m_nIndexSizeTotal; }
			FORCEINLINE uint32 GetTriangleCount() const { return m_nTriangleCount; }
			FORCEINLINE RHI::IShaderState* GetUsingShaderState() const { return m_pUsingShaderState; }
			FORCEINLINE const CShaderVariableTable* GetUsingShaderVarTable( RHI::EShaderType eShaderType ) const { return m_pUsingShaderVarTables[ (int)eShaderType ]; }

		private:
			RHI::ICommandContext* m_pRhiContext;
			std::vector<CMeshDrawCommand*> m_CommandList_Render;
			RHI::IShaderState* m_pUsingShaderState;
			const CShaderVariableTable* m_pUsingShaderVarTables[ (int)RHI::EShaderType::__Count ];

			uint32 m_nVertexUsage;
			CMaterial* m_pMaterial;
			//uint32 m_nVertexSlotCount;
			//uint32 m_nVertexSize;
			uint32 m_nVertexCount;
			//uint32 m_nVertexSizes[ RHI::MaxVertexElementCount ];
			//uint32 m_nVertexSizeTotal;
			uint32 m_nIndexCount;
			//uint32 m_nIndexSizeTotal;
			//uint32 m_nTriangleCount;
		};
	}
}