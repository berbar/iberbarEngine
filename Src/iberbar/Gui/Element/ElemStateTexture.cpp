


#include <iberbar/Gui/Element/ElemStateTexture.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Utility/RectClip2d.h>



iberbar::Gui::CElementStateTexture::CElementStateTexture()
	: CRenderElement()
	, m_Color( 1.0f, 1.0f, 1.0f, 1.0f )
	, m_UV( 0.0f, 0.0f, 1.0f, 1.0f )
	, m_MeshVertices()
	, m_MeshIndices()
	, m_pMaterial( nullptr )
	, m_RenderCommand()
{
	memset( m_MeshVertices, 0, sizeof( m_MeshVertices ) );
	memset( m_MeshIndices, 0, sizeof( m_MeshIndices ) );
}


iberbar::Gui::CElementStateTexture::CElementStateTexture(const CElementStateTexture& element )
	: CRenderElement( element )
	, m_Color( 1.0f, 1.0f, 1.0f, 1.0f )
	, m_UV( 0.0f, 0.0f, 1.0f, 1.0f )
	, m_MeshVertices()
	, m_MeshIndices()
	, m_pMaterial( nullptr )
	, m_RenderCommand()
{
	memset( m_MeshVertices, 0, sizeof( m_MeshVertices ) );
	memset( m_MeshIndices, 0, sizeof( m_MeshIndices ) );

	SetColor( element.m_Color );
	SetUV( element.m_UV );
	SetMaterial( element.m_pMaterial );
}


iberbar::Gui::CElementStateTexture::~CElementStateTexture()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pMaterial );
}


iberbar::Gui::CElementStateTexture* iberbar::Gui::CElementStateTexture::Clone() const
{
	return new CElementStateTexture( *this );
}


void iberbar::Gui::CElementStateTexture::Init()
{
	m_RenderCommand.SetTriangles( Renderer::CTrianglesCommand::UTriangles(
		m_MeshVertices,
		m_MeshIndices,
		sizeof( Renderer::UVertex_V3F_C4B_T2F ), 4, 6, 2
	) );
}


void iberbar::Gui::CElementStateTexture::SetZOrder( int nZOrder )
{
	CRenderElement::SetZOrder( nZOrder );

	m_RenderCommand.SetZOrder( nZOrder );
}


void iberbar::Gui::CElementStateTexture::UpdateRect()
{
	CRenderElement::UpdateRect();

	m_bDirtyMeshPositionOrUV = true;
}


void iberbar::Gui::CElementStateTexture::Render()
{
	if ( GetVisible() == false )
		return;
	if ( m_pMaterial == nullptr )
		return;

	if ( m_bDirtyMeshPositionOrUV == true )
	{
		m_bDirtyMeshPositionOrUV = false;

		const CRect2i* pViewport = CEngine::sGetInstance()->GetViewportState()->GetClipViewport();
		CRect2i rcBoundingFinal = m_pTransform->GetBounding();
		CRect2f rcTexCoordFinal = m_UV;

		if ( pViewport != nullptr )
		{
			if ( RectTestIntersection( &rcBoundingFinal, pViewport ) == false )
				return;
			RectClip2d( &rcTexCoordFinal, &rcBoundingFinal, pViewport );
		}

		if ( rcBoundingFinal.IsEmpty() == true )
		{
			m_bEmptyBoundingFinal = true;
		}

		Renderer::VerticesRectUpdatePosition( m_MeshVertices, rcBoundingFinal, 0 );
		Renderer::VerticesRectUpdateUV( m_MeshVertices, rcTexCoordFinal );
	}

	if ( m_bEmptyBoundingFinal == true )
	{
		return;
	}

	CEngine::sGetInstance()->GetRenderer()->AddCommand( &m_RenderCommand );

	CRenderElement::Render();
}


void iberbar::Gui::CElementStateTexture::SetColor( const CColor4F& Color )
{
	m_Color = Color;
	m_MeshVertices[ 0 ].color = Color;
	m_MeshVertices[ 1 ].color = Color;
	m_MeshVertices[ 2 ].color = Color;
	m_MeshVertices[ 3 ].color = Color;
}


void iberbar::Gui::CElementStateTexture::SetUV( const CRect2f& UV )
{
	m_UV = UV;
	m_bDirtyMeshPositionOrUV = true;
}


void iberbar::Gui::CElementStateTexture::SetMaterial( Renderer::CMaterial* pMaterial )
{
	if ( m_pMaterial == pMaterial )
		return;

	if ( m_pMaterial != nullptr )
		m_pMaterial->Release();

	m_pMaterial = pMaterial;

	if ( m_pMaterial != nullptr )
	{
		m_pMaterial->AddRef();
		m_RenderCommand.SetShaderVariableTables( m_pMaterial->GetShaderVariableTables() );
	}
	else
	{
		m_RenderCommand.SetShaderVariableTables( nullptr );
	}
}


