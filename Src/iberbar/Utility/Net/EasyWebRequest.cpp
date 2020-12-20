
#include "EasyWebRequest.h"
#include <cassert>





iberbar::Net::CEasyWebResponse::CEasyWebResponse()
	: m_pData( nullptr )
	, m_offset( 0 )
	, m_nDataSize( 0 )
{
}


iberbar::Net::CEasyWebResponse::~CEasyWebResponse()
{
	if ( m_pData != nullptr )
	{
		delete[] m_pData;
		m_pData = nullptr;
	}
}

void iberbar::Net::CEasyWebResponse::Write( void* data, size_t len )
{
	size_t sizeNew = m_offset + len + 1;

	// 初始化内存
	if ( m_pData == nullptr )
	{
		m_pData = new byte[sizeNew];
		m_nDataSize = sizeNew;
	}
	// 重新分配内存
	else if ( sizeNew > m_nDataSize )
	{
		byte* dataOld = (byte*)m_pData;
		m_pData = new byte[sizeNew];
		m_nDataSize = sizeNew;
		memcpy( m_pData, dataOld, m_offset );
		delete[] dataOld;
		dataOld = nullptr;
	}

	memcpy( (byte*)m_pData + m_offset, data, len );
	m_offset += len;
}

void iberbar::Net::CEasyWebResponse::Reset()
{
	m_offset = 0;
}


iberbar::Net::UEasyWebRequestOptions::UEasyWebRequestOptions()
	: url( "" )
	, method( UEasyWebRequestMethod::GET )
{

}




iberbar::Net::CEasyWebRequest::CEasyWebRequest()
	: m_context( nullptr )
	, m_url( nullptr )
{
}

iberbar::Net::CEasyWebRequest::~CEasyWebRequest()
{
	assert( m_context == nullptr );
}

void iberbar::Net::CEasyWebRequest::Init()
{
	m_context = curl_easy_init();
	m_url = curl_url();
	curl_easy_setopt( m_context, CURLOPT_CURLU, m_url );
	//curl_easy_setopt( m_context, CURLOPT_VERBOSE, 1L );
	curl_easy_setopt( m_context, CURLOPT_WRITEFUNCTION, &sOnWriteResponse );
	curl_easy_setopt( m_context, CURLOPT_WRITEDATA, this );
	curl_easy_setopt( m_context, CURLOPT_HEADERFUNCTION, &sOnWriteResponseHeader );
	curl_easy_setopt( m_context, CURLOPT_HEADERDATA, this );
}

void iberbar::Net::CEasyWebRequest::Destory()
{
	if ( m_context != nullptr )
	{
		curl_easy_cleanup( m_context );
		m_context = nullptr;
	}
	if ( m_url != nullptr )
	{
		curl_url_cleanup( m_url );
		m_url = nullptr;
	}
}

void iberbar::Net::CEasyWebRequest::Send( const UEasyWebRequestOptions& options, bool receive )
{
	m_response.Reset();

	if ( options.ignoreSSL == true )
	{
		curl_easy_setopt( m_url, CURLOPT_SSL_VERIFYPEER, 0L );//忽略证书检查
		curl_easy_setopt( m_url, CURLOPT_SSL_VERIFYHOST, 0L );
	}

	CURLUcode retSetUrl = curl_url_set( m_url, CURLUPART_URL, options.url.c_str(), 0 );
	if ( retSetUrl != CURLcode::CURLE_OK )
	{
		return;
	}

	CURLcode ret = curl_easy_perform( m_context );
	/* Check for errors */
	if ( ret == CURLE_OK )
	{
		char* ct;
		curl_easy_getinfo( m_context, CURLINFO_CONTENT_TYPE, &ct );
		//fprintf( stdout, "curl_easy_perform() succeed: %s\n", responseData.data );
		//fprintf( stdout, "Content-Type:%s", ct );
	}
	else
	{
		//fprintf( stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror( ret ) );
	}
}


//处理从服务器返回的数据，将数据拷贝到arg中
size_t iberbar::Net::CEasyWebRequest::sOnWriteResponse( void* ptr, size_t n, size_t m, void* data )
{
	CEasyWebRequest* request = (CEasyWebRequest*)data;
	size_t size = n * m;
	request->m_response.Write( ptr, size );
	return size;
	//return fwrite( ptr, n, m, writehere );
}

size_t iberbar::Net::CEasyWebRequest::sOnWriteResponseHeader( void* ptr, size_t n, size_t m, void* data )
{
	CEasyWebRequest* request = (CEasyWebRequest*)data;
	size_t size = n * m;
	return size;
}