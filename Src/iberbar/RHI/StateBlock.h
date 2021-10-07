#pragma once


#include <iberbar/RHI/Resource.h>
#include <iberbar/RHI/Types.h>


namespace iberbar
{
	namespace RHI
	{
		class __iberbarRHIApi__ CStateBlock
			: public IResource
		{
		public:
			CStateBlock();
			virtual CResult Capture() = 0;
			virtual CResult Apply() = 0;
		};
	}
}
