
#pragma once


#include <iberbar\Gui\Widgets\Button.h>

namespace iberbar
{
	namespace Gui
	{
		class __iberbarGuiApi__ CCheckBox
			: public CButton
		{
		public:
			enum class UCheckState
			{
				CheckedFalse,
				CheckedTrue,
				CheckedIndeterminate,
			};

		public:
			CCheckBox( void );

		protected:
			CCheckBox( const CCheckBox& checkbox );

		public:
			virtual CCheckBox* Clone() const override;
			virtual const char* GetWidgetType() override { return "CheckBox"; }
			virtual UHandleMessageResult HandleMouse( const UMouseEventData* EventData ) override;
			virtual UHandleMessageResult HandleKeyboard( const UKeyboardEventData* EventData ) override;
			virtual void Update( float nElapsedTime ) override;

		public:
			void SetRenderElementFalse( const char* strElementId );
			void SetRenderElementTrue( const char* strElementId );
			void SetRenderElementIndeterminate( const char* strElementId );

			void SetChecked( UCheckState nState ) { SetCheckedInternal( nState, false ); }
			UCheckState GetChecked() const { return m_nCheckState; }

		protected:
			virtual void SetCheckedInternal( UCheckState nState, bool bInternal );

		protected:
			UCheckState m_nCheckState;

			CRenderElement* m_pRenderElementCheckFalse;
			CRenderElement* m_pRenderElementCheckTrue;
			CRenderElement* m_pRenderElementCheckIndeterminate;
		};

		IBERBAR_UNKNOWN_PTR_DECLARE( CCheckBox );
	}

}
