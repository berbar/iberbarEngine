#pragma once

#include <iberbar/Base/Platform.h>


namespace iberbar
{
	class CFpsTick
	{
	public:
		void update( int64 nElapsedTime )
		{
			m_iFrameCount ++;
			m_fFpsTimeSlice += nElapsedTime;

			// 	if ( m_iFrameCount == 5 )
			// 	{
			// 		float lc_fFpsTimeCur = GetElapseTimeS();
			// 		m_fFpsTimeSlice = lc_fFpsTimeCur - m_fFpsTimePre;
			// 		m_fFPS = (float)m_iFrameCount / m_fFpsTimeSlice;
			// 
			// 		m_fFpsTimePre = lc_fFpsTimeCur;
			// 		m_iFrameCount = 0;
			// 	}

			if ( m_fFpsTimeSlice > 1.0f )
			{
				m_fFPS = (float)m_iFrameCount / m_fFpsTimeSlice;

				m_iFrameCount = 0;
				m_fFpsTimeSlice = 0.0f;
			}
		}
	private:
		int32  m_iFrameCount;
		int64   m_fFPS;
		int64   m_fFpsTimePre;
		int64   m_fFpsTimeSlice;
	};
}