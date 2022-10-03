
#include <iberbar/Javascript/Device.h>
#include <stdio.h>


void Run( iberbar::iJavascript::CDevice* pJsDevice )
{
	pJsDevice->ExecuteScript( "dumpStatisticsLog();" );
}

int main()
{
	printf_s( "test js" );

	iberbar::iJavascript::CDevice Device;
	Device.Initial();
	Run( &Device );
	Device.Shutdown();

	return 0;
}