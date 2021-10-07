
#include <iberbar/Gui/Widgets/ProgressBar.h>
#include <iberbar/Gui/RenderElement.h>


iberbar::Gui::CProgressBar::CProgressBar()
	: m_pRenderElementRef_Progress( nullptr )
	, m_nProgressValueMax( 100 )
	, m_nProgressValue( 100 )
{

}


iberbar::Gui::CProgressBar::CProgressBar( const CProgressBar& Other )
	: m_pRenderElementRef_Progress( nullptr )
	, m_nProgressValueMax( Other.m_nProgressValueMax )
	, m_nProgressValue( Other.m_nProgressValue )
{

}


iberbar::Gui::CProgressBar::~CProgressBar()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementRef_Progress );
}


void iberbar::Gui::CProgressBar::SetProgressRenderElementRef( CRenderElement* pElement )
{
	if ( m_pRenderElementRef_Progress == pElement )
		return;

	UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementRef_Progress );

	m_pRenderElementRef_Progress = pElement;

	if ( m_pRenderElementRef_Progress )
	{
		m_pRenderElementRef_Progress->AddRef();

		m_pRenderElementRef_Progress->SetPercentW( true );
		m_pRenderElementRef_Progress->SetPercentH( true );
	}
}


void iberbar::Gui::CProgressBar::UpdateProgressRenderElement()
{
	int nValueFix = tMin( m_nProgressValue, m_nProgressValueMax );
	nValueFix = tMax( 0, nValueFix );

	if ( m_pRenderElementRef_Progress )
	{
		int nSize = nValueFix * 100 / m_nProgressValueMax;
		m_pRenderElementRef_Progress->SetSize( nSize, 100 );
	}
}