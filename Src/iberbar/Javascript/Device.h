#pragma once

#include <iberbar/Javascript/Headers.h>
#include <iberbar/Utility/Result.h>


namespace iberbar
{
	namespace iJavascript
	{

		class CDevice
		{
		public:
			CDevice();
			~CDevice();

		public:
			CResult Initial();
			void Shutdown();

			void ExecuteFile( const char* pstrFilePath );

			v8::Isolate* GetMainIsolate() { return m_MainIsolate; }

		public:
			static CDevice* sGetInstance() {}
		private:
			static CDevice* sm_pInstance;

		protected:
			v8::Platform* m_DefaultPlatform;
			std::unique_ptr<v8::Platform> m_Platform;
			v8::Isolate* m_MainIsolate = nullptr;
			v8::Global<v8::Context> m_DefaultContext;
			v8::Local<v8::ObjectTemplate> m_GlobalObjs;
		};
	}
}