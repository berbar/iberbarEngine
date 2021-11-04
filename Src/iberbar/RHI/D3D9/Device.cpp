
#include <iberbar/RHI/D3D9/Device.h>
#include <iberbar/RHI/D3D9/Buffer.h>
#include <iberbar/RHI/D3D9/CommandContext.h>
#include <iberbar/RHI/D3D9/Shader.h>
#include <iberbar/RHI/D3D9/VertexDeclaration.h>
#include <iberbar/RHI/D3D9/ShaderState.h>
#include <iberbar/RHI/D3D9/ShaderVariables.h>
#include <iberbar/RHI/D3D9/Texture.h>
#include <iberbar/RHI/D3D9/RenderState.h>
#include <iberbar/RHI/D3D9/Types.h>
#include <iberbar/Utility/OS/Windows/Error.h>
#include <DxErr.h>


iberbar::RHI::D3D9::CDevice::CDevice()
	: IDevice( UApiType::D3D9 )
	, m_hWnd( nullptr )
	, m_hInstance( nullptr )
	, m_pD3D( nullptr )
	, m_pD3DDevice( nullptr )
	, m_pD3DVertexBuffer( nullptr )
	, m_nVertexBufferStride( 0 )
	, m_pD3DIndexBuffer( nullptr )
	, m_pD3DVertexShader( nullptr )
	, m_pD3DPixelShader( nullptr )
	, m_bIsHardwareVertexProcessing( false )
	, m_bIsRendering( false )
	, m_bHasLostDevice( false )
	, m_pD3DTextures()
	, m_D3DPresentParams()
{
	memset( m_pD3DTextures, 0, sizeof( m_pD3DTextures ) );
}


iberbar::RHI::D3D9::CDevice::~CDevice()
{
	for ( int i = 0; i < 8; i++ )
	{
		D3D_SAFE_RELEASE( m_pD3DTextures[ i ] );
	}
	D3D_SAFE_RELEASE( m_pD3DVertexBuffer );
	D3D_SAFE_RELEASE( m_pD3DIndexBuffer );
	D3D_SAFE_RELEASE( m_pD3DVertexShader );
	D3D_SAFE_RELEASE( m_pD3DPixelShader );
	D3D_SAFE_RELEASE( m_pD3DDevice );
	D3D_SAFE_RELEASE( m_pD3D );
}


void iberbar::RHI::D3D9::CDevice::LostDevice()
{
	m_bHasLostDevice = true;

	BindVertexBuffer( nullptr, 0 );
	BindIndexBuffer( nullptr );
	SetVertexShader( nullptr );
	SetPixelShader( nullptr );
	for ( int i = 0; i < 8; i++ )
	{
		SetTexture( i, nullptr );
	}

	if ( m_CallbackLost )
	{
		m_CallbackLost( this );
	}
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::ResetDevice( int nBackBufferWidth, int nBackBufferHeight, bool bIsWindow )
{
	m_ContextSize = CSize2i( nBackBufferWidth, nBackBufferHeight );
	m_bIsWindow = bIsWindow;

	m_D3DPresentParams.BackBufferWidth = nBackBufferWidth;
	m_D3DPresentParams.BackBufferHeight = nBackBufferHeight;
	m_D3DPresentParams.Windowed = bIsWindow;

	HRESULT hResult = m_pD3DDevice->Reset( &m_D3DPresentParams );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorStringA( hResult ) );
	}

	if ( m_CallbackReset )
	{
		CResult ret = m_CallbackReset( this );
		if ( ret.IsOK() == false )
			return ret;
	}

	return CResult();
}


void iberbar::RHI::D3D9::CDevice::CreateTexture( ITexture** ppOutTexture )
{
	assert( ppOutTexture );
	TSmartRefPtr<CTexture> pTexture = TSmartRefPtr<CTexture>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutTexture );
	(*ppOutTexture) = pTexture;
	(*ppOutTexture)->AddRef();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateVertexBuffer( uint32 nInSize, uint32 nUsage, IVertexBuffer** ppOutBuffer )
{
	assert( ppOutBuffer );
	TSmartRefPtr<CVertexBuffer> pVertexBuffer = TSmartRefPtr<CVertexBuffer>::_sNew( this, nInSize, nUsage );
	CResult ret = pVertexBuffer->Initial();
	if ( ret.IsOK() == false )
		return ret;
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutBuffer );
	(*ppOutBuffer) = pVertexBuffer;
	(*ppOutBuffer)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateIndexBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage, IIndexBuffer** ppOutBuffer )
{
	assert( ppOutBuffer );
	TSmartRefPtr<CIndexBuffer> pIndexBuffer = TSmartRefPtr<CIndexBuffer>::_sNew( this, nInSize, nUsage );
	CResult ret = pIndexBuffer->Initial();
	if ( ret.IsOK() == false )
		return ret;
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutBuffer );
	(*ppOutBuffer) = pIndexBuffer;
	(*ppOutBuffer)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateVertexShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CVertexShader> pShader = TSmartRefPtr<CVertexShader>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	(*ppOutShader) = pShader;
	(*ppOutShader)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreatePixelShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CPixelShader> pShader = TSmartRefPtr<CPixelShader>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	( *ppOutShader ) = pShader;
	( *ppOutShader )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateHullShader( IShader** ppOutShader )
{
	return MakeResult( ResultCode::Bad, "Not support hull shader" );
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateGeometryShader( IShader** ppOutShader )
{
	return MakeResult( ResultCode::Bad, "Not support geometry shader" );
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateDomainShader( IShader** ppOutShader )
{
	return MakeResult( ResultCode::Bad, "Not support domain shader" );
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateComputeShader( IShader** ppOutShader )
{
	return MakeResult( ResultCode::Bad, "Not support compute shader" );
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateVertexDeclaration( IVertexDeclaration** ppOutDeclaration, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride )
{
	assert( ppOutDeclaration );
	TSmartRefPtr<CVertexDeclaration> pDeclaration = TSmartRefPtr<CVertexDeclaration>::_sNew( this, pVertexElements, nVertexElementsCount, nStride );
	CResult ret = pDeclaration->Initial();
	if ( ret.IsOK() == false )
		return ret;
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutDeclaration );
	(*ppOutDeclaration) = pDeclaration;
	(*ppOutDeclaration)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateShaderState(
	IShaderState** ppOutShaderState,
	IVertexDeclaration* pVertexDeclaration,
	IShader* pVertexShader,
	IShader* pPixelShader,
	IShader* pHullShader,
	IShader* pGeometryShader,
	IShader* pDomainShader )
{
	assert( ppOutShaderState );
	TSmartRefPtr<CShaderState> pShaderState = TSmartRefPtr<CShaderState>::_sNew( this, (CVertexDeclaration*)pVertexDeclaration, (CVertexShader*)pVertexShader, (CPixelShader*)pPixelShader );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderState );
	(*ppOutShaderState) = pShaderState;
	(*ppOutShaderState)->AddRef();
	return CResult();
}


void iberbar::RHI::D3D9::CDevice::CreateShaderVariableTable( IShaderVariableTable** ppOutShaderVarTable )
{
	assert( ppOutShaderVarTable );
	TSmartRefPtr<CShaderVariableTable> pShaderVarTable = TSmartRefPtr<CShaderVariableTable>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderVarTable );
	(*ppOutShaderVarTable) = pShaderVarTable;
	(*ppOutShaderVarTable)->AddRef();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateBlendState( IBlendState** ppOutBlendState, const UBlendDesc& BlendDesc )
{
	assert( ppOutBlendState );
	TSmartRefPtr<CBlendState> pBlendState = TSmartRefPtr<CBlendState>::_sNew( BlendDesc );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutBlendState );
	( *ppOutBlendState ) = pBlendState;
	( *ppOutBlendState )->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateDepthStencilState( IDepthStencilState** ppOutDepthStencilState, const UDepthStencilDesc& DepthStencilDesc )
{
	assert( ppOutDepthStencilState );
	TSmartRefPtr<CDepthStencilState> pDepthStencilState = TSmartRefPtr<CDepthStencilState>::_sNew( DepthStencilDesc );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutDepthStencilState );
	(*ppOutDepthStencilState) = pDepthStencilState;
	(*ppOutDepthStencilState)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateSamplerState( ISamplerState** ppOutSamplerState, const UTextureSamplerState& SamplerDesc )
{
	assert( ppOutSamplerState );
	TSmartRefPtr<CSamplerState> pBlendState = TSmartRefPtr<CSamplerState>::_sNew( SamplerDesc );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutSamplerState );
	( *ppOutSamplerState ) = pBlendState;
	( *ppOutSamplerState )->AddRef();
	return CResult();
}


void iberbar::RHI::D3D9::CDevice::CreateCommandContext( ICommandContext** ppOutContext )
{
	assert( ppOutContext );
	TSmartRefPtr<CCommandContext> pNewContext = TSmartRefPtr<CCommandContext>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutContext );
	(*ppOutContext) = pNewContext;
	(*ppOutContext)->AddRef();
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::Begin()
{
	HRESULT hr;
	if ( m_bHasLostDevice == true )
	{
		if ( FAILED( hr = m_pD3DDevice->TestCooperativeLevel() ) )
		{
			// the device has been lost but cannot be reset at this time
			if ( hr == D3DERR_DEVICELOST )
			{
				// request repaint and exit
				InvalidateRect( m_hWnd, NULL, TRUE );
				return MakeResult( ResultCode::Bad, "device lost" );
			}

			// the device has been lost and can be reset
			if ( hr == D3DERR_DEVICENOTRESET )
			{
				// do lost/reset/restore cycle
				LostDevice();
				CResult RetReset = ResetDevice();
				if ( RetReset.IsOK() == false )
					return RetReset;
			}
		}

		m_bHasLostDevice = false;
	}

	m_pD3DDevice->Clear( NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, (uint32)m_ClearColor, 1.0f, 0 );
	m_pD3DDevice->BeginScene();

	return CResult();
}


void iberbar::RHI::D3D9::CDevice::End()
{
	HRESULT hResult;
	m_pD3DDevice->EndScene();
	hResult = m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
	m_bHasLostDevice = (hResult == D3DERR_DEVICELOST);
}


iberbar::CResult iberbar::RHI::D3D9::CDevice::CreateDevice( HWND hWnd, bool bWindowed, int nSuitedWidth, int nSuitedHeight )
{
	assert( m_hWnd == nullptr );

	HRESULT hResult = S_OK;

	m_hWnd = hWnd;
	

	if ( m_pD3D || m_pD3DDevice )
		return MakeResult( ResultCode::Bad, "has been initialized" );

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	D3DCAPS9 caps;
	m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );
	int  vp = 0;
	if ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		m_bIsHardwareVertexProcessing = true;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		m_bIsHardwareVertexProcessing = false;
	}
	

	// Default to none.
	// 默认不使用多采样
	D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;

	// Check if 4x AA is supported, if so use it.
	// 检查是否支持4倍速率采样
	if ( m_pD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_D16, TRUE, D3DMULTISAMPLE_4_SAMPLES, NULL ) == D3D_OK )
	{
		// 保存多采样类型
		multiType = D3DMULTISAMPLE_4_SAMPLES;
	}

	m_D3DPresentParams.BackBufferWidth = nSuitedWidth;
	m_D3DPresentParams.BackBufferHeight = nSuitedHeight;
	m_D3DPresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_D3DPresentParams.BackBufferCount = 1;
	m_D3DPresentParams.MultiSampleType = multiType;
	m_D3DPresentParams.MultiSampleQuality = 0;
	m_D3DPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPresentParams.hDeviceWindow = m_hWnd;
	m_D3DPresentParams.Windowed = TRUE;
	m_D3DPresentParams.EnableAutoDepthStencil = TRUE;
	m_D3DPresentParams.AutoDepthStencilFormat = D3DFMT_D16;
	m_D3DPresentParams.Flags = 0;
	m_D3DPresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_D3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if ( FAILED( hResult = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, vp, &m_D3DPresentParams, &m_pD3DDevice ) ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorStringA( hResult ) );
	}

	D3DSURFACE_DESC SurfaceDesc;
	IDirect3DSurface9* pD3DSurface = NULL;
	if ( FAILED( hResult = m_pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pD3DSurface ) ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorStringA( hResult ) );
	}

	pD3DSurface->GetDesc( &SurfaceDesc );
	pD3DSurface->Release();
	pD3DSurface = NULL;

	m_ContextSize = CSize2i( SurfaceDesc.Width, SurfaceDesc.Height );
	m_bIsWindow = bWindowed;

	if ( m_CallbackCreated )
	{
		CResult ret = m_CallbackCreated( this );
		if ( ret.IsOK() == false )
			return ret;
	}

	return CResult();
}


void iberbar::RHI::D3D9::CDevice::SetSamplerState( uint32 nStage, const UTextureSamplerState& SamplerState )
{
	m_pD3DDevice->SetSamplerState( nStage, D3DSAMP_MIPFILTER, ConvertTextureFilterType( SamplerState.nMipFilter ) );
	m_pD3DDevice->SetSamplerState( nStage, D3DSAMP_MINFILTER, ConvertTextureFilterType( SamplerState.nMinFilter ) );
	m_pD3DDevice->SetSamplerState( nStage, D3DSAMP_MAGFILTER, ConvertTextureFilterType( SamplerState.nMagFilter ) );
	m_pD3DDevice->SetSamplerState( nStage, D3DSAMP_ADDRESSU, ConvertTextureAddress( SamplerState.nAddressU ) );
	m_pD3DDevice->SetSamplerState( nStage, D3DSAMP_ADDRESSV, ConvertTextureAddress( SamplerState.nAddressV ) );
	m_pD3DDevice->SetSamplerState( nStage, D3DSAMP_ADDRESSW, ConvertTextureAddress( SamplerState.nAddressW ) );
}


void iberbar::RHI::D3D9::CDevice::SetBlendState( IBlendState* pBlendState )
{
	assert( m_pBlendState );

	const UBlendDesc& BlendDesc = m_pBlendState->GetDesc();

	BOOL AlphaTest = BlendDesc.RenderTargets[0].BlendEnable == true ? TRUE : FALSE;
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, AlphaTest );

	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, ConvertBlend( BlendDesc.RenderTargets[0].SrcBlend ) );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, ConvertBlend( BlendDesc.RenderTargets[0].DestBlend ) );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, AlphaTest );
	m_pD3DDevice->SetRenderState( D3DRS_BLENDOP, ConvertBlendOP( BlendDesc.RenderTargets[0].BlendOp ) );

	m_pD3DDevice->SetRenderState( D3DRS_SRCBLENDALPHA, ConvertBlend( BlendDesc.RenderTargets[0].SrcBlendAlpha ) );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLENDALPHA, ConvertBlend( BlendDesc.RenderTargets[0].DestBlendAlpha ) );
	m_pD3DDevice->SetRenderState( D3DRS_BLENDOPALPHA, ConvertBlendOP( BlendDesc.RenderTargets[0].BlendOpAlpha ) );

	m_pD3DDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );

	m_pD3DDevice->SetRenderState( D3DRS_BLENDFACTOR, 0 );
}
