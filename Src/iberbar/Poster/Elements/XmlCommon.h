#pragma once

#include <iberbar/Utility/Result.h>
#include <iberbar/Utility/Xml/Base.h>
#include <list>

namespace iberbar
{
	namespace Poster
	{

		class CResourcesManager;
		class CElementBase;

		enum class UXmlProcMessageCode
			: int
		{
			Debug,
			Info,
			Warning,
			Error,
		};

		struct __iberbarExports__ UXmlProcMessage
		{
		public:
			UXmlProcMessageCode code;
			std::string text;

		public:
			UXmlProcMessage( UXmlProcMessageCode code, const std::string& text )
				: code( code )
				, text( text )
			{
			}
		};

		class __iberbarExports__ CXmlProcMessageBuffer
		{
		private:
			std::list<UXmlProcMessage> m_messages;

		public:
			void AddMessage( UXmlProcMessageCode code, const char* format, ... );

			const std::list<UXmlProcMessage>& GetMessageList() { return m_messages; }
		};

		struct __iberbarExports__ UXmlReadProcContext
		{
		public:
			CResourcesManager* pResources;
			CElementBase** ppOutElement;
			CXmlProcMessageBuffer* pMessagesBuffer;
			Xml::PTR_CNodeA node;

		public:
			UXmlReadProcContext()
				: pResources( nullptr ) 
				, ppOutElement( nullptr )
				, pMessagesBuffer( nullptr )
				, node( nullptr )
			{
			}
		};
	}
}
