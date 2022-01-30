#pragma once

#include <iberbar/RHI/Types.h>
#include <iberbar/RHI/Resource.h>
//#include <unordered_map>

namespace iberbar
{
	namespace RHI
	{

		class __iberbarRHIApi__ IVertexDeclaration abstract
			: public IResource
		{
		public:
			IVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount, const uint32* pStrides, uint32 nSlotCount );

		public:
			inline uint32 GetSlotCount() const { return m_nSlotCount; }
			inline const uint32* GetStrides() const { return m_Strides; }
			inline const UVertexElement* GetVertexElements() const { return m_pVertexElements; }
			inline uint32 GetVertexElementsCount() const { return m_nVertexElementsCount; }

		protected:
			uint32 m_nVertexElementsCount;
			uint32 m_nSlotCount;
			uint32 m_Strides[ MaxVertexElementCount ];
			UVertexElement m_pVertexElements[ MaxVertexElementCount ];
		};
	}
}

