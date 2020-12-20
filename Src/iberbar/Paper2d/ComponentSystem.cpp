//
#include <iberbar/Paper2d/ComponentSystem.h>
//#include <iberbar/Paper2d/Component_Updatable.h>
//#include <iberbar/Paper2d/Component_HandleMouseInput.h>
//#include <iberbar/Paper2d/Component_HandleKeyboardInput.h>
//#include <iberbar/Paper2d/Node.h>
//
//
//
//iberbar::Paper2d::CComponentSystem_Updatable* iberbar::Paper2d::CComponentSystem_Updatable::sm_pSharedInstance = nullptr;
//
////
////iberbar::Paper2d::CComponentSystem_Updatable::CComponentSystem_Updatable()
////	: m_ComponentList()
////	, m_ComponentList_NewComing()
////{
////	sm_pSharedInstance = this;
////}
////
////
////iberbar::Paper2d::CComponentSystem_Updatable::~CComponentSystem_Updatable()
////{
////	sm_pSharedInstance = nullptr;
////
////	if ( m_ComponentList_NewComing.empty() == false )
////	{
////		auto iter = m_ComponentList_NewComing.begin();
////		auto end = m_ComponentList_NewComing.end();
////		for ( ; iter != end; iter ++ )
////		{
////			UNKNOWN_SAFE_RELEASE_NULL( *iter );
////		}
////	}
////
////	if ( m_ComponentList.empty() == false )
////	{
////		auto iter = m_ComponentList.begin();
////		auto end = m_ComponentList.end();
////		for ( ; iter != end; iter++ )
////		{
////			UNKNOWN_SAFE_RELEASE_NULL(*iter);
////		}
////	}
////
////	m_ComponentList_NewComing.clear();
////	m_ComponentList.clear();
////}
////
////
////void iberbar::Paper2d::CComponentSystem_Updatable::AddComponent( CComponent_Updatable* pComponent )
////{
////	if ( pComponent == nullptr )
////		return;
////
////	if ( m_ComponentList_NewComing.empty() == false )
////	{
////		auto iter = m_ComponentList_NewComing.begin();
////		auto end = m_ComponentList_NewComing.end();
////		for ( ; iter != end; iter++ )
////		{
////			if ( (*iter) == pComponent )
////				return;
////		}
////	}
////
////	if ( m_ComponentList.empty() == false )
////	{
////		auto iter = m_ComponentList.begin();
////		auto end = m_ComponentList.end();
////		for ( ; iter != end; iter++ )
////		{
////			if ( (*iter) == pComponent )
////				return;
////		}
////	}
////
////	pComponent->AddRef();
////	m_ComponentList_NewComing.push_back( pComponent );
////}
////
////
////void iberbar::Paper2d::CComponentSystem_Updatable::Update( float nDelta )
////{
////	ExecuteList( nDelta );
////}
//
//
//
//
//
//
//
//iberbar::Paper2d::CComponentSystem_HandleMouseInput* iberbar::Paper2d::CComponentSystem_HandleMouseInput::sm_pSharedInstance = nullptr;
//
//
//iberbar::Paper2d::CComponentSystem_HandleMouseInput::CComponentSystem_HandleMouseInput()
//	: m_ComponentList()
//	, m_ComponentList_NewComing()
//{
//	sm_pSharedInstance = this;
//}
//
//
//iberbar::Paper2d::CComponentSystem_HandleMouseInput::~CComponentSystem_HandleMouseInput()
//{
//	sm_pSharedInstance = nullptr;
//
//	if ( m_ComponentList_NewComing.empty() == false )
//	{
//		auto iter = m_ComponentList_NewComing.begin();
//		auto end = m_ComponentList_NewComing.end();
//		for ( ; iter != end; iter++ )
//		{
//			UNKNOWN_SAFE_RELEASE_NULL( *iter );
//		}
//	}
//
//	if ( m_ComponentList.empty() == false )
//	{
//		auto iter = m_ComponentList.begin();
//		auto end = m_ComponentList.end();
//		for ( ; iter != end; iter++ )
//		{
//			UNKNOWN_SAFE_RELEASE_NULL( *iter );
//		}
//	}
//
//	m_ComponentList_NewComing.clear();
//	m_ComponentList.clear();
//}
//
//
//void iberbar::Paper2d::CComponentSystem_HandleMouseInput::AddComponent( CComponent_HandleMouseInput* pComponent )
//{
//	if ( pComponent == nullptr )
//		return;
//
//	pComponent->AddRef();
//	m_ComponentList_NewComing.push_back( pComponent );
//}
//
//
//void iberbar::Paper2d::CComponentSystem_HandleMouseInput::HandleMouse( const UComponentMouseEventData* EventData )
//{
//	if ( m_ComponentList_NewComing.empty() == false )
//	{
//		auto iter = m_ComponentList_NewComing.begin();
//		auto end = m_ComponentList_NewComing.end();
//		for ( ; iter != end; iter++ )
//		{
//			m_ComponentList.push_back( (*iter) );
//		}
//		m_ComponentList_NewComing.clear();
//	}
//
//	if ( m_ComponentList.empty() == false )
//	{
//		auto iter = m_ComponentList.begin();
//		auto end = m_ComponentList.end();
//		for ( ; iter != end; )
//		{
//			if ( (*iter)->WillRemoveFromSystem() == true )
//			{
//				UNKNOWN_SAFE_RELEASE_NULL( *iter );
//				iter = m_ComponentList.erase( iter );
//			}
//			else
//			{
//				if ( (*iter)->GetOwner()->IsEnable() )
//				{
//					(*iter)->OnMouseEvent( EventData );
//				}
//				iter++;
//			}
//		}
//	}
//}
//
//
//
//
//
//
//
//
//iberbar::Paper2d::CComponentSystem_HandleKeyboardInput* iberbar::Paper2d::CComponentSystem_HandleKeyboardInput::sm_pSharedInstance = nullptr;
//
//
//iberbar::Paper2d::CComponentSystem_HandleKeyboardInput::CComponentSystem_HandleKeyboardInput()
//	: m_ComponentList()
//	, m_ComponentList_NewComing()
//{
//	sm_pSharedInstance = this;
//}
//
//
//iberbar::Paper2d::CComponentSystem_HandleKeyboardInput::~CComponentSystem_HandleKeyboardInput()
//{
//	sm_pSharedInstance = nullptr;
//
//	if ( m_ComponentList_NewComing.empty() == false )
//	{
//		auto iter = m_ComponentList_NewComing.begin();
//		auto end = m_ComponentList_NewComing.end();
//		for ( ; iter != end; iter++ )
//		{
//			UNKNOWN_SAFE_RELEASE_NULL( *iter );
//		}
//	}
//
//	if ( m_ComponentList.empty() == false )
//	{
//		auto iter = m_ComponentList.begin();
//		auto end = m_ComponentList.end();
//		for ( ; iter != end; iter++ )
//		{
//			UNKNOWN_SAFE_RELEASE_NULL( *iter );
//		}
//	}
//
//	m_ComponentList_NewComing.clear();
//	m_ComponentList.clear();
//}
//
//
//void iberbar::Paper2d::CComponentSystem_HandleKeyboardInput::AddComponent( CComponent_HandleKeyboardInput* pComponent )
//{
//	if ( pComponent == nullptr )
//		return;
//
//	pComponent->AddRef();
//	m_ComponentList_NewComing.push_back( pComponent );
//}
//
//
//void iberbar::Paper2d::CComponentSystem_HandleKeyboardInput::HandleKeyboard( const UComponentKeyboardEventData* EventData )
//{
//	if ( m_ComponentList_NewComing.empty() == false )
//	{
//		auto iter = m_ComponentList_NewComing.begin();
//		auto end = m_ComponentList_NewComing.end();
//		for ( ; iter != end; iter++ )
//		{
//			m_ComponentList.push_back( (*iter) );
//		}
//		m_ComponentList_NewComing.clear();
//	}
//
//	if ( m_ComponentList.empty() == false )
//	{
//		auto iter = m_ComponentList.begin();
//		auto end = m_ComponentList.end();
//		for ( ; iter != end; )
//		{
//			if ( (*iter)->WillRemoveFromSystem() == true )
//			{
//				UNKNOWN_SAFE_RELEASE_NULL( *iter );
//				iter = m_ComponentList.erase( iter );
//			}
//			else
//			{
//				if ( (*iter)->GetOwner()->IsEnable() )
//				{
//					(*iter)->OnKeyboardEvent( EventData );
//				}
//				iter++;
//			}
//		}
//	}
//}
//
