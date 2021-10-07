#pragma once


#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/RenderState.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CDevice;

			class CBlendState
				: public IBlendState
			{
			public:
				CBlendState( const UBlendDesc& Desc )
					: IBlendState( Desc )
					, m_pD3DBlendState( nullptr )
				{
				}

				CResult Create( CDevice* pDevice );
				FORCEINLINE ID3D11BlendState* GetD3DBlendState() { return m_pD3DBlendState.Get(); }

			protected:
				ComPtr<ID3D11BlendState> m_pD3DBlendState;
			};



			class CSamplerState
				: public ISamplerState
			{
			public:
				CSamplerState( const UTextureSamplerState& SamplerDesc )
					: ISamplerState( SamplerDesc )
					, m_pD3DSamplerState( nullptr )
				{
				}

				CResult Create( CDevice* pDevice );
				FORCEINLINE ID3D11SamplerState* GetD3DSamplerState() { return m_pD3DSamplerState.Get(); }

			protected:
				ComPtr<ID3D11SamplerState> m_pD3DSamplerState;
			};
		}
	}
}
