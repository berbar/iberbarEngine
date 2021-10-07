
#include <iberbar/RHI/OpenGL/CommonDynamic.h>
#include <iberbar/RHI/OpenGL/Device.h>


extern "C" __iberbarRHIOpenGLApi__ iberbar::RHI::IDevice* iberbarRhiDeviceCreate()
{
	return new iberbar::RHI::OpenGL::CDevice();
}

