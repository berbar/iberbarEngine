
#include "EasyWebRequestFactory.h"
#include "EasyWebRequest.h"


iberbar::Net::CEasyWebRequestReference::CEasyWebRequestReference()
	: m_request( nullptr )
{
}

iberbar::Net::CEasyWebRequestReference::CEasyWebRequestReference( CEasyWebRequest* request )
	: m_request( request )
{
}

iberbar::Net::CEasyWebRequestReference::~CEasyWebRequestReference()
{
	if ( m_request == nullptr )
		return;

	CEasyWebRequestFactory* factory = CEasyWebRequestFactory::sGetInstance();
	if ( factory == nullptr )
		delete m_request;
	else
		factory->ReturnRequest( m_request );
}




iberbar::Net::CEasyWebRequestFactory* s_factory = nullptr;

iberbar::Net::CEasyWebRequest* iberbar::Net::CEasyWebRequestFactory::GetRequest()
{
	CEasyWebRequest* request = nullptr;

	std::thread::id threadId = std::this_thread::get_id();

	request = GetRequestWithLock();
	
	if ( request == nullptr )
	{
		int tryTicks = 0;
		while ( tryTicks < m_options.retryTicks )
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( m_options.retryTimeout ) );

			request = GetRequestWithLock();
			if ( request != nullptr )
				break;

			tryTicks++;
		}
	}

	std::this_thread::yield();

	return request;
}

void iberbar::Net::CEasyWebRequestFactory::ReturnRequest( iberbar::Net::CEasyWebRequest* request )
{
	if ( request == nullptr )
		return;

	m_mutex.lock();

	m_requestsIdle.push_back( request );

	m_mutex.unlock();

	std::this_thread::yield();
}

iberbar::Net::CEasyWebRequest* iberbar::Net::CEasyWebRequestFactory::GetRequestWithLock()
{
	CEasyWebRequest* request = nullptr;

	m_mutex.lock();

	if ( m_requestsIdle.IsEmpty() == false )
	{
		request = m_requestsIdle.front();
		if ( request == nullptr )
		{
			request = new CEasyWebRequest();
			request->Init();
			m_requests.push_back( request );
		}
		m_requestsIdle.pop_front();
	}

	m_mutex.unlock();

	return request;
}


void iberbar::Net::CEasyWebRequestFactory::sInitial( const UOptions& options )
{
	if ( s_factory == nullptr )
	{
		if ( options.initCURL == 1 )
		{
			curl_global_init( CURL_GLOBAL_DEFAULT );
		}

		s_factory = new CEasyWebRequestFactory();
		s_factory->m_options = options;
		//s_factory->m_requests.resize( maxRequestCount );
		//s_factory->m_requestsIdle.resize( maxRequestCount );
		for ( int i = 0; i < options.maxRequestCount; i++ )
		{
			//CEasyWebRequest* request = new CEasyWebRequest();
			//s_factory->m_requests[i] = nullptr;
			s_factory->m_requestsIdle.push_back( nullptr );
		}
	}
}

void iberbar::Net::CEasyWebRequestFactory::sDestory()
{
	if ( s_factory == nullptr )
		return;

	auto iter = s_factory->m_requests.begin();
	auto end = s_factory->m_requests.end();
	for ( ; iter != end; iter++ )
	{
		if ( (*iter) )
		{
			(*iter)->Destory();
			delete (*iter);
			(*iter) = nullptr;
		}
	}
	s_factory->m_requests.Clear();

	if ( s_factory->m_options.initCURL == 1 )
	{
		curl_global_cleanup();
	}

	delete s_factory;
	s_factory = nullptr;
}

iberbar::Net::CEasyWebRequestFactory* iberbar::Net::CEasyWebRequestFactory::sGetInstance()
{
	return s_factory;
}
