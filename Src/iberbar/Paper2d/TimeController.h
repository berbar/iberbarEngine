#pragma once


#include <iberbar/Paper2d/Node.h>


namespace iberbar
{
	namespace Paper2d
	{
		class CTimeController
			: public CNode
		{
		public:
			CTimeController();

		public:
			virtual void Update( float nDelta ) override;

			void SetTimeScale( float nScale ) { m_nTimeScale = nScale; }
			float GetTimeScale() { return m_nTimeScale; }

		protected:
			float m_nTimeScale;
		};
	}
}
