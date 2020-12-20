#pragma once

#include <iberbar/GameEngine/Headers.h>
#include <iberbar/RHI/Texture.h>

namespace iberbar
{
	namespace RHI
	{
		class IDevice;
	}

	namespace Game
	{
		class __iberbarGameEngineApi__ CTextureManager
		{
		public:
			struct UTextureNode
			{
				UTextureNode()
					: pTexture( nullptr )
					, strName( "" )
				{
				}
				RHI::ITexture* pTexture;
				std::string strName;
			};


		public:
			CTextureManager( RHI::IDevice* pDevice );
			~CTextureManager();

		public:
			bool GetTextureA( const char* strName, RHI::ITexture** ppOutTexture = nullptr );
			CResult AddTexture( const char* strName, const void* pFileMemory, size_t nFileMemorySize );
			CResult GetOrCreateTextureA( const char* strName, RHI::ITexture** ppOutTexture );

		private:
			RHI::IDevice* m_pDevice;
			std::vector<UTextureNode> m_TextureList;
		};
	}
}

