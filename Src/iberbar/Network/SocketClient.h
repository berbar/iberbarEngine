#pragma once

#include <iberbar/Utility/Ref.h>
#include <functional>

namespace iberbar
{
	namespace IO
	{
        class ISocketClient;

        typedef std::function<void ( ISocketClient*, const char*, size_t )> UFunctionSocketListen;

        struct USocketOptions
        {
            bool bEnableRead;
            bool bEnableWrite;
            const char* pHead;
            const char* pTail;
            size_t nHeadLen;
            size_t nTailLen;
        };

        class ISocketClient abstract
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
	}
}
