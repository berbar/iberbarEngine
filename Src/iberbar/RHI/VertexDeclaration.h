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
			IVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount );

		public:
			inline const uint32* GetStrides() const { return m_Strides; }
			inline const UVertexElement* GetVertexElements() const { return m_VertexElements; }
			inline uint32 GetVertexElementsCount() const { return m_nVertexElementsCount; }

		protected:
			uint32 m_nVertexElementsCount;
			uint32 m_Strides[ MaxVertexElementCount ];
			UVertexElement m_VertexElements[ MaxVertexElementCount ];
		};
	}
}

