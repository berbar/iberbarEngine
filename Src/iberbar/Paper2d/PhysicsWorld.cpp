
#include <iberbar/Paper2d/PhysicsWorld.h>
#include <iberbar/Paper2d/Node.h>



iberbar::Paper2d::CPhysicsWorld* iberbar::Paper2d::CPhysicsWorld::sm_pSharedInstance = nullptr;

iberbar::Paper2d::CPhysicsWorld::CPhysicsWorld()
	: m_DirtyNodes()
{
	sm_pSharedInstance = this;
}


iberbar::Paper2d::CPhysicsWorld::~CPhysicsWorld()
{
	auto iter = m_DirtyNodes.begin();
	auto end = m_DirtyNodes.end();
	for ( ; iter != end; iter ++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( *iter );
	}
	m_DirtyNodes.clear();

	sm_pSharedInstance = nullptr;
}


void iberbar::Paper2d::CPhysicsWorld::AddDirtyNode( CNode* pNode )
{
	if ( pNode == nullptr )
		return;

	m_DirtyNodes.push_back( pNode );
	pNode->AddRef();
}


void iberbar::Paper2d::CPhysicsWorld::Update( float nDelta )
{
	if ( m_DirtyNodes.empty() == false )
	{
		auto iter = m_DirtyNodes.begin();
		auto end = m_DirtyNodes.end();
		for ( ; iter != end; iter++ )
		{
			if ( (*iter)->IsTransformDirty() == true )
			{
				(*iter)->UpdateTransform();
			}
			(*iter)->Release();
			(*iter) = nullptr;
		}
		m_DirtyNodes.clear();
	}
}

