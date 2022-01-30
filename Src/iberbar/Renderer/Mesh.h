#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/Vertex.h>


namespace iberbar
{
	namespace RHI
	{
		class IVertexDeclaration;
	}

	namespace Renderer
	{
		class __iberbarRendererApi__ IMesh
			: public CRef
		{
		public:
			IMesh();
			IMesh( const IMesh& ) = delete;
			virtual ~IMesh() {}
			
			inline uint32 GetVertexCount() const { return m_nVertexCount; }
			inline uint32 GetVertexSlotCount() const { return m_nVertexSlotCount; }
			inline const void* GetVertexData( int nSlot ) const { return m_VertexDataSlots[ nSlot ]; }
			inline uint32 GetVertexSize( int nSlot ) const { return m_VertexDataSizes[ nSlot ]; }
			inline void* GetIndexData() const { return m_IndexData; }
			inline uint32 GetIndexCount() const {}
			inline RHI::UIndexFormat GetIndexFormat() const { return m_nIndexFormat; }
			

		protected:
			uint32 m_nVertexCount;
			uint32 m_nVertexSlotCount;
			RHI::UIndexFormat m_nIndexFormat;
			uint32 m_nIndexCount;
			void* m_IndexData;
			void* m_VertexDataSlots[ RHI::MaxVertexElementCount ];
			uint32 m_VertexDataSizes[ RHI::MaxVertexElementCount ];
			
			//RHI::UVertexDeclarationDesc m_VertexDeclarationDesc;
		};


		class __iberbarRendererApi__ CMeshFilter
		{
		public:
			void SetMesh( IMesh* pMesh );
			IMesh* GetMesh();

		protected:
			IMesh* m_pMesh;
		};



		class __iberbarRendererApi__ CMesh_V3F_C4F_T2F
			: public IMesh
		{
		public:
			CMesh_V3F_C4F_T2F();

			Renderer::UVertex_V3F_C4B_T2F* GetVertexPtrInternal() { return m_MeshVertices; }
			const Renderer::UVertex_V3F_C4B_T2F* GetVertexPtrInternal() const { return m_MeshVertices; }

		protected:
			Renderer::UVertex_V3F_C4B_T2F m_MeshVertices[ 4 ];
			uint16 m_MeshIndices[ 6 ];
		};
	}
}