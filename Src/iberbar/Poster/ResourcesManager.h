#pragma once

#include <map>
#include <string>
#include <iberbar/Utility/Result.h>
#include <iberbar/Poster/TextureScale9.h>
#include <iberbar/Poster/Font.h>

namespace iberbar
{
	namespace Poster
	{
		class CSurface;

		class __iberbarExports__ CResourcesManager
		{
		public:
			CResourcesManager() {}
			~CResourcesManager();

		public:
			void SetFontDevice( CFontDeviceFreeType* fontDevice ) { m_fontDevice = fontDevice; }

			/*
			@cache ÊÇ·ñ»º´æÏÂÀ´
			*/
			CResult GetTextureFromFile( const char* name, CSurface** ppOut, bool cache = true );

			CResult GetTextureScale9( const char* filepath, CTextureScale9** ppOut );

			CResult BuildTextureScale9( const char* filepath, const UTextureScale9Paddings& paddings );

			CResult GetFont( const char* fontName, int fontSize, int fontWeight, CFont** ppOut );

			CResult GetFont( const UFontDesc& fontDesc, CFont** ppOut );

			void GC( bool clearAll );

		private:
			std::map< std::string, PTR_CSurface > m_surfaces;
			std::map< std::string, PTR_CTextureScale9 > m_textureScale9s;
			std::list< PTR_CFont > m_fonts;
			PTR_CFontDeviceFreeType m_fontDevice;
			std::mutex m_mutex;

		//public:
		//	static void sInit();
		//	static void sDestory();
		//	static CResourcesManager* sGetInstance() { return s_instance; }
		//private:
		//	static CResourcesManager* s_instance;
		};
	}
}
