#pragma once


#ifndef __iberbarGuiApi__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarGuiApiExport__ )
#			define __iberbarGuiApi__ __declspec(dllexport)
#		else
#			define __iberbarGuiApi__ __declspec(dllimport)
#		endif
#	else
#		define __iberbarGuiApi__
#	endif
#endif