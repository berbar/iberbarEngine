#pragma once

//#include <iberbar/Utility/Platform/MultiEventWaiter.h>

//#ifndef _INC_WINDOWS
//#error "Please include Windows.h before"
//#endif

#include <synchapi.h>
//#include <Windows.h>

namespace iberbar
{
	namespace OS
	{
		template <int tEventCount >
		class TMultiEventWaiter
		{
		public:
			TMultiEventWaiter();
			~TMultiEventWaiter();

			int Wait();
			int MsgWait();
			bool Active( int nId );

		private:
			HANDLE m_Handles[tEventCount];
		};
	}
}



template <int tEventCount >
iberbar::OS::TMultiEventWaiter<tEventCount>::TMultiEventWaiter()
	: m_Handles()
{
	memset( m_Handles, 0, sizeof( m_Handles ) );

	for ( int i = 0; i < tEventCount; i++ )
	{
		m_Handles[i] = ::CreateEvent( nullptr, 0, 0, nullptr );
	}
}


template <int tEventCount >
iberbar::OS::TMultiEventWaiter<tEventCount>::~TMultiEventWaiter()
{
	for ( int i = 0; i < tEventCount; i++ )
	{
		if ( m_Handles[i] )
		{
			::CloseHandle( m_Handles[i] );
			m_Handles[i] = nullptr;
		}
	}
}


template <int tEventCount >
int iberbar::OS::TMultiEventWaiter<tEventCount>::Wait()
{
	DWORD nWaitResult = ::WaitForMultipleObjects( tEventCount, m_Handles, FALSE, INFINITE );
	int nEventId = nWaitResult - WAIT_OBJECT_0;
	return nEventId;
}


template <int tEventCount >
int iberbar::OS::TMultiEventWaiter<tEventCount>::MsgWait()
{
	DWORD nWaitResult = ::MsgWaitForMultipleObjects( tEventCount, m_Handles, FALSE, INFINITE, QS_ALLEVENTS );
	int nEventId = nWaitResult - WAIT_OBJECT_0;
	return nEventId;
}


template <int tEventCount >
bool iberbar::OS::TMultiEventWaiter<tEventCount>::Active( int nId )
{
	assert( nId >= 0 && nId < tEventCount );
	return ::SetEvent( m_Handles[nId] ) == TRUE;
}
