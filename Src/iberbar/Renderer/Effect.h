#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Utility/Result.h>

namespace iberbar
{
	namespace RHI
	{
		class IUniformBuffer;
	}

	namespace Renderer
	{

		class __iberbarRendererApi__ CEffectBase
			: public CRef
		{
		public:
			CEffectBase();
			virtual ~CEffectBase();

		protected:
			CResult InitialRhiUniformBuffer( uint32 nSize );

		protected:
			RHI::IUniformBuffer* m_pRhiUniformBuffer;
		};


	}
}
