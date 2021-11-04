#pragma once

#include <iberbar/RHI/Types.h>
#include <iberbar/RHI/Resource.h>
//#include <unordered_map>

namespace iberbar
{
	namespace RHI
	{
		struct UVertexElement
		{
			uint32 nSlot;
			UVertexDeclareUsage nSemantic;
			uint32 nSemanticIndex;
			UVertexFormat nFormat;
			uint32 nOffset;
		};

		class __iberbarRHIApi__ IVertexDeclaration abstract
			: public IResource
		{
		public:
			IVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount, const uint32* pStrides, uint32 nSlotCount );

		public:
			inline const uint32* GetStrides() const { return m_Strides; }
			inline const UVertexElement* GetVertexElements() const { return m_pVertexElements; }
			inline uint32 GetVertexElementsCount() const { return m_nVertexElementsCount; }

		protected:
			uint32 m_nVertexElementsCount;
			uint32 m_Strides[ MaxVertexElementCount ];
			UVertexElement m_pVertexElements[ MaxVertexElementCount ];
		};
	}
}

