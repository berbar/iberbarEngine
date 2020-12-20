#pragma once


#include <iberbar/Utility/Unknown.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Color.h>

#ifndef __iberbarRHIApi__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarRHIApiExport__ )
#			define __iberbarRHIApi__ __declspec( dllexport )
#		else
#			define __iberbarRHIApi__ __declspec(dllimport)
#		endif
#	else
#		define __iberbarRHIApi__
#	endif
#endif
