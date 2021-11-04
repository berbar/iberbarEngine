#pragma once

#include <iberbar/RHI/Shader.h>

namespace iberbar
{
	namespace RHI
	{
		class IUniformBuffer;

		class __iberbarRHIApi__ IShaderState
			: public CRef
		{
		public:
			virtual IShader* GetShader( EShaderType eShaderType ) = 0;
			virtual IUniformBuffer** GetUniformBuffers( EShaderType eShaderType ) = 0;
		};
	}
}
