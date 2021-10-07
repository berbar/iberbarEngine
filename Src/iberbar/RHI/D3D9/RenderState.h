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
					memcpy_s( &m_Desc, sizeof( UBlendDesc ), &Desc, sizeof( UBlendDesc ) );
				}
			};


			//class CDepthStencilState
			//	: public IDepthStencilState
			//{
			//public:
			//	virtual bool Equal( const IDepthStencilState* Other ) override;
			//};


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

