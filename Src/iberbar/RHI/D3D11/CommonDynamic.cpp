


#include <iberbar/RHI/D3D11/Device.h>


extern "C" __iberbarD3DApi__ iberbar::RHI::IDevice * iberbarRhiDeviceCreate()
{
	return new iberbar::RHI::D3D11::CDevice();
}

