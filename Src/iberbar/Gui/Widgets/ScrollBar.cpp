

#include "..\BaseTypes.h"
#include "..\WidgetVisit.h"

#include "ScrollBar.h"

#include "iberbar\Base\Math.h"



//-------------------------------------
//  ScrollBar
//-------------------------------------

const float iberbar::CGuiScrollBar::Default_ArrowClick_Delay = 0.330f;
const float iberbar::CGuiScrollBar::Default_ArrowClick_Repeat = 0.050f;

//--------------------------------------------------------------------------------------
iberbar::CGuiScrollBar::CGuiScrollBar( void )
	: m_bVertical( true )
	, m_bShowThumb( true )

	, m_nStyle( 0 )
	, m_nMinThumbSize( Default_MinThumbSize )
	, m_nArrowClickDelay( Default_ArrowClick_Delay )
	, m_nArrowClickRepeat( Default_ArrowClick_Repeat )

	, m_bDrag( false )
	, m_nPosition( 0 )
	, m_nPageSize( 1 )
	, m_nStart( 0 )
	, m_nEnd( 1 )
	, m_ArrowState( CLEAR )
{
	RectSetEmpty( &m_rcButtonUp );
	RectSetEmpty( &m_rcButtonDown );
	RectSetEmpty( &m_rcTrack );
	RectSetEmpty( &m_rcThumb );
}


//--------------------------------------------------------------------------------------
iberbar::CGuiScrollBar::CGuiScrollBar( const CGuiScrollBar& copySrc )
	: CGuiWidget( copySrc )

	, m_bVertical( copySrc.m_bVertical )
	, m_bShowThumb( true )

	, m_nStyle( copySrc.m_nStyle )
	, m_nMinThumbSize( copySrc.m_nMinThumbSize )
	, m_nArrowClickDelay( copySrc.m_nArrowClickDelay )
	, m_nArrowClickRepeat( copySrc.m_nArrowClickRepeat )

	, m_bDrag( false )
	, m_nPosition( copySrc.m_nPosition )
	, m_nPageSize( copySrc.m_nPageSize )
	, m_nStart( copySrc.m_nStart )
	, m_nEnd( copySrc.m_nEnd )
	, m_ArrowState( CLEAR )
{
}


//--------------------------------------------------------------------------------------
void iberbar::CGuiScrollBar::updateRect()
{
	// Make the buttons square
	if ( m_bVertical == true )
	{
		m_rcButtonUp.left   = getBounding().left;
		m_rcButtonUp.right  = getBounding().right;
		m_rcButtonUp.top    = getBounding().top;
		m_rcButtonUp.bottom = getBounding().top + ( getBounding().right-getBounding().left );

		m_rcButtonDown.left   = getBounding().left;
		m_rcButtonDown.right  = getBounding().right;
		m_rcButtonDown.top    = getBounding().bottom - ( getBounding().right-getBounding().left );
		m_rcButtonDown.bottom = getBounding().bottom;

		m_rcTrack.left   = getBounding().left;
		m_rcTrack.right  = getBounding().right;
		m_rcTrack.top    = m_rcButtonUp.bottom;
		m_rcTrack.bottom = m_rcButtonDown.top;

		m_rcThumb.left = getBounding().left;
		m_rcThumb.right = getBounding().right;
	}
	else
	{
		m_rcButtonUp.left   = getBounding().left;
		m_rcButtonUp.right  = ( getBounding().bottom-getBounding().top );
		m_rcButtonUp.top    = getBounding().top;
		m_rcButtonUp.bottom = getBounding().bottom;

		m_rcButtonDown.left   = getBounding().right - ( getBounding().bottom-getBounding().top );
		m_rcButtonDown.right  = getBounding().right;
		m_rcButtonDown.top    = getBounding().top;
		m_rcButtonDown.bottom = getBounding().bottom;

		m_rcTrack.left   = m_rcButtonUp.right;
		m_rcTrack.right  = m_rcButtonDown.left;
		m_rcTrack.top    = getBounding().top;
		m_rcTrack.bottom = getBounding().bottom;

		m_rcThumb.top = getBounding().top;
		m_rcThumb.bottom = getBounding().bottom;
	}

	updateThumbRect();

	CGuiElement* lc_pElement = NULL;

// 	lc_pElement = GuiGetElement_ScrollBar_Track( this );
// 	if ( lc_pElement )
// 	{
// 		lc_pElement->setLocation( m_rcTrack.left-getBounding().left, m_rcTrack.top-getBounding().top );
// 		lc_pElement->setSize( m_rcTrack.width(), m_rcTrack.height() );
// 	}
// 
// 	lc_pElement = GuiGetElement_ScrollBar_ButtonUp( this );
// 	if ( lc_pElement )
// 	{
// 		lc_pElement->setLocation( m_rcButtonUp.left-getBounding().left, m_rcButtonUp.top-getBounding().top );
// 		lc_pElement->setSize( m_rcButtonUp.width(), m_rcButtonUp.height() );
// 	}
// 
// 	lc_pElement = GuiGetElement_ScrollBar_ButtonDown( this );
// 	if ( lc_pElement )
// 	{
// 		lc_pElement->setLocation( m_rcButtonDown.left-getBounding().left, m_rcButtonDown.top-getBounding().top );
// 		lc_pElement->setSize( m_rcButtonDown.width(), m_rcButtonDown.height() );
// 	}
}


//--------------------------------------------------------------------------------------
BOOL iberbar::CGuiScrollBar::handleMessage( __in const GuiMessage& MsgIn )
{
	if (isVisible() == false ||
		isEnable() == false )
		return FALSE;

#ifdef _WIN32
	if( WM_CAPTURECHANGED == MsgIn.message )
	{
		// The application just lost mouse capture. We may not have gotten
		// the WM_MOUSEUP message, so reset m_bDrag here.
		if( ( HWND )MsgIn.lParam != MsgIn.hWnd )
		{
			m_bDrag = false;
			m_ArrowState = CLEAR;
		}
	}
#endif

	// Check if the arrow button has been held for a while.
	// If so, update the thumb position to simulate repeated
	// scroll.
	if ( m_ArrowState != CLEAR )
		return TRUE;


	if ( m_bMouseInput == true && handleMouse( MsgIn ) == TRUE )
		return TRUE;

	if ( m_bKeyboardInput == true && handleKeyboard( MsgIn ) == TRUE )
		return TRUE;

	return FALSE;
}


//--------------------------------------------------------------------------------------
BOOL iberbar::CGuiScrollBar::handleMouse( __in const GuiMessage& MsgIn )
{
	m_LastMouse = MsgIn.point;

	switch( MsgIn.message )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			// Check for click on up button

			if( RectPointHitTest( &m_rcButtonUp, &MsgIn.point ) )
			{
				SetCapture( MsgIn.hWnd );
				sRequestFocus( this );
				if( m_nPosition > m_nStart )
					setTrackPos( m_nPosition-1 );
				m_ArrowState = CLICKED_UP;
				return TRUE;
			}

			// Check for click on down button

			if( RectPointHitTest( &m_rcButtonDown, &MsgIn.point ) )
			{
				::SetCapture( MsgIn.hWnd );
				sRequestFocus( this );
				if( m_nPosition + m_nPageSize <= m_nEnd )
					setTrackPos( 1+m_nPosition );
				//UpdateThumbRect();
				m_ArrowState = CLICKED_DOWN;
				//m_dwArrowTS = MsgIn.nTime;
				return TRUE;
			}

			// Check for click on thumb

			if( RectPointHitTest( &m_rcThumb, &MsgIn.point ) )
			{
				::SetCapture( MsgIn.hWnd );
				sRequestFocus( this );
				m_bDrag = true;
				if ( m_bVertical == true )
				    m_nThumbOffset = MsgIn.point.y - m_rcThumb.top;
				else
					m_nThumbOffset = MsgIn.point.x - m_rcThumb.left;
				return TRUE;
			}

			// Check for click on track
			// Move a page
			if( m_rcThumb.left <= MsgIn.point.x &&
				m_rcThumb.right > MsgIn.point.x )
			{
				if( m_rcThumb.top > MsgIn.point.y &&
					m_rcTrack.top <= MsgIn.point.y )
				{
					SetCapture( MsgIn.hWnd );
					sRequestFocus( this );
					scroll( -( m_nPageSize - 1 ) );
					return TRUE;
				}
				else if( m_rcThumb.bottom <= MsgIn.point.y &&
					m_rcTrack.bottom > MsgIn.point.y )
				{
					SetCapture( MsgIn.hWnd );
					sRequestFocus( this );
					scroll( m_nPageSize - 1 );
					return TRUE;
				}
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			m_bDrag = false;
			ReleaseCapture();
			m_ArrowState = CLEAR;
			break;
		}

	case WM_MOUSEMOVE:
		{
			if( m_bDrag )
			{
				m_rcThumb.bottom += MsgIn.point.y - m_nThumbOffset - m_rcThumb.top;
				m_rcThumb.top = MsgIn.point.y - m_nThumbOffset;
// 				if( m_rcThumb.top < m_rcTrack.top )
// 					OffsetRect( &m_rcThumb, 0, m_rcTrack.top - m_rcThumb.top );
// 				else if( m_rcThumb.bottom > m_rcTrack.bottom )
// 					OffsetRect( &m_rcThumb, 0, m_rcTrack.bottom - m_rcThumb.bottom );

				// Compute first item index based on thumb position

				int nMaxFirstItem = m_nEnd - m_nStart - m_nPageSize + 1;  // Largest possible index for first item
				int nMaxThumb = m_rcTrack.height() - m_rcThumb.height();  // Largest possible thumb position from the top

				setTrackPos( m_nStart +
					( m_rcThumb.top - m_rcTrack.top +
					nMaxThumb / ( nMaxFirstItem * 2 ) ) * // Shift by half a row to avoid last row covered by only one pixel
					nMaxFirstItem / nMaxThumb );

				return TRUE;
			}

			break;
		}
	}

	return FALSE;
}


//--------------------------------------------------------------------------------------
BOOL iberbar::CGuiScrollBar::handleKeyboard( __in const GuiMessage& MsgIn )
{
	return FALSE;
}


//--------------------------------------------------------------------------------------
void iberbar::CGuiScrollBar::update( float nElapsedTime )
{
	if ( m_ArrowState != CLEAR )
	{
		m_nArrowClickTS += nElapsedTime;
		if ( RectPointHitTest( &m_rcButtonUp, &m_LastMouse ) )
		{
			switch ( m_ArrowState )
			{
			case CLICKED_UP:
				if ( m_nArrowClickDelay < m_nArrowClickTS )
				{
					scroll( -1 );
					m_ArrowState = HELD_UP;
					m_nArrowClickTS -= m_nArrowClickDelay;
				}
				break;
			case HELD_UP:
				if ( m_nArrowClickRepeat < m_nArrowClickTS )
				{
					scroll( -1 );
					m_nArrowClickTS -= m_nArrowClickRepeat;
				}
				break;
			}
		}
		else if ( RectPointHitTest( &m_rcButtonDown, &m_LastMouse ) )
		{
			switch ( m_ArrowState )
			{
			case CLICKED_DOWN:
				if ( m_nArrowClickDelay < m_nArrowClickTS )
				{
					scroll( 1 );
					m_ArrowState = HELD_DOWN;
					m_nArrowClickTS -= m_nArrowClickDelay;
				}
				break;
			case HELD_DOWN:
				if ( m_nArrowClickRepeat < m_nArrowClickTS )
				{
					scroll( 1 );
					m_nArrowClickTS -= m_nArrowClickRepeat;
				}
				break;
			}
		}
	}


	if ( true )
	{
// 		GUI_IMP_STATE lc_StateButtonUp   = GUI_IMP_STATE_NORMAL;
// 		GUI_IMP_STATE lc_StateButtonDown = GUI_IMP_STATE_NORMAL;
// 		GUI_IMP_STATE lc_StateThumb      = GUI_IMP_STATE_NORMAL;
// 
// 		if ( m_bVisible == false )
// 		{
// 			lc_StateButtonUp   = GUI_IMP_STATE_HIDDEN;
// 			lc_StateButtonDown = GUI_IMP_STATE_HIDDEN;
// 			lc_StateThumb      = GUI_IMP_STATE_HIDDEN;
// 		}
// 		else if ( m_bEnable == false || m_bShowThumb == false )
// 		{
// 			lc_StateButtonUp   = GUI_IMP_STATE_DISABLED;
// 			lc_StateButtonDown = GUI_IMP_STATE_DISABLED;
// 			lc_StateThumb      = GUI_IMP_STATE_DISABLED;
// 		}
// 		else if ( m_bMouseOver )
// 		{
// 			lc_StateButtonUp   = GUI_IMP_STATE_MOUSEOVER;
// 			lc_StateButtonDown = GUI_IMP_STATE_MOUSEOVER;
// 			lc_StateThumb      = GUI_IMP_STATE_MOUSEOVER;
// 		}
// 		else
// 		{
// 			if ( m_bDrag == true )
// 			{
// 				lc_StateButtonUp   = GUI_STATE_MOUSEOVER;
// 				lc_StateButtonDown = GUI_STATE_MOUSEOVER;
// 				lc_StateThumb      = GUI_STATE_PRESSED;
// 			}
// 			else if ( m_ArrowState != CLEAR )
// 			{
// 				if ( m_ArrowState == CLICKED_UP || m_ArrowState == HELD_UP )
// 				{
// 					lc_StateButtonUp   = GUI_STATE_PRESSED;
// 					lc_StateButtonDown = GUI_STATE_MOUSEOVER;
// 					lc_StateThumb      = GUI_STATE_MOUSEOVER;
// 				}
// 				else if ( m_ArrowState == CLICKED_DOWN || m_ArrowState == HELD_DOWN )
// 				{
// 					lc_StateButtonUp   = GUI_STATE_MOUSEOVER;
// 					lc_StateButtonDown = GUI_STATE_PRESSED;
// 					lc_StateThumb      = GUI_STATE_MOUSEOVER;
// 				}
// 			}
// 		}
// 
// 		float lc_fBlendRateButtonUp   = ( lc_StateButtonUp   == GUI_STATE_PRESSED ) ? 0.0f : 0.8f;
// 		float lc_fBlendRateButtonDown = ( lc_StateButtonDown == GUI_STATE_PRESSED ) ? 0.0f : 0.8f;
// 		float lc_fBlendRateThumb      = ( lc_StateThumb      == GUI_STATE_PRESSED ) ? 0.0f : 0.8f;
	}
}


//--------------------------------------------------------------------------------------
// void lily::CGuiScrollBar::render()
// {
// 	if ( m_Elements == NULL )
// 		return ;
// 
// 	CGuiElement* lc_pElement = NULL;
// 
// 	lc_pElement = GuiGetElement_ScrollBar_Track( this );
// 	if ( lc_pElement )
// 		lc_pElement->render();
// 
// 	lc_pElement = GuiGetElement_ScrollBar_Thumb( this );
// 	if ( lc_pElement && m_bShowThumb )
// 		lc_pElement->render();
// 
// 	lc_pElement = GuiGetElement_ScrollBar_ButtonUp( this );
// 	if ( lc_pElement )
// 		lc_pElement->render();
// 
// 	lc_pElement = GuiGetElement_ScrollBar_ButtonDown( this );
// 	if ( lc_pElement )
// 		lc_pElement->render();
// 
// }


//--------------------------------------------------------------------------------------
void iberbar::CGuiScrollBar::setStyle( uint32 style )
{
	if ( tHasMask< uint32 >( style, Style_EnableButtonPage ) )
		style |= Style_EnableButtonScroll;
	m_nStyle = style;
	updateRect();
}


//--------------------------------------------------------------------------------------
void iberbar::CGuiScrollBar::setTrackRange( int nStart, int nEnd )
{
	m_nStart = nStart;
	m_nEnd = nEnd;
	cap();
	updateThumbRect();
}


//--------------------------------------------------------------------------------------
void iberbar::CGuiScrollBar::cap()  // Clips position at boundaries. Ensures it stays within legal range.
{
	if( m_nPosition < m_nStart ||
		m_nEnd - m_nStart <= m_nPageSize )
	{
		m_nPosition = m_nStart;
	}
	else if( m_nPosition + m_nPageSize > m_nEnd )
		m_nPosition = m_nEnd - m_nPageSize;
}


//--------------------------------------------------------------------------------------
// Compute the dimension of the scroll thumb
void iberbar::CGuiScrollBar::updateThumbRect()
{
	if ( m_nEnd - m_nStart > m_nPageSize )
	{
		int lc_nMaxPosition = m_nEnd - m_nStart - m_nPageSize;
		if ( m_bVertical == true )
		{
			// Vertical
			int lc_nTrackHeight = m_rcTrack.bottom - m_rcTrack.top;
			int lc_nThumbHeight = tMax( lc_nTrackHeight * m_nPageSize / ( m_nEnd - m_nStart ), m_nMinThumbSize );
			m_rcThumb.top = m_rcTrack.top + ( m_nPosition - m_nStart ) * ( lc_nTrackHeight - lc_nThumbHeight )
				/ lc_nMaxPosition;
			m_rcThumb.bottom = m_rcThumb.top + lc_nThumbHeight;
		}
		else
		{
			// Herizontal
			int lc_nTrackWidth = m_rcTrack.right - m_rcTrack.left;
			int lc_nThumbWidth = tMax( lc_nTrackWidth * m_nPageSize / ( m_nEnd - m_nStart ), m_nMinThumbSize );
			m_rcThumb.left = m_rcTrack.left + ( m_nPosition - m_nStart ) * ( lc_nTrackWidth - lc_nThumbWidth )
				/ lc_nMaxPosition;
			m_rcThumb.right = m_rcThumb.left + lc_nThumbWidth;
		}

		m_bShowThumb = true;
	}
	else
	{
		// No content to scroll
		if ( m_bVertical == true )
			m_rcThumb.bottom = m_rcThumb.top;
		else
			m_rcThumb.right = m_rcThumb.left;
		m_bShowThumb = false;
	}

	PTR_CGuiElement lc_pElemThumb;
	if ( getElementSpec( GuiElementComSpec_ScrollBar_Thumb, &lc_pElemThumb ) && lc_pElemThumb )
	{
		lc_pElemThumb->setLocation( m_rcThumb.left - getBounding().left, m_rcThumb.top - getBounding().top );
		lc_pElemThumb->setSize( m_rcThumb.width(), m_rcThumb.height() );
	}
}


//--------------------------------------------------------------------------------------
void iberbar::CGuiScrollBar::setPageSizeA( int nPageSize )
{
	m_nPageSize = nPageSize;
	cap();
	updateThumbRect();
}


//--------------------------------------------------------------------------------------
// Scroll() scrolls by nDelta items.  A positive value scrolls down, while a negative
// value scrolls up.
void iberbar::CGuiScrollBar::scroll( int nDelta )
{
	// Perform scroll
	m_nPosition += nDelta;

	// Cap position
	cap();

	// Update thumb position
	updateThumbRect();
}


//--------------------------------------------------------------------------------------
void iberbar::CGuiScrollBar::showItem( int nIndex )
{
	// Cap the index

	if ( nIndex < 0 )
		nIndex = 0;

	if ( nIndex >= m_nEnd )
		nIndex = m_nEnd - 1;

	// Adjust position

	if ( m_nPosition > nIndex )
		m_nPosition = nIndex;
	else if ( m_nPosition + m_nPageSize <= nIndex )
		m_nPosition = nIndex - m_nPageSize + 1;

	updateThumbRect();
}


