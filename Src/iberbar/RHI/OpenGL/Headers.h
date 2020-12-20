#pragma once


#include <iberbar/RHI/Headers.h>
#include <iberbar/Utility/Result.h>

#if _WIN32
#include <glew.h>
//#include <glxew.h>
//#include <wglew.h>
#endif

#if __ANDROID__
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#endif



#ifndef __iberbarRHIOpenGLApi__
#	if defined ( _WINDOWS )
#		if defined ( __iberbarRHIOpenGLApiExport__ )
#			define __iberbarRHIOpenGLApi__ __declspec( dllexport )
#		else
#			define __iberbarRHIOpenGLApi__ __declspec(dllimport)
#		endif
#	else
#		define __iberbarRHIOpenGLApi__
#	endif
#endif

