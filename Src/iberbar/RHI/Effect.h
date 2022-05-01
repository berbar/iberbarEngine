#pragma once

#include <iberbar/RHI/Resource.h>

namespace iberbar
{
	namespace RHI
	{
		class IShaderState;
		class IShaderVariableTable;

		class IEffect abstract
			: public IResource
		{
		public:
			IEffect()
				: IResource( UResourceType::Effect )
			{
			}
		public:
			virtual void SetShaderVariables( EShaderType nShaderType, IShaderVariableTable* pShaderVariables ) = 0;
		};
	}
}
