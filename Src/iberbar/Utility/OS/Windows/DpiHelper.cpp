
#include <iberbar/Utility/OS/Windows/DpiHelper.h>
#include <iberbar/Utility/OS/Windows/Error.h>
#include <ShellScalingApi.h>



iberbar::OS::CDpiHelper::CDpiHelper()
	: m_eAwareness( EDpiAwareness::Unaware )
{
}


iberbar::CResult iberbar::OS::CDpiHelper::SetAwareness( EDpiAwareness eAwareness )
{
	PROCESS_DPI_AWARENESS eWinDpiAwareness = PROCESS_DPI_UNAWARE;
	if ( eAwareness == EDpiAwareness::System )
		eWinDpiAwareness = PROCESS_SYSTEM_DPI_AWARE;
	HRESULT hResult = SetProcessDpiAwareness( eWinDpiAwareness );
	if ( FAILED( hResult ) )
		return ConvertHResultA( hResult );
	return CResult();
}


int iberbar::OS::CDpiHelper::GetDpi() const
{
	return (int)GetDpiForSystem();
}


int iberbar::OS::CDpiHelper::GetScaleFactor() const
{
	return GetDpi() / USER_DEFAULT_SCREEN_DPI;
}
