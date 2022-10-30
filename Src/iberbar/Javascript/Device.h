#pragma once

#include <iberbar/Javascript/Headers.h>
#include <iberbar/Utility/Result.h>


namespace iberbar
{
	namespace iJavascript
	{

		class ILogger;
		class IJsModuleLoader;

		class __iberbarJavascriptApi__ CDevice
		{
		public:
			CDevice();
			~CDevice();

		public:
			CResult Initial();
			void Shutdown();

			void ExecuteFile( const char* pstrFilePath );
			CResult ExecuteScript( const char* pstrScript );

			v8::Isolate* GetMainIsolate() { return m_MainIsolate; }

		private:
			void DumpStatisticsLog( const v8::FunctionCallbackInfo<v8::Value>& Info );
			void FindModule( const v8::FunctionCallbackInfo<v8::Value>& Info );
			void LoadModule( const v8::FunctionCallbackInfo<v8::Value>& Info );

			CResult LoadFile( const std::string& RequiringDir, const std::string& ModuleName,
				std::string& OutPath, std::string& OutDebugPath,
				std::string& Data );

		public:
			static CDevice* sGetInstance() { return sm_pInstance; }
		private:
			static CDevice* sm_pInstance;

		protected:
			v8::Platform* m_DefaultPlatform;
			std::unique_ptr<v8::Platform> m_Platform;
			v8::Isolate* m_MainIsolate = nullptr;
			v8::Global<v8::Context> m_DefaultContext;
			v8::Local<v8::ObjectTemplate> m_GlobalObjs;

			ILogger* m_pLogger;
			IJsModuleLoader* m_pModuleLoader;
		};
	}
}