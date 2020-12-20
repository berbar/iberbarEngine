

#pragma once

#include <iberbar/RHI/OpenGL/Headers.h>
#include <iberbar/RHI/Texture.h>

namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			class CTexture;

			IBERBAR_UNKNOWN_PTR_DECLARE( CTexture );

			typedef bool(*PFunctionCreateTextureFromFileA)( const char* strFilepath, GLuint* pTextureUint, CSize2i* pSize, uint32* pFormat );
			typedef bool(*PFunctionCreateTextureFromFileW)( const wchar_t* strFilepath, GLuint* pTextureUint, CSize2i* pSize, uint32* pFormat );

			class __iberbarRHIOpenGLApi__ CTexture
				: public ITexture
			{
			public:
				CTexture( void );
				virtual ~CTexture() override;


			public:
				inline GLuint GetTextureUint() { return m_TextureUint; }

			public:
				virtual CResult CreateEmpty( int w, int h ) override;
				virtual CResult CreateFromFileInMemory( const void* pData, uint32 nDataSize ) override;
				virtual CResult CreateFromFileA( const char* filename ) override;
#ifdef _WIN32
				virtual CResult CreateFromFileW( const wchar_t* filename ) override;
#endif
				virtual CResult CreateFromPixels( int w, int h, void* pixels ) override;

				//bool getPixels( void* buffer );
				//bool getPixels( void* buffer, int nx, int ny, int nw, int nh );
				//bool SetPixels( const void* pixels );
				virtual CResult SetPixels( const void* pixels, int nx, int ny, int nw, int nh ) override;
				virtual CResult SaveToFileA( const char* strFile ) override;
#ifdef _WIN32
				virtual CResult SaveToFileW( const wchar_t* strFile ) override;
#endif

			protected:
				GLuint m_TextureUint;
			};
		}
	}
	

}
