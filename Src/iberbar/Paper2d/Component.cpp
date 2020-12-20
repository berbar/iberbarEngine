
#include <iberbar/Paper2d/Component.h>
#include <iberbar/Paper2d/ComponentSystem.h>
#include <iberbar/Paper2d/Component_CustomAttributes.h>
#include <iberbar/Paper2d/Component_Updatable.h>
#include <iberbar/Paper2d/Component_HandleMouseInput.h>
#include <iberbar/Paper2d/Component_HandleKeyboardInput.h>
#include <iberbar/Paper2d/Component_Transform.h>
#include <iberbar/Paper2d/Component_Bounding.h>
#include <iberbar/Paper2d/Director.h>
#include <iberbar/Paper2d/Node.h>



iberbar::Paper2d::CComponent::CComponent( UComponentType nComponentType )
	: m_nComponentType( nComponentType )
	, m_pOwner( nullptr )
	, m_bEnable( true )
	, m_bRemoveFromSystem( false )
	, m_strName( "" )
{
}


iberbar::Paper2d::CComponent::~CComponent()
{

}


void iberbar::Paper2d::CComponent::OnAttach()
{
	m_bRemoveFromSystem = false;
}


void iberbar::Paper2d::CComponent::OnRemove()
{
	m_bRemoveFromSystem = true;
}


void iberbar::Paper2d::CComponent::SetEnable( bool bEnable )
{
	m_bEnable = bEnable;
}


void iberbar::Paper2d::CComponent::SetOwner( CNode* pNode )
{
	if ( pNode == nullptr )
	{
		if ( m_pOwner == nullptr )
			return;

		m_pOwner = nullptr;
		OnRemove();
	}
	else
	{
		if ( m_pOwner == nullptr )
		{
			m_pOwner = pNode;
			OnAttach();
		}
		else
		{
			return;
		}
	}
}


bool iberbar::Paper2d::CComponent::IsEnable() const
{
	if ( m_bEnable == false )
		return false;

	if ( m_pOwner != nullptr )
		return m_pOwner->IsEnable();

	return true;
}






const char iberbar::Paper2d::CComponent_CustomAttributes::sm_DefaultName[] = "_DefaultCustomAttributes";


iberbar::Paper2d::CComponent_CustomAttributes::CComponent_CustomAttributes()
	: CComponent( UComponentType::CustomAttributes )
	, m_AttrsMap()
{
	SetName( sm_DefaultName );
}


iberbar::Paper2d::CComponent_CustomAttributes::~CComponent_CustomAttributes()
{
	auto iter = m_AttrsMap.begin();
	auto end = m_AttrsMap.end();
	for ( ; iter != end; iter++ )
	{
		SAFE_DELETE( iter->second );
	}
	m_AttrsMap.clear();
}







iberbar::Paper2d::CComponent_Updatable::CComponent_Updatable()
	: CComponent( UComponentType::Updatable )
{

}






iberbar::Paper2d::CComponent_HandleMouseInput::CComponent_HandleMouseInput()
	: CComponent( UComponentType::HandleMouseInput )
{

}


void iberbar::Paper2d::CComponent_HandleMouseInput::OnAttach()
{
	CComponent::OnAttach();
	CDirector::sGetShared()->GetComponentSystem_HandleMouseInput()->AddComponent( this );
}


void iberbar::Paper2d::CComponent_HandleMouseInput::OnRemove()
{
	CComponent::OnRemove();
}







iberbar::Paper2d::CComponent_HandleKeyboardInput::CComponent_HandleKeyboardInput()
	: CComponent( UComponentType::HandleKeyboardInput )
{

}


void iberbar::Paper2d::CComponent_HandleKeyboardInput::OnAttach()
{
	CComponent::OnAttach();
	CDirector::sGetShared()->GetComponentSystem_HandleKeyboardInput()->AddComponent( this );
}


void iberbar::Paper2d::CComponent_HandleKeyboardInput::OnRemove()
{
	CComponent::OnRemove();
}







iberbar::Paper2d::CComponent_Transform::CComponent_Transform()
	: CComponent( UComponentType::Transform )
{
}








iberbar::Paper2d::CComponent_Bounding::CComponent_Bounding( UComponentBoundingType nBoundingType )
	: CComponent( UComponentType::Bounding )
	, m_nBoundingType( nBoundingType )
{
}








iberbar::Paper2d::CComponent_BoundingBox::CComponent_BoundingBox()
	: CComponent_Bounding( UComponentBoundingType::BoundingBox )
	, m_Box()
{
}


void iberbar::Paper2d::CComponent_BoundingBox::OnUpdateTransform()
{
	if ( GetOwner() == nullptr )
		return;

	const CNodeTransform* pTransform = GetOwner()->GetTransform();
	const DirectX::XMFLOAT2& size = pTransform->GetContentSize();
	DirectX::XMFLOAT2 floats[ 2 ] = {
		DirectX::XMFLOAT2( 0, 0 ),
		DirectX::XMFLOAT2( size.x, size.y )
	};
	DirectX::XMVECTOR points[ 2 ] = {
		DirectX::XMLoadFloat2( &floats[ 0 ] ),
		DirectX::XMLoadFloat2( &floats[ 1 ] )
	};
	DirectX::BoundingBox BoxTemp;
	DirectX::BoundingBox::CreateFromPoints( BoxTemp, points[ 0 ], points[ 1 ] );
	BoxTemp.Transform( m_Box, pTransform->GetMatrix() );
}


