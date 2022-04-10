

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

		class __iberbarGuiApi__ CDialog
			: public CObject
		{
		public:
			virtual ~CDialog();

		protected:
			CDialog( CDialog* pDlgParent );
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
			void SetModal( bool bModal ) { m_bModal = bModal; }
			bool IsModal() const { return m_bModal; }
			void RequestTop();

			void SetWidgetRoot( CWidget* pContainer );
			CWidget* GetWidgetRoot() { return m_pContainer; }
			const CWidget* GetWidgetRoot() const { return m_pContainer; }
			
		protected:
			void HandleMouseMoveInContainer( CWidget* pContainer, const UMouseEventData* pEventData );
			bool HandleMouseInContainer( CWidget* pContainer, const UMouseEventData* pEventData );
			bool HandleKeyboardInContainer( CWidget* pContainer, const UKeyboardEventData* pEventData );
			void RemoveContainer();

		protected:
			CDialog* m_pDialogParent;
			CWidget* m_pContainer;
			bool m_bMouseInput;
			bool m_bKeyboardInput;
			bool m_bNeedClip;
			bool m_bModal;


		public:
			static CResult sCreateDialog( CDialog** ppOutDialog, CDialog* pDlgParent = nullptr );
			static void sDestroyDialog( CDialog* pDialog );
		};

		IBERBAR_UNKNOWN_PTR_DECLARE( CDialog );
	}

}

