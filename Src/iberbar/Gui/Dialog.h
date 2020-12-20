

#pragma once

#include <iberbar/Gui/Object.h>
#include <iberbar/Utility/Result.h>
#include <functional>

namespace iberbar
{

	namespace Gui
	{
		class CWidget;


		enum class UDialogStyle
		{
			Overlapped,
			Child,
		};

		class __iberbarGuiApi__ CDialog final
			: public CObject
		{
		public:
			virtual ~CDialog();

		protected:
			CDialog( UDialogStyle nStyle, CDialog* pDlgParent );
			CDialog( const CDialog& dialog );

		public:
			virtual CDialog* Clone() const override { return new CDialog( *this ); }
			virtual void UpdateRect() override;
			virtual void Refresh() override;
			virtual void Update( float nElapsedTime ) override;
			virtual void Render() override;

			virtual UHandleMessageResult HandleMouse( const UMouseEventData* EventData );
			virtual UHandleMessageResult HandleKeyboard( const UKeyboardEventData* pEvent );

		public:
			CDialog* GetParentDialog() { return m_pDialogParent; }
			bool IsVisible() const { return GetVisible(); }
			bool IsEnable() const { return GetEnable(); }
			void SetMouseInput( bool bEnable = true ) { m_bMouseInput = bEnable; }
			bool IsMouseInputEnabled() const { return m_bMouseInput; }
			void SetKeyBoardInput( bool bEnable = true ) { m_bKeyboardInput = bEnable; }
			bool IsKeyboardInputEnabled() const { return m_bKeyboardInput; }
			void SetNeedClip( bool bNeedClip ) { m_bNeedClip = bNeedClip; }

			void AddWidget( CWidget* pWidget );
			CWidget* FindWidget( const char* strId );
			void RemoveWidget( CWidget* pWidget );
			void RemoveWidgetsAll();
			void RequestTop();
			void ForeachWidgets( std::function<void(CWidget*)> Func );
			
		protected:
			CWidget* GetActiveWidgetAtPoint( const CPoint2i& point );
		private:
			void AddDialog( CDialog* pDialog );
			void RemoveDialogs();

		protected:
			UDialogStyle m_nDialogStyle;
			CDialog* m_pDialogParent;
			CWidget* m_pWidgetMouseOver;
			bool m_bMouseInput;
			bool m_bKeyboardInput;
			bool m_bNeedClip;
			std::vector<CWidget*> m_Widgets;
			std::vector<CDialog*> m_Dialogs;
			std::vector<CDialog*> m_DialogsOverlapped;


		public:
			static CResult sCreateDialog( CDialog** ppOutDialog, UDialogStyle nStyle, CDialog* pDlgParent = nullptr );
			static void sDestroyDialog( CDialog* pDialog );
		};

		IBERBAR_UNKNOWN_PTR_DECLARE( CDialog );
	}

}

