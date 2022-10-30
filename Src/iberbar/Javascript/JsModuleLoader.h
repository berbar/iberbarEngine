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
				std::string& OutPath, std::string& OutDebugPath ) = 0;
			virtual bool Load( const std::string& Path, std::string& OutData ) = 0;
		};
	}
}