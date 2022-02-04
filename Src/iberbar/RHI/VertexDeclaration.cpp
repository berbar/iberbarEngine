
#include <iberbar/RHI/VertexDeclaration.h>



iberbar::RHI::IVertexDeclaration::IVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount )
	: IResource( UResourceType::VertexDeclaration )
	, m_nVertexElementsCount( 0 )
	, m_Strides()
	, m_VertexElements()
{
	assert( nVertexElementsCount <= MaxVertexElementCount );

	uint16 UsedStreamsMask = 0;
	memset( m_Strides, 0, sizeof( m_Strides ) );
	memset( m_VertexElements, 0, sizeof( m_VertexElements ) );

	for ( uint32 nElementIndex = 0; nElementIndex < nVertexElementsCount; nElementIndex++ )
	{
		const UVertexElement& Element = pVertexElements[ nElementIndex ];
		m_VertexElements[ nElementIndex ] = Element;

		if ( ( UsedStreamsMask & 1 << Element.nSlot ) != 0 )
		{
			assert( m_Strides[ Element.nSlot ] == Element.nStride );
		}
		else
		{
			UsedStreamsMask = UsedStreamsMask | (1 << Element.nSlot);
			m_Strides[ Element.nSlot ] = Element.nStride;
		}
	}
	
	m_nVertexElementsCount = nVertexElementsCount;
}

