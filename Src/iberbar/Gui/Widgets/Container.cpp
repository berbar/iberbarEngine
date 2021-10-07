

#include <iberbar/Gui/Widgets/Container.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Utility/Input.h>
#include <iberbar/Utility/String.h>



iberbar::Gui::CContainer::CContainer()
	: m_Widgets()
{
	m_bCanFocus = false;
}


iberbar::Gui::CContainer::CContainer( const CContainer& container )
	: CWidget( container )
	, m_Widgets()
{
	m_bCanFocus = false;
}


iberbar::Gui::CContainer::~CContainer()
{
	RemoveWidgetsAll();
}


iberbar::Gui::CContainer* iberbar::Gui::CContainer::Clone() const
{
	return new CContainer( *this );
}


void iberbar::Gui::CContainer::SetDialog( CDialog* pDialog )
{
	CWidget::SetDialog( pDialog );

	for ( CWidget* pWidget : m_Widgets )
	{
		pWidget->SetDialog( pDialog );
	}
}


void iberbar::Gui::CContainer::AddWidget( CWidget* pWidget )
{
	if ( pWidget == nullptr )
		return;

	if ( pWidget->GetDialog() != nullptr )
		return;

	for ( auto& pWidgetTemp : m_Widgets )
	{
		if ( pWidgetTemp == pWidget )
			return;
	}

	m_Widgets.push_back( pWidget );
	pWidget->SetWidgetParent( this );
	pWidget->AddRef();
}


iberbar::Gui::CWidget* iberbar::Gui::CContainer::FindWidget( const char* strId )
{
	if ( StringIsNullOrEmpty( strId ) == true )
		return nullptr;

	for ( auto& pWidget : m_Widgets )
	{
		if ( strcmp( pWidget->GetId().c_str(), strId ) == 0 )
			return pWidget;
	}

	return nullptr;
}


void iberbar::Gui::CContainer::RemoveWidget( CWidget* pWidget )
{
	if ( pWidget == nullptr )
		return;

	auto iter = m_Widgets.begin();
	auto end = m_Widgets.end();
	for ( ; iter != end; iter++ )
	{
		if ( ( *iter ) == pWidget )
		{
			if ( CEngine::sGetInstance()->GetFocus() == pWidget )
			{
				CEngine::sGetInstance()->ClearFocus( true );
			}

			pWidget->SetWidgetParent( nullptr );
			pWidget->Release();

			m_Widgets.erase( iter );

			return;
		}
	}
}


void iberbar::Gui::CContainer::RemoveWidgetsAll()
{
	if ( m_Widgets.empty() == false )
	{
		auto iter = m_Widgets.begin();
		auto end = m_Widgets.end();
		for ( ; iter != end; iter++ )
		{
			if ( ( *iter ) == CEngine::sGetInstance()->GetFocus() )
			{
				CEngine::sGetInstance()->ClearFocus( false );
			}

			( *iter )->SetWidgetParent( nullptr );
			( *iter )->Release();
			( *iter ) = nullptr;
		}
		m_Widgets.clear();
	}
}


void iberbar::Gui::CContainer::ForeachWidgets( std::function<void( CWidget* )> Func, int nDepth )
{
	if ( m_Widgets.empty() )
		return;

	if ( nDepth == 0 )
		return;

	for ( CWidget* pWidget : m_Widgets )
	{
		Func( pWidget );
		if ( pWidget->IsContainer() )
		{
			( (CContainer*)pWidget )->ForeachWidgets( Func, nDepth - 1 );
		}
	}
}

