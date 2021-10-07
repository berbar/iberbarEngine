#pragma once


#include <iberbar/RHI/OpenGL/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		class IDevice;
	}
}

extern "C" __iberbarRHIOpenGLApi__ iberbar::RHI::IDevice* iberbarRhiDeviceCreate();