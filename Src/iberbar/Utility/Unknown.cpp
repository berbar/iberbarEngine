

#include "Unknown.h"
#include <iberbar/Utility/RefStatistics.h>

#include <stdexcept>
#if defined (_DEBUG_USE_UNKNOWN_POOL)
#include "iberbar\Base\data\Vector_SortDichotomy.h"
#include "iberbar\Base\data\DataMap.h"
#include <map>
#endif



#ifdef _DEBUG_USE_UNKNOWN_POOL

//---------------------------------------------------------------------------------------------------------------------------------
typedef iberbar::TVector_SortDichotomy< uintptr_t, const iberbar::CRef* > UNDebugPool;
bool        gUNDebugPoolOpen = false;
UNDebugPool gUNDebugPool;
uint32      gUNDebugPoolCount = 0;


//---------------------------------------------------------------------------------------------------------------------------------
// 添加CUnknown实例到UNDebug池中
inline void UNDebugPoolInsert( iberbar::CRef* object )
{
	gUNDebugPoolCount++;
	if ( gUNDebugPoolOpen == true )
	{
		gUNDebugPool.add( (uintptr_t)object, (const iberbar::CRef*)object );
	}
}


//---------------------------------------------------------------------------------------------------------------------------------
// 从UNDebug池移除CUnknown实例
inline void UNDebugPoolRemove( iberbar::CRef* object )
{
	gUNDebugPoolCount--;
	if ( gUNDebugPoolOpen == true )
	{
		gUNDebugPool.remove( (uintptr_t)object );
	}
}


//---------------------------------------------------------------------------------------------------------------------------------
// 初始化UNDebug
void iberbar::CUnknownDebug::initial( bool openPool )
{
	assert( gUNDebugPool.size() == 0 );

	gUNDebugPoolOpen = openPool;
}


//---------------------------------------------------------------------------------------------------------------------------------
// 查询UNDebug里面泄漏的对象
uint32 iberbar::CUnknownDebug::query( CUnknownDebug::PUNQuery process )
{
	if ( gUNDebugPoolOpen == false )
		return gUNDebugPoolCount;

	if ( gUNDebugPool.size() == 0 )
		return 0;

	if ( process )
	{
		UNDebugPool::node_iterator lc_node = gUNDebugPool.begin();
		UNDebugPool::node_iterator lc_end = gUNDebugPool.end();
		for ( ; lc_node != lc_end; lc_node++ )
		{
			(process)((*lc_node)->data);
		}
	}

	return (uint32)gUNDebugPool.size();
}

#endif





//---------------------------------------------------------------------------------------------------------------------------------
iberbar::CRef::CRef( void )
	: m_nRef( 1 )
{
	CRefStatistics::sGetShared()->Add( this );
}



//---------------------------------------------------------------------------------------------------------------------------------
iberbar::CRef::CRef( const CRef& unknown )
	: m_nRef( 1 )
{
	CRefStatistics::sGetShared()->Add( this );
}


//---------------------------------------------------------------------------------------------------------------------------------
iberbar::CRef::~CRef()
{
	CRefStatistics::sGetShared()->Remove( this );
}


//---------------------------------------------------------------------------------------------------------------------------------
uint32 iberbar::CRef::AddRef()
{
	m_nRef++;
	return m_nRef;
}


//---------------------------------------------------------------------------------------------------------------------------------
uint32 iberbar::CRef::Release()
{
	uint32 lc_nRef = --m_nRef;
	if ( m_nRef == 0 )
	{
		delete this;
	}

	return lc_nRef;
}








