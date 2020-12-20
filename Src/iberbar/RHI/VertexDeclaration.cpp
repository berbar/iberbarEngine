
#include <iberbar/RHI/VertexDeclaration.h>



iberbar::RHI::IVertexDeclaration::IVertexDeclaration( const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride )
	: IResource( UResourceType::VertexDeclaration )
	, m_nStride( nStride )
	, m_pVertexElements( nullptr )
	, m_nVertexElementsCount( 0 )
{
	m_pVertexElements = new UVertexElement[ nVertexElementsCount ];
	uint32 nCopySize = sizeof( UVertexElement ) * nVertexElementsCount;
	memcpy_s( m_pVertexElements, nCopySize, pVertexElements, nCopySize );
	m_nVertexElementsCount = nVertexElementsCount;
}


iberbar::RHI::IVertexDeclaration::~IVertexDeclaration()
{
	SAFE_DELETE_ARRAY( m_pVertexElements );
}

//
//iberbar::RHI::CVertexDeclaration::CVertexDeclaration()
//	: m_Attributes()
//	, m_nStride( 0 )
//{
//}
//
//void iberbar::RHI::CVertexDeclaration::SetAttribute( const std::string& strName, uint32 nIndex, UVertexFormat nFormat, uint32 nOffset, bool bNormallized )
//{
//	m_Attributes[ strName ] = UAttribute( strName, nIndex, nFormat, nOffset, bNormallized );
//}
//
//void iberbar::RHI::CVertexDeclaration::SetStride( uint32 nStride )
//{
//	m_nStride = nStride;
//}
