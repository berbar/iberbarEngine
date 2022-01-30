
#include <iberbar/RHI/D3D11/Device.h>
#include <iberbar/RHI/D3D11/Texture.h>
#include <iberbar/RHI/D3D11/ShaderState.h>
#include <iberbar/RHI/D3D11/ShaderVariables.h>
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/Buffer.h>
#include <iberbar/RHI/D3D11/VertexDeclaration.h>
#include <iberbar/RHI/D3D11/CommandContext.h>
#include <iberbar/RHI/D3D11/RenderState.h>




iberbar::RHI::D3D11::CDevice::CDevice()
	: IDevice( UApiType::D3D11 )
	, m_hWnd( nullptr )
	, m_hInstance( nullptr )
	, m_pDXGIFactory( nullptr )
	, m_pDXGIAdapter( nullptr )
	, m_DXGIOutputArraySize( 0 )
	, m_pDXGISwapChain( nullptr )
	, m_D3DFeatureLevel()
	, m_pD3DDevice( nullptr )
	, m_pD3DDeviceContext( nullptr )
	, m_pD3DDepthStencil( nullptr )
	, m_pD3DRenderTargetView( nullptr )
	, m_pD3DRasterizerState( nullptr )
	, m_bEnableMultisampleQuality( false )
	, m_nMultisampleQualityLevels( 0 )
	, m_pCommandContext( nullptr )
{
}


iberbar::RHI::D3D11::CDevice::~CDevice()
{
	SAFE_DELETE( m_pCommandContext );
}


void iberbar::RHI::D3D11::CDevice::LostDevice()
{
	if ( m_pD3DDevice )
	{
	}
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::ResetDevice( int nBackBufferWidth, int nBackBufferHeight, bool bIsWindow )
{
	return CResult();
}


void iberbar::RHI::D3D11::CDevice::CreateTexture( ITexture** ppTexture )
{
	assert( ppTexture );
	TSmartRefPtr<CTexture> pTexture = TSmartRefPtr<CTexture>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppTexture );
	( *ppTexture ) = pTexture;
	( *ppTexture )->AddRef();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateVertexBuffer( uint32 nInSize, uint32 nUsage, IVertexBuffer** ppOutBuffer )
{
	assert( ppOutBuffer );
	TSmartRefPtr<CVertexBuffer> pVertexBuffer = TSmartRefPtr<CVertexBuffer>::_sNew( this, nInSize, nUsage );
	CResult ret = pVertexBuffer->Initial();
	if ( ret.IsOK() == false )
		return ret;
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutBuffer );
	( *ppOutBuffer ) = pVertexBuffer;
	( *ppOutBuffer )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateIndexBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage, IIndexBuffer** ppOutBuffer )
{
	assert( ppOutBuffer );
	TSmartRefPtr<CIndexBuffer> pIndexBuffer = TSmartRefPtr<CIndexBuffer>::_sNew( this, nStride, nInSize, nUsage );
	CResult ret = pIndexBuffer->Initial();
	if ( ret.IsOK() == false )
		return ret;
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutBuffer );
	( *ppOutBuffer ) = pIndexBuffer;
	( *ppOutBuffer )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateVertexShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CVertexShader> pShader = TSmartRefPtr<CVertexShader>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	( *ppOutShader ) = pShader;
	( *ppOutShader )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreatePixelShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CPixelShader> pShader = TSmartRefPtr<CPixelShader>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	( *ppOutShader ) = pShader;
	( *ppOutShader )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateHullShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CHullShader> pShader = TSmartRefPtr<CHullShader>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	( *ppOutShader ) = pShader;
	( *ppOutShader )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateGeometryShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CGeometryShader> pShader = TSmartRefPtr<CGeometryShader>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	( *ppOutShader ) = pShader;
	( *ppOutShader )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateDomainShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CDomainShader> pShader = TSmartRefPtr<CDomainShader>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	( *ppOutShader ) = pShader;
	( *ppOutShader )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateComputeShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CComputeShader> pShader = TSmartRefPtr<CComputeShader>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	( *ppOutShader ) = pShader;
	( *ppOutShader )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateVertexDeclaration( IVertexDeclaration** ppOutDeclaration, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, const uint32* pStrides, uint32 nSlotCount )
{
	assert( ppOutDeclaration );
	TSmartRefPtr<CVertexDeclaration> pDeclaration = TSmartRefPtr<CVertexDeclaration>::_sNew( pVertexElements, nVertexElementsCount, pStrides, nSlotCount );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutDeclaration );
	( *ppOutDeclaration ) = pDeclaration;
	( *ppOutDeclaration )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateShaderProgram( IShaderProgram** ppOutShaderProgram, IShader* pVertexShader, IShader* pPixelShader, IShader* pHullShader, IShader* pGeometryShader, IShader* pDomainShader )
{
	assert( ppOutShaderProgram );
	TSmartRefPtr<CShaderProgram> pShaderProgram = TSmartRefPtr<CShaderProgram>::_sNew( this,
		(CVertexShader*)pVertexShader,
		(CPixelShader*)pPixelShader,
		(CHullShader*)pHullShader,
		(CGeometryShader*)pGeometryShader,
		(CDomainShader*)pDomainShader );
	pShaderProgram->Initial();
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderProgram );
	(*ppOutShaderProgram) = pShaderProgram;
	(*ppOutShaderProgram)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateShaderState( IShaderState** ppOutShaderState, IVertexDeclaration* pVertexDeclaration, IShaderProgram* pShaderProgram )
{
	assert( ppOutShaderState );
	TSmartRefPtr<CShaderState> pShaderState = TSmartRefPtr<CShaderState>::_sNew( this,
		(CVertexDeclaration*)pVertexDeclaration,
		(CShaderProgram*)pShaderProgram );
	CResult cRet = pShaderState->Initial();
	if ( cRet.IsOK() == false )
		return cRet;
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderState );
	( *ppOutShaderState ) = pShaderState;
	( *ppOutShaderState )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateBlendState( IBlendState** ppOutBlendState, const UBlendDesc& BlendDesc )
{
	assert( ppOutBlendState );
	TSmartRefPtr<CBlendState> pBlendState = TSmartRefPtr<CBlendState>::_sNew( BlendDesc );
	CResult cRet = pBlendState->Create( this );
	if ( cRet.IsOK() == false )
		return cRet;
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutBlendState );
	( *ppOutBlendState ) = pBlendState;
	( *ppOutBlendState )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateDepthStencilState( IDepthStencilState** ppOutDepthStencilState, const UDepthStencilDesc& DepthStencilDesc )
{
	assert(ppOutDepthStencilState);
	TSmartRefPtr<CDepthStencilState> pDepthStencilState = TSmartRefPtr<CDepthStencilState>::_sNew(DepthStencilDesc);
	CResult cRet = pDepthStencilState->Create(this);
	if (cRet.IsOK() == false)
		return cRet;
	UNKNOWN_SAFE_RELEASE_NULL(*ppOutDepthStencilState );
	(*ppOutDepthStencilState) = pDepthStencilState;
	(*ppOutDepthStencilState)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateSamplerState( ISamplerState** ppOutSamplerState, const UTextureSamplerState& SamplerDesc )
{
	assert( ppOutSamplerState );

	CSamplerState* pSamplerStateInCache = nullptr;
	for ( int i = 0, s = m_SamplerStatesCache.size(); i < s; i++ )
	{
		pSamplerStateInCache = m_SamplerStatesCache[ i ];
		if ( pSamplerStateInCache->GetSamplerDesc() == SamplerDesc )
		{
			UNKNOWN_SAFE_RELEASE_NULL( *ppOutSamplerState );
			( *ppOutSamplerState ) = pSamplerStateInCache;
			( *ppOutSamplerState )->AddRef();
			return CResult();
		}
	}

	TSmartRefPtr<CSamplerState> pSamplerState = TSmartRefPtr<CSamplerState>::_sNew( SamplerDesc );
	CResult cRet = pSamplerState->Create( this );
	if ( cRet.IsOK() == false )
		return cRet;

	m_SamplerStatesCache.push_back( pSamplerState );
	m_SamplerStatesCache[ m_SamplerStatesCache.size() - 1 ]->AddRef();

	UNKNOWN_SAFE_RELEASE_NULL( *ppOutSamplerState );
	( *ppOutSamplerState ) = pSamplerState;
	( *ppOutSamplerState )->AddRef();
	return CResult();
}


iberbar::RHI::ICommandContext* iberbar::RHI::D3D11::CDevice::GetDefaultContext()
{
	return m_pCommandContext;
}


iberbar::CResult iberbar::RHI::D3D11::CDevice::Begin()
{
	m_pD3DDeviceContext->ClearRenderTargetView( m_pD3DRenderTargetView.Get(), m_D3DClearColorRGBA );
	m_pD3DDeviceContext->ClearDepthStencilView( m_pD3DDepthStencilView.Get(), D3D11_CLEAR_DEPTH| D3D11_CLEAR_STENCIL, 1.0f, 0 );
	return CResult();
}


void iberbar::RHI::D3D11::CDevice::End()
{
	if ( true )
	{
		m_pDXGISwapChain->Present( 1, 0 );
	}
	else
	{
		m_pDXGISwapChain->Present( 0, 0 );
	}
}


void iberbar::RHI::D3D11::CDevice::SetClearColor( const CColor4B& color )
{
	IDevice::SetClearColor( color );
	m_D3DClearColorRGBA[ 0 ] = m_ClearColor.r;
	m_D3DClearColorRGBA[ 1 ] = m_ClearColor.g;
	m_D3DClearColorRGBA[ 2 ] = m_ClearColor.b;
	m_D3DClearColorRGBA[ 3 ] = m_ClearColor.a;

}


iberbar::CResult iberbar::RHI::D3D11::CDevice::CreateDevice( HWND hWnd, bool bWindowed, int nSuitedWidth, int nSuitedHeight )
{
	assert( hWnd );

	HRESULT hResult;

	m_hWnd = hWnd;
	if ( nSuitedWidth <= 0 || nSuitedHeight <= 0 )
	{
		RECT ClientRect;
		if ( ::GetClientRect( m_hWnd, &ClientRect ) == FALSE )
			return MakeResult( ResultCode::Bad, "" );
		nSuitedWidth = ClientRect.right - ClientRect.left;
		nSuitedHeight = ClientRect.bottom - ClientRect.top;
	}

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
	ComPtr<IDXGIFactory1> pDXGIFactory1 = nullptr;	// D3D11.0(����DXGI1.1)�Ľӿ���
	ComPtr<IDXGIFactory2> pDXGIFactory2 = nullptr;	// D3D11.1(����DXGI1.2)���еĽӿ���
	ComPtr<ID3D11Device> pD3DDevice = nullptr;
	ComPtr<ID3D11DeviceContext> pD3DDeviceContext = nullptr;
	ComPtr<ID3D11Device1> pD3DDevice1 = nullptr;
	ComPtr<ID3D11DeviceContext1> pD3DDeviceContext1 = nullptr;
	ComPtr<IDXGISwapChain> pDXGISwapChain = nullptr;
	ComPtr<IDXGISwapChain1> pDXGISwapChain1 = nullptr;


	// ����D3D�豸 �� D3D�豸������
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// ������������
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	// ���Եȼ�����
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE d3dDriverType;
	for ( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		d3dDriverType = driverTypes[driverTypeIndex];
		hResult = D3D11CreateDevice(
			nullptr,
			d3dDriverType,
			nullptr,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&pD3DDevice,
			&featureLevel,
			&pD3DDeviceContext );

		if ( hResult == E_INVALIDARG )
		{
			// Direct3D 11.0 ��API������D3D_FEATURE_LEVEL_11_1������������Ҫ�������Եȼ�11.0�Լ����µİ汾
			hResult = D3D11CreateDevice(
				nullptr,
				d3dDriverType,
				nullptr,
				createDeviceFlags,
				&featureLevels[1],
				numFeatureLevels - 1,
				D3D11_SDK_VERSION,
				&pD3DDevice,
				&featureLevel,
				&pD3DDeviceContext );
		}

		if ( SUCCEEDED( hResult ) )
			break;
	}

	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	// ����Ƿ�֧�����Եȼ�11.0��11.1
	if ( featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1 )
		return MakeResult( ResultCode::Bad, "Direct3D Feature Level 11 unsupported." );

	// ��� MSAA֧�ֵ������ȼ�
	pD3DDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_nMultisampleQualityLevels );
	assert( m_nMultisampleQualityLevels > 0 );


	// Ϊ����ȷ���� DXGI������������������Ҫ��ȡ���� D3D�豸 �� DXGI���������������������
	// "IDXGIFactory::CreateSwapChain: This function is being called with a device from a different IDXGIFactory."
	HR( pD3DDevice.As( &pDXGIDevice ), "" );
	HR( pDXGIDevice->GetAdapter( &pDXGIAdapter ), "" );
	HR( pDXGIAdapter->GetParent( __uuidof( IDXGIFactory1 ), reinterpret_cast<void**>( pDXGIFactory1.GetAddressOf() ) ), "" );

	// �鿴�ö����Ƿ����IDXGIFactory2�ӿ�
	HR( pDXGIFactory1.As( &pDXGIFactory2 ), "" );

	// �����������˵��֧��D3D11.1
	if ( pDXGIFactory2 != nullptr )
	{
		HR( pD3DDevice.As( &pD3DDevice1 ), "" );
		HR( pD3DDeviceContext.As( &pD3DDeviceContext1 ), "" );

		// �����ֽṹ����������������
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.Width = nSuitedWidth;
		sd.Height = nSuitedHeight;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// �Ƿ���4�����ز�����
		if ( m_bEnableMultisampleQuality )
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_nMultisampleQualityLevels - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd;
		fd.RefreshRate.Numerator = 60;
		fd.RefreshRate.Denominator = 1;
		fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fd.Windowed = TRUE;
		// Ϊ��ǰ���ڴ���������
		HR( pDXGIFactory2->CreateSwapChainForHwnd( pD3DDevice.Get(), m_hWnd, &sd, &fd, nullptr, &pDXGISwapChain1 ), "" );
		HR( pDXGISwapChain1.As( &pDXGISwapChain ), "" );
	}
	else
	{
		// ���DXGI_SWAP_CHAIN_DESC��������������
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferDesc.Width = nSuitedWidth;
		sd.BufferDesc.Height = nSuitedHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		// �Ƿ���4�����ز�����
		if ( m_bEnableMultisampleQuality )
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_nMultisampleQualityLevels - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_hWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;
		HR( pDXGIFactory1->CreateSwapChain( pD3DDevice.Get(), &sd, &pDXGISwapChain ), "" );
	}

	m_pD3DDevice = pD3DDevice;
	m_pD3DDeviceContext = pD3DDeviceContext;
	m_pDXGIFactory = pDXGIFactory1;
	m_pDXGISwapChain = pDXGISwapChain;
	m_D3DFeatureLevel = featureLevel;
	m_ContextSize = CSize2i( nSuitedWidth, nSuitedHeight );

	ComPtr<ID3D11Texture2D> pD3DBackBuffer = nullptr;
	HR( pDXGISwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pD3DBackBuffer ), "" );

	HR( m_pD3DDevice->CreateRenderTargetView( pD3DBackBuffer.Get(), nullptr, &m_pD3DRenderTargetView ), "" );

	D3D11_TEXTURE2D_DESC D3DDepthBufferDesc;
	memset( &D3DDepthBufferDesc, 0, sizeof( D3DDepthBufferDesc ) );
	D3DDepthBufferDesc.Width = m_ContextSize.w;
	D3DDepthBufferDesc.Height = m_ContextSize.h;
	D3DDepthBufferDesc.MipLevels = 1;
	D3DDepthBufferDesc.ArraySize = 1;
	D3DDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	D3DDepthBufferDesc.SampleDesc.Count = 1;
	D3DDepthBufferDesc.SampleDesc.Quality = 0;
	D3DDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3DDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	D3DDepthBufferDesc.CPUAccessFlags = 0;
	D3DDepthBufferDesc.MiscFlags = 0;
	HR( m_pD3DDevice->CreateTexture2D( &D3DDepthBufferDesc, nullptr, &m_pD3DDepthStencil ), "" );

	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	memset( &DepthStencilDesc, 0, sizeof( DepthStencilDesc ) );
	DepthStencilDesc.DepthEnable = false;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xff;
	DepthStencilDesc.StencilWriteMask = 0xff;
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HR( m_pD3DDevice->CreateDepthStencilState( &DepthStencilDesc, &m_pD3DDepthStencilState ), "" );

	m_pD3DDeviceContext->OMSetDepthStencilState( m_pD3DDepthStencilState.Get(), 1 );

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	memset( &DepthStencilViewDesc, 0, sizeof( DepthStencilViewDesc ) );
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;
	HR( m_pD3DDevice->CreateDepthStencilView( m_pD3DDepthStencil.Get(), &DepthStencilViewDesc, &m_pD3DDepthStencilView ), "");

	//D3D11_RASTERIZER_DESC RasterizerDesc;
	//memset( &RasterizerDesc, 0, sizeof( RasterizerDesc ) );
	//RasterizerDesc.AntialiasedLineEnable = false;
	//RasterizerDesc.CullMode = D3D11_CULL_BACK;
	//RasterizerDesc.DepthBias = 0;
	//RasterizerDesc.DepthBiasClamp = 0.0f;
	//RasterizerDesc.DepthClipEnable = true;
	//RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//RasterizerDesc.FrontCounterClockwise = false;
	//RasterizerDesc.MultisampleEnable = false;
	//RasterizerDesc.ScissorEnable = false;
	//RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	//HR( m_pD3DDevice->CreateRasterizerState( &RasterizerDesc, &m_pD3DRasterizerState ), "" );
	//
	//m_pD3DDeviceContext->RSSetState( m_pD3DRasterizerState.Get() );

		// ����ȾĿ����ͼ�����/ģ�建������ϵ�����
	m_pD3DDeviceContext->OMSetRenderTargets(1, m_pD3DRenderTargetView.GetAddressOf(), m_pD3DDepthStencilView.Get());

	D3D11_VIEWPORT Viewport;
	memset( &Viewport, 0, sizeof( Viewport ) );
	Viewport.Width = m_ContextSize.w;
	Viewport.Height = m_ContextSize.h;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	m_pD3DDeviceContext->RSSetViewports( 1, &Viewport );

	// ���Խ�ֹalt+enterȫ��
	pDXGIFactory1->MakeWindowAssociation( hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES );

	m_pCommandContext = new CCommandContext( this );

	return CResult();
}



