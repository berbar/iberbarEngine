
#include <iberbar/Paper2d/Image.h>
#include <iberbar/Paper2d/DrawContext.h>
#include <iberbar/Renderer/TrianglesCommand.h>
#include <iberbar/RHI/Texture.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/RHI/VertexBuilder.h>




iberbar::Paper2d::CImage::CImage()
	: m_pTexture( nullptr )
	, m_Color( 255, 255, 255, 255 )
	, m_rcUV( 0, 0, 1, 1 )

	, m_pVertices()
	, m_pIndices()
	, m_pRenderCommand( new Renderer::CTrianglesCommand() )
{
	memset( m_pVertices, 0, sizeof( m_pVertices ) );
	memset( m_pIndices, 0, sizeof( m_pIndices ) );
}


iberbar::Paper2d::CImage::CImage( const CImage& image )
	: CNode( image )
	, m_pTexture( image.m_pTexture )
	, m_Color( image.m_Color )
	, m_rcUV( image.m_rcUV )

	, m_pVertices()
	, m_pIndices()
	, m_pRenderCommand( new Renderer::CTrianglesCommand() )
{
	if ( m_pTexture )
		m_pTexture->AddRef();

	memset( m_pVertices, 0, sizeof( m_pVertices ) );
	memset( m_pIndices, 0, sizeof( m_pIndices ) );
}


iberbar::Paper2d::CImage::~CImage()
{
	SAFE_DELETE( m_pRenderCommand );
	UNKNOWN_SAFE_RELEASE_NULL( m_pTexture );
}


void iberbar::Paper2d::CImage::SetTexture( RHI::ITexture* pTexture )
{
	if ( pTexture != m_pTexture )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pTexture );
		m_pTexture = pTexture;
		UNKNOWN_SAFE_ADDREF( m_pTexture );

		m_pShaderVariableTableUnion.GetVariableTableForPixelShader()->SetTexture( Renderer::s_strShaderVarName_Texture, m_pTexture );
		m_pShaderVariableTableUnion.GetVariableTableForPixelShader()->SetSamplerState( Renderer::s_strShaderVarName_TextureSampler, RHI::UTextureSamplerState() );
	}
}


void iberbar::Paper2d::CImage::Init()
{
	CNode::Init();

	m_pShaderVariableTableUnion.GetVariableTableForVertexShader()->SetBool( Renderer::s_strShaderVarName_RHW, false );
	m_pShaderVariableTableUnion.GetVariableTableForPixelShader()->SetBool( Renderer::s_strShaderVarName_UseTexture, true );

	RHI::BuildRectVertexIndices( m_pIndices, 0 );
	SetColor( 0xffffffff );
	Renderer::CTrianglesCommand::UTriangles Triangles(
		m_pVertices,
		m_pIndices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ),
		4,
		6,
		2
	);
	m_pRenderCommand->SetTriangles( Triangles );
	m_pRenderCommand->SetShaderState( m_pShaderState );
	m_pRenderCommand->SetShaderVariableTable( RHI::EShaderType::VertexShader, m_pShaderVariableTableUnion.GetVariableTableForVertexShader() );
	m_pRenderCommand->SetShaderVariableTable( RHI::EShaderType::PixelShader, m_pShaderVariableTableUnion.GetVariableTableForPixelShader() );
}


void iberbar::Paper2d::CImage::UpdateTransform()
{
	CNode::UpdateTransform();

	const auto& ContentSize = m_pTransform->GetContentSize();
	DirectX::XMFLOAT3 v4[ 4 ] = {
		DirectX::XMFLOAT3( 0.0f, ContentSize.y, 0.0f ),
		DirectX::XMFLOAT3( ContentSize.x, ContentSize.y, 0.0f ),
		DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ),
		DirectX::XMFLOAT3( ContentSize.x, 0.0f, 0.0f )
	};
	DirectX::XMVector3TransformCoordStream(
		(DirectX::XMFLOAT3*)m_pVertices, sizeof( RHI::UVertex_V3F_C4B_T2F ),
		v4, sizeof( DirectX::XMFLOAT3 ),
		4,
		m_pTransform->GetMatrix() );
}


void iberbar::Paper2d::CImage::DrawSelf( CDrawContext* pContext )
{
	m_pShaderVariableTableUnion.GetVariableTableForVertexShader()->SetMatrix( Renderer::s_strShaderVarName_MatViewProjection, pContext->GetMatViewProjection() );
	m_pRenderCommand->SetZOrder( m_nZOrder );
	pContext->GetRenderer()->AddCommand( m_pRenderCommand );
}


void iberbar::Paper2d::CImage::SetUV( const CRect2f& rcUV )
{
	m_rcUV = rcUV;
	RHI::RectBuildVertices_TexCoord( m_pVertices, rcUV );
}


void iberbar::Paper2d::CImage::SetColor( const CColor4B& Color )
{
	m_Color = Color;
	m_pVertices[ 0 ].color = m_pVertices[ 1 ].color = m_pVertices[ 2 ].color = m_pVertices[ 3 ].color = Color;
}


void iberbar::Paper2d::CImage::SetContentSizeAccordingToTexture()
{
	if ( m_pTexture != nullptr )
	{
		SetContentSize( (float)m_pTexture->GetSize().w * m_rcUV.Width(), (float)m_pTexture->GetSize().h * m_rcUV.Height() );
	}
}