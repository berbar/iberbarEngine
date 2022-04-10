

#include <iberbar/RHI/D3D11/TestDraw.h>
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/Buffer.h>
#include <iberbar/RHI/D3D11/ShaderState.h>
#include <iberbar/RHI/D3D11/VertexDeclaration.h>
#include <iberbar/RHI/D3D11/Device.h>
#include <DirectXMath.h>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT4;

#ifdef _DEBUG


const char TestDrawVS[] = R"(
struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

// 顶点着色器
VertexOut Main(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = float4(vIn.pos, 1.0f);
    vOut.color = vIn.color; // 这里alpha通道的值默认为1.0
    vOut.texcoord = vIn.texcoord;
    return vOut;
}
)";

const char TestDrawPS[] = R"(
struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_textureSampler : register(s0);

// 像素着色器
float4 Main(VertexOut pIn) : SV_Target
{
    //return g_texture.Sample(g_textureSampler, pIn.texcoord) * pIn.color;
    return pIn.color;
}
)";

struct VertexPosColor
{
	XMFLOAT2 pos;
	XMFLOAT4 color;
	XMFLOAT2 texcoord;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[ 3 ];
};
const D3D11_INPUT_ELEMENT_DESC VertexPosColor::inputLayout[ 3 ] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof( VertexPosColor, color ), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof( VertexPosColor, texcoord ), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


iberbar::RHI::D3D11::CTestDraw::CTestDraw()
	: m_pDevice( nullptr )
	, m_pD3DDeviceContext( nullptr )
	, m_pVertexBuffer( nullptr )
	//, m_pVertexLayout( nullptr )
	, m_pVertexShader( nullptr )
	, m_pPixelShader( nullptr )
	, m_pShaderProgram( nullptr )
	, m_pVertexDecl( nullptr )
	, m_pShaderState( nullptr )
{

}


iberbar::RHI::D3D11::CTestDraw::~CTestDraw()
{
	//D3D_SAFE_RELEASE( m_pVertexLayout );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pPixelShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderProgram );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexDecl );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
}


void iberbar::RHI::D3D11::CTestDraw::Initial( CDevice* pDevice )
{
	m_pDevice = pDevice;
	m_pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();

	m_pDevice->CreateVertexBuffer( sizeof( VertexPosColor ) * 3, UBufferUsageFlags::Dynamic, (IVertexBuffer**)&m_pVertexBuffer );
	//m_pDevice->CreateIndexBuffer( sizeof( uint ) * 3, UBufferUsageFlags::Dynamic, (IVertexBuffer**)&m_pVertexBuffer );

	m_pDevice->CreateVertexShader( (IShader**)&m_pVertexShader );
	m_pVertexShader->LoadFromSource( TestDrawVS );

	m_pDevice->CreatePixelShader( (IShader**)&m_pPixelShader );
	m_pPixelShader->LoadFromSource( TestDrawPS );

	m_pDevice->CreateShaderProgram( (IShaderProgram**)&m_pShaderProgram, m_pVertexShader, m_pPixelShader, nullptr, nullptr, nullptr );

	//uint32 nSlot;
	//UVertexDeclareUsage nSemantic;
	//uint32 nSemanticIndex;
	//UVertexFormat nFormat;
	//uint32 nOffset;
	//uint32 nStride;
	UVertexElement VertexElements[ 3 ] = {
		{ 0, UVertexDeclareUsage::Position, 0, UVertexFormat::FLOAT2, 0, sizeof( VertexPosColor ) },
		{ 0, UVertexDeclareUsage::Color, 0, UVertexFormat::FLOAT4, offsetof( VertexPosColor, color ), sizeof( VertexPosColor ) },
		{ 0, UVertexDeclareUsage::TexCoord, 0, UVertexFormat::FLOAT2, offsetof( VertexPosColor, texcoord ), sizeof( VertexPosColor ) }
	};
	m_pDevice->CreateVertexDeclaration( (IVertexDeclaration**)&m_pVertexDecl, VertexElements, 3 );

	m_pDevice->CreateShaderState( (IShaderState**)&m_pShaderState, m_pVertexDecl, m_pShaderProgram );

	// 创建并绑定顶点布局
	//m_pDevice->GetD3DDevice()->CreateInputLayout( VertexPosColor::inputLayout, ARRAYSIZE( VertexPosColor::inputLayout ),
	//	m_pVertexShader->GetCodePointer(), m_pVertexShader->GetCodeSize(), m_pVertexLayout.GetAddressOf() );
}

void iberbar::RHI::D3D11::CTestDraw::Draw()
{
	VertexPosColor vertices[] =
	{
		{ XMFLOAT2( 0.0f, 1.5f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
		{ XMFLOAT2( 0.5f, -0.5f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
		{ XMFLOAT2( -0.5f, -0.5f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) }
	};

	//D3D11_MAPPED_SUBRESOURCE m_D3DMappedSubResource;
	//memset( &m_D3DMappedSubResource, 0, sizeof( m_D3DMappedSubResource ) );
	//m_pD3DDeviceContext->Map( m_pVertexBuffer->GetD3DBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &m_D3DMappedSubResource );

	//memcpy_s( m_D3DMappedSubResource.pData, sizeof( vertices ), vertices, sizeof( vertices ) );
	//m_pD3DDeviceContext->Unmap( m_pVertexBuffer->GetD3DBuffer(), 0 );

	int verticesSize = sizeof( VertexPosColor ) * 3;
	void* verticesTemp = nullptr;
	m_pVertexBuffer->Lock( 0, verticesSize, &verticesTemp );
	memcpy_s( verticesTemp, verticesSize, vertices, verticesSize );
	m_pVertexBuffer->Unlock();

	//memset( &m_D3DMappedSubResource, 0, sizeof( m_D3DMappedSubResource ) );
	//m_pD3DDeviceContext->Map( m_pVertexBuffer_2.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &m_D3DMappedSubResource );
	//VertexPosColor vertices_2[] =
	//{
	//	
	//	{ XMFLOAT3( 0.5f, 0.5f, 0.5f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
	//	{ XMFLOAT3( 0.0f, -0.5f, 0.5f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
	//	{ XMFLOAT3( -0.5f, 0.5f, 0.5f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) }
	//};
	//memcpy_s( m_D3DMappedSubResource.pData, sizeof( vertices_2 ), vertices_2, sizeof( vertices_2 ) );
	//m_pD3DDeviceContext->Unmap( m_pVertexBuffer_2.Get(), 0 );

	//ID3D11Buffer* vbs[ 2 ] = { m_pVertexBuffer->GetD3DBuffer() };
	//UINT strides[ 2 ] = { sizeof( VertexPosColor ) };	// 跨越字节数
	//UINT offsets[ 2 ] = { 0 };						// 起始偏移量
	//m_pD3DDeviceContext->IASetVertexBuffers( 0, 1, vbs, strides, offsets );
	m_pDevice->GetDefaultContext()->SetShaderState( m_pShaderState );
	m_pDevice->GetDefaultContext()->SetVertexBuffer( 0, m_pVertexBuffer, 0 );
	//UINT stride = sizeof( VertexPosColor );	// 跨越字节数
	//UINT offset = 0;
	//m_pD3DDeviceContext->IASetVertexBuffers( 0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset );
	//m_pD3DDeviceContext->IASetVertexBuffers( 1, 1, m_pVertexBuffer_2.GetAddressOf(), &stride, &offset );
	// 设置图元类型，设定输入布局
	//m_pD3DDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	m_pDevice->GetDefaultContext()->SetPrimitiveTopology( UPrimitiveType::Triangle );
	//m_pD3DDeviceContext->IASetInputLayout( m_pShaderState->GetD3DInputLayout() );
	// 将着色器绑定到渲染管线
	//m_pD3DDeviceContext->VSSetShader( m_pVertexShader->GetD3DShader(), nullptr, 0 );
	//m_pD3DDeviceContext->PSSetShader( m_pPixelShader->GetD3DShader(), nullptr, 0 );
	//m_pDevice->GetDefaultContext()->SetShaderState( m_pShaderState );

	// 绘制三角形
	m_pD3DDeviceContext->Draw( 6, 0 );
}


#endif