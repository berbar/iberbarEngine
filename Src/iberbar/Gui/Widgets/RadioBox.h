
#pragma once


#include <iberbar\Gui\Widgets\CheckBox.h>

namespace iberbar
{
	namespace Gui
	{
		class __iberbarGuiApi__ CRadioBox
			: public CCheckBox
		{
		public:
			CRadioBox( void );
		protected:
			CRadioBox( const CRadioBox& radiobox );

		public:
			virtual CRadioBox* Clone() const override;
			virtual const char* GetWidgetType() override { return "RadioBox"; }
			virtual UHandleMessageResult HandleMouse( const UMouseEventData* EventData ) override;
			virtual UHandleMessageResult HandleKeyboard( const UKeyboardEventData* EventData ) override;

		protected:
			virtual void SetCheckedInternal( UCheckState nState, bool bInternal ) override;
			void ResetRadioGroupChecked();
		};

		IBERBAR_UNKNOWN_PTR_DECLARE( CRadioBox );
	}
}

