#pragma once

#include <iberbar/RHI/Headers.h>
#include <iberbar/RHI/Resource.h>

namespace iberbar
{
	namespace RHI
	{

		class __iberbarRHIApi__ IBlendState
			: public IResource
		{
		public:
			IBlendState( const UBlendDesc& Desc );
			bool Equal( const IBlendState* Other );
			const UBlendDesc& GetDesc() const { return m_Desc; }

		protected:
			UBlendDesc m_Desc;
		};


		class __iberbarRHIApi__ IDepthStencilState
			: public IResource
		{
		public:
			IDepthStencilState(const UDepthStencilDesc& Desc);
			bool Equal(const IDepthStencilState* Other) const;
			const UDepthStencilDesc& GetDesc() const { return m_Desc; }

		protected:
			UDepthStencilDesc m_Desc;
		};


		class __iberbarRHIApi__ ISamplerState
			: public IResource
		{
		public:
			ISamplerState( const UTextureSamplerState& SamplerStateDesc );
			bool Equal( const ISamplerState* Other ) const;
			const UTextureSamplerState& GetSamplerDesc() const { return m_SamplerStateDesc; }

		protected:
			UTextureSamplerState m_SamplerStateDesc;
		};
	}
}



FORCEINLINE bool iberbar::RHI::IBlendState::Equal( const IBlendState* Other )
{
	return memcmp( &m_Desc, &Other->m_Desc, sizeof( UBlendDesc ) ) == 0;
}


FORCEINLINE bool iberbar::RHI::IDepthStencilState::Equal(const IDepthStencilState* Other) const
{
	return memcmp(&m_Desc, &Other->m_Desc, sizeof(UDepthStencilDesc)) == 0;
}


FORCEINLINE bool iberbar::RHI::ISamplerState::Equal( const ISamplerState* Other ) const
{
	return m_SamplerStateDesc == Other->m_SamplerStateDesc;
}

