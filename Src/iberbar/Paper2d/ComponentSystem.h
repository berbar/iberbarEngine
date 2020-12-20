#pragma once


#include <iberbar/Paper2d/Component.h>
#include <iberbar/Paper2d/Component_Updatable.h>
#include <iberbar/Paper2d/Component_HandleMouseInput.h>
#include <iberbar/Paper2d/Component_HandleKeyboardInput.h>


namespace iberbar
{
	namespace Paper2d
	{

		template < typename TComponent >
		class TComponentSystem
		{
		public:
			TComponentSystem();
			~TComponentSystem();

		public:
			void AddComponent( TComponent* pComponent );

			template < typename... Ts >
			void ExecuteList( Ts&&... xs );

		private:
			std::list<TComponent*> m_ComponentList;
			std::list<TComponent*> m_ComponentList_NewComing;
		};


		class CComponent_HandleMouseInput;
		class CComponent_HandleKeyboardInput;
		struct UComponentMouseEventData;
		struct UComponentKeyboardEventData;



		void ExecuteComponent( CComponent_HandleMouseInput* pComponent, const UComponentMouseEventData* EventData );
		void ExecuteComponent( CComponent_HandleKeyboardInput* pComponent, const UComponentKeyboardEventData* EventData );


		class CComponentSystem_HandleMouseInput
			: public TComponentSystem< CComponent_HandleMouseInput >
		{
		public:
			void HandleMouse( const UComponentMouseEventData* EventData );
		};


		class CComponentSystem_HandleKeyboardInput
			: public TComponentSystem< CComponent_HandleKeyboardInput >
		{
		public:
			void HandleKeyboard( const UComponentKeyboardEventData* EventData );
		};
	}
}



template < typename TComponent >
iberbar::Paper2d::TComponentSystem<TComponent>::TComponentSystem()
	: m_ComponentList()
	, m_ComponentList_NewComing()
{
}


template < typename TComponent >
iberbar::Paper2d::TComponentSystem<TComponent>::~TComponentSystem()
{
	if ( m_ComponentList_NewComing.empty() == false )
	{
		auto iter = m_ComponentList_NewComing.begin();
		auto end = m_ComponentList_NewComing.end();
		for ( ; iter != end; iter++ )
		{
			UNKNOWN_SAFE_RELEASE_NULL( *iter );
		}
	}

	if ( m_ComponentList.empty() == false )
	{
		auto iter = m_ComponentList.begin();
		auto end = m_ComponentList.end();
		for ( ; iter != end; iter++ )
		{
			UNKNOWN_SAFE_RELEASE_NULL( *iter );
		}
	}

	m_ComponentList_NewComing.clear();
	m_ComponentList.clear();
}


template < typename TComponent >
void iberbar::Paper2d::TComponentSystem<TComponent>::AddComponent( TComponent* pComponent )
{
	if ( pComponent == nullptr )
		return;

	if ( m_ComponentList_NewComing.empty() == false )
	{
		auto iter = m_ComponentList_NewComing.begin();
		auto end = m_ComponentList_NewComing.end();
		for ( ; iter != end; iter++ )
		{
			if ( (*iter) == pComponent )
				return;
		}
	}

	if ( m_ComponentList.empty() == false )
	{
		auto iter = m_ComponentList.begin();
		auto end = m_ComponentList.end();
		for ( ; iter != end; iter++ )
		{
			if ( (*iter) == pComponent )
				return;
		}
	}

	pComponent->AddRef();
	m_ComponentList_NewComing.push_back( pComponent );
}


template < typename TComponent >
template < typename... Ts >
void iberbar::Paper2d::TComponentSystem<TComponent>::ExecuteList( Ts&&... xs )
{
	if ( m_ComponentList_NewComing.empty() == false )
	{
		auto iter = m_ComponentList_NewComing.begin();
		auto end = m_ComponentList_NewComing.end();
		for ( ; iter != end; iter++ )
		{
			m_ComponentList.push_back( (*iter) );
		}
		m_ComponentList_NewComing.clear();
	}

	if ( m_ComponentList.empty() == false )
	{
		auto iter = m_ComponentList.begin();
		auto end = m_ComponentList.end();
		for ( ; iter != end; )
		{
			if ( (*iter)->WillRemoveFromSystem() == true )
			{
				UNKNOWN_SAFE_RELEASE_NULL( *iter );
				iter = m_ComponentList.erase( iter );
			}
			else
			{
				if ( (*iter)->IsEnable() )
				{
					ExecuteComponent( *iter, std::forward<Ts>( xs )... );
					//(*iter)->Update( nDelta );
				}

				iter++;
			}
		}
	}
}






FORCEINLINE void iberbar::Paper2d::CComponentSystem_HandleMouseInput::HandleMouse( const UComponentMouseEventData* EventData )
{
	ExecuteList( EventData );
}


FORCEINLINE void iberbar::Paper2d::ExecuteComponent( CComponent_HandleMouseInput* pComponent, const UComponentMouseEventData* EventData )
{
	pComponent->OnMouseEvent( EventData );
}






FORCEINLINE void iberbar::Paper2d::CComponentSystem_HandleKeyboardInput::HandleKeyboard( const UComponentKeyboardEventData* EventData )
{
	ExecuteList( EventData );
}


FORCEINLINE void iberbar::Paper2d::ExecuteComponent( CComponent_HandleKeyboardInput* pComponent, const UComponentKeyboardEventData* EventData )
{
	pComponent->OnKeyboardEvent( EventData );
}


