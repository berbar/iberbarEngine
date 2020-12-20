


#include <iberbar/Paper2d/Node.h>
#include <iberbar/Paper2d/Director.h>
#include <iberbar/Paper2d/ComponentContainer.h>
#include <iberbar/Paper2d/Component.h>
#include <iberbar/Paper2d/Component_Transform.h>
#include <iberbar/Paper2d/Component_Bounding.h>
#include <iberbar/Paper2d/Transform.h>
#include <iberbar/Paper2d/PhysicsWorld.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/Utility/StdHelper_List.h>

iberbar::Paper2d::CNode::CNode()
	: m_pParentNode( nullptr )
	, m_pComponentContainer( new CComponentContainer( this ) )
	, m_bVisible( true )
	, m_bEnable( true )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTable( nullptr )
	, m_nZOrder( 0 )
	, m_Prepare()
	, m_Children()

	, m_pTransform( new CNodeTransform() )
	, m_bTransformDirty( false )
{
}


iberbar::Paper2d::CNode::CNode( const CNode& node )
	: m_pParentNode( nullptr )
	, m_pComponentContainer( new CComponentContainer( this ) )
	, m_bVisible( node.m_bVisible )
	, m_bEnable( node.m_bEnable )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTable( nullptr )
	, m_nZOrder( node.m_nZOrder )
	, m_Prepare()
	, m_Children()

	, m_pTransform( new CNodeTransform() )
	, m_bTransformDirty( false )
{
}


iberbar::Paper2d::CNode::~CNode()
{
	SAFE_DELETE( m_pTransform );
	SAFE_DELETE( m_pComponentContainer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTable );
	RemoveAllChildren();
}


iberbar::Paper2d::CNode* iberbar::Paper2d::CNode::Clone() const
{
	return new CNode( *this );
}


bool iberbar::Paper2d::CNode::IsVisible() const
{
	if ( m_pParentNode == nullptr )
		return m_bVisible;

	return m_bVisible && m_pParentNode->IsVisible();
}


bool iberbar::Paper2d::CNode::IsEnable() const
{
	if ( m_bEnable == false )
		return false;

	if ( m_pParentNode != nullptr )
		return m_pParentNode->IsEnable();

	return true;
}


void iberbar::Paper2d::CNode::Init()
{
	Renderer::CRendererSprite* pRendererSprite = CDirector::sGetShared()->GetRendererSprite();
	pRendererSprite->GetDefaultShaderState( &m_pShaderState );
	pRendererSprite->CreateDefaultShaderVariableTable( &m_pShaderVariableTable );

	m_pShaderVariableTable->SetBool( RHI::UShaderType::Vertex, Renderer::s_strShaderVarName_RHW, false );
}


void iberbar::Paper2d::CNode::UpdateTransform()
{
	if ( m_pParentNode == nullptr )
	{
		m_pTransform->UpdateTransform();
	}
	else
	{
		m_pTransform->UpdateTransform( m_pParentNode->m_pTransform->GetMatrixForChildren() );
	}

	// 更新Bounding组件
	int nComponentCount = m_pComponentContainer->GetCount();
	CComponent* pComponentTemp = nullptr;
	for ( int i = 0; i < nComponentCount; i++ )
	{
		pComponentTemp = m_pComponentContainer->Get( i );
		if ( pComponentTemp->IsComponentType( UComponentType::Bounding ) == true )
		{
			((CComponent_Bounding*)pComponentTemp)->OnUpdateTransform();
		}
		else if ( pComponentTemp->IsComponentType( UComponentType::Transform ) == true )
		{
			((CComponent_Transform*)pComponentTemp)->OnUpdateTransform();
		}
	}

	// 更新子节点的Transform
	auto iter = m_Children.begin();
	auto end = m_Children.end();
	for ( ; iter != end; iter ++ )
	{
		(*iter)->UpdateTransform();
	}

	m_bTransformDirty = false;
}


void iberbar::Paper2d::CNode::InsertChild( CNode* pNode )
{
	if ( pNode->GetParentNode() != nullptr )
	{
		return;
	}

	pNode->m_pParentNode = this;
	pNode->AddRef();
	//m_Prepare.push_back( pNode );
	m_Children.push_back( pNode );
	pNode->SetTransformDirty();
	
	//SetTransformDirty();
}


void iberbar::Paper2d::CNode::RemoveChild( CNode* pNode )
{
	if ( pNode->GetParentNode() == this )
	{
		auto iter = m_Children.begin();
		auto end = m_Children.end();
		for ( ; iter != end; iter ++ )
		{
			if ( (*iter) == pNode )
			{
				(*iter)->m_pParentNode = nullptr;
				UNKNOWN_SAFE_RELEASE_NULL( *iter );
				m_Children.erase( iter );
				break;
			}
		}
	}
}


void iberbar::Paper2d::CNode::RemoveAllChildren()
{
	//if ( m_Prepare.empty() == false )
	//{
	//	StdHelper::ListSafeClearRef( m_Prepare, true );
	//	//auto iter = m_Children.begin();
	//	//auto end = m_Children.end();
	//	//for ( ; iter != end; iter++ )
	//	//{
	//	//	(*iter)->m_pParentNode = nullptr;
	//	//	UNKNOWN_SAFE_RELEASE_NULL( *iter );
	//	//}
	//}

	if ( m_Children.empty() == false )
	{
		StdHelper::ListSafeClearRef( m_Children, true );
		//auto iter = m_Children.begin();
		//auto end = m_Children.end();
		//for ( ; iter != end; iter++ )
		//{
		//	(*iter)->m_pParentNode = nullptr;
		//	UNKNOWN_SAFE_RELEASE_NULL( *iter );
		//}
	}

}


iberbar::Paper2d::CNode* iberbar::Paper2d::CNode::FindChild( const char* strName ) const
{
	auto iter = m_Children.begin();
	auto end = m_Children.end();
	for ( ; iter != end; iter++ )
	{
		if ( strcmp( (*iter)->m_strId.c_str(), strName ) == 0 )
			return (*iter);
	}

	CNode* pNodeFind = nullptr;
	iter = m_Children.begin();
	end = m_Children.end();
	for ( ; iter != end; iter++ )
	{
		pNodeFind = (*iter)->FindChild( strName );
		if ( pNodeFind != nullptr )
			return pNodeFind;
	}

	return nullptr;
}


void iberbar::Paper2d::CNode::Draw( CDrawContext* pContext )
{
	if ( GetVisible() == false )
		return;

	DrawSelf( pContext );

	auto iter = m_Children.begin();
	auto end = m_Children.end();
	for ( ; iter != end; iter ++ )
	{
		(*iter)->Draw( pContext );
	}
}


void iberbar::Paper2d::CNode::Update( float nDelta )
{
	//if ( m_Prepare.empty() == false )
	//{
	//	auto iter = m_Prepare.begin();
	//	auto end = m_Prepare.end();
	//	for ( ; iter != end; iter++ )
	//	{
	//		m_Children.push_back( *iter );
	//		(*iter)->SetTransformDirty();
	//	}
	//	m_Prepare.clear();
	//}

	if ( GetEnable() == false )
		return;

	UpdateSelf( nDelta );

	if ( m_Children.empty() == false )
	{
		auto iter = m_Children.begin();
		auto end = m_Children.end();
		for ( ; iter != end; iter++ )
		{
			(*iter)->Update( nDelta );
		}
	}
}


void iberbar::Paper2d::CNode::SetTransformDirty()
{
	if ( m_bTransformDirty == false )
	{
		m_bTransformDirty = true;
		CPhysicsWorld::GetShared()->AddDirtyNode( this );
	}
}


void iberbar::Paper2d::CNode::DrawSelf( CDrawContext* pContext )
{
}


void iberbar::Paper2d::CNode::UpdateSelf( float nDelta )
{
	m_pComponentContainer->ExecuteUpdatableComponents( nDelta );
}

