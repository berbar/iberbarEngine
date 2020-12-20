#pragma once

#include <iberbar/RHI/Texture.h>
#include <iberbar/RHI/D3D9/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CDevice;

			class __iberbarD3DApi__ CTexture
				: public ITexture
			{
			public:
				CTexture( CDevice* pDevice );
				virtual ~CTexture();

			public:
				virtual CResult CreateEmpty( int w, int h ) override;
				virtual CResult CreateFromFileInMemory( const void* pData, uint32 nDataSize ) override;
				virtual CResult CreateFromFileA( const char* strFile ) override;
				virtual CResult CreateFromFileW( const wchar_t* strFile ) override;
				virtual CResult CreateFromPixels( int w, int h, void* pixels ) override;
				virtual CResult SetPixels( const void* pixels, int nx, int ny, int nw, int nh ) override;
				virtual CResult SaveToFileA( const char* strFile ) override;
				virtual CResult SaveToFileW( const wchar_t* strFile ) override;

			public:
				inline IDirect3DTexture9* GetD3DTexture() { return m_pD3DTexture; }

			protected:
				CDevice* m_pDevice;
				IDirect3DTexture9* m_pD3DTexture;
			};
		}
	}
}
