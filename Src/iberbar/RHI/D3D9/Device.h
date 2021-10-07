#pragma once

#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/D3D9/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class __iberbarD3DApi__ CDevice
				: public IDevice
			{
			public:
				CDevice();
				virtual ~CDevice();

			public:
				virtual CResult CreateDevice( HWND hWnd, bool bWindowed, int nSuitedWidth, int nSuitedHeight ) override;
				virtual void LostDevice() override;
				virtual CResult ResetDevice( int nBackBufferWidth, int nBackBufferHeight, bool bIsWindow ) override;
				virtual void CreateTexture( ITexture** ppTexture ) override;
				virtual CResult CreateVertexBuffer( uint32 nInSize, uint32 nUsage, IVertexBuffer** ppOutBuffer ) override;
				virtual CResult CreateIndexBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage, IIndexBuffer** ppOutBuffer ) override;
				virtual CResult CreateVertexShader( IShader** ppOutShader ) override;
				virtual CResult CreatePixelShader( IShader** ppOutShader ) override;
				virtual CResult CreateHullShader( IShader** ppOutShader ) override;
				virtual CResult CreateGeometryShader( IShader** ppOutShader ) override;
				virtual CResult CreateDomainShader( IShader** ppOutShader ) override;
				virtual CResult CreateComputeShader( IShader** ppOutShader ) override;
				virtual CResult CreateVertexDeclaration( IVertexDeclaration** ppOutDeclaration, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride ) override;
				virtual CResult CreateShaderState( IShaderState** ppOutShaderState, IVertexDeclaration* pVertexDeclaration, IShader* pVertexShader, IShader* pPixelShader, IShader* pHullShader, IShader* pGeometryShader, IShader* pDomainShader ) override;
				virtual void CreateShaderVariableTable( IShaderVariableTable** ppOutShaderVariableTable ) override;
				virtual CResult CreateBlendState( IBlendState** ppOutBlendState, const UBlendDesc& BlendDesc ) override;
				virtual CResult CreateSamplerState( ISamplerState** ppOutSamplerState, const UTextureSamplerState& SamplerDesc ) override;
				virtual void CreateCommandContext( ICommandContext** ppOutContext ) override;
				virtual CResult Begin() override;
				virtual void End() override;

			public:
				

				void BindVertexBuffer( IDirect3DVertexBuffer9* pD3DVertexBuffer, uint32 stride );
				void BindIndexBuffer( IDirect3DIndexBuffer9* pD3DIndexBuffer );
				void SetVertexShader( IDirect3DVertexShader9* pD3DVertexShader );
				void SetPixelShader( IDirect3DPixelShader9* pD3DVertexShader );
				void SetTexture( uint32 nStage, IDirect3DTexture9* pD3DTexture );

			public:
				void SetSamplerState( uint32 nStage, const UTextureSamplerState& SamplerState );

			public:
				IDirect3DDevice9* GetD3DDevice() { return m_pD3DDevice; }
				// Ó²¼þ´¦Àí
				bool IsHardwareVertexProcessing() { return m_bIsHardwareVertexProcessing; }

			protected:
				CResult ResetDevice() { return ResetDevice( m_ContextSize.w, m_ContextSize.h, m_bIsWindow ); }

			protected:
				HWND m_hWnd;
				HINSTANCE m_hInstance;
				IDirect3D9* m_pD3D;
				IDirect3DDevice9* m_pD3DDevice;

				IDirect3DVertexBuffer9* m_pD3DVertexBuffer;
				uint32 m_nVertexBufferStride;
				IDirect3DIndexBuffer9* m_pD3DIndexBuffer;
				IDirect3DVertexShader9* m_pD3DVertexShader;
				IDirect3DPixelShader9* m_pD3DPixelShader;

				bool m_bIsHardwareVertexProcessing;
				bool m_bIsRendering;
				bool m_bHasLostDevice;

				IDirect3DTexture9* m_pD3DTextures[ 8 ];

				D3DPRESENT_PARAMETERS m_D3DPresentParams;
			};
		}
	}
}



inline void iberbar::RHI::D3D9::CDevice::BindVertexBuffer( IDirect3DVertexBuffer9* pD3DVertexBuffer, uint32 nStride )
{
	if ( m_pD3DVertexBuffer != pD3DVertexBuffer || m_nVertexBufferStride != nStride )
	{
		D3D_SAFE_RELEASE( m_pD3DVertexBuffer );
		m_pD3DVertexBuffer = pD3DVertexBuffer;
		m_nVertexBufferStride = nStride;
		m_pD3DDevice->SetStreamSource( 0, pD3DVertexBuffer, 0, m_nVertexBufferStride );
		D3D_SAFE_ADDREF( m_pD3DVertexBuffer );
	}
}

inline void iberbar::RHI::D3D9::CDevice::BindIndexBuffer( IDirect3DIndexBuffer9* pD3DIndexBuffer )
{
	if ( m_pD3DIndexBuffer != pD3DIndexBuffer )
	{
		D3D_SAFE_RELEASE( m_pD3DIndexBuffer );
		m_pD3DIndexBuffer = pD3DIndexBuffer;
		m_pD3DDevice->SetIndices( m_pD3DIndexBuffer );
		D3D_SAFE_ADDREF( m_pD3DIndexBuffer );
	}
}

inline void iberbar::RHI::D3D9::CDevice::SetVertexShader( IDirect3DVertexShader9* pD3DVertexShader )
{
	if ( m_pD3DVertexShader != pD3DVertexShader )
	{
		D3D_SAFE_RELEASE( m_pD3DVertexShader );
		m_pD3DVertexShader = pD3DVertexShader;
		m_pD3DDevice->SetVertexShader( m_pD3DVertexShader );
		D3D_SAFE_ADDREF( m_pD3DVertexShader );
	}
}

inline void iberbar::RHI::D3D9::CDevice::SetPixelShader( IDirect3DPixelShader9* pPixelShader )
{
	if ( m_pD3DPixelShader != pPixelShader )
	{
		D3D_SAFE_RELEASE( m_pD3DPixelShader );
		m_pD3DPixelShader = pPixelShader;
		m_pD3DDevice->SetPixelShader( m_pD3DPixelShader );
		D3D_SAFE_ADDREF( m_pD3DPixelShader );
	}
}


inline void iberbar::RHI::D3D9::CDevice::SetTexture( uint32 nStage, IDirect3DTexture9* pD3DTexture )
{
	if ( nStage >= 8 )
		return;

	if ( m_pD3DTextures[ nStage ] != pD3DTexture )
	{
		D3D_SAFE_RELEASE( m_pD3DTextures[ nStage ] );
		m_pD3DTextures[ nStage ] = pD3DTexture;
		D3D_SAFE_ADDREF( m_pD3DTextures[ nStage ] );
		m_pD3DDevice->SetTexture( nStage, m_pD3DTextures[ nStage ] );
	}
}

