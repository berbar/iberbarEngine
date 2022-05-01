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
				CVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount )
					: IVertexDeclaration( pVertexElements, nVertexElementsCount )
				{
				}
				void BuildD3DInputElementDescritions( D3D11_INPUT_ELEMENT_DESC* pRefArray );
			};
		}
	}
}