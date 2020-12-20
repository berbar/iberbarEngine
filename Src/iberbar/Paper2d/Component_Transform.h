#pragma once


#include <iberbar/Paper2d/Component.h>
#include <DirectXCollision.h>


namespace iberbar
{
	namespace Paper2d
	{
		class __iberbarPaper2dApis__ CComponent_Transform abstract
			: public CComponent
		{
		public:
			CComponent_Transform();
		public:
			virtual void OnUpdateTransform() = 0;
		};
	}
}