#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/Material.h>
#include <iberbar/RHI/Types.h>

namespace iberbar
{
	namespace RHI
	{
		struct UVertexElement;
	}

	namespace Renderer
	{


		class CBaseMesh
			: public CRef
		{
		public:
			CBaseMesh();
			virtual ~CBaseMesh();

			void Reset( const RHI::UVertexElement* pVertexLayout, RHI::UPrimitiveType nPrimitiveType, uint32 nNumPrimitives );

			virtual const void* GetVertices() const = 0;
			virtual const uint16* GetIndices() const = 0;

		protected:
			uint32 m_nMeshInstanceId;
			uint8* m_pVertices;
			uint16* m_pIndices;
		};

		class CBaseMeshDrawCommand
		{
		public:
			void SetMesh( CBaseMesh* pMesh );

		protected:
			CBaseMesh* m_pMesh;
			CMaterial* m_pMaterial;
		};
	}
}