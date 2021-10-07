#pragma once

#include <iberbar/RHI/Headers.h>
#include <map>
#include <functional>

namespace iberbar
{
	namespace RHI
	{
		class IResource;

		class __iberbarRHIApi__ CTrace
		{
			friend class IResource;

		public:
			CTrace();

		private:
			void AddResource( IResource* pResource );
			void RemoveResource( IResource* pResource );

		public:
			void ForeachResource( std::function<void(IResource*)> Proc );

		private:
			std::map<uintptr_t, IResource*> m_ResourcesMap;

			static CTrace sm_Instance;

		public:
			static CTrace* sGetInstance()
			{
				return &sm_Instance;
			}
		};
	}
}