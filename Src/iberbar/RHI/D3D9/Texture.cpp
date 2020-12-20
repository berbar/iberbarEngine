
#include <iberbar/RHI/D3D9/Texture.h>
#include <iberbar/RHI/D3D9/Device.h>

iberbar::RHI::D3D9::CTexture::CTexture( CDevice* pDevice )
	: ITexture()
	, m_pDevice( pDevice )
	, m_pD3DTexture( nullptr )
{
	m_pDevice->AddRef();
}


iberbar::RHI::D3D9::CTexture::~CTexture()
{
	D3D_SAFE_RELEASE( m_pD3DTexture );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


iberbar::CResult iberbar::RHI::D3D9::CTexture::CreateEmpty( int w, int h )
{
	HRESULT hRet;

	//hRet = m_pDevice->GetD3DDevice()->CreateTexture( w, h, 1, 0, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL::D3DPOOL_MANAGED, &m_pD3DTexture, nullptr );
	//if ( FAILED( hRet ) )
	//{
	//	return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	//}

	hRet = D3DXCreateTexture( m_pDevice->GetD3DDevice(), w, h, 1, 0, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL::D3DPOOL_MANAGED, &m_pD3DTexture );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	D3DSURFACE_DESC lc_SurfaceDesc;
	m_pD3DTexture->GetLevelDesc( 0, &lc_SurfaceDesc );
	if ( lc_SurfaceDesc.Format != D3DFMT_A8R8G8B8 )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	m_Size = CSize2i( w, h );

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CTexture::CreateFromFileInMemory( const void* pData, uint32 nDataSize )
{
	HRESULT hRet;

	D3DXIMAGE_INFO info;
	hRet = D3DXGetImageInfoFromFileInMemory( pData, nDataSize, &info );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	//hRet = D3DXCreateTextureFromFileInMemory( m_pDevice->GetD3DDevice(), pData, nDataSize, &m_pD3DTexture );
	hRet = D3DXCreateTextureFromFileInMemoryEx(
		m_pDevice->GetD3DDevice(),
		pData,
		nDataSize,
		info.Width, info.Height,
		D3DFMT_FROM_FILE,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, // NONE真实像素，其他会拉伸，会出现模糊
		D3DX_DEFAULT,
		0, nullptr, nullptr,
		&m_pD3DTexture );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	IDirect3DSurface9* pSurface = nullptr;
	hRet = m_pD3DTexture->GetSurfaceLevel( 0, &pSurface );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	D3DSURFACE_DESC SurfaceDesc;
	pSurface->GetDesc( &SurfaceDesc );

	m_Size = CSize2i( SurfaceDesc.Width, SurfaceDesc.Height );

	pSurface->Release();
	pSurface = nullptr;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CTexture::CreateFromFileA( const char* strFile )
{
	HRESULT hRet;

	D3DXIMAGE_INFO info;
	hRet = D3DXGetImageInfoFromFileA( strFile, &info );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	hRet = D3DXCreateTextureFromFileExA(
		m_pDevice->GetD3DDevice(),
		strFile,
		info.Width, info.Height,
		D3DFMT_FROM_FILE,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, // NONE真实像素，其他会拉伸，会出现模糊
		D3DX_DEFAULT,
		0, nullptr, nullptr,
		&m_pD3DTexture );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	IDirect3DSurface9* pSurface = nullptr;
	hRet = m_pD3DTexture->GetSurfaceLevel( 0, &pSurface );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	D3DSURFACE_DESC SurfaceDesc;
	pSurface->GetDesc( &SurfaceDesc );

	m_Size = CSize2i( SurfaceDesc.Width, SurfaceDesc.Height );

	pSurface->Release();
	pSurface = nullptr;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CTexture::CreateFromFileW( const wchar_t* strFile )
{
	HRESULT hRet = D3DXCreateTextureFromFileW( m_pDevice->GetD3DDevice(), strFile, &m_pD3DTexture );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	IDirect3DSurface9* pSurface = nullptr;
	hRet = m_pD3DTexture->GetSurfaceLevel( 0, &pSurface );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	D3DSURFACE_DESC SurfaceDesc;
	pSurface->GetDesc( &SurfaceDesc );

	m_Size = CSize2i( SurfaceDesc.Width, SurfaceDesc.Height );

	pSurface->Release();
	pSurface = nullptr;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CTexture::CreateFromPixels( int w, int h, void* pPixels )
{
	assert( m_pD3DTexture );

	HRESULT hRet;

	hRet = D3DXCreateTexture( m_pDevice->GetD3DDevice(), w, h, 0, 0, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL::D3DPOOL_MANAGED, &m_pD3DTexture );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	D3DLOCKED_RECT D3DLockRect;
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = w;
	rect.bottom = h;
	hRet = m_pD3DTexture->LockRect( 0, &D3DLockRect, &rect, 0 );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	uint32 nByteCount = w * h * 4;
	memcpy_s( D3DLockRect.pBits, nByteCount, pPixels, nByteCount );

	m_pD3DTexture->UnlockRect( 0 );

	m_Size = CSize2i( w, h );

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CTexture::SetPixels( const void* pPixels, int nx, int ny, int nw, int nh )
{
	assert( (nx + nw) <= m_Size.w );
	assert( (ny + nh) <= m_Size.h );

	HRESULT hRet;

	D3DLOCKED_RECT D3DLockRect;
	RECT rect;
	rect.left = nx;
	rect.top = ny;
	rect.right = nx + nw;
	rect.bottom = ny + nh;
	hRet = m_pD3DTexture->LockRect( 0, &D3DLockRect, &rect, 0 );
	if ( FAILED( hRet ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	}

	// 
	int nCopyPerRow = nw * 4;
	uint8* pLockBits = (uint8*)D3DLockRect.pBits;
	const uint8* pPixelsTemp = (const uint8*)pPixels;
	int nLockedStride = D3DLockRect.Pitch;
	int nPixelsStride = nw * 4;
	for ( int i = 0; i < nh; i++ )
	{
		memcpy_s( pLockBits, nPixelsStride, pPixelsTemp, nPixelsStride );
		pLockBits += nLockedStride;
		pPixelsTemp += nPixelsStride;
	}

	m_pD3DTexture->UnlockRect( 0 );

	//hRet = D3DXFilterTexture( m_pD3DTexture, 0, 0, D3DX_DEFAULT );
	//if ( FAILED( hRet ) )
	//{
	//	return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hRet ) );
	//}

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CTexture::SaveToFileA( const char* strFile )
{
	HRESULT hResult = D3DXSaveTextureToFileA( strFile, D3DXIFF_PNG, m_pD3DTexture, nullptr );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
	}
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CTexture::SaveToFileW( const wchar_t* strFile )
{
	HRESULT hResult = D3DXSaveTextureToFileW( strFile, D3DXIFF_PNG, m_pD3DTexture, nullptr );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
	}
	return CResult();
}
