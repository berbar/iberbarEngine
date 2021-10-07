#pragma once

#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Result.h>

namespace iberbar
{
	namespace OS
	{
		class __iberbarUtilityApi__ CDynamicLibrary
		{
		public:
			CDynamicLibrary();
			~CDynamicLibrary();

		public:
			CResult Load( const char* strLibName );
			void Free();
			void* GetProcAddress( const char* strProcName );

		private:
			std::string m_strLibName;
			HMODULE m_hModule;
		};
	}
}
