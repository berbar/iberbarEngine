#pragma once


#include "../IO.h"
#include <iberbar/Utility/MsgQueue.h>
#include <iberbar/Utility/String.h>


namespace iberbar
{

	namespace IO
	{
        class CSocketClient_UseLibevent;


        // string 使用 polymorphic_allocator 分配器
        typedef std::basic_string<char, std::char_traits<char>, std::pmr::polymorphic_allocator<char> > IOString;


#define IOStringFormat( buffer, format, ... ) StringFormat<char,std::pmr::polymorphic_allocator<char>>( buffer, format, __VA_ARGS__ );

#define IOStringFormatVa( buffer, format, va ) StringFormatVa<char,std::pmr::polymorphic_allocator<char>>( buffer, format, va );


        // scoket
        struct USocketWriteMsg
        {
            USocketWriteMsg()
                : pSocketClient( nullptr )
                , Data()
            {
            }

            USocketWriteMsg( CSocketClient_UseLibevent* pClient, const std::pmr::polymorphic_allocator<char>& AL )
                : pSocketClient( pClient )
                , Data( AL )
            {
            }

            USocketWriteMsg( const USocketWriteMsg& Msg )
                : pSocketClient( Msg.pSocketClient )
                , Data( Msg.Data )
            {
            }

            CSocketClient_UseLibevent* pSocketClient;
            IOString Data;
        };





        // IO消息队列，调度
        // 线程安全
        class CMsgQueue
        {
        public:
            typedef TMsgQueue<USocketWriteMsg, std::pmr::polymorphic_allocator<USocketWriteMsg>> _SocketQueue;

        public:
            CMsgQueue( std::pmr::memory_resource* pMemoryRes );

            _SocketQueue* GetQueue_SocketWrite() { return &m_Queue_SocketWrite; }
            _SocketQueue* GetQueue_SocketRead() { return &m_Queue_SocketRead; }

        private:
            _SocketQueue m_Queue_SocketWrite;
            _SocketQueue m_Queue_SocketRead;
        };


        extern CMsgQueue* g_pMsgQueues;
	}
}