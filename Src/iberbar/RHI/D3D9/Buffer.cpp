
#include <iberbar/RHI/D3D9/Buffer.h>
#include <iberbar/RHI/D3D9/Device.h>



iberbar::RHI::D3D9::CVertexBuffer::CVertexBuffer( CDevice* pDevice, uint32 nInSize, uint32 nUsage )
	: IVertexBuffer( nInSize, nUsage )
	, m_pDevice( pDevice )
	, m_pD3DVertexBuffer( nullptr )
	, m_bLocked( false )
{
	m_pDevice->AddRef();
}


iberbar::RHI::D3D9::CVertexBuffer::~CVertexBuffer()
{
	Destroy();
}


iberbar::CResult iberbar::RHI::D3D9::CVertexBuffer::Initial()
{
	IDirect3DDevice9* pD3DDevice = m_pDevice->GetD3DDevice();
	uint32 nInSize = GetInSize();
	uint32 nFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	if ( m_pDevice->IsHardwareVertexProcessing() == true )
	{
		nFVF = 0;
	}
	HRESULT hResult = pD3DDevice->CreateVertexBuffer(
		nInSize,
		GetD3DUsage(),
		nFVF,
		D3DPOOL_DEFAULT,
		&m_pD3DVertexBuffer,
		nullptr );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
	}
	m_bIsLost = false;
	return CResult();
}


void iberbar::RHI::D3D9::CVertexBuffer::Destroy()
{
	D3D_SAFE_RELEASE( m_pD3DVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


void iberbar::RHI::D3D9::CVertexBuffer::OnLost()
{
	if ( m_bIsManaged == true )
		return;

	D3D_SAFE_RELEASE( m_pD3DVertexBuffer );
	m_bIsLost = true;
}


iberbar::CResult iberbar::RHI::D3D9::CVertexBuffer::OnReset()
{
	if ( m_bIsManaged == true )
		return CResult();

	if ( m_pD3DVertexBuffer == nullptr )
	{
		return Initial();
	}

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CVertexBuffer::Lock( uint32 nOffset, uint32 nSize, void** ppData, uint32 nFlags )
{
	assert( (nOffset + nSize) <= GetInSize() );
	assert( m_bLocked == false );

	HRESULT hResult = m_pD3DVertexBuffer->Lock( nOffset, nSize, ppData, 0 );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );

	m_bLocked = true;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CVertexBuffer::Unlock()
{
	if ( m_bLocked == false )
		return CResult();

	HRESULT hResult = m_pD3DVertexBuffer->Unlock();
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );

	m_bLocked = false;

	return CResult();
}






iberbar::RHI::D3D9::CIndexBuffer::CIndexBuffer( CDevice* pDevice, uint32 nInSize, uint32 nUsage )
	: IIndexBuffer( 0, nInSize, nUsage )
	, m_pDevice( pDevice )
	, m_pD3DIndexBuffer( nullptr )
	, m_bLocked( false )
{
	m_pDevice->AddRef();
}


iberbar::RHI::D3D9::CIndexBuffer::~CIndexBuffer()
{
	Destroy();
}


iberbar::CResult iberbar::RHI::D3D9::CIndexBuffer::Initial()
{
	IDirect3DDevice9* pD3DDevice = m_pDevice->GetD3DDevice();
	HRESULT hResult = pD3DDevice->CreateIndexBuffer(
		GetInSize(),
		GetD3DUsage(),
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pD3DIndexBuffer,
		0 );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
	}
	m_bIsLost = false;
	return CResult();
}


void iberbar::RHI::D3D9::CIndexBuffer::Destroy()
{
	D3D_SAFE_RELEASE( m_pD3DIndexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


void iberbar::RHI::D3D9::CIndexBuffer::OnLost()
{
	if ( m_bIsManaged == true )
		return;

	D3D_SAFE_RELEASE( m_pD3DIndexBuffer );
	m_bIsLost = true;
}


iberbar::CResult iberbar::RHI::D3D9::CIndexBuffer::OnReset()
{
	if ( m_bIsManaged == true )
		return CResult();

	if ( m_pD3DIndexBuffer == nullptr )
	{
		return Initial();
	}

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CIndexBuffer::Lock( uint32 nOffset, uint32 nSize, void** ppData, uint32 nFlags )
{
	assert( (nOffset + nSize) <= GetInSize() );
	assert( m_bLocked == false );

	HRESULT hResult = m_pD3DIndexBuffer->Lock( nOffset, nSize, ppData, 0 );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );

	m_bLocked = true;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CIndexBuffer::Unlock()
{
	assert( m_bLocked == true );
	if ( m_bLocked == false )
		return CResult();

	HRESULT hResult = m_pD3DIndexBuffer->Unlock();
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );

	m_bLocked = false;

	return CResult();
}
