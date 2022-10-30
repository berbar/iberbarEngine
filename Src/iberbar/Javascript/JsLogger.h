#pragma once

#include <iberbar/Javascript/Headers.h>

namespace iberbar
{
	namespace iJavascript
	{
		class __iberbarJavascriptApi__ ILogger abstract
		{
		public:
			virtual void Log( const std::string& strMessage ) const = 0;
			virtual void Info( const std::string& strMessage ) const = 0;
			virtual void Warn( const std::string& strMessage ) const = 0;
			virtual void Error( const std::string& strMessage ) const = 0;
		};


		class __iberbarJavascriptApi__ CDefaultLogger
			: public ILogger
		{
		public:
			virtual void Log( const std::string& strMessage ) const override;
			virtual void Info( const std::string& strMessage ) const override;
			virtual void Warn( const std::string& strMessage ) const override;
			virtual void Error( const std::string& strMessage ) const override;
		};
	}
}
