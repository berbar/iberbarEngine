#pragma once

#include <iberbar/RHI/Shader.h>

namespace iberbar
{
	namespace RHI
	{
		class IVertexDeclaration;

		class __iberbarRHIApi__ IShaderState
			: public CRef
		{
		public:
			virtual IShaderProgram* GetShaderProgram() = 0;
			virtual IVertexDeclaration* GetVertexDeclaration() = 0;
		};
	}
}
