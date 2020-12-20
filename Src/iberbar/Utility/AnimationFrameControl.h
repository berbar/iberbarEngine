

#ifndef __iberbar__Base__AnimationFrameControl_h__
#define __iberbar__Base__AnimationFrameControl_h__

#include <iberbar\Base\Unknown.h>


namespace iberbar
{
	
	class IBERBAR_DLL CAnimationFrameController
		: public CUnknown
	{
	public:
		enum _FrameSet
		{
			FrameSet_Zero,       // 从0开始偏移，即真实数值
			FrameSet_Begin,      // 从第一帧开始偏移
			FrameSet_Current,    // 从当前帧开始偏移
			FrameSet_End,        // 从最后一帧开始偏移，正数即向前偏移
		};

		IBERBAR_UNKNOWN_CLONE_DISABLED( CAnimationFrameController );

	public:
		CAnimationFrameController( void );


	public:
		void   setFrameLength( int nBegin, int nEnd );
		void   setFrameCur( int nOffset, int nType = FrameSet_Zero ); // 设置当前帧的位置
		void   setRepeatTimesMax( int nTimesMax ); // 混合次数，1次相当于有0引索渐变到1引索，2次等于先从0到1渐变再从1渐变到0，以次类推，当次数小于等于0时为无限循环
		void   setRepeatTimesMaxInfinite()                { setRepeatTimesMax( 0 ); }
		void   setTimePerFrame( float fTime );
		void   start();
		void   stop();
		void   resume();
		void   pause();
		void   update( float nElapsedTime ); //通过时间来跟新当前帧


	public:
		int    getFrameBegin() const          { return m_nFrameBegin; }
		int    getFrameEnd() const            { return m_nFrameEnd; }
		int    getFrameCur() const            { return m_nFrameCur; }
		bool   getIsRepeat() const            { return ( m_nRepeatTimesMax <= 0 ) ? true : false; }
		int    getRepeatTimesMax() const      { return m_nRepeatTimesMax; }
		int    getRepeatTimesAlready() const  { return m_nRepeatTimesAlready; }
		float  getTimePerFrame() const        { return m_fTimePerFrame; }
		bool   getHasStarted() const          { return m_bHasStarted; }
		bool   getIsPaused() const            { return m_bPaused; }


	private:
		int    m_nFrameCur;
		int    m_nFrameBegin;
		int    m_nFrameEnd;    
		int    m_nRepeatTimesMax; // 是否循环播放
		int    m_nRepeatTimesAlready;
		float  m_fTimePerFrame; //一帧所需时间
		float  m_fElapsedTime;
		bool   m_bHasStarted;
		bool   m_bPaused;
	};

	IBERBAR_UNKNOWN_PTR_DECLARE( CAnimationFrameController );
}

#endif