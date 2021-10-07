#pragma once


#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Unknown.h>
#include <iberbar/RHI/Types.h>

//#ifndef __iberbarRendererApi__
//#if defined ( _WINDOWS ) && ( __iberbarRendererApiExport__ )
//#define __iberbarRendererApi__ _declspec( dllexport )
//#endif
//#endif

#ifndef __iberbarRendererApi__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarRendererApiExport__ )
#			define __iberbarRendererApi__ __declspec( dllexport )
#		else
#			define __iberbarRendererApi__ __declspec(dllimport)
#		endif
#	else
#		define __iberbarRendererApi__
#	endif
#endif