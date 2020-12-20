#pragma once

#include <iberbar/Utility/Unknown.h>
#include <iberbar/Utility/Result.h>
#include <mutex>
#include <list>

namespace iberbar
{
	namespace Net
	{
		class CEasyWebRequest;
		class CEasyWebRequestReference;
		class CEasyWebRequestFactory;

		class __iberbarExports__ CEasyWebRequestReference
		{
			friend CEasyWebRequestFactory;

		public:
			CEasyWebRequestReference();
			CEasyWebRequestReference( CEasyWebRequest* request );
			~CEasyWebRequestReference();

		public:
			CEasyWebRequest* Request() { return m_request; }

		private:
			CEasyWebRequest* m_request;
		};

		class __iberbarExports__ CEasyWebRequestFactory
		{
		public:
			struct UOptions
			{
				int maxRequestCount;
				int retryTicks;
				int retryTimeout;
				int initCURL;

				UOptions()
					: maxRequestCount( 5 )
					, retryTicks( 10 )
					, retryTimeout( 500 )
					, initCURL( 1 )
				{

				}
			};

		public:
			CEasyWebRequest* GetRequest();
			void ReturnRequest( CEasyWebRequest* request );

		private:
			CEasyWebRequest* GetRequestWithLock();

		private:
			std::vector<CEasyWebRequest*> m_requests;
			std::list<CEasyWebRequest*> m_requestsBusy;
			std::list<CEasyWebRequest*> m_requestsIdle;
			std::mutex m_mutex;
			UOptions m_options;

		public:
			static void sInitial( const UOptions& options );
			static void sDestory();
			static CEasyWebRequestFactory* sGetInstance();
		//private:
		//	static CEasyWebRequestFactory* s_factory;
		};
	}
}
