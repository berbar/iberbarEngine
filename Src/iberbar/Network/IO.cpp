
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN // 修正windows上socket头文件重复问题
#endif
#include "IO.h"
#include "Private/Device.h"
#include "Private/Socket.h"
#include "Private/MsgQueue.h"

#include <iberbar/Utility/MsgQueue.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/thread.h>

#include <thread>
#include <memory_resource>
#include <csignal>



iberbar::CResult iberbar::IO::Initial()
{
    assert( g_pDevice == nullptr );
    assert( g_pMsgQueues == nullptr );
    g_pDevice = new CDevice_UseLibevent();
    g_pMsgQueues = new CMsgQueue( g_pDevice->GetMemoryResource() );
    return g_pDevice->CreateIO();
}


void iberbar::IO::Destroy()
{
    if ( g_pDevice != nullptr )
    {
        g_pDevice->DestoryIO();
    }

    SAFE_DELETE( g_pMsgQueues );
    SAFE_DELETE( g_pDevice );
}


void iberbar::IO::Read()
{
    if ( g_pMsgQueues == nullptr )
        return;

    auto pQueueSocketRead = g_pMsgQueues->GetQueue_SocketRead();
    if ( pQueueSocketRead->IsEmpty() == false )
    {
        bool bRetPop = false;
        USocketWriteMsg Msg;

        for ( ;; )
        {
            if ( pQueueSocketRead->TryLock() == true )
            {
                bRetPop = pQueueSocketRead->PopMsg( Msg );
                pQueueSocketRead->Unlock();
            }

            if ( bRetPop == false )
                break;

            Msg.pSocketClient->ExecuteListenerInner( Msg.Data.c_str(), Msg.Data.size() );
        }
    }
}


iberbar::CResult iberbar::IO::CreateSocketClient( ISocketClient** ppOutClient, const std::string& strUrl, const USocketOptions& Options )
{
    if ( Options.pHead == nullptr || Options.nHeadLen == 0 )
        return MakeResult( ResultCode::Bad, "protocol must have head" );

    if ( Options.pTail == nullptr || Options.nTailLen == 0 )
        return MakeResult( ResultCode::Bad, "protocol must have tail" );

    return g_pDevice->CreateClient( ppOutClient, strUrl, Options );
}

//
//iberbar::CResult iberbar::IO::CreateLogger( ILogger** ppOutLogger, const ULoggerOptions& Options )
//{
//    //TSmartRefPtr<CLogger_UseSpdlog> pLogger = nullptr;
//
//    //for ( auto& pLoggerTemp : g_Loggers )
//    //{
//    //    if ( pLoggerTemp->CompareLogger( Options.strName ) == true )
//    //    {
//    //        pLogger = pLoggerTemp;
//    //        break;
//    //    }
//    //}
//
//    //if ( pLogger == nullptr )
//    //{
//    //    pLogger = TSmartRefPtr<CLogger_UseSpdlog>::_sNew();
//    //    CResult ret = pLogger->Create( Options );
//    //    if ( ret.IsOK() == false )
//    //        return ret;
//    //}
//
//    TSmartRefPtr<CLogger_UseSpdlog> pLogger = TSmartRefPtr<CLogger_UseSpdlog>::_sNew();
//    CResult ret = pLogger->Create( Options );
//    if ( ret.IsOK() == false )
//        return ret;
//
//    if ( ppOutLogger )
//    {
//        if ( (*ppOutLogger) )
//            (*ppOutLogger)->Release();
//        (*ppOutLogger) = pLogger;
//        (*ppOutLogger)->AddRef();
//    }
//
//    return CResult();
//}


//bool iberbar::IO::GetLogger( ILogger** ppOutLogger, const char* strName )
//{
//    for ( auto& pLoggerTemp : g_Loggers )
//    {
//        if ( pLoggerTemp->CompareLogger( strName ) == true )
//        {
//            if ( ppOutLogger )
//            {
//                if ( (*ppOutLogger) )
//                    (*ppOutLogger)->Release();
//                (*ppOutLogger) = pLoggerTemp;
//                (*ppOutLogger)->AddRef();
//            }
//            return true;
//        }
//    }
//    return false;
//}







