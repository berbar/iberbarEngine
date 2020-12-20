#pragma once

#include <iberbar/GameEngine/Headers.h>
#include <iberbar/Renderer/Font.h>
#include <iberbar/Font/FreeType.h>

namespace iberbar
{
	namespace RHI
	{
		class IDevice;
	}

	namespace Game
	{
		class __iberbarGameEngineApi__ CFontManager
		{
		private:
			struct _FontFaceNode
			{
				CFontFaceFt* pFace;
				std::string strName;
			};

		public:
			CFontManager( RHI::IDevice* pRHIDevice );
			~CFontManager();

		public:
			CResult Initial();
			CResult GetOrCreateFont( Renderer::CFont** ppFont, const UFontDesc& FontDesc, Renderer::UFontCharVocabularyType nVocabularyType );
			bool GetFont( Renderer::CFont** ppFont, const UFontDesc& FontDesc );
			bool GetFontDefault( Renderer::CFont** ppFont );

		private:
			CResult GetOrCreateFontFace( CFontFaceFt** ppOutFace, const char* strName );

		public:
			RHI::IDevice* m_pRHIDevice;
			CFontDeviceFt* m_pFontDevice;
			std::vector<_FontFaceNode> m_FontFaceList;
			std::vector<Renderer::CFont*> m_FontList;
		};
	}
}

