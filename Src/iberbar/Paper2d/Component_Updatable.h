#pragma once


#include <iberbar/Paper2d/Component.h>


namespace iberbar
{
	namespace Paper2d
	{
		class __iberbarPaper2dApis__ CComponent_Updatable abstract
			: public CComponent
		{
		public:
			CComponent_Updatable();
		public:
			virtual void Update( float nDelta ) = 0;
		};
	}
}

