
#pragma once


#include <iberbar/Gui/Widget.h>


namespace iberbar
{
	namespace Gui
	{
		class __iberbarGuiApi__ CContainer
			: public CWidget
		{
		public:
			CContainer();
		protected:
			CContainer( const CContainer& container );

		public:
			virtual CContainer* Clone() const override;
			virtual const char* GetWidgetType() override { return "Container"; }
			virtual UHandleMessageResult HandleMouse( const UMouseEventData* EventData ) override;
			//virtual BOOL HandleKeyboard( const GuiMessage& MsgIn ) { return FALSE; }

		public:
			CWidget* GetActiveWidgetAtPoint( const CPoint2i& point );

		protected:
			std::list<CWidget*> m_Widgets;
		};
	}
}

