

#include "iberbar\Base\UnknownObject.h"


namespace iberbar
{

	struct CEventHandler
		: public CRef
	{
		IBERBAR_UNKNOWN_CLONE_DISABLED( CEventHandler );


	public:
		CUnknownObject*      pHandler; // 该handler不增加引用计数
		PUNOBJECT_EVENT_PROC pHandlProc;
		uint32               nSpecialEvent;

		CEventHandler( void )
			: pHandler( NULL )
			, pHandlProc( NULL )
			, nSpecialEvent( 0 )
		{
		}

		inline void execate( CUnknownObject* sender, const UnknownEventParams& msg )
		{
			assert( sender );
			assert( pHandlProc );

			if ( pHandler == NULL )
				return ;

			if ( nSpecialEvent == 0 || msg.nEventId == nSpecialEvent )
				( pHandler->*pHandlProc )( sender, msg );
		}
	};


	typedef TSmartRefPtr< CEventHandler >  EventMediator;




	class CUnknownEventDelegate
		: public CRef
	{
	public:
		CUnknownEventDelegate( void )
		{
		}

		~CUnknownEventDelegate()
		{
			std::vector< EventMediator >::iterator lc_iter = m_Restores.begin();
			std::vector< EventMediator >::iterator lc_end  = m_Restores.end();
			for ( ; lc_iter != lc_end; lc_iter ++ )
				( *lc_iter )->pHandler = NULL;
		}


		IBERBAR_UNKNOWN_CLONE_DISABLED( CUnknownEventDelegate );

	public:
		EventMediator addEvent( CUnknownObject* pHandler, PUNOBJECT_EVENT_PROC pProc, uint32 nSpecialEvent );
		void execate( CUnknownObject* sender, const UnknownEventParams& msg );
		void restore( EventMediator mediator ); // Receiver 调用该函数来存储事件中介者

	private:
		std::vector< EventMediator >  m_EventHandles;
		std::vector< EventMediator >  m_Restores;
	};
}




//---------------------------------------------------------------------------------------------------------------------------------
iberbar::CUnknownObject::CUnknownObject( void )
	: m_EventDelegate( new CUnknownEventDelegate() )
	, m_TagsMap()
{
}


//---------------------------------------------------------------------------------------------------------------------------------
iberbar::CUnknownObject::CUnknownObject(const CUnknownObject& copySrc ) throw()
	: CRef( copySrc )

	, m_EventDelegate( new CUnknownEventDelegate() )
	, m_TagsMap()
{
}


//---------------------------------------------------------------------------------------------------------------------------------
iberbar::CUnknownObject::~CUnknownObject()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_EventDelegate );
}


//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CUnknownObject::addEventProc( CUnknownObject* pHandler, PUNOBJECT_EVENT_PROC pProc, uint32 nSpecialEvent )
{
	assert( pHandler );
	assert( pProc );

	EventMediator lc_temp = m_EventDelegate->addEvent( pHandler, pProc, nSpecialEvent );
	pHandler->m_EventDelegate->restore( lc_temp );
}


//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CUnknownObject::sendEvent( const UnknownEventParams& msgParam )
{
	if ( m_EventDelegate == NULL )
		return;

	TSmartRefPtr< CUnknownEventDelegate > lc_EventDelegate = m_EventDelegate;
	lc_EventDelegate->execate( this, msgParam );
}


//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CUnknownObject::sendEventLong( uint32 nEvent, uint64 wParam, int64 lParam, void* data, const char* cParam )
{
	UnknownEventParams msg;
	msg.nEventId = nEvent;
	msg.wParam = wParam;
	msg.lParam = lParam;
	msg.pData = data;
	if ( cParam )
		msg.cParam = cParam;
	sendEvent( msg );
}


//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CUnknownObject::setTag( const TCHAR* key, const TCHAR* tag )
{
	if ( key == NULL )
		return ;

	if ( tag == NULL )
	{
		_TAGMAP::iterator lc_iter = m_TagsMap.begin();
		_TAGMAP::iterator lc_end  = m_TagsMap.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
		{
			if ( tstrcmp( ( *lc_iter ).key.c_str(), key ) == 0 )
			{
				m_TagsMap.erase( lc_iter );
				break;
			}
		}
	}
	else
	{
		_TAGNODE* lc_tagnode = getTagNode( key );
		if ( lc_tagnode == NULL )
		{
			m_TagsMap.push_back( _TAGNODE() );
			lc_tagnode = &(*(m_TagsMap.rbegin()));
			lc_tagnode->key = key;
		}

		lc_tagnode->tag = tag;
	}
}


//---------------------------------------------------------------------------------------------------------------------------------
const TCHAR* iberbar::CUnknownObject::getTag( const TCHAR* key )
{
	_TAGNODE* lc_tagnode = getTagNode( key );
	if ( lc_tagnode == NULL )
		return NULL;
	return lc_tagnode->tag.c_str();
}


//---------------------------------------------------------------------------------------------------------------------------------
iberbar::CUnknownObject::_TAGNODE* iberbar::CUnknownObject::getTagNode( const TCHAR* key )
{
	if ( m_TagsMap.IsEmpty() )
		return NULL;

	if ( key == NULL )
		return NULL;

	_TAGMAP::iterator lc_iter = m_TagsMap.begin();
	_TAGMAP::iterator lc_end  = m_TagsMap.end();
	for ( ; lc_iter != lc_end; lc_iter ++ )
	{
		if ( tstrcmp( (*lc_iter).key.c_str(), key ) == 0 )
			return &(*lc_iter);
	}

	return NULL;
}







//---------------------------------------------------------------------------------------------------------------------------------
iberbar::EventMediator iberbar::CUnknownEventDelegate::addEvent( CUnknownObject* pHandler, PUNOBJECT_EVENT_PROC pProc, uint32 nSpecialEvent )
{
	assert( pHandler );
	assert( pProc );

	EventMediator lc_handler = EventMediator::_sNew();
	lc_handler->pHandler = pHandler;
	lc_handler->pHandlProc = pProc;
	lc_handler->nSpecialEvent = nSpecialEvent;
	m_EventHandles.push_back( lc_handler );

	return lc_handler;
}


//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CUnknownEventDelegate::execate( CUnknownObject* sender, const UnknownEventParams& msg )
{
	if ( m_EventHandles.IsEmpty() )
		return;

	std::vector< EventMediator >::iterator lc_iter = m_EventHandles.begin();
	std::vector< EventMediator >::iterator lc_end  = m_EventHandles.end();
	for ( ; lc_iter != lc_end; ++lc_iter )
		( *lc_iter )->execate( sender, msg );
}


//---------------------------------------------------------------------------------------------------------------------------------
void iberbar::CUnknownEventDelegate::restore( EventMediator mediator )
{
	m_Restores.push_back( mediator );
}
