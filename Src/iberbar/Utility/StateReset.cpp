
#include "StateReset.h"




void iberbar::CStateResetQue::onLost()
{
	if ( m_Que.empty() )
		return ;

	PTR_CUnknown         lc_object;
	PUnknownStateOnLost lc_func;
	size_t lc_size = m_Que.size();
	for ( size_t lc_i = 0; lc_i < lc_size; lc_i ++ )
	{
		lc_object = m_Que[ lc_i ].pObject;
		lc_func = m_Que[ lc_i ].pOnLost;
		( lc_object->*lc_func )();
	}
}


bool iberbar::CStateResetQue::onReset()
{
	if ( m_Que.empty() )
		return true;

	PTR_CUnknown          lc_object;
	PUnknownStateOnReset lc_func;
	size_t lc_size = m_Que.size();
	for ( size_t lc_i = 0; lc_i < lc_size; lc_i ++ )
	{
		lc_object = m_Que[ lc_i ].pObject;
		lc_func = m_Que[ lc_i ].pOnReset;

		if ( ( lc_object->*lc_func )( ) == false )
			return false;
	}

	return true;
}


void iberbar::CStateResetQue::add( PTR_CUnknown pObject, PUnknownStateOnLost pLost, PUnknownStateOnReset pReset )
{
	if ( pObject == NULL ||
		pLost == NULL ||
		pReset == NULL )
		return ;

	if ( find( pObject ) >= 0 )
		return ;

	StateResetNode lc_node;
	lc_node.pObject = pObject;
	lc_node.pOnLost = pLost;
	lc_node.pOnReset = pReset;

	m_Que.push_back( lc_node );
}


void iberbar::CStateResetQue::remove( PTR_CUnknown pObject )
{
	if ( pObject == NULL )
		return ;

	int lc_pos = find( pObject );
	if ( lc_pos < 0 )
		return ;

	m_Que.erase( m_Que.begin() + lc_pos );
}


void iberbar::CStateResetQue::removeAll()
{
	m_Que.clear();
}


int iberbar::CStateResetQue::find( PTR_CUnknown pObject )
{
	if ( m_Que.empty() )
		return -1;
	size_t lc_size = m_Que.size();
	for ( size_t lc_i = 0; lc_i < lc_size; lc_i ++ )
	{
		if ( pObject == m_Que[ lc_i ].pObject )
			return lc_i;
	}

	return -1;
}