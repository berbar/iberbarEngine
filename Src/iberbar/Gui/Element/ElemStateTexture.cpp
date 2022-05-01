


#include <iberbar/Gui/Element/ElemStateTexture.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/Mesh.h>
#include <iberbar/Renderer/MeshRendererComponent.h>
#include <iberbar/Utility/RectClip2d.h>



iberbar::Gui::CElementStateTexture::CElementStateTexture()
	: CRenderElement()
	, m_Color( 1.0f, 1.0f, 1.0f, 1.0f )
	, m_UV( 0.0f, 0.0f, 1.0f, 1.0f )
	, m_bDirtyMeshPositionOrUV( true )
	, m_bEmptyBoundingFinal( true )
	, m_pMesh( new Renderer::CMeshForUI_1() )
	, m_pMeshRendererComponent( new Renderer::CMeshRendererComponent() )
	, m_pMaterial( nullptr )
{
	SetColor( m_Color );
	//SetUV( m_UV );

	m_pMeshRendererComponent->SetMesh( m_pMesh );
}


iberbar::Gui::CElementStateTexture::CElementStateTexture(const CElementStateTexture& element )
	: CRenderElement( element )
	, m_Color( 1.0f, 1.0f, 1.0f, 1.0f )
	, m_UV( 0.0f, 0.0f, 1.0f, 1.0f )
	, m_bDirtyMeshPositionOrUV( true )
	, m_bEmptyBoundingFinal( true )
	, m_pMesh( new Renderer::CMeshForUI_1() )
	, m_pMeshRendererComponent( new Renderer::CMeshRendererComponent() )
	, m_pMaterial( nullptr )
{
	SetColor( element.m_Color );
	SetUV( element.m_UV );
	SetMaterial( element.m_pMaterial );

	m_pMeshRendererComponent->SetMesh( m_pMesh );
}


iberbar::Gui::CElementStateTexture::~CElementStateTexture()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pMesh );
	SAFE_DELETE( m_pMeshRendererComponent );
	UNKNOWN_SAFE_RELEASE_NULL( m_pMaterial );
}


iberbar::Gui::CElementStateTexture* iberbar::Gui::CElementStateTexture::Clone() const
{
	return new CElementStateTexture( *this );
}


void iberbar::Gui::CElementStateTexture::Init()
{
	//m_RenderCommand.SetTriangles( Renderer::CTrianglesCommand::UTriangles(
	//	m_MeshVertices,
	//	m_MeshIndices,
	//	sizeof( Renderer::UVertex_V3F_C4B_T2F ), 4, 6, 2
	//) );
}


void iberbar::Gui::CElementStateTexture::SetZOrder( int nZOrder )
{
	CRenderElement::SetZOrder( nZOrder );

	m_pMeshRendererComponent->SetZOrder( nZOrder );
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
		else
		{
			m_bEmptyBoundingFinal = false;
		}

		rcBoundingFinal.t = - rcBoundingFinal.t;
		rcBoundingFinal.b = - rcBoundingFinal.b;
		Renderer::VerticesRectUpdatePosition( m_pMesh->GetBufferPositions(), rcBoundingFinal, 0 );
		Renderer::VerticesRectUpdateUV( m_pMesh->GetBufferTexcoords(0), rcTexCoordFinal );
	}

	if ( m_bEmptyBoundingFinal == true )
	{
		return;
	}

	m_pMeshRendererComponent->Render();

	CRenderElement::Render();
}


void iberbar::Gui::CElementStateTexture::SetColor( const CColor4F& Color )
{
	m_Color = Color;
	CColor4F* pBuffer = m_pMesh->GetBufferColors();
	pBuffer[ 0 ] = Color;
	pBuffer[ 1 ] = Color;
	pBuffer[ 2 ] = Color;
	pBuffer[ 3 ] = Color;
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
		m_pMeshRendererComponent->SetMaterial( m_pMaterial );
	}
	else
	{
		m_pMeshRendererComponent->SetMaterial( nullptr );
	}
}


