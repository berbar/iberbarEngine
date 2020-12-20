
#pragma once

#include <iberbar\Utility\Unknown.h>
#include <string>
#include <list>

namespace iberbar
{
	class CTimeline;
	class CTimer;
	IBERBAR_UNKNOWN_PTR_DECLARE( CTimeline );
	IBERBAR_UNKNOWN_PTR_DECLARE( CTimer );
	typedef PTR_CTimer     Timer;
	typedef PTR_CTimeline  Timeline;

	typedef void (CRef::*PTimerProc)( CTimer* );


	//////////////////////////////////////////////////////////////////////////
	//
	// 时间线 CTimeline
	//
	// 时间线包含子时间线和计时器
	//////////////////////////////////////////////////////////////////////////
	class CTimeline
		: public CRef
	{

	public:
		typedef std::list< Timer >  TimerGroup;
		typedef std::list< Timeline > TimelineGroup;

	public:
		CTimeline( void );
		~CTimeline() override;


	public:
		// 开启时间线的运行
		void resume() { m_bStarted = true; }

		// 停止时间线的运行
		void pause() { m_bStarted = false; }

		// 缩放时间线的维度
		void scale( float param ) { m_nScaleParam = param; }

		// 运行时间线
		// 移除已经销毁的时间线
		// 移除已经停止的计时器
		void run( float nElapsedTime );

		// 销毁时间线
		void destroy() { m_bEnable = false; }

	public:
		bool isStarted()    { return m_bStarted; }
		bool IsEnable()     { return m_bEnable; }

	public:
		// 添加计时器
		Timer addTimer();

		Timer getTimer( const std::string& name );

		// 移除计时器
		// 先暂停再停止计时器，然后从时间线中移除计时器
		void  removeTimer( Timer timer );

		// 添加子时间轴
		Timeline addTimeline();


	protected:
		bool m_bStarted;
		bool m_bEnable;
		float m_nScaleParam;
		TimerGroup m_timersTemp;
		TimerGroup m_timers;
		TimelineGroup m_timelines;
		TimelineGroup m_timelinesTemp;
	};

	

	class CTimer
		: public CRef
	{
		friend class CTimeline;

	public:
		CTimer( void );
		~CTimer() override;
		

	public:
		// 暂时只支持单个回调接口
		bool start( float fTimeout, bool bLoop, PTR_CUnknown pTimerHandler, PTimerProc pTimerProc );

		// 停止timer，将timer设置为disable，同时调用onDestroy
		// 如果想重用timer需要在stop后重新start，否则disable的timer将会被从系统中清除
		void stop();

		// 只是暂停timer，可以使用resume来恢复
		void pause();

		// 回复timer的运行
		void resume();

		// 缩放时间维度，该维度参数只是相对于所在时间线，并非绝对的维度缩放
		void scale( bool bEnable, float nScaleParams );
		

	private:
		// 
		void onRun( float fElapseTime );

		// Timer并不会立刻被析构，但会吧被绑定的handler给release掉
		// 暂时减缓handler和timer相互包含造成的内存泄漏的现象
		void onDestroy();


	public:
		float getTimeout()          { return m_fTimeout; }
		bool  IsEnable()            { return m_bEnabled; }
		bool  isLoop()              { return m_bLoop; }
		bool  isPause()             { return m_bPause; }
		bool  enableScale()         { return m_bEnableScale; }
		float getScaleParam()       { return m_nScale; }



	private:
		std::string m_TimerName;

		float   m_fTimeout;
		float   m_fCountdown;
		bool    m_bEnabled; // it will be destroy
		bool    m_bLoop;
		bool    m_bPause; // stop running temporarily

		bool    m_bEnableScale;
		float   m_nScale;

		PTR_CUnknown m_pTimerHandler;
		PTimerProc   m_pTimerFunc;


// 
// 	public:
// 		static Timer CreateTimer();
// 		static void  DestroyTimer( Timer timer );
// 		static void  RunSystem( float nElapsedTime );
// 		static bool  InitialSystem();
// 		static void  DestroySystem();
	};
	

// 	bool    IBERBAR_DLL InitialTimerSystem();
// 	void    IBERBAR_DLL DestroyTimerSystem();
// 	void    IBERBAR_DLL RunTimerSystem( float nElapsedTime );
// 
// 	CTimer* IBERBAR_DLL CreateTimer();
// 	void    IBERBAR_DLL StartTimer( CTimer* timer, float fTimeout, bool bLoop, PTR_CUnknown pTimerHandler, PTimerProc pTimerProc );
// 	void    IBERBAR_DLL PauseTimer( CTimer* timer );
// 	void    IBERBAR_DLL ResumeTimer( CTimer* timer );
// 	void    IBERBAR_DLL DestroyTimer( CTimer* timer );
// 
// #define SAFE_RELEASE_TIMER( timer )     { timer = NULL; }
// #define SAFE_DESTROY_TIMER( timer )     { DestroyTimer( timer ); timer = NULL; }
}
