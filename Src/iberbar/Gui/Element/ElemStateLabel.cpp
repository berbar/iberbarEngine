
#include <iberbar/Gui/Element/ElemStateLabel.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/Renderer/Font.h>
#include <iberbar/Utility/String.h>

#include <codecvt>



iberbar::Gui::CElementStateLabel::CElementStateLabel( void )
	: CRenderElement()
	, m_pFont( nullptr )
	, m_BlendColorRate( BlendColorRate_Normal )
	, m_nAlignHorizental( UAlignHorizental::Left )
	, m_nAlignVertical( UAlignVertical::Top )
	, m_strText( L"" )
	, m_BlendColor()
{
}



iberbar::Gui::CElementStateLabel::CElementStateLabel( const CElementStateLabel& element )
	: CRenderElement( element )
	, m_pFont( element.m_pFont )
	, m_BlendColorRate( element.m_BlendColorRate )
	, m_nAlignHorizental( element.m_nAlignHorizental )
	, m_nAlignVertical( element.m_nAlignVertical )
	, m_strText( element.m_strText )
	, m_BlendColor( element.m_BlendColor )
{
	if ( m_pFont )
		m_pFont->AddRef();
}



iberbar::Gui::CElementStateLabel::~CElementStateLabel()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pFont );
}



void iberbar::Gui::CElementStateLabel::Refresh()
{
	m_BlendColor.Refresh();

	CRenderElement::Refresh();
}



void iberbar::Gui::CElementStateLabel::Update( float nElapsedTime )
{
	if ( GetVisible() == false )
		return;

	float lc_BlendColorRate = ( m_nState == (int)UWidgetState::Pressed ) ? BlendColorRate_Quick : m_BlendColorRate;
	m_BlendColor.Blend( m_nState, nElapsedTime, lc_BlendColorRate );

	CRenderElement::Update( nElapsedTime );
}



void iberbar::Gui::CElementStateLabel::Render()
{
	if ( GetVisible() == false )
		return;

	if ( m_pFont )
	{
		const CRect2i* pViewport = CEngine::sGetInstance()->GetViewportState()->GetClipViewport();
		CRect2i rcDst = GetBounding();
		UFontDrawTextOptions Options;
		Options.alignHorizental = m_nAlignHorizental;
		Options.alignVertical = m_nAlignVertical;
		Options.nWrapType = TextDraw::UFontDrawTextWorkBreak::NoBreak;
		CEngine::sGetInstance()->GetRendererSprite()->DrawText(
			m_nZOrder,
			m_pFont,
			pViewport,
			m_strText.c_str(),
			-1,
			&rcDst,
			m_BlendColor.currentColor,
			Options );
	}

	CRenderElement::Render();
}


void iberbar::Gui::CElementStateLabel::SetFont( Renderer::CFont* pFont )
{
	if ( m_pFont != pFont )
	{
		if ( m_pFont )
		{
			m_pFont->Release();
		}
		m_pFont = pFont;
		if ( m_pFont )
		{
			m_pFont->AddRef();
			m_pFont->LoadText( m_strText.c_str() );
		}
	}
}


void iberbar::Gui::CElementStateLabel::SetTextA( const char* strText )
{
	m_strText = Utf8ToUnicode( strText );
	if ( m_pFont )
	{
		m_pFont->LoadText( m_strText.c_str() );
	}
}


void iberbar::Gui::CElementStateLabel::SetTextW( const wchar_t* strText )
{
	m_strText = strText;
	if ( m_pFont )
	{
		m_pFont->LoadText( m_strText.c_str() );
	}
}


