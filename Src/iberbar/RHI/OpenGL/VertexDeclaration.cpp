
#include <iberbar/RHI/OpenGL/VertexDeclaration.h>
#include <iberbar/RHI/OpenGL/Device.h>
#include <iberbar/RHI/OpenGL/ShaderProgram.h>
#include <iberbar/RHI/OpenGL/Types.h>

iberbar::RHI::OpenGL::CVertexDeclaration::CVertexDeclaration( CDevice* pDevice, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride )
	: IVertexDeclaration( pVertexElements, nVertexElementsCount, nStride )
	, m_pDevice( pDevice )
	, m_pGLVertexAttributes( nullptr )
	, m_nGLVertexAttributesCount( 0 )
{
	m_pDevice->AddRef();
}


iberbar::RHI::OpenGL::CVertexDeclaration::~CVertexDeclaration()
{
	SAFE_DELETE_ARRAY( m_pGLVertexAttributes );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


void iberbar::RHI::OpenGL::CVertexDeclaration::Initial( CShaderProgram* pShader )
{
	m_nGLVertexAttributesCount = m_nVertexElementsCount;
	m_pGLVertexAttributes = new UGLVertexAttribute[ m_nVertexElementsCount ];
	memset( m_pGLVertexAttributes, 0, sizeof( UGLVertexAttribute ) * m_nGLVertexAttributesCount );
	std::string strAttrName;
	const UVertexElement* pRHIVertexElement = nullptr;
	for ( uint32 i = 0; i < m_nVertexElementsCount; i++ )
	{
		pRHIVertexElement = &m_pVertexElements[ i ];
		ConvertVertexAttrName( strAttrName, pRHIVertexElement->nSemantic, pRHIVertexElement->nSemanticIndex );
		m_pGLVertexAttributes[ i ].nLocation = pShader->GetAttributeLocation( strAttrName.c_str() );
		m_pGLVertexAttributes[ i ].nOffset = pRHIVertexElement->nOffset;
		ConvertVertexFormat( pRHIVertexElement->nFormat, &m_pGLVertexAttributes[ i ].nSize, &m_pGLVertexAttributes[ i ].nType );
		if ( pRHIVertexElement->nFormat == UVertexFormat::COLOR )
			m_pGLVertexAttributes[ i ].bNormallized = true;
	}
}
