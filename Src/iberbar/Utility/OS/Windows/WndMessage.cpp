
#include <iberbar/Utility/OS/Windows/WndMessage.h>
#include <map>


namespace iberbar
{
	namespace OS
	{


		static const std::map<UINT,UWndMessageDescription> s_WndMessageDescriptionMap =
		{
			{ WM_ACTIVATE, { WM_ACTIVATE, "WM_ACTIVATE" } },
			{ WM_SETFOCUS, { WM_SETFOCUS, "WM_SETFOCUS" } },
			{ WM_GETOBJECT, { WM_GETOBJECT, "WM_GETOBJECT" } },
			{ WM_GETMINMAXINFO, { WM_GETMINMAXINFO, "WM_GETMINMAXINFO" } },
			{ WM_WINDOWPOSCHANGING, { WM_WINDOWPOSCHANGING, "WM_WINDOWPOSCHANGING" } },
			{ WM_DISPLAYCHANGE, { WM_DISPLAYCHANGE, "WM_DISPLAYCHANGE" } },
			{ WM_GETICON, { WM_GETICON, "WM_GETICON" } },
			{ WM_KEYUP, { WM_KEYUP, "WM_KEYUP" } },
			{ WM_DWMCOLORIZATIONCOLORCHANGED, { WM_DWMCOLORIZATIONCOLORCHANGED, "WM_DWMCOLORIZATIONCOLORCHANGED" } },
			{ WM_SIZING, { WM_SIZING, "WM_SIZING" } },
			{ WM_CAPTURECHANGED, { WM_CAPTURECHANGED, "WM_CAPTURECHANGED" } },
			{ WM_EXITSIZEMOVE, { WM_EXITSIZEMOVE, "WM_EXITSIZEMOVE" } },
			{ WM_WININICHANGE, { WM_WININICHANGE, "WM_WININICHANGE" } },
			{ WM_NCCALCSIZE, { WM_NCCALCSIZE, "WM_NCCALCSIZE" } },
			{ WM_WINDOWPOSCHANGED, { WM_WINDOWPOSCHANGED, "WM_WINDOWPOSCHANGED" } },
			{ WM_ACTIVATEAPP, { WM_ACTIVATEAPP, "WM_ACTIVATEAPP" } },
			{ WM_NCACTIVATE, { WM_NCACTIVATE, "WM_NCACTIVATE" } },
		};
	}
}


const iberbar::OS::UWndMessageDescription* iberbar::OS::WndMessageGetDescription( UINT nMsg )
{
	auto iter = s_WndMessageDescriptionMap.find( nMsg );
	if ( iter == s_WndMessageDescriptionMap.end() )
		return nullptr;
	return &iter->second;
}
