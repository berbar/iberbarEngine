
#include <iberbar/Paper2d/Terrain.h>
#include <iberbar/Paper2d/DrawContext.h>
#include <iberbar/Renderer/TrianglesCommand.h>
#include <iberbar/RHI/Texture.h>
#include <iberbar/RHI/VertexBuilder.h>


iberbar::Paper2d::CGridTerrain::CGridTerrain()
	: m_nGridRow( 0 )
	, m_nGridCol( 0 )
	, m_GridSize()
	, m_pTexture( nullptr )
	, m_pVertices( nullptr )
	, m_pIndices( nullptr )
	, m_pRenderCommand( new Renderer::CTrianglesCommand() )
{

}


iberbar::Paper2d::CGridTerrain::CGridTerrain( const CGridTerrain& terrain )
	: CNode( terrain )
	, m_nGridRow( 0 )
	, m_nGridCol( 0 )
	, m_GridSize()
	, m_pTexture( terrain.m_pTexture )
	, m_pVertices( nullptr )
	, m_pIndices( nullptr )
	, m_pRenderCommand( new Renderer::CTrianglesCommand() )
{
	UNKNOWN_SAFE_ADDREF( m_pTexture );
}


iberbar::Paper2d::CGridTerrain::~CGridTerrain()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pTexture );
	SAFE_DELETE( m_pRenderCommand );
	SAFE_DELETE( m_pVertices );
	SAFE_DELETE( m_pIndices );
}


iberbar::Paper2d::CGridTerrain* iberbar::Paper2d::CGridTerrain::Clone() const
{
	return new CGridTerrain( *this );
}


void iberbar::Paper2d::CGridTerrain::Init()
{
	CNode::Init();

	m_pShaderVariableTableUnion.GetVariableTableForPixelShader()->SetBool( Renderer::s_strShaderVarName_UseTexture, true );
	m_pRenderCommand->SetShaderState( m_pShaderState );
	m_pRenderCommand->SetShaderVariableTable( RHI::EShaderType::VertexShader, m_pShaderVariableTableUnion.GetVariableTableForVertexShader() );
	m_pRenderCommand->SetShaderVariableTable( RHI::EShaderType::PixelShader, m_pShaderVariableTableUnion.GetVariableTableForPixelShader() );
}


void iberbar::Paper2d::CGridTerrain::UpdateTransform()
{
	CNode::UpdateTransform();

	BuildGrids();
	//DirectX::XMVector3TransformCoordStream(
	//	(DirectX::XMFLOAT3*)m_pVertices,
	//	sizeof( RHI::UVertex_V3F_C4B_T2F ),
	//	nullptr,
	//	0,
	//	nVertexCount,
	//	m_pTransform->GetMatrix()
	//);
	//BuildGrids();
}


void iberbar::Paper2d::CGridTerrain::DrawSelf( CDrawContext* pContext )
{
	if ( m_pVertices != nullptr &&
		m_pIndices != nullptr &&
		m_pShaderState != nullptr )
	{
		m_pShaderVariableTableUnion.GetVariableTableForVertexShader()->SetMatrix( Renderer::s_strShaderVarName_MatViewProjection, pContext->GetMatViewProjection() );
		m_pRenderCommand->SetZOrder( m_nZOrder );
		pContext->GetRenderer()->AddCommand( m_pRenderCommand );
	}
}


void iberbar::Paper2d::CGridTerrain::SetGridSize( float nWidth, float nHeight )
{
	m_GridSize = CSize2f( nWidth, nHeight );

	BuildGrids();
}


void iberbar::Paper2d::CGridTerrain::SetGrids( int nGridRow, int nGridCol, const UGridPaletteNode* pPalette, int nPaletteSize, const int* pGrids )
{
	if ( nGridRow <= 0 || nGridCol <= 0 )
		return;
	if ( pPalette == nullptr || pGrids == nullptr )
		return;

	SAFE_DELETE( m_pVertices );
	SAFE_DELETE( m_pIndices );

	m_nGridRow = nGridRow;
	m_nGridCol = nGridCol;

	int nGridCount = m_nGridRow * m_nGridCol;
	m_pVertices = new RHI::UVertex_V3F_C4B_T2F[ nGridCount * 4 ];
	m_pIndices = new uint16[ nGridCount * 6 ];

	BuildGrids( pPalette, nPaletteSize, pGrids );
}


void iberbar::Paper2d::CGridTerrain::SetTexture( RHI::ITexture* pTexture )
{
	if ( m_pTexture != pTexture )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pTexture );
		m_pTexture = pTexture;
		UNKNOWN_SAFE_ADDREF( m_pTexture );
		m_pShaderVariableTableUnion.GetVariableTableForPixelShader()->SetTexture( Renderer::s_strShaderVarName_Texture, m_pTexture );
		m_pShaderVariableTableUnion.GetVariableTableForPixelShader()->SetSamplerState( Renderer::s_strShaderVarName_TextureSampler, RHI::UTextureSamplerState() );
	}
}


void iberbar::Paper2d::CGridTerrain::BuildGrids( const UGridPaletteNode* pPalette, int nPaletteSize, const int* pGrids )
{
	if ( m_pVertices == nullptr || m_pIndices == nullptr )
		return;

	int nGridCount = m_nGridRow * m_nGridCol;
	int nVertexCount = nGridCount * 4;
	int nIndexCount = nGridCount * 6;
	int nTriangleCount = nGridCount * 2;

	RHI::UVertex_V3F_C4B_T2F* pVerticesTemp = (RHI::UVertex_V3F_C4B_T2F*)m_pVertices;
	uint16* pIndicesTemp = m_pIndices;
	float nx = 0;
	float ny = m_GridSize.h * (float)m_nGridRow;
	float nxTemp = nx;
	float nyTemp = ny;
	CRect2f rcTexCoord;
	CColor4B color;
	const UGridPaletteNode* pPaletteNode = nullptr;
	int nGridIndex = 0;
	for ( int r = 0; r < m_nGridRow; r++ )
	{
		for ( int c = 0; c < m_nGridCol; c++ )
		{
			if ( (*pGrids) >= 0 && (*pGrids) < nPaletteSize )
			{
				pPaletteNode = &pPalette[ (*pGrids) ];
				rcTexCoord = pPaletteNode->rcTexCoord;
				color = pPaletteNode->Color;
			}
			else
			{
				rcTexCoord = CRect2f( 0, 0, 0, 0 );
				color = CColor4B( 0, 0, 0, 0 );
			}

			pVerticesTemp[ 0 ].position = UVector3f( nxTemp, nyTemp, 0.0f );
			pVerticesTemp[ 1 ].position = UVector3f( nxTemp + m_GridSize.w, nyTemp, 0.0f );
			pVerticesTemp[ 2 ].position = UVector3f( nxTemp, nyTemp - m_GridSize.h, 0.0f );
			pVerticesTemp[ 3 ].position = UVector3f( nxTemp + m_GridSize.w, nyTemp - m_GridSize.h, 0.0f );
			pVerticesTemp[ 0 ].color = pVerticesTemp[ 1 ].color = pVerticesTemp[ 2 ].color = pVerticesTemp[ 3 ].color = color;
			RHI::RectBuildVertices_TexCoord( pVerticesTemp, rcTexCoord );
			RHI::BuildRectVertexIndices( pIndicesTemp, nGridIndex );

			pVerticesTemp += 4;
			pIndicesTemp += 6;
			pGrids++;
			nGridIndex++;

			nxTemp += m_GridSize.w;
		}

		nxTemp = nx;
		nyTemp -= m_GridSize.h;
	}

	DirectX::XMVector3TransformCoordStream(
		(DirectX::XMFLOAT3*)m_pVertices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ),
		(const DirectX::XMFLOAT3*)m_pVertices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ),
		nVertexCount,
		m_pTransform->GetMatrix()
	);

	Renderer::CTrianglesCommand::UTriangles triangles = Renderer::CTrianglesCommand::UTriangles(
		m_pVertices,
		m_pIndices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ),
		nVertexCount,
		nIndexCount,
		nTriangleCount );
	m_pRenderCommand->SetTriangles( triangles );
}


void iberbar::Paper2d::CGridTerrain::BuildGrids()
{
	if ( m_pVertices == nullptr || m_pIndices == nullptr )
		return;

	int nGridCount = m_nGridRow * m_nGridCol;
	int nVertexCount = nGridCount * 4;
	RHI::UVertex_V3F_C4B_T2F* pVerticesTemp = (RHI::UVertex_V3F_C4B_T2F*)m_pVertices;
	float nx = 0;
	float ny = m_GridSize.h * (float)m_nGridRow;
	float nxTemp = nx;
	float nyTemp = ny;
	for ( int c = 0; c < m_nGridRow; c++ )
	{
		for ( int r = 0; r < m_nGridCol; r++ )
		{
			pVerticesTemp[ 0 ].position = UVector3f( nxTemp, nyTemp, 0.0f );
			pVerticesTemp[ 1 ].position = UVector3f( nxTemp + m_GridSize.w, nyTemp, 0.0f );
			pVerticesTemp[ 2 ].position = UVector3f( nxTemp, nyTemp - m_GridSize.h, 0.0f );
			pVerticesTemp[ 3 ].position = UVector3f( nxTemp + m_GridSize.w, nyTemp - m_GridSize.h, 0.0f );

			pVerticesTemp += 4;

			nxTemp += m_GridSize.w;
		}

		nxTemp = nx;
		nyTemp -= m_GridSize.h;
	}

	DirectX::XMVector3TransformCoordStream(
		(DirectX::XMFLOAT3*)m_pVertices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ),
		(const DirectX::XMFLOAT3*)m_pVertices,
		sizeof( RHI::UVertex_V3F_C4B_T2F ),
		nVertexCount,
		m_pTransform->GetMatrix()
	);
}

