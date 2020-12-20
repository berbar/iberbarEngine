#pragma once

#include <iberbar/Paper2d/Node.h>

namespace iberbar
{
	struct UMouseEventData;
	struct UKeyboardEventData;

	namespace Renderer
	{
		class CRendererSprite;
	}

	namespace Paper2d
	{
		class CScene;
		class CPhysicsWorld;
		class CComponentSystem_HandleMouseInput;
		class CComponentSystem_HandleKeyboardInput;

		class __iberbarPaper2dApis__ CDirector
		{
		public:
			CDirector( Renderer::CRendererSprite* pRendererSprite );
			~CDirector();

		public:
			void RunScene( CScene* pScene );
			void UpdateScene( float delta );
			void DrawScene();
			void OnResizeViewport( int nWidth, int nHeight );
			void HandleMouse( const UMouseEventData* pEventData );
			void HandleKeyboard( const UKeyboardEventData* pEventData );

			Renderer::CRendererSprite* GetRendererSprite() { return m_pRendererSprite; }
			CScene* GetRunningScene() { return m_pRunningScene; }

			// ·µ»ØComponentÏµÍ³
			CComponentSystem_HandleMouseInput* GetComponentSystem_HandleMouseInput() { return m_pComponentSystem_HandleMouseInput; }
			CComponentSystem_HandleKeyboardInput* GetComponentSystem_HandleKeyboardInput() { return m_pComponentSystem_HandleKeyboardInput; }

		private:
			Renderer::CRendererSprite* m_pRendererSprite;
			CScene* m_pRunningScene;
			CPhysicsWorld* m_pPhysicsWorld;

			CComponentSystem_HandleMouseInput* m_pComponentSystem_HandleMouseInput;
			CComponentSystem_HandleKeyboardInput* m_pComponentSystem_HandleKeyboardInput;

		private:
			static CDirector* sm_pSharedInstance;
		public:
			static CDirector* sGetShared() { return sm_pSharedInstance; }
		};
	}
}
