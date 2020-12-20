#pragma once

#include <curl/curl.h>
#include <string>
#include <iberbar/Utility/Platform.h>


namespace iberbar
{
	namespace Net
	{

		class CEasyWebResponse;
		struct UEasyWebRequestOptions;
		class CEasyWebRequest;

		class CEasyWebHeader
		{

		};

		class __iberbarExports__ CEasyWebResponse
		{
		public:
			struct _Binary
			{
			public:
				_Binary() : data( nullptr ), size( 0 ) {}
				_Binary( const byte* bytes, size_t s ) : data( bytes ), size( s ) {}
			public:
				const byte* data;
				size_t size;
			};

			struct _Text
			{
			public:
				_Text( const char* text ) : text( text ) {}
			public:
				const char* text;
			};

		public:
			CEasyWebResponse();
			~CEasyWebResponse();

		public:
			void Write( void* data, size_t len );

			void Reset();

			_Binary ToBinary() const
			{
				return _Binary( (byte*)m_pData, m_offset );
			}

			_Text ToText() const
			{
				((char*)m_pData)[m_offset] = 0;
				return _Text( (char*)m_pData );
			}

		private:
			void* m_pData;
			size_t m_offset;
			size_t m_nDataSize;
		};

		enum class UEasyWebRequestMethod
		{
			GET = 0,
			POST = 1
		};

		struct __iberbarExports__ UEasyWebRequestOptions
		{
		public:
			UEasyWebRequestOptions();

		public:
			UEasyWebRequestMethod method;
			std::string url;
			bool ignoreSSL;
		};

		class __iberbarExports__ CEasyWebRequest
		{
		protected:
			struct UWriteFunctionBinder
			{
				CEasyWebRequest* request;
				bool receive;
			};

		public:
			CEasyWebRequest();
			~CEasyWebRequest();

		public:
			void Init();
			void Destory();

			void Send( const UEasyWebRequestOptions& options, bool receive = true );

			const CEasyWebResponse* GetResponse() { return &m_response; }

		protected:
			CURL* m_context;
			CURLU* m_url;
			CEasyWebResponse m_response;

		protected:
			static size_t sOnWriteResponse( void* ptr, size_t n, size_t m, void* arg );
			static size_t sOnWriteResponseHeader( void* ptr, size_t n, size_t m, void* data );
		};

	}
}
