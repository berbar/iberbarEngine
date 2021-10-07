#pragma once

#include <iberbar/RHI/Shader.h>

namespace iberbar
{
	namespace RHI
	{
		class IShaderVariableTable;

		class __iberbarRHIApi__ IShaderState
			: public CRef
		{
		public:
			virtual IShader* GetShader( EShaderType eShaderType ) = 0;
		};
	}
}
