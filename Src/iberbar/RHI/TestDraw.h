#pragma once

#ifdef _DEBUG

#include <iberbar/RHI/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		class __iberbarRHIApi__ ITestDraw
		{
		public:
			ITestDraw() {}
			virtual ~ITestDraw() {}
			virtual void Draw() = 0;
		};
	}
}

#endif
