#pragma once

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/VertexDeclaration.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CVertexDeclaration
				: public IVertexDeclaration
			{
			public:
				CVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount, const uint32* pStrides, uint32 nSlotCount )
					: IVertexDeclaration( pVertexElements, nVertexElementsCount, pStrides, nSlotCount )
				{
				}
				void BuildD3DInputElementDescritions( D3D11_INPUT_ELEMENT_DESC* pRefArray );
			};
		}
	}
}