#pragma once

#include <iberbar/RHI/OpenGL/Headers.h>
#include <iberbar/Utility/Size.h>

namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			bool CreateTextureGLuintFromPngA( const char* filename, GLuint* pGLTex, CSize2i* pGLTexSize, uint32* format );
#ifdef _WIN32
			bool CreateTextureGLuintFromPngW( const wchar_t* filename, GLuint* pGLTex, CSize2i* pGLTexSize, uint32* format );
#endif

			bool CreateTextureGLuintFromPngFile( FILE* pFile, GLuint* pGLTex, CSize2i* pGLTexSize, uint32* format );
		}
	}
}