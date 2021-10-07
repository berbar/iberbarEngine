#pragma once



#include <pthread.h>
#include <memory.h>

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
			pthread_cond_t m_Condition;
			int m_nSignalCount;
			bool m_SignalStates[tEventCount];
			int m_Signals[ tEventCount ];
		};
	}
}



template <int tEventCount >
iberbar::OS::TMultiEventWaiter<tEventCount>::TMultiEventWaiter()
	: m_Condition()
	, m_nSignalCount( 0 )
	, m_SignalStates()
	, m_Signals()
{
	memset( m_SignalStates, 0, sizeof( m_SignalStates ) );
	memset( m_Signals, 0, sizeof( m_Signals ) );

	pthread_cond_init( &m_Condition, nullptr );
}


template <int tEventCount >
iberbar::OS::TMultiEventWaiter<tEventCount>::~TMultiEventWaiter()
{
	pthread_cond_destroy( &m_Condition );
}


template <int tEventCount >
int iberbar::OS::TMultiEventWaiter<tEventCount>::Wait()
{
}


template <int tEventCount >
int iberbar::OS::TMultiEventWaiter<tEventCount>::MsgWait()
{
}


template <int tEventCount >
bool iberbar::OS::TMultiEventWaiter<tEventCount>::Active( int nId )
{
}
