#pragma once


#include "../SocketClient.h"
#include <iberbar/Utility/Result.h>

struct addrinfo;
struct event_base;
struct bufferevent;

namespace iberbar
{
	namespace IO
	{
        class CAddrInfoHelper
        {
        public:
            CAddrInfoHelper();
            ~CAddrInfoHelper();

            const addrinfo* Set( const char* strNodeName, const char* pServiceName, const addrinfo* pHints );
            const addrinfo* Set_Socket( const std::string& url );
            const addrinfo* Get() const;

        private:
            addrinfo* m_pResult;
        };


        class CSocketClient_UseLibevent
            : public ISocketClient
        {
        public:
            CSocketClient_UseLibevent( event_base* base, const std::string& strUrl, const USocketOptions& Opts );
            virtual ~CSocketClient_UseLibevent();

            virtual void Reconnect() override;
            virtual void Disconnect() override;
            virtual void Send( const char* pData, size_t nSize ) override;
            virtual void Listen( UFunctionSocketListen Func ) override;

            CResult Connect();
            void SendInner( const char* pData, size_t nSize );
            void ExecuteListenerInner( const char* pData, size_t nSize );

        private:
            void OnConnectEvent_Inner( short nEvents );


        private:
            event_base* m_base;
            bufferevent* m_pBufferEvent;
            std::string m_strUrl;
            bool m_bSocketRead;
            bool m_bSocketWrite;
            char* m_pHead;
            size_t m_nHeadLen;
            char* m_pTail;
            size_t m_nTailLen;
            UFunctionSocketListen m_Listener;

        private:
            static void OnSocketConnWrite( bufferevent*, void* );
            static void OnSocketConnRead( bufferevent*, void* );
            static void OnSocketConnEvent( bufferevent*, short, void* );
            static int sSocketProtocol_FindHead( const char* pFd, size_t nFdLen, size_t nFdStart, const char* pHead, size_t nHeadLen, int& nMaybe );
        };
	}
}
