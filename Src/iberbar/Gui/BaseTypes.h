
#pragma once


namespace iberbar
{
	struct UMouseEventData;
	struct UKeyboardEventData;

	namespace Gui
	{
		enum class UWidgetState
		{
			Normal = 0,
			MouseOver = 1,
			Pressed = 2,
			Focus = 3,
			Disabled = 4,
			Hidden = 5,
		};
		static const int uWidgetStateCount = 6;
		static const int GUI_STATE_ALL = -1;


		enum class UHandleMessageResult
		{
			Ignore = 0,
			Succeed = 1
		};



	}
}