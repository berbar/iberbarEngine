
#pragma once


#include <iberbar/Utility/Unknown.h>
#include <functional>


namespace iberbar
{
//
//#ifndef _ELYQ_FPS_
//#define _ELYQ_FPS_
//
//#define ELYQ_FPS_MS24  42
//#define ELYQ_FPS_S24   0.042f
//#define ELYQ_FPS_MS30  33
//#define ELYQ_FPS_S30   0.033f
//#define ELYQ_FPS_MS45  22
//#define ELYQ_FPS_S45   0.022f
//#define ELYQ_FPS_MS50  20
//#define ELYQ_FPS_S50   0.020f
//#define ELYQ_FPS_MS60  17
//#define ELYQ_FPS_S60   0.017f
//#define ELYQ_FPS_MS70  14
//#define ELYQ_FPS_S70   0.014f
//#define ELYQ_FPS_MS100 10
//#define ELYQ_FPS_S100  0.010f
//
//#endif
//
	static const int   FPS_MS30 = 33;
	static const float FPS_S30 = 0.033f;
	static const int   FPS_MS45 = 22;
	static const float FPS_S45 = 0.022f;
	static const int   FPS_MS50 = 20;
	static const float FPS_S50 = 0.020f;
	static const int   FPS_MS60 = 17;
	static const float FPS_S60 = 0.017f;
	static const int   FPS_MS70 = 14;
	static const float FPS_S70 = 0.014f;
	//static const int   FPS_MS100 = 10;
	//static const float FPS_S100 = 0.010f;

	class CGlobalTimer;
	IBERBAR_UNKNOWN_PTR_DECLARE( CGlobalTimer );

	class __iberbarUtilityApi__ CGlobalTimer
	{
	public:
		typedef void (UFunctionOnRun)( int64 nElapsedTimeMilliSecond, float nElapsedTimeSecond );

	public:
		CGlobalTimer( void );
		~CGlobalTimer();

	public:
		void Initial( bool accurate );
		void SetTimeout( int64 timeout ) { m_nMinSliceRun = timeout; }
		void SetFunctionOnRun( std::function<UFunctionOnRun> func ) { m_funcOnRun = func; }
		void Run();

	protected:
		int64 GetElapsedTime();

	protected:
		bool m_bInit;
		int64   m_nOneSecondTicks;
		int64   m_nTimeStart;

		int64   m_nTimePrevRun;
		int64   m_nMinSliceRun;

		int64   m_nLastSliceRun;

		std::function<UFunctionOnRun> m_funcOnRun;

		bool m_bUserLargeTime;
	};




}



