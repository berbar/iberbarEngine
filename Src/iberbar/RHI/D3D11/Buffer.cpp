
#include <iberbar/RHI/D3D11/Buffer.h>
#include <iberbar/RHI/D3D11/Device.h>
#include <DirectXTK/BufferHelpers.h>


iberbar::RHI::D3D11::CVertexBuffer::CVertexBuffer( CDevice* pDevice, uint32 nInSize, uint32 nUsage )
	: IVertexBuffer( nInSize, nUsage )
	, m_pDevice( pDevice )
	, m_pD3DBuffer( nullptr )
	, m_bLocked( false )
{
	assert( m_pDevice );
	m_pDevice->AddRef();
}


iberbar::RHI::D3D11::CVertexBuffer::~CVertexBuffer()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


void iberbar::RHI::D3D11::CVertexBuffer::OnLost()
{
	if ( m_bIsManaged == true )
		return;

	m_pD3DBuffer = nullptr;
	m_bIsLost = true;
}


iberbar::CResult iberbar::RHI::D3D11::CVertexBuffer::OnReset()
{
	if ( m_bIsManaged == true )
		return CResult();

	if ( m_pD3DBuffer == nullptr )
	{
		return Initial();
	}

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CVertexBuffer::Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags )
{
	assert( ( nOffset + nSize ) <= GetInSize() );
	assert( m_bLocked == false );

	if ( IsDynamic() == false )
		return MakeResult( ResultCode::Bad, "the buffer isn't dynamic" );

	memset( &m_D3DMappedSubResource, 0, sizeof( m_D3DMappedSubResource ) );
	HRESULT hResult = m_pDevice->GetD3DDeviceContext()->Map( m_pD3DBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &m_D3DMappedSubResource );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );

	m_bLocked = true;
	( *ppOut ) = m_D3DMappedSubResource.pData;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CVertexBuffer::Unlock()
{
	if ( m_bLocked == false )
		return CResult();

	m_pDevice->GetD3DDeviceContext()->Unmap( m_pD3DBuffer.Get(), 0 );
	m_bLocked = false;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CVertexBuffer::Initial()
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	D3D11_BUFFER_DESC Desc;
	memset( &Desc, 0, sizeof( D3D11_BUFFER_DESC ) );
	Desc.Usage = GetD3DUsage();
	Desc.ByteWidth = GetInSize();
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.CPUAccessFlags = GetD3DCpuAccess();
	HRESULT hResult = pD3DDevice->CreateBuffer( &Desc, nullptr, &m_pD3DBuffer );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}
	return CResult();
}


void iberbar::RHI::D3D11::CVertexBuffer::Destroy()
{
	m_pD3DBuffer = nullptr;
}







iberbar::RHI::D3D11::CIndexBuffer::CIndexBuffer( CDevice* pDevice, uint32 nInStride, uint32 nInSize, uint32 nUsage )
	: IIndexBuffer( nInStride, nInSize, nUsage )
	, m_pDevice( pDevice )
	, m_pD3DBuffer( nullptr )
	, m_bLocked( false )
{
	assert( m_pDevice );
	m_pDevice->AddRef();
}


iberbar::RHI::D3D11::CIndexBuffer::~CIndexBuffer()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


void iberbar::RHI::D3D11::CIndexBuffer::OnLost()
{
	if ( m_bIsManaged == true )
		return;

	m_pD3DBuffer = nullptr;
	m_bIsLost = true;
}


iberbar::CResult iberbar::RHI::D3D11::CIndexBuffer::OnReset()
{
	if ( m_bIsManaged == true )
		return CResult();

	if ( m_pD3DBuffer == nullptr )
	{
		return Initial();
	}

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CIndexBuffer::Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags )
{
	assert( ( nOffset + nSize ) <= GetInSize() );
	assert( m_bLocked == false );

	if ( IsDynamic() == false )
		return MakeResult( ResultCode::Bad, "the buffer isn't dynamic" );

	memset( &m_D3DMappedSubResource, 0, sizeof( m_D3DMappedSubResource ) );
	HRESULT hResult = m_pDevice->GetD3DDeviceContext()->Map( m_pD3DBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &m_D3DMappedSubResource );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );

	m_bLocked = true;
	( *ppOut ) = m_D3DMappedSubResource.pData;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CIndexBuffer::Unlock()
{
	if ( m_bLocked == false )
		return CResult();

	m_pDevice->GetD3DDeviceContext()->Unmap( m_pD3DBuffer.Get(), 0 );
	m_bLocked = false;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CIndexBuffer::Initial()
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	D3D11_BUFFER_DESC Desc;
	memset( &Desc, 0, sizeof( D3D11_BUFFER_DESC ) );
	Desc.Usage = GetD3DUsage();
	Desc.ByteWidth = GetInSize();
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Desc.CPUAccessFlags = GetD3DCpuAccess();
	HRESULT hResult = pD3DDevice->CreateBuffer( &Desc, nullptr, &m_pD3DBuffer );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}
	return CResult();
}


void iberbar::RHI::D3D11::CIndexBuffer::Destroy()
{
	m_pD3DBuffer = nullptr;
}





iberbar::RHI::D3D11::CConstantBuffer::CConstantBuffer( CDevice* pDevice, uint32 nSize, uint32 nUsage )
	: m_pDevice( pDevice )
	, m_nSize( nSize )
	, m_nUsage( nUsage )
	, m_pD3DBuffer( nullptr )
{
	assert( m_pDevice );
	m_pDevice->AddRef();
}


iberbar::RHI::D3D11::CConstantBuffer::~CConstantBuffer()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


iberbar::CResult iberbar::RHI::D3D11::CConstantBuffer::Initial()
{
	D3D11_BUFFER_DESC Desc;
	memset( &Desc, 0, sizeof( D3D11_BUFFER_DESC ) );
	Desc.Usage = GetD3DUsage();
	Desc.ByteWidth = m_nSize;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = GetD3DCpuAccess();
	HRESULT hResult = m_pDevice->GetD3DDevice()->CreateBuffer( &Desc, nullptr, &m_pD3DBuffer );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );

	return CResult();
}


void iberbar::RHI::D3D11::CConstantBuffer::Destroy()
{
	m_pD3DBuffer = nullptr;
}

