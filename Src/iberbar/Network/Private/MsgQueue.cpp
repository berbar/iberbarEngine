
#include "MsgQueue.h"



extern iberbar::IO::CMsgQueue* iberbar::IO::g_pMsgQueues = nullptr;


iberbar::IO::CMsgQueue::CMsgQueue( std::pmr::memory_resource* pMemoryRes )
	: m_Queue_SocketWrite( std::pmr::polymorphic_allocator<USocketWriteMsg>( pMemoryRes ) )
	, m_Queue_SocketRead( std::pmr::polymorphic_allocator<USocketWriteMsg>( pMemoryRes ) )
{
}
