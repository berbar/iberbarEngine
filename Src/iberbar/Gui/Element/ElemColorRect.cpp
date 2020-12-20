
#include <iberbar/Gui/Element/ElemColorRect.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Utility/RectClip2d.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/RHI/VertexBuilder.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/RendererSprite.h>




iberbar::Gui::CElementColorRect::CElementColorRect()
	: CRenderElement()
	, m_BlendColorRate( BlendColorRate_Normal )
	, m_BlendColor()
{
}


iberbar::Gui::CElementColorRect::CElementColorRect( const CElementColorRect& element )
	: CRenderElement( element )
	, m_BlendColorRate( element.m_BlendColorRate )
	, m_BlendColor( element.m_BlendColor )
{
}


iberbar::Gui::CElementColorRect::~CElementColorRect()
{
}


void iberbar::Gui::CElementColorRect::Init()
{
	CRenderElement::Init();

	m_pShaderVariableTable->SetBool( RHI::UShaderType::Pixel, Renderer::s_strShaderVarName_UseTexture, false );
}


void iberbar::Gui::CElementColorRect::Refresh()
{
	m_BlendColor.Refresh();

	CRenderElement::Refresh();
}


void iberbar::Gui::CElementColorRect::Update( float nElapsedTime )
{
	if ( GetVisible() == false )
		return;

	float lc_BlendColorRate = ( m_nState == (int)UWidgetState::Pressed ) ? BlendColorRate_Quick : m_BlendColorRate;
	m_BlendColor.Blend( m_nState, nElapsedTime, lc_BlendColorRate );

	CRenderElement::Update( nElapsedTime );
}


void iberbar::Gui::CElementColorRect::Render()
{
	if ( GetVisible() == false )
		return;
	if ( m_pShaderState == nullptr || m_pShaderVariableTable == nullptr )
		return;

	const CRect2i* pViewport = CEngine::sGetInstance()->GetViewportState()->GetClipViewport();
	CRect2i rectDst = m_pTransform->GetBounding();

	if ( pViewport != nullptr )
	{
		if ( RectTestIntersection( &rectDst, pViewport ) == false )
			return;
		RectClip2d( &rectDst, pViewport );
	}

	if ( rectDst.IsEmpty() == true )
		return;

	CEngine::sGetInstance()->GetRendererSprite()->DrawRectRhwEx(
		m_nZOrder,
		CRect2f( (float)rectDst.l, (float)rectDst.t, (float)rectDst.r, (float)rectDst.b ),
		m_BlendColor.currentColor,
		CRect2f(),
		m_pShaderState,
		m_pShaderVariableTable
	);

	CRenderElement::Render();
}


