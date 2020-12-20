#pragma once

#include <iberbar/Poster/Elements/ElementBase.h>
#include <iberbar/Poster/Elements/XmlCommon.h>
#include <iberbar/Utility/Xml/Base.h>
#include <list>
#include <unordered_map>


namespace iberbar
{
	namespace Poster
	{
		class CResourcesManager;
		class CXmlEngine;
		class CXmlReadProcFactory;
		class CXmlReadMessageBuffer;

		struct UXmlReadingMessage
		{
			int code;
			std::string text;

		public:
			UXmlReadingMessage( int code, const std::string& text )
				: code( code )
				, text( text )
			{

			}
		};

		typedef CResult (*PCallbackXmlReadElementProc)( UXmlReadProcContext& context );

		// 元素读取工厂
		class __iberbarExports__ CXmlReadProcFactory
		{
		public:
			void AddReadElementProc( const std::string& type, PCallbackXmlReadElementProc proc ) { m_readElementProcList.insert( std::make_pair( type, proc ) ); }
			PCallbackXmlReadElementProc GetReadElementProc( const std::string& type ) const
			{
				auto proc = m_readElementProcList.find( type );
				if ( proc == m_readElementProcList.end() )
					return nullptr;
				return proc->second;
			}

		private:
			std::unordered_map< std::string, PCallbackXmlReadElementProc > m_readElementProcList;
		};

		// xml读写引擎
		class __iberbarExports__ CXmlEngine
		{
		public:
			CXmlEngine();

		public:
			CResult Read( const std::string& str, CElementBase** ppOutElementRoot, CXmlProcMessageBuffer* pOutMessages = nullptr );

			CResult ReadFromFile( const std::string& filepath, CElementBase** ppOutElementRoot, CXmlProcMessageBuffer* pOutMessages = nullptr );

		public:
			void SetResourcesManager( CResourcesManager* pManager ) { m_resourcesManager = pManager; }

			CXmlReadProcFactory* GetReadProcFactory() { return &m_readProcFactory; }

		private:
			CResourcesManager* m_resourcesManager;
			CXmlReadProcFactory m_readProcFactory;
		};
	}
}
