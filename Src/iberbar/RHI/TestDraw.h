#pragma once

#ifdef _DEBUG

#include <iberbar/RHI/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
		class IShaderProgram;

		class __iberbarRHIApi__ ITestDraw
		{
		public:
			ITestDraw() {}
			virtual ~ITestDraw() {}
			virtual void Draw() = 0;
			virtual void SetShaderProgram( IShaderProgram* pShaderProgram ) = 0;
			virtual void SetTexture( ITexture* pTexture ) = 0;
		};
	}
}

#endif
