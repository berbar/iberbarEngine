#pragma once

#ifndef __iberbarGameEngineApi__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarGameEngineApiExport__ )
#			define __iberbarGameEngineApi__ _declspec( dllexport )
#		else
#			define __iberbarGameEngineApi__ _declspec( dllimport )
#		endif
#	else
#		define __iberbarGameEngineApi__
#	endif
#endif
