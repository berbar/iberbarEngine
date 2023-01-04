#pragma once

#include <iberbar/Javascript/Headers.h>


namespace iberbar
{
	namespace iJavascript
	{
		class __iberbarJavascriptApi__ IJsModuleLoader abstract
		{
		public:
			virtual bool Search(
				const std::string& RequiringDir, const std::string& ModuleName,
				std::string& OutPath, std::string& OutAbsolutePath ) = 0;
			virtual bool Load( const std::string& Path, std::string& OutData ) = 0;
		};


		class __iberbarJavascriptApi__ CDefaultModuleLoader
			: public IJsModuleLoader
		{
		public:
			virtual bool Search(
				const std::string& RequiringDir, const std::string& ModuleName,
				std::string& OutPath, std::string& OutAbsolutePath ) override;
			virtual bool Load( const std::string& Path, std::string& OutData ) override;

		private:
			bool SearchModuleInDir(
				const std::string& Dir, const std::string& RequiredModule,
				std::string& OutPath, std::string& OutAbsolutePath );
			bool SearchModuleWithExtInDir(
				const std::string& Dir, const std::string& RequiredModule,
				std::string& OutPath, std::string& OutAbsolutePath );
			bool CheckExists( const std::string& InPath, std::string& OutPath, std::string& OutAbsolutePath );
		};
	}
}