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
			UVertexDeclareUsage nSemantic;
			uint32 nSemanticIndex;
			UVertexFormat nFormat;
			uint32 nOffset;
		};

		class __iberbarRHIApi__ IVertexDeclaration
			: public IResource
		{
		public:
			IVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride );
			virtual ~IVertexDeclaration();

		public:
			inline uint32 GetStride() const { return m_nStride; }
			inline const UVertexElement* GetVertexElements() const { return m_pVertexElements; }
			inline uint32 GetVertexElementsCount() const { return m_nVertexElementsCount; }

		protected:
			uint32 m_nStride;
			UVertexElement* m_pVertexElements;
			uint32 m_nVertexElementsCount;
		};
	}
}

