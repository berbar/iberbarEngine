#pragma once


#include <iberbar/Paper2d/Node.h>


namespace iberbar
{
	namespace Paper2d
	{
		class CCamera;

		class __iberbarPaper2dApis__ CScene
			: public CNode
		{
		public:
			CScene();
			virtual ~CScene();
		protected:
			CScene( const CScene& scene );

		public:
			virtual CScene* Clone() const override;
			virtual void UpdateTransform() override;
			virtual void Draw( CDrawContext* pContext ) override;

			CCamera* GetCamera();

		protected:
			virtual void DrawSelf( CDrawContext* pContext ) override;

		private:
			CCamera* m_pCamera;
		};
	}
}


FORCEINLINE iberbar::Paper2d::CCamera* iberbar::Paper2d::CScene::GetCamera()
{
	return m_pCamera;
}

