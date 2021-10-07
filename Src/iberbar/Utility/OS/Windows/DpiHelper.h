#pragma once

#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Result.h>

namespace iberbar
{
	namespace OS
	{
		enum class EDpiAwareness
		{
			Unaware,
			System
		};

		class __iberbarUtilityApi__ CDpiHelper
		{
		public:
			CDpiHelper();

			CResult SetAwareness( EDpiAwareness awareness );
			EDpiAwareness GetAwareness() const { return m_eAwareness; }
			int GetDpi() const;
			int GetScaleFactor() const;

		public:
			EDpiAwareness m_eAwareness;
			int m_nScaleFactor;
		};
	}
}
