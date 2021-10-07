#pragma once


#include "../IO.h"
#include <iberbar/Utility/OS/MultiEventWaiter.h>
#include <thread>
#include <memory_resource>


struct event_base;
struct event;

namespace iberbar
{
	namespace IO
	{
        class CSocketClient_UseLibevent;


		class CDevice_UseLibevent
		{
        private:
            enum class UEventIdForThread
            {
                Shutdown = 0,
                Wakeup,
                __Count
            };

        public:
            CDevice_UseLibevent();
            ~CDevice_UseLibevent();

        public:
            CResult CreateIO();
            void DestoryIO();
            CResult CreateClient( ISocketClient** ppOutClient, const std::string& url, const USocketOptions& options );
            void ActiveEvent_SocketWrite();

            std::pmr::memory_resource* GetMemoryResource() { return &m_Pool; }

            template < typename TElem >
            std::pmr::polymorphic_allocator<TElem> GetMemoryAllocator() { return std::pmr::polymorphic_allocator<TElem>( &m_Pool ); }

        private:
            void WakeupThread();
            void Run();

        private:
            event_base* m_base;
            event* m_pEv2_SignalExit;
            event* m_pEvent_SocketWrite;
            event* m_pEvent_LoggerWrite;
            std::thread m_thread;
            bool m_bRunThread;
            bool m_bIsWaiting;

            std::vector<CSocketClient_UseLibevent*> m_SocketClients;

            // ÄÚ´æ³Ø
            std::pmr::synchronized_pool_resource m_Pool;

//#ifdef _WINDOWS
//            HANDLE m_hEventsForThread[ (int)UEventIdForThread::__Count ];
//#endif
            OS::TMultiEventWaiter<(int)UEventIdForThread::__Count> m_EventWaiter;
		};

        extern CDevice_UseLibevent* g_pDevice;
	}
}

