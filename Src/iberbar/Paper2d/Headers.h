#pragma once

#include <iberbar/Utility/Unknown.h>
#include <iberbar/Utility/Color.h>
#include <iberbar/Utility/Rect.h>

#ifndef __iberbarPaper2dApis__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarPaper2dApisExport__ )
#			define __iberbarPaper2dApis__ _declspec( dllexport )
#		else
#			define __iberbarPaper2dApis__ _declspec( dllimport )
#		endif
#	else
#		define __iberbarPaper2dApis__
#	endif
#endif
