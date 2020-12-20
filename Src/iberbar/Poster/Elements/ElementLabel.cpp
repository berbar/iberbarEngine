
#include <iberbar/Poster/Elements/ElementLabel.h>
#include <iberbar/Poster/Font.h>


iberbar::Poster::CElementLabel::CElementLabel()
	: m_font( nullptr )
	, m_text( U"" )
	, m_textOptions()
{
}

iberbar::Poster::CElementLabel::~CElementLabel()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_font );
}

void iberbar::Poster::CElementLabel::RenderSelf( CSurface* target )
{
	int maxLine = (m_textOptions.maxLine > 0) ? m_textOptions.maxLine : MAXINT32;
	UFontDrawTextOptions drawOptions;
	drawOptions.alignHorizental = m_textOptions.alignHorizental;
	drawOptions.alignVertical = m_textOptions.alignVertical;
	drawOptions.nWrapType = UFontDrawTextWorkBreak::BreakAll;
	if ( m_textOptions.maxLine == 1 )
	{
		RenderText_SingleLine( target, m_font, m_text.c_str(), -1, &m_bounding, m_textOptions.foregroundColor, drawOptions );
	}
	else
	{

	}
}

void iberbar::Poster::CElementLabel::SetFont( CFont* font )
{
	UNKNOWN_SAFE_RELEASE_NULL( m_font );
	m_font = font;
	if ( m_font != nullptr )
	{
		m_font->AddRef();
	}
}
