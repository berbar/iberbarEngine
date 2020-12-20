#pragma once

#include <iberbar/Paper2d/Component.h>
#include <iberbar/Utility/Input.h>
#include <DirectXMath.h>


namespace iberbar
{
	namespace Paper2d
	{
		struct UComponentMouseEventData
		{
			UMouseEvent nEvent;
			int nMouseId;
			int nMouseWheel;
			DirectX::XMFLOAT2 MousePoint;
			DirectX::XMFLOAT2 WorldPoint;
		};

		class __iberbarPaper2dApis__ CComponent_HandleMouseInput abstract
			: public CComponent
		{
		public:
			CComponent_HandleMouseInput();
		protected:
			virtual void OnAttach() override;
			virtual void OnRemove() override;
		public:
			virtual void OnMouseEvent( const UComponentMouseEventData* EventData ) = 0;
		};
	}
}