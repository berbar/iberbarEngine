#pragma once


#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/RenderState.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CBlendState
				: public IBlendState
			{
			public:
				CBlendState( const UBlendDesc& Desc )
					: IBlendState( Desc )
				{
				}
			};


			class CDepthStencilState
				: public IDepthStencilState
			{
			public:
				CDepthStencilState( const UDepthStencilDesc& Desc )
					: IDepthStencilState( Desc )
				{
				}
			};


			class CSamplerState
				: public ISamplerState
			{
			public:
				CSamplerState( const UTextureSamplerState& SamplerDesc )
					: ISamplerState( SamplerDesc )
				{
				}
			};
		}
	}
}

