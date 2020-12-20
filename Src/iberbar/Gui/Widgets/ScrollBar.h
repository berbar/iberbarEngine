

#ifndef __lily_gui_wScrollBar_h__
#define __lily_gui_wScrollBar_h__


namespace iberbar
{
	// Minimum scroll bar thumb size
#define LILY_GUI_SCROLLBAR_MINTHUMBSIZE 8

	// Delay and repeat period when clicking on the scroll bar arrows (ms)
#define LILY_GUI_SCROLLBAR_ARROWCLICK_DELAY  330
#define LILY_GUI_SCROLLBAR_ARROWCLICK_REPEAT 50



	class IBERBAR_DLL CGuiScrollBar
		: public CGuiWidget
	{
	protected:
		enum ARROWSTATE
		{
			CLEAR,
			CLICKED_UP,
			CLICKED_DOWN,
			HELD_UP,
			HELD_DOWN
		};

	public:
		static const uint32 Style_EnableButtonPage = 0x01;
		static const uint32 Style_EnableButtonScroll = 0x02;
		static const uint32 Style_StaticThumbSize = 0x04;

		static const int Default_MinThumbSize = 8;
		static const float Default_ArrowClick_Delay;
		static const float Default_ArrowClick_Repeat;



	public:
		CGuiScrollBar( void );

		IBERBAR_UNKNOWN_CLONE_AUTO( CGuiScrollBar );

		// overload from CGUIControl
	public:
		virtual void onFocusOut() { m_bDrag = false; CGuiWidget::onFocusOut(); }
		//virtual bool hitTest( CPoint point );
		virtual void updateRect();
		virtual BOOL handleMessage( __in const GuiMessage& MsgIn );
		virtual BOOL handleMouse( __in const GuiMessage& MsgIn );
		virtual BOOL handleKeyboard( __in const GuiMessage& MsgIn );
		virtual void update( float nElapsedTime );
		//virtual void render();


	public:
		void     setStyle( uint32 style );
		uint32 getStyle() const;

		void setMinThumbSize( int nSize );
		int  getMinThumbSize() const;

		void setArrowClickDelay( int val );
		int  getArrowClickDelay() const;

		void setArrowClickRepeat( int val );
		int  getArrowClickRepeat() const;

		void setTrackRange( int nStart, int nEnd );
		int  getTrackPos()
		{
			return m_nPosition;
		}
		void setTrackPos( int nPosition )
		{
			m_nPosition = nPosition; cap(); updateThumbRect();
		}
		int  getPageSize()
		{
			return m_nPageSize;
		}

		void scroll( int nDelta );    // Scroll by nDelta items (plus or minus)
		void showItem( int nIndex );  // Ensure that item nIndex is displayed, scroll if necessary
		void setPageSizeA( int nPageSize );





	protected:
		void updateThumbRect();
		void cap();  // Clips position at boundaries. Ensures it stays within legal range.

	protected:
		bool m_bVertical; // default true
		bool m_bShowThumb;
		bool m_bDrag;
		bool m_bPressed;

		uint32 m_nStyle;
		int m_nMinThumbSize;
		float m_nArrowClickDelay;
		float m_nArrowClickRepeat;
		float m_nArrowClickTS;

		int m_nPosition;  // Position of the first displayed item
		int m_nPageSize;  // How many items are displayable in one page
		int m_nStart;     // First item
		int m_nEnd;       // The index after the last item
		int m_nThumbOffset;
		CPoint m_LastMouse;// Last mouse position
		ARROWSTATE m_ArrowState;
		//DWORD m_dwArrowTS;  // Timestamp of last arrow event.

		CRect m_rcButtonUp;
		CRect m_rcButtonDown;
		CRect m_rcThumb;
		CRect m_rcTrack;
	};



	iberbar_gui_defineElementComponentNeed( ScrollBar, 6 );

	iberbar_gui_defineElementComponentSpec( ScrollBar, Track, 0 );
	iberbar_gui_defineElementComponentSpec( ScrollBar, Thumb, 1 );

	iberbar_gui_defineElementComponentSpec( ScrollBar, ButtonUp, 2 );
	iberbar_gui_defineElementComponentSpec( ScrollBar, ButtonDown, 3 );
	iberbar_gui_defineElementComponentSpec( ScrollBar, PageUp, 4 );
	iberbar_gui_defineElementComponentSpec( ScrollBar, PageDown, 5 );

	iberbar_gui_defineElementComponentSpec( ScrollBar, ButtonLeft, 2 );
	iberbar_gui_defineElementComponentSpec( ScrollBar, ButtonRight, 3 );
	iberbar_gui_defineElementComponentSpec( ScrollBar, PageLeft, 4 );
	iberbar_gui_defineElementComponentSpec( ScrollBar, PageRight, 5 );
}



#endif