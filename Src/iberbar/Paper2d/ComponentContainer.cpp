
#include <iberbar/Paper2d/ComponentContainer.h>
#include <iberbar/Paper2d/Component.h>
#include <iberbar/Paper2d/Component_Updatable.h>
#include <iberbar/Utility/StdHelper_Vector.h>



iberbar::Paper2d::CComponentContainer::CComponentContainer( CNode* pOwner )
	: m_pOwner( pOwner )
	, m_ComponentList()
	, m_ComponentList_Updatable()
{
}


iberbar::Paper2d::CComponentContainer::~CComponentContainer()
{
	m_pOwner = nullptr;
	RemoveAll();
}


bool iberbar::Paper2d::CComponentContainer::Add( CComponent* pComponent )
{
	assert( pComponent != nullptr );
	assert( pComponent->GetOwner() == nullptr );

	const char* strName = pComponent->GetName().c_str();
	for ( size_t i = 0, s = m_ComponentList.size(); i < s; i++ )
	{
		if ( m_ComponentList[ i ] == pComponent )
			return false;

		if ( strcmp( m_ComponentList[ i ]->GetName().c_str(), strName ) == 0 )
			return false;
	}

	m_ComponentList.push_back( pComponent );
	if ( pComponent->IsComponentType( UComponentType::Updatable ) == true )
	{
		m_ComponentList_Updatable.push_back( (CComponent_Updatable*)pComponent );
	}

	pComponent->AddRef();
	pComponent->SetOwner( m_pOwner );

	return true;
}


bool iberbar::Paper2d::CComponentContainer::Remove( const char* name )
{
	auto iter = m_ComponentList.begin();
	auto end = m_ComponentList.end();
	CComponent* pComponent;
	for ( ; iter != end; iter++ )
	{
		pComponent = *iter;
		if ( strcmp( name, pComponent->GetName().c_str() ) == 0 )
		{
			if ( pComponent->IsComponentType( UComponentType::Updatable ) == true )
				StdHelper::VectorSafeEraseRef( m_ComponentList_Updatable, (CComponent_Updatable*)pComponent, false );

			UNKNOWN_SAFE_RELEASE_NULL( pComponent );

			m_ComponentList.erase( iter );

			return true;
		}
	}
	return false;
}


bool iberbar::Paper2d::CComponentContainer::Remove( CComponent* pComponent )
{
	if ( pComponent->IsComponentType( UComponentType::Updatable ) == true )
		StdHelper::VectorSafeEraseRef( m_ComponentList_Updatable, (CComponent_Updatable*)pComponent, false );

	return StdHelper::VectorSafeEraseRef( m_ComponentList, pComponent, true );
}


void iberbar::Paper2d::CComponentContainer::RemoveAll()
{
	auto iter = m_ComponentList.begin();
	auto end = m_ComponentList.end();
	for ( ; iter != end; iter++ )
	{
		(*iter)->SetOwner( nullptr );
		UNKNOWN_SAFE_RELEASE_NULL( *iter );
	}
	m_ComponentList.clear();

	m_ComponentList_Updatable.clear();
}


iberbar::Paper2d::CComponent* iberbar::Paper2d::CComponentContainer::Get( const char* name ) const
{
	auto iter = m_ComponentList.begin();
	auto end = m_ComponentList.end();
	for ( ; iter != end; iter++ )
	{
		if ( strcmp( name, (*iter)->GetName().c_str() ) == 0 )
		{
			return (*iter);
		}
	}
	return nullptr;
}


void iberbar::Paper2d::CComponentContainer::ExecuteUpdatableComponents( float nDelta )
{
	auto iter = m_ComponentList_Updatable.begin();
	auto end = m_ComponentList_Updatable.end();
	for ( ; iter != end; iter++ )
	{
		if ( (*iter)->GetEnable() == true )
		{
			(*iter)->Update( nDelta );
		}
	}
}

