#pragma once



#include <iberbar/Paper2d/Headers.h>


namespace iberbar
{
	namespace Paper2d
	{
		class CNode;
		class CComponent;
		class CComponent_Updatable;

		class __iberbarPaper2dApis__ CComponentContainer
		{
		public:
			CComponentContainer( CNode* pOwner );
			~CComponentContainer();

		public:
			bool Add( CComponent* pComponent );
			bool Remove( const char* name );
			bool Remove( CComponent* pComponent );
			void RemoveAll();
			bool IsEmpty() const { return m_ComponentList.empty(); }
			CComponent* Get( const char* name ) const;
			CComponent* Get( int nIndex ) const;
			int GetCount() const;

			void ExecuteUpdatableComponents( float nDelta );

		public:
			CNode* m_pOwner;
			std::vector<CComponent*> m_ComponentList;
			std::vector<CComponent_Updatable*> m_ComponentList_Updatable;
		};
	}
}




FORCEINLINE iberbar::Paper2d::CComponent* iberbar::Paper2d::CComponentContainer::Get( int nIndex ) const
{
	assert( nIndex >= 0 && nIndex < m_ComponentList.size() );
	return m_ComponentList[ nIndex ];
}


FORCEINLINE int iberbar::Paper2d::CComponentContainer::GetCount() const
{
	return m_ComponentList.size();
}

