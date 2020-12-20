#pragma once

#include <iberbar/Utility/Unknown.h>
#include <map>

namespace iberbar
{
	template < typename TKey, typename TValue >
	class TResourceManager
	{
	public:
		typedef std::map<TKey, TValue*> _Map;

	public:
		TResourceManager() {}
		~TResourceManager();

	public:
		bool Find( const TKey& key, TValue** ppOut );

	protected:
		_Map m_Cache;
	};
}


template < typename TKey, typename TValue >
iberbar::TResourceManager< TKey, TValue >::~TResourceManager()
{
	_Map::iterator iter = m_Cache.begin();
	_Map::iterator end  = m_Cache.end();
	for ( ; iter != end; iter++ )
	{
		if ( iter->second != nullptr )
		{
			UNKNOWN_SAFE_RELEASE_NULL( iter->second );
			iter->second = nullptr;
		}
	}
	m_Cache.Clear();
}

template < typename TKey, typename TValue >
bool iberbar::TResourceManager< TKey, TValue >::Find( const TKey& key, TValue** ppOut )
{

	TValue* pValue;
	std::map<int, int*> s;
	_Map::iterator iter = m_Cache.find( key );
	if ( iter == m_Cache.end() )
		return false;
	pValue = iter->second;

	if ( ppOut != nullptr )
	{
		if ( *ppOut )
			(*ppOut)->Release();
		(*ppOut) = pValue;
		(*ppOut)->AddRef();
	}

	return true;
}