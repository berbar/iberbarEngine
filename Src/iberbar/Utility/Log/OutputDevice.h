#pragma once

#include <iberbar/Utility/Log/BaseTypes.h>
#include <iberbar/Utility/Ref.h>
#include <iberbar/Utility/Result.h>

namespace iberbar
{

	namespace Logging
	{

		class __iberbarUtilityApi__ COutputDevice abstract
			: public CRef
		{
		public:
			virtual ~COutputDevice() {}
			virtual void Serialize( ULevel nLevel, const char* strText, const char* strCategory, int64 nTime = -1 ) = 0;
		};
	}
}



