#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{
	namespace OS
	{
		struct UWndMessageDescription
		{
			UINT nMsg;
			const char* strMacroText;
		};


		__iberbarUtilityApi__ const UWndMessageDescription* WndMessageGetDescription( UINT nMsg );

		inline const char* WndMessageGetMacroText( UINT nMsg )
		{
			auto pDesc = WndMessageGetDescription( nMsg );
			return ( pDesc == nullptr ) ? nullptr : pDesc->strMacroText;
		}
	}
}