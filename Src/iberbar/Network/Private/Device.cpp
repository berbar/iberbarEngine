
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN // 修正windows上socket头文件重复问题
#endif

#include "Device.h"
#include "MsgQueue.h"
#include "Socket.h"

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/thread.h>

#include <thread>
#include <memory_resource>
#include <csignal>


namespace iberbar
{
    namespace IO
    {
        void OnEvent_SignalExit( evutil_socket_t sig, short events, void* user_data );
        void OnEvent_SocketWrite( evutil_socket_t Sock, short nEvents, void* pUserData );
    }
}




extern iberbar::IO::CDevice_UseLibevent* iberbar::IO::g_pDevice = nullptr;


iberbar::IO::CDevice_UseLibevent::CDevice_UseLibevent()
    : m_base( nullptr )
    , m_thread()
    , m_bRunThread( false )
    , m_bIsWaiting( false )
    , m_hEventsForThread()
    , m_SocketClients()
{
    memset( m_hEventsForThread, 0, sizeof( m_hEventsForThread ) );
}


iberbar::IO::CDevice_UseLibevent::~CDevice_UseLibevent()
{
    if ( m_SocketClients.empty() == false )
    {
        m_SocketClients.clear();
    }

    if ( m_pEvent_SocketWrite != nullptr )
    {
        event_free( m_pEvent_SocketWrite );
        m_pEvent_SocketWrite = nullptr;
    }

    if ( signal_event != nullptr )
    {
        event_free( signal_event );
        signal_event = nullptr;
    }

    if ( m_base != nullptr )
    {
        event_base_free( m_base );
        m_base = nullptr;
    }

    libevent_global_shutdown();

#ifdef _WINDOWS
    for ( int i = 0, s = (int)UEventIdForThread::__Count; i < s; i++ )
    {
        if ( m_hEventsForThread[ i ] != nullptr )
        {
            ::CloseHandle( m_hEventsForThread[ i ] );
            m_hEventsForThread[ i ] = 0;
        }
    }
#endif
}


iberbar::CResult iberbar::IO::CDevice_UseLibevent::CreateClient( ISocketClient** ppOutClient, const std::string& strUrl, const USocketOptions& Options )
{
    TSmartRefPtr<CSocketClient_UseLibevent> pSocketClient = TSmartRefPtr<CSocketClient_UseLibevent>::_sNew( m_base, strUrl, Options );

    CResult retConn = pSocketClient->Connect();
    if ( retConn.IsOK() == false )
        return retConn;

    WakeupThread();

    (*ppOutClient) = pSocketClient;
    (*ppOutClient)->AddRef();
    m_SocketClients.push_back( pSocketClient );

    return CResult();
}


void iberbar::IO::CDevice_UseLibevent::ActiveEvent_SocketWrite()
{
    event_active( m_pEvent_SocketWrite, EV_SIGNAL, -1 );
}


iberbar::CResult iberbar::IO::CDevice_UseLibevent::CreateIO()
{

    if ( m_base == nullptr )
    {
#ifdef _WINDOWS
        //Init Windows Socket
        WSADATA  Ws;
        if ( WSAStartup( MAKEWORD( 2, 2 ), &Ws ) != 0 )
        {
            return MakeResult( ResultCode::Bad, "failed to call WSAStartup" );
        }
#endif

#ifdef _WINDOWS
        evthread_use_windows_threads();
#else
#endif

        m_base = event_base_new();
        if ( !m_base )
        {
            return MakeResult( ResultCode::Bad, "Could not initialize libevent" );
        }

        signal_event = evsignal_new( m_base, SIGINT, OnEvent_SignalExit, (void*)m_base );
        if ( !signal_event || event_add( signal_event, NULL ) < 0 ) {
            return MakeResult( ResultCode::Bad, "Could not create/add a signal event!\n" );
        }
    }

    m_pEvent_SocketWrite = event_new( m_base, -1, EV_PERSIST, OnEvent_SocketWrite, nullptr );
    event_add( m_pEvent_SocketWrite, NULL );

    return CResult();
}


void iberbar::IO::CDevice_UseLibevent::DestoryIO()
{
    if ( m_base != nullptr )
    {
        // 发送退出信号
        raise( SIGINT );

        // 发送结束线程事件
        ::SetEvent( m_hEventsForThread[ (int)UEventIdForThread::Shutdown ] );

        // 调用join，IO线程结束
        if ( m_thread.joinable() )
        {
            m_thread.join();
        }
    }
}


void iberbar::IO::CDevice_UseLibevent::WakeupThread()
{
    if ( m_bRunThread == true )
    {
        if ( m_bIsWaiting == true )
        {
#ifdef _WINDOWS
            ::SetEvent( m_hEventsForThread[ (int)UEventIdForThread::Wakeup ] );
#endif
        }
    }
    else
    {
#ifdef _WINDOWS
        for ( int i = 0, s = (int)UEventIdForThread::__Count; i < s; i++ )
        {
            if ( m_hEventsForThread[ i ] == nullptr )
            {
                m_hEventsForThread[ i ] = ::CreateEventW( NULL, FALSE, FALSE, NULL );
            }
        }

        m_thread = std::thread( &CDevice_UseLibevent::Run, this );
        m_bRunThread = true;
        if ( m_hEventsForThread[ (int)UEventIdForThread::Wakeup ] != nullptr )
        {
            ::SetEvent( m_hEventsForThread[ (int)UEventIdForThread::Wakeup ] );
        }
#endif
    }
}


void iberbar::IO::CDevice_UseLibevent::Run()
{
#ifdef _WINDOWS
    while ( true )
    {
        DWORD nWaitRet = WaitForMultipleObjects( (int)UEventIdForThread::__Count, m_hEventsForThread, FALSE, INFINITE );
        DWORD nEvent = nWaitRet - WAIT_OBJECT_0;

        // Destroy命令
        if ( nEvent == (int)UEventIdForThread::Shutdown )
            break;

        m_bIsWaiting = false;

        event_base_dispatch( m_base );

        m_bIsWaiting = true;
    }
#endif
}


void iberbar::IO::OnEvent_SignalExit( evutil_socket_t sig, short events, void* pUserData )
{
    event_base* pEventBase = (event_base*)pUserData;

    struct timeval delay = { 2, 0 };

    printf( "Caught an interrupt signal; exiting cleanly in two seconds.\n" );

    event_base_loopbreak( pEventBase );
    event_base_loopexit( pEventBase, &delay );
}


void iberbar::IO::OnEvent_SocketWrite( evutil_socket_t Sock, short nEvents, void* pUserData )
{
    auto pQueue = g_pMsgQueues->GetQueue_SocketWrite();
    if ( pQueue->IsEmpty() == true )
        return;

    if ( pQueue->TryLock() == true )
    {
        USocketWriteMsg Msg;
        while ( pQueue->PopMsg( Msg ) )
        {
            Msg.pSocketClient->SendInner( Msg.Data.c_str(), Msg.Data.size() );

            // yield 让出时间片给主线程
            //std::this_thread::yield();
        }
        pQueue->Unlock();
    }
}
