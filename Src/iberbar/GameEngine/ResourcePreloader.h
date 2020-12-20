#pragma once


#include <iberbar/GameEngine/Headers.h>
#include <iberbar/Utility/Result.h>
#include <functional>
//#include <iberbar/RHI/TextureManager.h>
//#include <iberbar/Renderer/Font.h>


namespace iberbar
{
	namespace Game
	{
		class CResourceXmlReader;
		class CResourceJsonReader;

		class __iberbarGameEngineApi__ CResourcePreloader
		{
		public:
			struct ULoadTextureContext
			{
				const char* strFile;
			};
			struct ULoadFontContext
			{
				const char* strFamily;
				int nSize;
				bool bIsBold;
			};

		public:
			typedef void UCallbackLoadTexture( const ULoadTextureContext& );
			typedef void UCallbackLoadFont( const ULoadFontContext& );

		public:
			CResourcePreloader();
			~CResourcePreloader() {}
			CResult ReadFile( const char* pFile );

		public:
			std::function<UCallbackLoadTexture> m_LoadTexture;
			std::function<UCallbackLoadFont> m_LoadFont;
		};
	}
}
