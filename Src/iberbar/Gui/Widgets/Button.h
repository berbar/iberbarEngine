
#pragma once

#include <iberbar/Gui/Widget.h>


namespace iberbar
{
	namespace Gui
	{
		class __iberbarGuiApi__ CButton
			: public CWidget
		{
		public:
			CButton();

		protected:
			CButton( const CButton& btn );

		public:
			virtual CButton* Clone() const override;
			virtual const char* GetWidgetType() override { return "Button"; }
			virtual UHandleMessageResult HandleMouse( const UMouseEventData* EventData ) override;
			virtual UHandleMessageResult HandleKeyboard( const UKeyboardEventData* EventData ) override;
			virtual void Update( float nElapsedTime ) override;

		public:
			void SetHotKey( int nKey ) { m_nHotKey = nKey; }
			int GetHotKey() const { return m_nHotKey; }
			bool IsPressed() const { return m_bPressed; }

		protected:
			int m_nHotKey;
			bool m_bPressed;
		};


		IBERBAR_UNKNOWN_PTR_DECLARE( CButton );
	}

}
