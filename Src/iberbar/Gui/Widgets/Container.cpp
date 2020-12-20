

#include <iberbar/Gui/Widgets/Container.h>
#include <iberbar/Utility/Input.h>



iberbar::Gui::CContainer::CContainer()
{
	m_bCanFocus = false;
}


iberbar::Gui::CContainer::CContainer( const CContainer& container )
	: CWidget( container )
{
	m_bCanFocus = false;
}


iberbar::Gui::CContainer* iberbar::Gui::CContainer::Clone() const
{
	return new CContainer( *this );
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CContainer::HandleMouse( const UMouseEventData* EventData )
{
	if ( IsVisible() == false ||
		IsEnable() == false )
		return UHandleMessageResult::Ignore;

	//--------------------------------------
	// handle child widgets
	//--------------------------------------
	if ( GetLastChild() == nullptr )
		return UHandleMessageResult::Ignore;

	// 2. Handle Mouse Message
	if ( m_bMouseInput == false )
		return UHandleMessageResult::Ignore;

	// 1. Handle focus control first
	if ( sGetFocus() != NULL &&
		sGetFocus()->IsEnable() &&
		sGetFocus()->IsVisible() &&
		sGetFocus()->GetParent() == this )
	{
		if ( sGetFocus()->HandleMouse( EventData ) == UHandleMessageResult::Succeed )
			return UHandleMessageResult::Succeed;
	}

	//  hit test
	CWidget* lc_pWidget = GetActiveWidgetAtPoint( EventData->MousePoint );
	if ( lc_pWidget )
	{
		if ( lc_pWidget->HandleMouse( EventData ) == UHandleMessageResult::Succeed )
			return UHandleMessageResult::Succeed;
	}
	else
	{
		// Mouse not over any controls in this dialog, if there was a control
		// which had focus it just lost it
		if ( EventData->nMouseEvent == UMouseEvent::LDown )
		{
			if ( sGetFocus() != NULL &&
				sGetFocus()->GetParent() == this )
			{
				sClearFocus();
			}
		}
	}


	if ( EventData->nMouseEvent == UMouseEvent::Move )
	{
		// If the mouse is still over the same control, nothing needs to be done
		if ( lc_pWidget != m_pWidgetMouseOver )
		{
			// Handle mouse leaving the old control
			if ( m_pWidgetMouseOver )
				m_pWidgetMouseOver->OnMouseLeave();

			// Handle mouse entering the new control
			m_pWidgetMouseOver = lc_pWidget;
			if ( lc_pWidget != NULL )
				m_pWidgetMouseOver->OnMouseEnter();
		}
	}

	return UHandleMessageResult::Ignore;
}


//
//void iberbar::Gui::CContainer::Refresh()
//{
//	CWidget::Refresh();
//
//	CWidget* pWidget = GetFirstChild();
//	while ( pWidget != nullptr )
//	{
//		pWidget->Refresh();
//		pWidget = pWidget->GetNextBrother();
//	}
//}
//
//
//void iberbar::Gui::CContainer::Update( float nElapsedTime )
//{
//	CWidget* pWidget = GetFirstChild();
//	while ( pWidget != nullptr )
//	{
//		pWidget->Update( nElapsedTime );
//		pWidget = pWidget->GetNextBrother();
//	}
//}
//
//
//void iberbar::Gui::CContainer::Render()
//{
//	if ( m_bNeedClip == true )
//	{
//		m_pEngine->GetViewportState()->AddViewport( m_pTransform->GetBounding() );
//	}
//
//	CWidget* pWidget = GetFirstChild();
//	while ( pWidget != nullptr )
//	{
//		pWidget->Render();
//		pWidget = pWidget->GetNextBrother();
//	}
//}


iberbar::Gui::CWidget* iberbar::Gui::CContainer::GetActiveWidgetAtPoint( const CPoint2i& point )
{
	CWidget* lc_pTemp = GetLastChild();
	while ( lc_pTemp )
	{
		if ( lc_pTemp->IsEnable() == true &&
			lc_pTemp->IsVisible() == true &&
			//lc_pTemp->GetCanFocus() == true &&
			lc_pTemp->HitTest( point ) == true )
		{
			return lc_pTemp;
		}

		lc_pTemp = lc_pTemp->GetPrevBrother();
	}

	return nullptr;
}
