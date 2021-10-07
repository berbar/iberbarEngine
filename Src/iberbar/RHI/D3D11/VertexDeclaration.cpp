
#include <iberbar/RHI/D3D11/VertexDeclaration.h>
#include <iberbar/RHI/D3D11/Types.h>


void iberbar::RHI::D3D11::CVertexDeclaration::BuildD3DInputElementDescritions( D3D11_INPUT_ELEMENT_DESC* pRefArray )
{
	const UVertexElement* pRHIElement = nullptr;
	memset( pRefArray, 0, sizeof( D3D11_INPUT_ELEMENT_DESC ) * 16 );
	uint32 i = 0;
	for ( ; i < m_nVertexElementsCount; i++ )
	{
		pRHIElement = &m_pVertexElements[ i ];
		pRefArray[ i ].SemanticName = ConvertVertexDeclareUsage( pRHIElement->nSemantic );
		pRefArray[ i ].SemanticIndex = (UINT)pRHIElement->nSemanticIndex;
		pRefArray[ i ].Format = ConvertVertexFormat( pRHIElement->nFormat );
		pRefArray[ i ].InputSlot = 0;
		pRefArray[ i ].AlignedByteOffset = pRHIElement->nOffset;
		pRefArray[ i ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pRefArray[ i ].InstanceDataStepRate = 0;
	}
}
