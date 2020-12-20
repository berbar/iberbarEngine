


#include <iberbar/Gui/Element/ElemStateTexture.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/RHI/Texture.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/RHI/VertexBuilder.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/Utility/RectClip2d.h>



iberbar::Gui::CElementStateTexture::CElementStateTexture()
	: CRenderElement()
	, m_BlendColorRate()
	, m_BlendColor()
	, m_ppTextures()
	, m_nRenderUV()
{
	memset( m_ppTextures, 0, sizeof( m_ppTextures ) );
	memset( m_nRenderUV, 0, sizeof( m_nRenderUV ) );
	m_BlendColorRate[ (int)UWidgetState::Normal ]    = BlendColorRate_Normal;
	m_BlendColorRate[ (int)UWidgetState::MouseOver ] = BlendColorRate_Normal;
	m_BlendColorRate[ (int)UWidgetState::Pressed ]   = BlendColorRate_Quick;
	m_BlendColorRate[ (int)UWidgetState::Focus ]     = BlendColorRate_Normal;
	m_BlendColorRate[ (int)UWidgetState::Hidden ]    = BlendColorRate_Normal;
	m_BlendColorRate[ (int)UWidgetState::Disabled ]  = BlendColorRate_Normal;
}


iberbar::Gui::CElementStateTexture::CElementStateTexture(const CElementStateTexture& element )
	: CRenderElement( element )
	, m_BlendColorRate()
	, m_BlendColor( element.m_BlendColor )
	, m_ppTextures()
	, m_nRenderUV()
{
	for ( int i = 0; i < uWidgetStateCount; i ++ )
	{
		m_ppTextures[ i ] = element.m_ppTextures[ i ];
		if ( m_ppTextures[ i ] )
			m_ppTextures[ i ]->AddRef();

		m_nRenderUV[ i ] = element.m_nRenderUV[ i ];
	}

	memcpy_s(
		m_BlendColorRate, sizeof( m_BlendColorRate ),
		element.m_BlendColorRate, sizeof( m_BlendColorRate ) );
}


iberbar::Gui::CElementStateTexture::~CElementStateTexture()
{
	for ( int i = 0; i < uWidgetStateCount; i ++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_ppTextures[ i ] );
	}
}


iberbar::Gui::CElementStateTexture* iberbar::Gui::CElementStateTexture::Clone() const
{
	return new CElementStateTexture( *this );
}


void iberbar::Gui::CElementStateTexture::Init()
{
	CRenderElement::Init();

	m_pShaderVariableTable->SetBool( RHI::UShaderType::Pixel, Renderer::s_strShaderVarName_UseTexture, true );
}


void iberbar::Gui::CElementStateTexture::Refresh()
{
	m_BlendColor.Refresh();

	CRenderElement::Refresh();
}


void iberbar::Gui::CElementStateTexture::Update( float nElapsedTime )
{
	if ( GetVisible() == false )
		return;

	m_BlendColor.Blend( m_nState, nElapsedTime, m_BlendColorRate[ (int)m_nState ] );

	CRenderElement::Update( nElapsedTime );
}


void iberbar::Gui::CElementStateTexture::Render()
{
	if ( GetVisible() == false )
		return;
	if ( m_pShaderState == nullptr || m_pShaderVariableTable == nullptr )
		return;

	int nIndex = m_nState;

	const CRect2i* pViewport = CEngine::sGetInstance()->GetViewportState()->GetClipViewport();
	CRect2i rectDst = m_pTransform->GetBounding();
	CRect2f rectTexCoord = m_nRenderUV[ nIndex ];

	if ( pViewport != nullptr )
	{
		if ( RectTestIntersection( &rectDst, pViewport ) == false )
			return;
		RectClip2d( &rectTexCoord, &rectDst, pViewport );
	}

	if ( rectDst.IsEmpty() == true )
		return;

	m_pShaderVariableTable->SetSampler( Renderer::s_strShaderVarName_TextureSampler, m_ppTextures[ nIndex ], RHI::UTextureSamplerState() );

	CEngine::sGetInstance()->GetRendererSprite()->DrawRectRhwEx(
		m_nZOrder,
		CRect2f( (float)rectDst.l, (float)rectDst.t, (float)rectDst.r, (float)rectDst.b ),
		m_BlendColor.currentColor,
		m_nRenderUV[ nIndex ],
		m_pShaderState,
		m_pShaderVariableTable
	);

	CRenderElement::Render();
}


void iberbar::Gui::CElementStateTexture::SetTexture( int state, RHI::ITexture* pTexture )
{
	if ( state == -1 )
	{
		for ( int i = 0; i < uWidgetStateCount; i ++ )
		{
			UNKNOWN_SAFE_RELEASE_NULL( m_ppTextures[ i ] );
			m_ppTextures[ i ] = pTexture;
			UNKNOWN_SAFE_ADDREF( m_ppTextures[ i ] );
		}
	}
	else if ( state < uWidgetStateCount && state >= 0 )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_ppTextures[ state ] );
		m_ppTextures[ state ] = pTexture;
		UNKNOWN_SAFE_ADDREF( m_ppTextures[ state ] );
	}
}


void iberbar::Gui::CElementStateTexture::SetUV( int state, const CRect2f& uv )
{
	if ( state == -1 )
	{
		for ( int i = 0; i < uWidgetStateCount; i ++ )
		{
			m_nRenderUV[ i ] = uv;
		}
	}
	else if ( state < uWidgetStateCount && state >= 0 )
	{
		m_nRenderUV[ state ] = uv;
	}
}


void iberbar::Gui::CElementStateTexture::SetColorBlendRate( int state, float nRate )
{
	if ( state == -1 )
	{
		for ( int i = 0; i < uWidgetStateCount; i ++ )
		{
			m_BlendColorRate[ i ] = nRate;
		}
	}
	else if ( state < uWidgetStateCount && state >= 0 )
	{
		m_BlendColorRate[ state ] = nRate;
	}
}


