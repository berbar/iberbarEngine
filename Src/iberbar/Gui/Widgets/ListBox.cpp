
#include <iberbar/Gui/Widgets/ListBox.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Utility/Input.h>
#include <iberbar/Utility/Math.h>




iberbar::Gui::CListBoxNoData::CListBoxNoData( void )
	: m_nSelType( UListBoxSelectType::Single )
	, m_nItemMouseOver( -1 )
	, m_nItemPressed( -1 )

	, m_pRenderElementItemsRoot( new CRenderElement() )

	, m_bDragView( false )
	, m_bPressed( false )
	, m_bIsDragging( false )
	, m_nDragX( 0 )
	, m_nDragY( 0 )
	, m_nDragScrollSense( 10 )

	, m_nItemDirection( UListBoxDirection::Vertical )
	, m_nItemWidth( 0 )
	, m_nItemHeight( 0 )
	, m_nItemSpacing( 0 )
	, m_nItemVirtualSize( 0 )
	, m_nShowX( 0 )
	, m_nShowY( 0 )
{
	m_bCanFocus = true;
	SetNeedClip( true );
	m_pRenderElementItemsRoot->GetTransform()->SetParentTransform( this->GetTransform() );
}



iberbar::Gui::CListBoxNoData::CListBoxNoData( const CListBoxNoData& listBox )
	: CWidget( listBox )

	, m_nSelType( listBox.m_nSelType )
	, m_nItemMouseOver( -1 )
	, m_nItemPressed( -1 )

	, m_pRenderElementItemsRoot( new CRenderElement() )

	, m_bDragView( false )
	, m_bPressed( false )
	, m_bIsDragging( false )
	, m_nDragX( 0 )
	, m_nDragY( 0 )
	, m_nDragScrollSense( 10 )

	, m_nItemDirection( listBox.m_nItemDirection )
	, m_nItemWidth( listBox.m_nItemWidth )
	, m_nItemHeight( listBox.m_nItemHeight )
	, m_nItemSpacing( listBox.m_nItemSpacing )
	, m_nItemVirtualSize( listBox.m_nItemVirtualSize )
	, m_nShowX( 0 )
	, m_nShowY( 0 )
{
	m_bCanFocus = true;
	SetNeedClip( true );
	m_pRenderElementItemsRoot->GetTransform()->SetParentTransform( this->GetTransform() );
}


iberbar::Gui::CListBoxNoData::~CListBoxNoData()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pRenderElementItemsRoot );
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CListBoxNoData::HandleMouse( const UMouseEventData* pEvent )
{
	if ( IsVisible() == false ||
		IsEnable() == false )
		return UHandleMessageResult::Ignore;

	if ( m_pRenderElementItemsRoot == nullptr )
		return UHandleMessageResult::Ignore;

	int nBoxW = GetBounding().Width();
	int nBoxH = GetBounding().Height();
	auto pPanelTransform = m_pRenderElementItemsRoot->GetTransform();

	switch ( pEvent->nMouseEvent )
	{
		case UMouseEvent::MouseWheel:
		{
			if ( GetMouseOver() == true )
			{
				int nScrollAmount = pEvent->nMouseWheel;

				int lc_xNew = pPanelTransform->GetPositionX();
				int lc_yNew = pPanelTransform->GetPositionY();
				int lc_xMax, lc_yMax;
				int lc_nItemsLength = m_nItemVirtualSize * GetItemCount();

				if ( IsVerticalList() )
				{
					lc_yNew += nScrollAmount * m_nDragScrollSense;
					lc_xMax = (nBoxW >= m_nItemWidth) ? 0 : (m_nItemWidth - nBoxW);
					lc_yMax = (nBoxH >= lc_nItemsLength) ? 0 : (lc_nItemsLength - nBoxH);
				}
				else
				{
					lc_xNew += nScrollAmount * m_nDragScrollSense;
					lc_xMax = (nBoxW >= lc_nItemsLength) ? 0 : (lc_nItemsLength - nBoxW);
					lc_yMax = (nBoxH >= m_nItemHeight) ? 0 : (m_nItemHeight - nBoxH);
				}

				lc_xNew = tMax( lc_xNew, -lc_xMax );
				lc_yNew = tMax( lc_yNew, -lc_yMax );

				lc_xNew = tMin( lc_xNew, 0 );
				lc_yNew = tMin( lc_yNew, 0 );

				pPanelTransform->SetPosition( CPoint2i( lc_xNew, lc_yNew ) );
				m_nShowX = -pPanelTransform->GetPositionX();
				m_nShowY = -pPanelTransform->GetPositionY();

				if ( HitTest( pEvent->MousePoint ) )
				{
					SetItemMouseOver( GetItemAtPoint( pEvent->MousePoint ) );
				}

				return UHandleMessageResult::Succeed;
			}
		}
		break;

		case UMouseEvent::Move:
		{
			if ( m_bDragView == true )
			{
				if ( m_bPressed == true )
					m_bIsDragging = true;

				if ( m_bIsDragging == true )
				{
					int lc_cx = pEvent->MousePoint.x - m_nDragX;
					int lc_cy = pEvent->MousePoint.y - m_nDragY;

					int lc_xNew = pPanelTransform->GetPositionX() + lc_cx;
					int lc_yNew = pPanelTransform->GetPositionY() + lc_cy;
					int lc_xMax, lc_yMax;
					int lc_nItemsLength = m_nItemVirtualSize * GetItemCount();
					if ( IsVerticalList() )
					{
						lc_xMax = (nBoxW >= m_nItemWidth) ? 0 : (m_nItemWidth - nBoxW);
						lc_yMax = (nBoxH >= lc_nItemsLength) ? 0 : (lc_nItemsLength - nBoxH);
					}
					else
					{
						lc_xMax = (nBoxW >= lc_nItemsLength) ? 0 : (lc_nItemsLength - nBoxW);
						lc_yMax = (nBoxH >= m_nItemHeight) ? 0 : (m_nItemHeight - nBoxH);
					}

					lc_xNew = tMax( lc_xNew, -lc_xMax );
					lc_yNew = tMax( lc_yNew, -lc_yMax );

					lc_xNew = tMin( lc_xNew, 0 );
					lc_yNew = tMin( lc_yNew, 0 );

					pPanelTransform->SetPosition( CPoint2i( lc_xNew, lc_yNew ) );
					m_nShowX = -pPanelTransform->GetPositionX();
					m_nShowY = -pPanelTransform->GetPositionY();

					m_nDragX = pEvent->MousePoint.x;
					m_nDragY = pEvent->MousePoint.y;

					return UHandleMessageResult::Succeed;
				}
			}

			if ( HitTest( pEvent->MousePoint ) )
			{
				SetItemMouseOver( GetItemAtPoint( pEvent->MousePoint ) );
			}
		}
		break;

		case UMouseEvent::LDown:
		{
			if ( HitTest( pEvent->MousePoint ) )
			{
				m_bPressed = true;

				SetItemPressed( GetItemAtPoint( pEvent->MousePoint ) );

				m_nDragX = pEvent->MousePoint.x;
				m_nDragY = pEvent->MousePoint.y;

				if ( m_bFocus == false )
					RequestFocus();

				return UHandleMessageResult::Succeed;
			}
		}
		break;

		case UMouseEvent::LUp:
		{
			if ( HitTest( pEvent->MousePoint ) )
			{
				if ( m_bPressed == true && m_bIsDragging == false )
				{
					int nHit = GetItemAtPoint( pEvent->MousePoint );
					if ( nHit >= 0 && nHit == m_nItemPressed )
					{
						SendEvent( BaseEvent::nClicked, nHit, 0 );
						if ( GetSelectType() == UListBoxSelectType::Single )
							SetSelectInternal( nHit, true, true );
						else if ( GetSelectType() == UListBoxSelectType::Multiple )
							SetSelectInternal( nHit, !IsSelect( nHit ), true );
					}
				}
			}

			m_bPressed = false;
			m_bIsDragging = false;

			return UHandleMessageResult::Succeed;
		}
		break;

		case UMouseEvent::LDoubleClick:
		{
			if ( HitTest( pEvent->MousePoint ) )
			{
				int nHit = GetItemAtPoint( pEvent->MousePoint );
				SendEvent( BaseEvent::nClicked, nHit, 0 );
				SetSelectInternal( nHit, true, true );
				return UHandleMessageResult::Succeed;
			}
		}
		break;

		default:
			break;
	}

	return UHandleMessageResult::Ignore;
}


iberbar::Gui::UHandleMessageResult iberbar::Gui::CListBoxNoData::HandleKeyboard( const UKeyboardEventData* pEvent )
{
	return UHandleMessageResult::Ignore;
}


void iberbar::Gui::CListBoxNoData::OnMouseLeave()
{
	SetItemMouseOver( -1 );
	SetItemPressed( -1 );

	CWidget::OnMouseLeave();
}


void iberbar::Gui::CListBoxNoData::Update( float nDelta )
{
	CWidget::Update( nDelta );

	m_pRenderElementItemsRoot->Update( nDelta );
}


void iberbar::Gui::CListBoxNoData::Render()
{
	bool bPopViewport = false;
	if ( m_bNeedClip == true )
	{
		CEngine::sGetInstance()->GetViewportState()->AddViewport( m_pTransform->GetBounding() );
		bPopViewport = true;
	}

	if ( m_pRenderElementDefault )
		m_pRenderElementDefault->Render();

	m_pRenderElementItemsRoot->Render();

	if ( bPopViewport == true )
	{
		CEngine::sGetInstance()->GetViewportState()->PopViewport();
	}
}


int iberbar::Gui::CListBoxNoData::GetItemAtPoint( const CPoint2i& point ) const
{
	if ( HitTest( point ) == false )
		return -1;

	int nHit;
	if ( IsVerticalList() )
		nHit = point.y - GetBounding().t + m_nShowY;
	else
		nHit = point.x - GetBounding().l + m_nShowX;

	int nIndex = nHit / m_nItemVirtualSize;
	int nTop = nIndex * m_nItemVirtualSize;
	int nBottom = nTop + m_nItemVirtualSize;
	nTop += m_nItemSpacing;
	nBottom -= m_nItemSpacing;
	if ( nHit < nTop || nHit > nBottom )
		nIndex = -1;

	return nIndex;
}



void iberbar::Gui::CListBoxNoData::SetItemDirection( UListBoxDirection nDirection )
{
	if ( m_nItemDirection != nDirection )
	{
		m_nItemDirection = nDirection;
		ResizeItemElements();
	}
}



void iberbar::Gui::CListBoxNoData::SetItemSize( int width, int height )
{
	if ( m_nItemWidth != width || m_nItemHeight != height )
	{
		m_nItemWidth = width;
		m_nItemHeight = height;
		ResizeItemElements();
	}
}



void iberbar::Gui::CListBoxNoData::SetItemMargin( int spacing )
{
	if ( spacing != m_nItemSpacing )
	{
		m_nItemSpacing = spacing;
		ResizeItemElements();
	}
}



void iberbar::Gui::CListBoxNoData::SetSelectType( UListBoxSelectType nType )
{
	if ( m_nSelType != nType )
	{
		m_nSelType = nType;

		if ( m_nSelType == UListBoxSelectType::None )
		{
			ClearSelect();
		}
		else
		{
			int nSelectFirst = GetSelect();
			if ( nSelectFirst >= 0 )
			{
				SetSelectInternal( nSelectFirst, true, false );
			}
		}
	}
}


void iberbar::Gui::CListBoxNoData::ResizeItemElements( int nOffset, int nCount )
{
	int nX;
	int nY;

	m_nItemVirtualSize = ((IsVerticalList()) ? m_nItemHeight : m_nItemWidth) + m_nItemSpacing * 2;
	int s = nCount < 0 ? GetItemCount() : tMin( nCount + nOffset, GetItemCount() );
	CRenderElement* pElement = nullptr;

	if ( IsVerticalList() == true )
	{
		nX = 0;
		nY = nOffset * m_nItemVirtualSize;

		for ( int i = nOffset; i < s; i++ )
		{
			pElement = m_pRenderElementItemsRoot->GetElement( i );
			if ( pElement == nullptr )
				continue;
			pElement->SetPosition( nX, nY + m_nItemSpacing );
			pElement->SetSize( m_nItemWidth, m_nItemHeight );
			nY += m_nItemVirtualSize;
		}
	}
	else
	{
		nX = nOffset * m_nItemVirtualSize;
		nY = 0;

		for ( int i = nOffset; i < s; i++ )
		{
			pElement = m_pRenderElementItemsRoot->GetElement( i );
			if ( pElement == nullptr )
				continue;
			pElement->SetPosition( nX + m_nItemSpacing, nY );
			pElement->SetSize( m_nItemWidth, m_nItemHeight );
			nX += m_nItemVirtualSize;
		}
	}
}


void iberbar::Gui::CListBoxNoData::SetItemMouseOver( int nIndex )
{
	if ( m_nItemMouseOver == nIndex )
		return;

	if ( m_nItemMouseOver >= 0 )
	{
		auto pElement = m_pRenderElementItemsRoot->GetElement( m_nItemMouseOver );
		if ( pElement )
		{
			if ( IsSelect( m_nItemMouseOver ) == true )
			{
			}
			else
			{
				pElement->SetState( (int)UWidgetState::Normal );
			}
		}
	}

	m_nItemMouseOver = nIndex;

	if ( m_nItemMouseOver >= 0 )
	{
		auto pElement = m_pRenderElementItemsRoot->GetElement( m_nItemMouseOver );
		if ( pElement )
		{
			if ( IsSelect( nIndex ) == true )
			{

			}
			else
			{
				pElement->SetState( (int)UWidgetState::MouseOver );
			}
		}
	}
}


void iberbar::Gui::CListBoxNoData::SetItemPressed( int nIndex )
{
	if ( m_nItemPressed == nIndex )
		return;

	if ( m_nItemPressed >= 0 )
	{
		auto pElement = m_pRenderElementItemsRoot->GetElement( m_nItemPressed );
		if ( pElement )
		{
			if ( IsSelect( m_nItemPressed ) == true )
			{
			}
			else
			{
				pElement->SetState( (int)UWidgetState::Normal );
			}
		}
	}

	m_nItemPressed = nIndex;

	if ( m_nItemPressed >= 0 )
	{
		auto pElement = m_pRenderElementItemsRoot->GetElement( m_nItemPressed );
		if ( pElement )
		{
			if ( IsSelect( m_nItemPressed ) == true )
			{
			}
			else
			{
				pElement->SetState( (int)UWidgetState::Pressed );
			}
		}
	}
}



void iberbar::Gui::CListBoxNoData::SetItemFocus( int nIndex, bool bFocus )
{
	if ( nIndex >= 0 )
	{
		auto pElement = m_pRenderElementItemsRoot->GetElement( nIndex );
		if ( pElement )
		{
			if ( bFocus == true )
				pElement->SetState( (int)UWidgetState::Focus );
			else
				pElement->SetState( (int)UWidgetState::Normal );
		}
	}
}
