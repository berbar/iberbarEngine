

#include <iberbar/GameEngine/Headers.h>
#include <iberbar/Utility/Unknown.h>
#include <functional>


namespace iberbar
{

	namespace Game
	{
		class CTimer;
		class CTimerEasySystem;

		typedef void UFunction_TimerExecute( CTimer* pTimer );

		class __iberbarGameEngineApi__ CTimer
			: public CRef
		{
		public:
			CTimer();
			~CTimer();

		public:
			// 暂时只支持单个回调接口
			bool Start( float fTimeout, bool bLoop );

			// 停止timer，将timer设置为disable，同时调用onDestroy
			// 如果想重用timer需要在stop后重新start，否则disable的timer将会被从系统中清除
			void Stop();

			// 只是暂停timer，可以使用resume来恢复
			void Pause();

			// 回复timer的运行
			void Resume();

			// 缩放时间维度，该维度参数只是相对于所在时间线，并非绝对的维度缩放
			void Scale( float nScaleParams );

			// 设置执行回调函数
			void SetCallbackExecute( std::function<UFunction_TimerExecute> Callback ) { m_CallbackExecute = Callback; }

			// 执行
			void Run( float nDelta );

			void SetId( const std::string& strId ) { m_strId = strId; }
			const std::string& GetId() const { return m_strId; }


		public:
			float GetTimeout() { return m_nTimeout; }
			bool  IsEnable() { return m_bEnabled; }
			bool  IsLoop() { return m_bLoop; }
			bool  IsPause() { return m_bPause; }
			float GetScaleParam() { return m_nScale; }



		private:
			bool m_bEnabled;
			bool m_bLoop;
			bool m_bPause;
			bool m_bRemoveSelf;
			float m_nTimeout;
			float m_nCountdown;
			float m_nScale;

			std::string m_strId;
			std::function<UFunction_TimerExecute> m_CallbackExecute;
		};


		class __iberbarGameEngineApi__ CTimerEasySystem
		{
		private:
			struct _TimerNode
			{
				bool bRemove;
				CTimer* pTimer;
			};

		public:
			CTimerEasySystem();
			~CTimerEasySystem();

		public:
			void Run( float nDelta );
			void AddTimer( CTimer* pTimer );
			bool FindTimer( const std::string& strId, CTimer** ppOutTimer );
			void RemoveTimer( CTimer* pTimer );

		private:
			std::list<_TimerNode> m_Timers;
		};
	}
}

