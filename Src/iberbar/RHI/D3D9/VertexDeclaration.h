#pragma once

#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/VertexDeclaration.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CDevice;

			class __iberbarD3DApi__ CVertexDeclaration
				: public IVertexDeclaration
			{
			public:
				CVertexDeclaration( CDevice* pDevice, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride );
				virtual ~CVertexDeclaration();

			public:
				CResult Initial();
				inline IDirect3DVertexDeclaration9* GetD3DVertexDeclaration() { return m_pD3DVertexDeclaration; }

			private:
				CDevice* m_pDevice;
				IDirect3DVertexDeclaration9* m_pD3DVertexDeclaration;
			};
		}
	}
}

