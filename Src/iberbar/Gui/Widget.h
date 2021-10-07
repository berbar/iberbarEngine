#pragma once

#include <iberbar/Gui/Object.h>
#include <iberbar/Utility/Ascii_6Bit.hpp>
#include <functional>

namespace iberbar
{
	struct UMouseEventData;
	struct UKeyboardEventData;

	namespace Gui
	{
		class CWidget;
		class CDialog;
		class CRenderElement;

		IBERBAR_UNKNOWN_PTR_DECLARE( CWidget );



		typedef void UCallbackWidgetEventProc( CWidget* pWidget, uint64 nEvent, uint64 nValueUint, const void* pValueExt );


		enum UWidgetType
		{
			Unknown = 0,
			Container,
			UserType = 0xff,
		};



		class __iberbarGuiApi__ CWidget
			: public CObject
		{
		private:
			struct UWidgetEventListener
			{
				bool bDefer;
				int nHandle;
				uint64 nEvent;
				std::function<UCallbackWidgetEventProc> callback;
			};

		public:
			CWidget();
			virtual ~CWidget();

		protected:
			CWidget( const CWidget& widget );

		public:
			void SetDialog( CDialog* pDialog );
			CDialog* GetDialog() const { return m_pDialog; }
			
			CWidget* GetWidgetParent() const { return m_pWidgetParent; }

			void SetRenderElement( CRenderElement* pRenderElement );
			bool FindElement( const char* strName, CRenderElement** ppOutElement );

			void SetGroup( uint32 nGroup ) { m_nGroup = nGroup; }
			uint32 GetGroup() const { return m_nGroup; }

			void SetMouseInput( bool bEnable = true );
			bool IsMouseInputEnabled() const;
			void SetKeyBoardInput( bool bEnable = true );
			bool IsKeyboardInputEnabled() const;

			void RequestFocus();
			bool GetFocus() const { return m_bFocus; }
			bool GetMouseOver() const { return m_bMouseOver; }
			bool GetCanFocus() const { return m_bCanFocus; }
			bool IsVisible() const;
			bool IsEnable() const;

			void SetNeedClip( bool bNeedClip ) { m_bNeedClip = bNeedClip; }

			int AddEventCallback( std::function<UCallbackWidgetEventProc> callback, uint64 nEvent = 0, bool bDefer = true );
			void RemoveEventCallback( int nHandle );
			void RemoveEventCallbacksAll();
			void SendEvent( uint64 nEvent, uint64 nValueUint = 0, const void* pValueExt = nullptr );
			

		public:
			virtual const char* GetWidgetType() { return "Widget"; }
			virtual CWidget* Clone() const override { return new CWidget( *this ); }
			virtual void OnFocusIn();
			virtual void OnFocusOut();
			virtual void OnMouseEnter();
			virtual void OnMouseLeave();
			virtual UHandleMessageResult HandleMouse( const UMouseEventData* EventData ) { return UHandleMessageResult::Ignore; }
			virtual UHandleMessageResult HandleKeyboard( const UKeyboardEventData* EventData ) { return UHandleMessageResult::Ignore; };
			virtual void UpdateRect() override;
			virtual void Refresh() override;
			virtual void Update( float nElapsedTime ) override;
			virtual void Render() override;

		public:
			virtual int AddWidget( CWidget* pWidget );
			virtual int RemoveWidget( CWidget* pWidget );
			virtual void RemoveWidgetsAll();
			CWidget* FindWidget( const char* strId );
			int GetWidgetCount() const { return (int)m_Widgets.size(); }
			CWidget* GetWidgetAt( int nIndex ) { return m_Widgets[nIndex]; }
			void ForeachWidgets( std::function<void( CWidget* )> Func, int nDepth = -1 );

		private:

			void SetWidgetParent( CWidget* pWidgetParent );

		private:
			CDialog* m_pDialog;
			CWidget* m_pWidgetParent;

		protected:
			bool m_bCanFocus;  // default value is true
			bool m_bFocus;
			bool m_bMouseOver;
			bool m_bMouseInput;
			bool m_bKeyboardInput;
			uint32 m_nGroup;

			// 裁剪自己和子部件
			bool m_bNeedClip;

			CRenderElement* m_pRenderElementDefault;

			std::vector<CWidget*> m_Widgets;
			int m_nEventListenerHandleAlloc;
			std::vector<UWidgetEventListener> m_EventListeners;


		public:
			static CWidget* sCloneWidgetEx( CWidget* pWidgetSrc, bool bRecurse );
		};



		


		namespace BaseEvent
		{
			static const uint64 nMouseEnter = iberbar::Ascii_6Bit::atom_uint( "MouseEnter" );
			static const uint64 nMouseLeave = iberbar::Ascii_6Bit::atom_uint( "MouseLeave" );
			static const uint64 nFocusIn = iberbar::Ascii_6Bit::atom_uint( "FocusIn" );
			static const uint64 nFocusOut = iberbar::Ascii_6Bit::atom_uint( "FocusOut" );
			static const uint64 nValueChanged = iberbar::Ascii_6Bit::atom_uint( "ValChanged" );
			static const uint64 nClicked = iberbar::Ascii_6Bit::atom_uint( "Clicked" );
			static const uint64 nContextMenu = iberbar::Ascii_6Bit::atom_uint( "CtxMenu" );
		};
	}
}

