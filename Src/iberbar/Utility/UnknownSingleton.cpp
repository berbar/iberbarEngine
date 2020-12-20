

#ifdef _DEBUG

#include "iberbar\Base\UnknownSingleton.h"
#include "iberbar\Base\LogText.h"

namespace iberbar
{
	std::vector< PTR_CUnknown >  g_Debug_UnknownSingletonPool;
}

void iberbar::Debug_AddUnknownSingleton( CUnknown* singleton )
{
	if ( g_Debug_UnknownSingletonPool.empty() == false )
	{
		std::vector< PTR_CUnknown >::iterator lc_iter = g_Debug_UnknownSingletonPool.begin();
		std::vector< PTR_CUnknown >::iterator lc_end  = g_Debug_UnknownSingletonPool.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
		{
			if ( ( *lc_iter ).equal( singleton ) )
				return;
		}
	}

	g_Debug_UnknownSingletonPool.push_back( singleton );
}


void iberbar::Debug_RemoveUnknownSingleton( CUnknown* singleton )
{
	if ( g_Debug_UnknownSingletonPool.empty() == false )
	{
		std::vector< PTR_CUnknown >::iterator lc_iter = g_Debug_UnknownSingletonPool.begin();
		std::vector< PTR_CUnknown >::iterator lc_end  = g_Debug_UnknownSingletonPool.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
		{
			if ( ( *lc_iter ).equal( singleton ) )
			{
				g_Debug_UnknownSingletonPool.erase( lc_iter );
				return;
			}
		}
	}
}


void iberbar::Debug_CheckUnknownSingleton()
{
	if ( g_Debug_UnknownSingletonPool.empty() )
	{
		GetLogFile()->logva(
			PRIORITY_INFO,
			"UnknownSingleton",
			"No singleton leave." );
	}
	else
	{
		GetLogFile()->logva(
			PRIORITY_ALERT,
			"UnknownSingleton",
			"There are singletons not delete!!!" );

		std::vector< PTR_CUnknown >::iterator lc_iter = g_Debug_UnknownSingletonPool.begin();
		std::vector< PTR_CUnknown >::iterator lc_end  = g_Debug_UnknownSingletonPool.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
		{
			GetLogFile()->logva(
				PRIORITY_ALERT,
				"UnknownSingleton",
				"  Not delete singleton - <%s>, reference=%d",
				typeid( (*(*lc_iter)) ),
				( *lc_iter )->refcount() );
		}
	}
}


#endif