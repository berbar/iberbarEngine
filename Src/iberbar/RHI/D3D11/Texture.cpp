

#include <iberbar/RHI/D3D11/Texture.h>
#include <iberbar/RHI/D3D11/Device.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/ScreenGrab.h>
#include <iberbar/Utility/String.h>

#include <Wincodec.h>


iberbar::RHI::D3D11::CTexture::CTexture( CDevice* pDevice )
	: m_pDevice( pDevice )
	, m_pD3DShaderResourceView( nullptr )
	, m_bDynamic( false )
{
	assert( m_pDevice );
}


iberbar::RHI::D3D11::CTexture::~CTexture()
{
	D3D_SAFE_RELEASE( m_pD3DShaderResourceView );
}


iberbar::CResult iberbar::RHI::D3D11::CTexture::CreateEmpty( int w, int h )
{
	HRESULT hResult;

	ComPtr<ID3D11Texture2D> pD3DTexture = nullptr;
	ComPtr<ID3D11ShaderResourceView> pD3DShaderResourceView = nullptr;

	D3D11_TEXTURE2D_DESC TexDesc;
	memset( &TexDesc, 0, sizeof( D3D11_TEXTURE2D_DESC ) );
	TexDesc.Width = (UINT)w;
	TexDesc.Height = (UINT)h;
	TexDesc.MipLevels = 1;
	TexDesc.ArraySize = 1;
	TexDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UINT;
	TexDesc.SampleDesc.Quality = 0;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC; // ¶¯Ì¬Ð´Èë
	TexDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	TexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TexDesc.MiscFlags = 0;
	hResult = m_pDevice->GetD3DDevice()->CreateTexture2D( &TexDesc, nullptr, &pD3DTexture );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	hResult = m_pDevice->GetD3DDevice()->CreateShaderResourceView( pD3DTexture.Get(), NULL, &pD3DShaderResourceView );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	m_Format = 0;
	m_bIsManaged = true;
	m_Size = CSize2i( w, h );

	m_pD3DTexture = pD3DTexture.Get();
	m_pD3DTexture->AddRef();
	m_pD3DShaderResourceView = pD3DShaderResourceView.Get();
	m_pD3DShaderResourceView->AddRef();

	m_bDynamic = true;

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CTexture::CreateFromFileInMemory( const void* pData, uint32 nDataSize )
{
	HRESULT hResult;
	hResult = DirectX::CreateWICTextureFromMemory( m_pDevice->GetD3DDevice(), (const uint8*)pData, nDataSize, (ID3D11Resource**)&m_pD3DTexture, &m_pD3DShaderResourceView );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	D3D11_TEXTURE2D_DESC Desc;
	memset( &Desc, 0, sizeof( Desc ) );
	m_pD3DTexture->GetDesc( &Desc );
	m_Size = CSize2i( Desc.Width, Desc.Height );

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CTexture::CreateFromFileA( const char* strFile )
{
	HRESULT hResult;
	wchar_t strFileWBuffer[ MAX_PATH ];
	Utf8ToUnicode( strFile, strFileWBuffer, MAX_PATH );
	hResult = DirectX::CreateWICTextureFromFile( m_pDevice->GetD3DDevice(), strFileWBuffer, (ID3D11Resource**)&m_pD3DTexture, &m_pD3DShaderResourceView );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	D3D11_TEXTURE2D_DESC Desc;
	memset( &Desc, 0, sizeof( Desc ) );
	m_pD3DTexture->GetDesc( &Desc );
	m_Size = CSize2i( Desc.Width, Desc.Height );

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CTexture::CreateFromFileW( const wchar_t* strFile )
{
	HRESULT hResult;
	hResult = DirectX::CreateWICTextureFromFile( m_pDevice->GetD3DDevice(), strFile, (ID3D11Resource**)&m_pD3DTexture, &m_pD3DShaderResourceView );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	D3D11_TEXTURE2D_DESC Desc;
	memset( &Desc, 0, sizeof( Desc ) );
	m_pD3DTexture->GetDesc( &Desc );
	m_Size = CSize2i( Desc.Width, Desc.Height );

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CTexture::CreateFromPixels( int w, int h, void* pixels )
{
	CResult cResult = CreateEmpty( w, h );
	if ( cResult.IsOK() == false )
		return cResult;

	return SetPixels( pixels, 0, 0, w, h );
}


iberbar::CResult iberbar::RHI::D3D11::CTexture::SetPixels( const void* pixels, int nx, int ny, int nw, int nh )
{
	if ( m_bDynamic == false )
		return MakeResult( ResultCode::Bad, "" );

	HRESULT hResult;
	ID3D11DeviceContext* pImmediateContext = m_pDevice->GetD3DDeviceContext();
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	hResult = pImmediateContext->Map( m_pD3DTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );

	BYTE* pRow = (BYTE*)MappedResource.pData;
	int nCopyBytes = nw * 4;
	int nCopyOffset = nx * 4;
	for ( int j = ny; j < m_Size.h; j++ )
	{
		memcpy( pRow + nCopyOffset, pixels, nCopyBytes );
		pRow += MappedResource.RowPitch;
	}
	pImmediateContext->Unmap( m_pD3DTexture, 0 );

	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CTexture::SaveToFileA( const char* strFile )
{
	assert( strFile );

	std::wstring strFileUnicode = Utf8ToUnicode( strFile );
	return SaveToFileW( strFileUnicode.c_str() );
}


iberbar::CResult iberbar::RHI::D3D11::CTexture::SaveToFileW( const wchar_t* strFile )
{
	assert( strFile );

	std::wstring strExt = StdWStringGetExtLowerCase( strFile );

	GUID format;
	if ( wcscmp( strExt.c_str(), L".png" ) == 0 )
		format = GUID_ContainerFormatPng;
	else if ( wcscmp( strExt.c_str(), L".jpeg" ) == 0 )
		format = GUID_ContainerFormatJpeg;
	else
		return MakeResult( ResultCode::Bad, "unknown picture format" );

	HRESULT hResult = DirectX::SaveWICTextureToFile( m_pDevice->GetD3DDeviceContext(), m_pD3DTexture, format, strFile );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );

	return CResult();
}
