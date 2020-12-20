

#include <iberbar/RHI/D3D9/VertexDeclaration.h>
#include <iberbar/RHI/D3D9/Device.h>
#include <iberbar/RHI/D3D9/Types.h>


iberbar::RHI::D3D9::CVertexDeclaration::CVertexDeclaration( CDevice* pDevice, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride )
	: IVertexDeclaration( pVertexElements, nVertexElementsCount, nStride )
	, m_pDevice( pDevice )
	, m_pD3DVertexDeclaration( nullptr )
{
	m_pDevice->AddRef();
	if ( nStride == 0 )
	{

	}
}


iberbar::RHI::D3D9::CVertexDeclaration::~CVertexDeclaration()
{
	D3D_SAFE_RELEASE( m_pD3DVertexDeclaration );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


iberbar::CResult iberbar::RHI::D3D9::CVertexDeclaration::Initial()
{
	D3DVERTEXELEMENT9 pD3DElements[ 16 ];
	const UVertexElement* pRHIElement = nullptr;
	memset( pD3DElements, 0, sizeof( pD3DElements ) );
	uint32 i = 0;
	for ( ; i < m_nVertexElementsCount; i++ )
	{
		pRHIElement = &m_pVertexElements[ i ];
		pD3DElements[ i ].Usage = ConvertVertexDeclareUsage( pRHIElement->nSemantic );
		pD3DElements[ i ].UsageIndex = (BYTE)pRHIElement->nSemanticIndex;
		pD3DElements[ i ].Stream = 0;
		pD3DElements[ i ].Offset = (WORD)pRHIElement->nOffset;
		pD3DElements[ i ].Type = ConvertVertexFormat( pRHIElement->nFormat );
		pD3DElements[ i ].Method = D3DDECLMETHOD_DEFAULT;
	}
	pD3DElements[ i ] = D3DDECL_END();

	HRESULT hResult = m_pDevice->GetD3DDevice()->CreateVertexDeclaration( pD3DElements, &m_pD3DVertexDeclaration );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );

	return CResult();
}


