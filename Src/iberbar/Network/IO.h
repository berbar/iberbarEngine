#pragma once

#include <iberbar/Network/Headers.h>
#include <iberbar/Utility/Unknown.h>
#include <iberbar/Utility/Result.h>
#include <string>
#include <functional>
#include <optional>

namespace iberbar
{
	namespace IO
	{
        class ISocketClient;
        class IHttpClient;

        struct USocketOptions
        {
            bool bEnableRead;
            bool bEnableWrite;
            const char* pHead;
            const char* pTail;
            size_t nHeadLen;
            size_t nTailLen;
        };

        struct UHttpClientOptions
        {
        };

        __iberbarNetworkApis__ CResult Initial();
        __iberbarNetworkApis__ void Destroy();
        __iberbarNetworkApis__ void Read();
        __iberbarNetworkApis__ CResult CreateSocketClient( ISocketClient** ppOutClient, const std::string& strUrl, const USocketOptions& Options );
        __iberbarNetworkApis__ CResult CreateHttpClient( IHttpClient** ppOutClient, const std::string& strHostName, const UHttpClientOptions& Options );


        typedef std::function<void ( ISocketClient*, const char*, size_t )> UFunctionSocketListen;

        class __iberbarNetworkApis__ ISocketClient abstract
            : public CRef
        {
        public:
            ISocketClient() {}
            virtual ~ISocketClient() {}
            virtual void Reconnect() = 0;
            virtual void Disconnect() = 0;
            virtual void Send( const char* pData, size_t nSize ) = 0;
            virtual void Listen( UFunctionSocketListen func ) = 0;
        };

        struct UHttpRequestOptions;
        struct UHttpResponse;
        typedef std::function<void ( IHttpClient*, const UHttpResponse* )> UFunction_OnHttpResponse;

        enum class UHttpMethod
        {
            Get,
            Post,
            Delete
        };

        struct UHttpRequestOptions
        {
            UHttpMethod Method;
            std::optional<std::string> Data;
            std::optional<UFunction_OnHttpResponse> OnResponse;
        };
        struct UHttpResponse
        {
        };
        

        class __iberbarNetworkApis__ IHttpClient abstract
            : public CRef
        {
        public:
            virtual ~IHttpClient() {}
            virtual void AddHeaders() = 0;
            virtual void Send( const UHttpRequestOptions& pReqOptions ) = 0;
        };


        class __iberbarNetworkApis__ IRunnable abstract
        {
        public:
            virtual void Trigger() = 0;
            virtual void Run() = 0;
        };
	}
}
