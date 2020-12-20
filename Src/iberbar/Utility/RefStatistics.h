#pragma once

#include <iberbar/Utility/Unknown.h>
#include <map>
#include <functional>


namespace iberbar
{
	class __iberbarUtilityApi__ CRefStatistics final
	{
		friend CRef;
	private:
		CRefStatistics();
		~CRefStatistics();
		void Add( CRef* pRef );
		void Remove( CRef* pRef );
	public:
		void ForEach( std::function<void(CRef*)> func ) const;
		size_t GetCount() const { return m_Refs.size(); }
	private:
		std::map<uint64, CRef*> m_Refs;

	public:
		static CRefStatistics* sGetShared() { return &sm_pSharedInstance; }
	private:
		static CRefStatistics sm_pSharedInstance;
	};
}



FORCEINLINE void iberbar::CRefStatistics::Add( CRef* pRef )
{
	auto iter = m_Refs.find( (uint64)pRef );
	if ( iter != m_Refs.end() )
		return;
	m_Refs.insert( std::make_pair( (uint64)pRef, pRef ) );
}


FORCEINLINE void iberbar::CRefStatistics::Remove( CRef* pRef )
{
	m_Refs.erase( (uint64)pRef );
}

