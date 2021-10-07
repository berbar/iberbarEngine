#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN // 修正windows上socket头文件重复问题
#endif

#include "Socket.h"
#include "Device.h"
#include "MsgQueue.h"



#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>









inline iberbar::IO::CAddrInfoHelper::CAddrInfoHelper()
    : m_pResult( nullptr )
{
}


inline iberbar::IO::CAddrInfoHelper::~CAddrInfoHelper()
{
    if ( m_pResult != nullptr )
    {
        freeaddrinfo( m_pResult );
        m_pResult = nullptr;
    }
}


inline const addrinfo* iberbar::IO::CAddrInfoHelper::Set( const char* strNodeName, const char* pServiceName, const addrinfo* pHints )
{
    if ( m_pResult != nullptr )
    {
        freeaddrinfo( m_pResult );
        m_pResult = nullptr;
    }

    int c = getaddrinfo( strNodeName, pServiceName, pHints, &m_pResult );
    if ( c != 0 )
    {

    }

    return m_pResult;
}


inline const addrinfo* iberbar::IO::CAddrInfoHelper::Set_Socket( const std::string& url )
{
    size_t p = url.find( ':' );
    if ( p == std::string::npos || p == (url.length() - 1) || p == 0 )
        return nullptr;
    std::string host = url.substr( 0, p );
    std::string port = url.substr( p + 1 );

    struct addrinfo hints;
    //struct addrinfo* res, * cur;
    memset( &hints, 0, sizeof( struct addrinfo ) );
    hints.ai_family = AF_INET;     /* Allow IPv4 */
    hints.ai_flags = AI_ALL;
    hints.ai_protocol = 0;         /* Any protocol */
    hints.ai_socktype = SOCK_STREAM;

    return Set( host.c_str(), port.c_str(), &hints );
}


inline const addrinfo* iberbar::IO::CAddrInfoHelper::Get() const
{
    return m_pResult;
}









iberbar::IO::CSocketClient_UseLibevent::CSocketClient_UseLibevent( event_base* base, const std::string& strUrl, const USocketOptions& Opts )
    : m_base( base )
    , m_strUrl( strUrl )
    , m_bSocketRead( Opts.bEnableRead )
    , m_bSocketWrite( Opts.bEnableWrite )
    , m_pHead( nullptr )
    , m_pTail( nullptr )
    , m_nHeadLen( 0 )
    , m_nTailLen( 0 )
    , m_Listener()
    , m_pBufferEvent( nullptr )
{
    m_nHeadLen = Opts.nHeadLen;
    m_pHead = new char[ m_nHeadLen ];
    memcpy_s( m_pHead, m_nHeadLen, Opts.pHead, m_nHeadLen );

    m_nTailLen = Opts.nTailLen;
    m_pTail = new char[ m_nTailLen ];
    memcpy_s( m_pTail, m_nTailLen, Opts.pTail, m_nTailLen );
}


iberbar::IO::CSocketClient_UseLibevent::~CSocketClient_UseLibevent()
{
    SAFE_DELETE_ARRAY( m_pHead );
    SAFE_DELETE_ARRAY( m_pTail );

    if ( m_pBufferEvent != nullptr )
    {
        bufferevent_free( m_pBufferEvent );
        m_pBufferEvent = nullptr;
    }
}


void iberbar::IO::CSocketClient_UseLibevent::Reconnect()
{

}


void iberbar::IO::CSocketClient_UseLibevent::Disconnect()
{
}


void iberbar::IO::CSocketClient_UseLibevent::Send( const char* pData, size_t nSize )
{
    USocketWriteMsg Msg( this, g_pDevice->GetMemoryAllocator<char>() );

    size_t nBufferSize = m_nHeadLen + nSize + sizeof( uint32 ) + m_nTailLen;
    Msg.Data.resize( nBufferSize );
    char* ptr = &Msg.Data.front();

    memcpy_s( ptr, nBufferSize, m_pHead, m_nHeadLen );
    ptr += m_nHeadLen;

    (*((uint32*)ptr)) = (uint32)nSize;
    ptr += sizeof( uint32 );

    memcpy_s( ptr, nSize, pData, nSize );
    ptr += nSize;

    memcpy_s( ptr, m_nTailLen, m_pTail, m_nTailLen );

    if ( g_pMsgQueues->GetQueue_SocketWrite()->TryLock() == true )
    {
        g_pMsgQueues->GetQueue_SocketWrite()->AddMsg( Msg );
        g_pMsgQueues->GetQueue_SocketWrite()->Unlock();
    }

    g_pDevice->ActiveEvent_SocketWrite();
}


void iberbar::IO::CSocketClient_UseLibevent::Listen( UFunctionSocketListen Func )
{
    m_Listener = Func;
}


iberbar::CResult iberbar::IO::CSocketClient_UseLibevent::Connect()
{
    CAddrInfoHelper AddrInfo;
    if ( AddrInfo.Set_Socket( m_strUrl ) == nullptr )
    {
        return MakeResult( ResultCode::Bad, "getaddrinfo failed" );
    }

    int opts = BEV_OPT_CLOSE_ON_FREE;
    //opts |= BEV_OPT_THREADSAFE;
    m_pBufferEvent = bufferevent_socket_new( m_base, -1, opts );
    if ( m_pBufferEvent == NULL )
    {
        return MakeResult( ResultCode::Bad, "socket init failed" );
    }
    bufferevent_setcb( m_pBufferEvent, OnSocketConnRead, OnSocketConnWrite, OnSocketConnEvent, this );

    //连接服务端
    int nConnRet = bufferevent_socket_connect( m_pBufferEvent, AddrInfo.Get()->ai_addr, sizeof( sockaddr ) );
    if ( -1 == nConnRet )
    {
        return MakeResult( ResultCode::Bad, "connect failed" );
    }

    short nFlag = 0;
    if ( m_bSocketRead == true )
        nFlag |= EV_READ;
    if ( m_bSocketWrite == true )
        nFlag |= EV_WRITE;
    bufferevent_enable( m_pBufferEvent, nFlag );

    return CResult();
}


void iberbar::IO::CSocketClient_UseLibevent::SendInner( const char* pData, size_t nSize )
{
    if ( m_pBufferEvent == nullptr )
        return;

    bufferevent_write( m_pBufferEvent, pData, nSize );
}


void iberbar::IO::CSocketClient_UseLibevent::ExecuteListenerInner( const char* pData, size_t nSize )
{
    if ( m_Listener )
        m_Listener( this, pData, nSize );
}


void iberbar::IO::CSocketClient_UseLibevent::OnConnectEvent_Inner( short nEvents )
{
    if ( nEvents & BEV_EVENT_EOF )
    {
        printf( "Connection closed.\n" );
    }
    else if ( nEvents & BEV_EVENT_ERROR )
    {
        char errbuf[ 1024 ];
        strerror_s( errbuf, errno );
        printf( "Got an error on the connection: %s\n", errbuf );
    }
    else if ( nEvents & BEV_EVENT_CONNECTED )
    {
        //连接成功时走这里，并且要客户端第一次触发读事件后连接才真正建立起来

        return;
    }
    bufferevent_free( m_pBufferEvent );
    m_pBufferEvent = nullptr;
}


void iberbar::IO::CSocketClient_UseLibevent::OnSocketConnWrite( struct bufferevent*, void* )
{

}


void iberbar::IO::CSocketClient_UseLibevent::OnSocketConnRead( struct bufferevent* bev, void* pUserData )
{
    CSocketClient_UseLibevent* pClient = (CSocketClient_UseLibevent*)pUserData;

    evbuffer* buf = bufferevent_get_input( bev );
    int len = evbuffer_get_length( buf );

    if ( len == 0 )
        return;

    // 锁住消息队列
    if ( g_pMsgQueues->GetQueue_SocketRead()->TryLock() == false )
        return;

    //
    // 由于peek出来的chain可能不止一条，暂时粗暴的copyout，迟点可能改成peek配合部分copyout
    //
    IOString MemoryBuffer( g_pDevice->GetMemoryAllocator<char>() );
    MemoryBuffer.resize( len );
    evbuffer_copyout( buf, &MemoryBuffer.front(), len );

    const char* pFd = MemoryBuffer.c_str();
    size_t nFdLen = MemoryBuffer.size();
    const char* pHead = pClient->m_pHead;
    const char* pTail = pClient->m_pTail;
    size_t nHeadLen = pClient->m_nHeadLen;
    size_t nTailLen = pClient->m_nTailLen;
    int nHeadPos, nHeadFindStart = 0, nMaybe;
    int nTempLen;
    unsigned int nBodyLen;
    size_t nDrain = 0;

    while ( (nHeadPos = sSocketProtocol_FindHead( pFd, nFdLen, nHeadFindStart, pHead, nHeadLen, nMaybe )) >= 0 )
    {
        nBodyLen = *((unsigned int*)(pFd + nHeadPos + nHeadLen));

        nTempLen = nHeadPos + nHeadLen + 4;
        // len不完整
        if ( nTempLen > nFdLen )
        {
            nMaybe = nHeadPos;
            break;
        }

        nTempLen += nBodyLen;
        // body不完整
        if ( nTempLen > nFdLen )
        {
            nMaybe = nHeadPos;
            break;
        }

        nTempLen += nTailLen;
        // tail不完整
        if ( nTempLen > nFdLen )
        {
            nMaybe = nHeadPos;
            break;
        }

        if ( memcmp( pFd + (nTempLen - nTailLen), pTail, nTailLen ) == 0 )
        {
            USocketWriteMsg Msg( pClient, g_pDevice->GetMemoryAllocator<char>() );
            Msg.Data.resize( nBodyLen );
            memcpy_s( &Msg.Data.front(), nBodyLen, pFd + (nTempLen - nTailLen - nBodyLen), nBodyLen );
            g_pMsgQueues->GetQueue_SocketRead()->AddMsg( Msg );
        }

        nHeadFindStart = nTempLen;
    }

    if ( nMaybe < 0 )
    {
        evbuffer_drain( buf, nFdLen );
    }
    else if ( nMaybe > 0 )
    {
        evbuffer_drain( buf, nMaybe );
    }

    // 解锁
    g_pMsgQueues->GetQueue_SocketRead()->Unlock();
}


void iberbar::IO::CSocketClient_UseLibevent::OnSocketConnEvent( struct bufferevent* pBufferEvent, short nEvents, void* pUserData )
{
    ((CSocketClient_UseLibevent*)pUserData)->OnConnectEvent_Inner( nEvents );
}


int iberbar::IO::CSocketClient_UseLibevent::sSocketProtocol_FindHead(
    const char* pFd,
    size_t nFdLen,
    size_t nFdStart,
    const char* pHead,
    size_t nHeadLen,
    int& nMaybe )
{
    if ( nFdLen <= nFdStart )
    {
        nMaybe = -1;
        return -1;
    }

    size_t nFdLenNew = nFdLen - nFdStart;
    const char* pFdNew = pFd + nFdStart;
    while ( nFdLenNew >= nHeadLen )
    {
        if ( memcmp( pFdNew, pHead, nHeadLen ) == 0 )
        {
            return ((int)nFdLen - (int)nFdLenNew);
        }
        pFdNew++;
        nFdLenNew--;
    }

    if ( memcmp( pFdNew, pHead, (nHeadLen - nFdLenNew) ) == 0 )
    {
        nMaybe = ((int)nFdLen - (int)nFdLenNew);
    }
    else
    {
        nMaybe = -1;
    }

    return -1;
}
