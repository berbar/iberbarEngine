#pragma once

#include <iberbar/Paper2d/Component.h>
#include <iberbar/Utility/Input.h>

namespace iberbar
{
	namespace Paper2d
	{
		struct UComponentKeyboardEventData
		{
			UKeyboardEvent nEvent;
			int nKeyCode;
		};

		class __iberbarPaper2dApis__ CComponent_HandleKeyboardInput abstract
			: public CComponent
		{
		public:
			CComponent_HandleKeyboardInput();
		protected:
			virtual void OnAttach() override;
			virtual void OnRemove() override;
		public:
			virtual void OnKeyboardEvent( const UComponentKeyboardEventData* EventData ) = 0;
		};
	}
}