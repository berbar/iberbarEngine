#pragma once

#ifndef __iberbarNetworkApis__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarNetworkApisExport__ )
#			define __iberbarNetworkApis__ _declspec( dllexport )
#		else
#			define __iberbarNetworkApis__ _declspec( dllimport )
#		endif
#	else
#		define __iberbarNetworkApis__
#	endif
#endif
