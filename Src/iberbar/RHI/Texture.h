#pragma once

#include <iberbar/RHI/Resource.h>


namespace iberbar
{
	namespace RHI
	{
		class __iberbarRHIApi__ ITexture abstract
			: public IResource
		{
		public:
			ITexture( void );

		public:
			inline const CSize2i& GetSize() const { return m_Size; }

		public:
			virtual CResult CreateEmpty( int w, int h ) = 0;
			virtual CResult CreateFromFileInMemory( const void* pData, uint32 nDataSize ) = 0;
			virtual CResult CreateFromFileA( const char* strFile ) = 0;
#ifdef _WIN32
			virtual CResult CreateFromFileW( const wchar_t* strFile ) = 0;
#endif
			virtual CResult CreateFromPixels( int w, int h, void* pixels ) = 0;
			virtual CResult SetPixels( const void* pixels, int nx, int ny, int nw, int nh ) = 0;
			virtual CResult SaveToFileA( const char* strFile ) = 0;
#ifdef _WIN32
			virtual CResult SaveToFileW( const wchar_t* strFile ) = 0;
#endif

			inline CResult CreateFromFile( const TCHAR* strFile )
			{
#ifdef UNICODE
				return CreateFromFileW( strFile );
#else
				return CreateFromFileA( strFile );
#endif
			}

			inline CResult SaveToFile( const TCHAR* strFile )
			{
#ifdef UNICODE
				return SaveToFileW( strFile );
#else
				return SaveToFileA( strFile );
#endif
			}


		protected:
			uint32 m_Format;
			CSize2i m_Size;
		};
	}
}
