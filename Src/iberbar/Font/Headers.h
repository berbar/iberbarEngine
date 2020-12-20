#pragma once


#include <iberbar/Utility/Unknown.h>
#include <iberbar/Utility/Color.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Result.h>

#ifdef _WIN32
#	ifdef __iberbarFontApiExport__
#		define __iberbarFontApi__ __declspec(dllexport)
#	else
#		define __iberbarFontApi__ __declspec(dllimport)
#	endif
#else
#define __iberbarFontApi__
#endif
