#pragma once

#include <iberbar/Utility/Unknown.h>
#include <list>

namespace iberbar
{
	namespace StdHelper
	{
		template < typename T >
		void ListSafeClearRef( std::list<T*>& data, bool release )
		{
			if ( release == true )
			{
				auto iter = data.begin();
				auto end = data.end();
				for ( ; iter != end; iter++ )
				{
					UNKNOWN_SAFE_RELEASE_NULL( *iter );
				}
			}
			data.clear();
		}


		template < typename T >
		bool ListSafeEraseRef( std::list<T*>& data, T* ref, bool release )
		{
			auto iter = data.begin();
			auto end = data.end();
			for ( ; iter != end; iter++ )
			{
				if ( ref == (*iter) )
				{
					if ( release == true )
					{
						UNKNOWN_SAFE_RELEASE_NULL( *iter );
					}
					data.erase( iter );
					return true;
				}
			}
			return false;
		}
	}



	
}
