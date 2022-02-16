
#include <iberbar/Renderer/Mesh.h>

iberbar::Renderer::CMesh::CMesh()
	: m_nIndexCount( 0 )
	, m_IndexData( nullptr )
	, m_nVertexCount( 0 )
	, m_Positions( nullptr )
	, m_Normals( nullptr )
	, m_TexcoordSlots()
{
	memset( m_TexcoordSlots, 0, sizeof( m_TexcoordSlots ) );
}








iberbar::Renderer::CMeshDefault::CMeshDefault()
	: m_nVertexUsage( 0 )
{

}


iberbar::Renderer::CMeshDefault::~CMeshDefault()
{
	SAFE_DELETE_ARRAY( m_IndexData );
	SAFE_DELETE_ARRAY( m_Positions );
	SAFE_DELETE_ARRAY( m_Normals );
	for ( int i = 0; i < 8; i++ )
	{
		SAFE_DELETE_ARRAY( m_TexcoordSlots[ i ] );
	}
}


void iberbar::Renderer::CMeshDefault::NewVertices( uint32 nVertexCount, uint32 nVertexUsage )
{

	SAFE_DELETE_ARRAY( m_Positions );
	SAFE_DELETE_ARRAY( m_Normals );
	for ( int i = 0; i < 8; i++ )
	{
		SAFE_DELETE_ARRAY( m_TexcoordSlots[ i ] );
	}
	m_nVertexCount = 0;

	if ( nVertexCount == 0 )
		return;

	m_Positions = new UVector3f[ nVertexCount ];

	if ( (nVertexUsage & EVertexElementUsage::Normal) == EVertexElementUsage::Normal )
	{
		m_Normals = new UVector3f[ nVertexCount ];
	}

	for ( int i = 0; i < 8; i++ )
	{
		if ( (nVertexUsage & (EVertexElementUsage::Tex0 << i)) == (EVertexElementUsage::Tex0 << i) )
		{
			m_TexcoordSlots[ i ] = new UVector2f[ nVertexCount ];
		}
	}

	m_nVertexCount = nVertexCount;
}


void iberbar::Renderer::CMeshDefault::NewIndices( uint32 nIndexCount )
{
	SAFE_DELETE_ARRAY( m_IndexData );
	m_nIndexCount = 0;

	if ( nIndexCount == 0 )
		return;

	m_IndexData = new uint16[ nIndexCount ];
	m_nIndexCount = nIndexCount;
}



