
#include <iberbar/RHI/VertexDeclaration.h>



iberbar::RHI::IVertexDeclaration::IVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount, const uint32* pStrides, uint32 nSlotCount )
	: IResource( UResourceType::VertexDeclaration )
	, m_nVertexElementsCount( 0 )
	, m_Strides()
	, m_pVertexElements()
{
	assert( nVertexElementsCount <= MaxVertexElementCount );
	assert( nSlotCount <= MaxVertexElementCount );

	memset( m_Strides, 0, sizeof( m_Strides ) );
	memset( m_pVertexElements, 0, sizeof( m_pVertexElements ) );

	m_nVertexElementsCount = nVertexElementsCount;
	m_nSlotCount = nSlotCount;
	memcpy_s( m_Strides, sizeof( m_Strides ), pStrides, sizeof( uint32 ) * nSlotCount );
	memcpy_s( m_pVertexElements, sizeof( m_pVertexElements ), pVertexElements, sizeof( UVertexElement ) * nVertexElementsCount );
}

