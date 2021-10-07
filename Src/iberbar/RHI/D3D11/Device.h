#pragma once

#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/D3D11/Types.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CSamplerState;

			class CDevice
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
				virtual void SetClearColor( const CColor4B& color ) override;

			public:
				CResult CreateDevice( HWND hWnd, bool bWindowed, int nSuitedWidth, int nSuitedHeight );

				void BindVertexBuffer( ID3D11Buffer* pD3DVertexBuffer, uint32 stride );
				void BindIndexBuffer( ID3D11Buffer* pD3DIndexBuffer );
				void SetVertexShader( ID3D11VertexShader* pD3DShader );
				void SetPixelShader( ID3D11PixelShader* pD3DShader );
				void SetHullShader( ID3D11HullShader* pD3DShader );
				void SetGeometryShader( ID3D11GeometryShader* pD3DShader );
				void SetDomainShader( ID3D11DomainShader* pD3DShader );
				void SetComputeShader( ID3D11ComputeShader* pD3DShader );

				void SetTexture( uint32 nStage, ID3D11ShaderResourceView* pD3DShaderResourceView );
				void SetSamplerState( uint32 nStage, ID3D11SamplerState* pD3DSamplerState );

			public:
				ID3D11Device* GetD3DDevice() { return m_pD3DDevice.Get(); }
				ID3D11DeviceContext* GetD3DDeviceContext() { return m_pD3DDeviceContext.Get(); }
				// Ó²¼þ´¦Àí
				//bool IsHardwareVertexProcessing() { return m_bIsHardwareVertexProcessing; }

			protected:
				CResult ResetDevice() { return ResetDevice( m_ContextSize.w, m_ContextSize.h, m_bIsWindow ); }

			protected:
				HWND m_hWnd;
				HINSTANCE m_hInstance;
				float m_D3DClearColorRGBA[ 4 ];
				ComPtr<IDXGIFactory> m_pDXGIFactory;             // DXGI Factory object
				ComPtr<IDXGIAdapter1> m_pDXGIAdapter;            // The DXGI adapter object for the D3D11 device
				//ComPtr<IDXGIOutput** m_DXGIOutputArray;        // The array of output obj for the D3D11 adapter obj
				UINT                    m_DXGIOutputArraySize;    // Number of elements in m_D3D11OutputArray
				ComPtr<IDXGISwapChain> m_pDXGISwapChain;          // the D3D11 swapchain
				//DXGI_SURFACE_DESC       m_BackBufferSurfaceDescDXGI; // D3D11 back buffer surface description
				//bool                    m_RenderingOccluded;       // Rendering is occluded by another window
				//bool                    m_DoNotStoreBufferSize;    // Do not store the buffer size on WM_SIZE messages

				// D3D11 specific
				D3D_FEATURE_LEVEL		m_D3DFeatureLevel;
				ComPtr<ID3D11Device> m_pD3DDevice;
				ComPtr<ID3D11DeviceContext> m_pD3DDeviceContext;
				ComPtr<ID3D11Texture2D> m_pD3DDepthStencil;
				ComPtr<ID3D11DepthStencilState> m_pD3DDepthStencilState;
				ComPtr<ID3D11DepthStencilView> m_pD3DDepthStencilView;
				ComPtr<ID3D11RenderTargetView> m_pD3DRenderTargetView;
				ComPtr<ID3D11RasterizerState> m_pD3DRasterizerState;

				bool m_bEnableMultisampleQuality;
				UINT m_nMultisampleQualityLevels;
				//ID3D11Device* m_pD3DDevice;

				ID3D11Buffer* m_pD3DVertexBuffers[ MAX_VERTEX_BUFFERS_COUNT ];
				uint32 m_nVertexBufferStride;
				ComPtr<ID3D11Buffer> m_pD3DIndexBuffer;
				//IDirect3DVertexShader9* m_pD3DVertexShader;
				//IDirect3DPixelShader9* m_pD3DPixelShader;

				//bool m_bIsHardwareVertexProcessing;
				//bool m_bIsRendering;
				//bool m_bHasLostDevice;

				ComPtr<ID3D11ShaderResourceView> m_pD3DTextures[ 8 ];
				ComPtr<ID3D11SamplerState> m_pD3DSamplerStates[ 8 ];

				std::vector<CSamplerState*> m_SamplerStatesCache;
				//D3DPRESENT_PARAMETERS m_D3DPresentParams;
			};
		}
	}

}
