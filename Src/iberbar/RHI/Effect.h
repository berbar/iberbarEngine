#pragma once

#include <iberbar/RHI/Resource.h>

namespace iberbar
{
	namespace RHI
	{
		class IShader;

		class IEffect abstract
			: public IResource
		{
		public:
			IEffect()
				: IResource( UResourceType::Effect )
			{
			}

		public:
			virtual void SetShader( EShaderType eShaderType, IShader* pShader ) = 0;
			virtual CResult Generate() = 0;
		};
	}
}
