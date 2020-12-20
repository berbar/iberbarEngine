#pragma once


#include <iberbar/Paper2d/Headers.h>


namespace iberbar
{
	namespace Paper2d
	{
		class CNode;

		class __iberbarPaper2dApis__ CPhysicsWorld
		{
		public:
			CPhysicsWorld();
			~CPhysicsWorld();

		public:
			void AddDirtyNode( CNode* pNode );
			void Update( float nDelta );

		private:
			std::vector<CNode*> m_DirtyNodes;


		private:
			static CPhysicsWorld* sm_pSharedInstance;
		public:
			static CPhysicsWorld* GetShared() { return sm_pSharedInstance; }
		};
	}
}

