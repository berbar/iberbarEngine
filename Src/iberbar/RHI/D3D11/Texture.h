#pragma once

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/Texture.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CDevice;

			class CTexture
				: public ITexture
			{
			public:
				CTexture( CDevice* pDevice );
				virtual ~CTexture();

			public:
				virtual CResult CreateEmpty( int w, int h ) override;
				virtual CResult CreateFromFileInMemory( const void* pData, uint32 nDataSize ) override;
				virtual CResult CreateFromFileA( const char* strFile ) override;
#ifdef _WIN32
				virtual CResult CreateFromFileW( const wchar_t* strFile ) override;
#endif
				virtual CResult CreateFromPixels( int w, int h, void* pixels ) override;
				virtual CResult SetPixels( const void* pixels, int nx, int ny, int nw, int nh ) override;
				virtual CResult SaveToFileA( const char* strFile ) override;
#ifdef _WIN32
				virtual CResult SaveToFileW( const wchar_t* strFile ) override;
#endif

				FORCEINLINE ID3D11Texture2D* GetD3DTexture() { return m_pD3DTexture; }
				FORCEINLINE ID3D11ShaderResourceView* GetD3DShaderResourceView() { return m_pD3DShaderResourceView; }

			protected:
				CDevice* m_pDevice;
				ID3D11Texture2D* m_pD3DTexture;
				ID3D11ShaderResourceView* m_pD3DShaderResourceView;
				bool m_bDynamic;
			};
		}
	}
}