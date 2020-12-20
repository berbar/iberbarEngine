#pragma once

#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/D3D11/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class __iberbarD3DApi__ CDevice
				: public IDevice
			{
			public:
				CDevice();
				virtual ~CDevice();

			public:
				virtual void LostDevice() override;
				virtual CResult ResetDevice( int nBackBufferWidth, int nBackBufferHeight, bool bIsWindow ) override;
				virtual void CreateTexture( ITexture** ppTexture ) override;
				virtual CResult CreateVertexBuffer( uint32 nInSize, uint32 nUsage, IVertexBuffer** ppOutBuffer ) override;
				virtual CResult CreateIndexBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage, IIndexBuffer** ppOutBuffer ) override;
				virtual CResult CreateShader( IShader** ppOutShader );
				virtual CResult CreateVertexDeclaration( IVertexDeclaration** ppOutDeclaration, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride ) override;
				virtual CResult CreateShaderState( IShaderState** ppOutShaderState, IShader* pShader, IVertexDeclaration* pVertexDeclaration ) override;
				virtual void CreateShaderVariableTable( IShaderVariableTable** ppOutShaderVariableTable ) override;
				virtual void CreateCommandContext( ICommandContext** ppOutContext ) override;
				virtual void Begin() override;
				virtual void End() override;

			public:
				CResult CreateDevice( HWND hWnd, bool bWindowed, int nSuitedWidth, int nSuitedHeight );

				void BindVertexBuffer( IDirect3DVertexBuffer11* pD3DVertexBuffer, uint32 stride );
				void BindIndexBuffer( IDirect3DIndexBuffer9* pD3DIndexBuffer );
				void SetVertexShader( IDirect3DVertexShader9* pD3DVertexShader );
				void SetPixelShader( IDirect3DPixelShader9* pD3DVertexShader );
				void SetTexture( uint32 nStage, IDirect3DTexture9* pD3DTexture );

			public:
				void SetSamplerState( uint32 nStage, const UTextureSamplerState& SamplerState );

			public:
				ID3D11Device* GetD3DDevice() { return m_pD3DDevice; }
				// Ó²¼þ´¦Àí
				bool IsHardwareVertexProcessing() { return m_bIsHardwareVertexProcessing; }

			protected:
				CResult ResetDevice() { return ResetDevice( m_ContextSize.w, m_ContextSize.h, m_bIsWindow ); }

			protected:
				HWND m_hWnd;
				HINSTANCE m_hInstance;
				IDXGIFactory1* m_DXGIFactory;             // DXGI Factory object
				IDXGIAdapter1* m_DXGIAdapter;            // The DXGI adapter object for the D3D11 device
				IDXGIOutput** m_DXGIOutputArray;        // The array of output obj for the D3D11 adapter obj
				UINT                    m_DXGIOutputArraySize;    // Number of elements in m_D3D11OutputArray
				IDXGISwapChain* m_DXGISwapChain;          // the D3D11 swapchain
				DXGI_SURFACE_DESC       m_BackBufferSurfaceDescDXGI; // D3D11 back buffer surface description
				bool                    m_RenderingOccluded;       // Rendering is occluded by another window
				bool                    m_DoNotStoreBufferSize;    // Do not store the buffer size on WM_SIZE messages

				// D3D11 specific
				ID3D11Device* m_D3D11Device;             // the D3D11 rendering device
				ID3D11DeviceContext* m_D3D11DeviceContext;	   // the D3D11 immediate device context
				D3D_FEATURE_LEVEL		m_D3D11FeatureLevel;	   // the D3D11 feature level that this device supports
				ID3D11Texture2D* m_D3D11DepthStencil;       // the D3D11 depth stencil texture (optional)
				ID3D11DepthStencilView* m_D3D11DepthStencilView;   // the D3D11 depth stencil view (optional)
				ID3D11RenderTargetView* m_D3D11RenderTargetView;   // the D3D11 render target view
				ID3D11RasterizerState* m_D3D11RasterizerState;    // the D3D11 Rasterizer state

				ID3D11Device* m_pD3DDevice;

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
