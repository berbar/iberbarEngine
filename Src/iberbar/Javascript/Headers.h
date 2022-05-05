#pragma once

#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Ref.h>

#include <v8.h>
#include <libplatform/libplatform.h>
#include <Blob/Win64/SnapshotBlob.h>

#ifndef __iberbarJavascriptApi__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarJavascriptApiExport__ )
#			define __iberbarJavascriptApi__ __declspec( dllexport )
#		else
#			define __iberbarJavascriptApi__ __declspec(dllimport)
#		endif
#	else
#		define __iberbarJavascriptApi__
#	endif
#endif