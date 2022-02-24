#pragma once

#include <iberbar/Renderer/Types.h>
#include <iberbar/Renderer/RenderCommandQueue.h>
//#include <iberbar/Renderer/Processor/BaseRendererProcessor.h>
#include <iberbar/Utility/Result.h>
#include <stack>

namespace iberbar
{
	namespace RHI
	{
		class IDevice;
	}


	namespace Renderer
	{
		class CRenderCommand;
		class CTrianglesCommand;
		class CRenderGroupCommand;
		class CRenderGroupCommandManager;

		class CBaseRendererProcessor;
		class CDefaultRendererProcessor;



		class __iberbarRendererApi__ IRenderer abstract
		{
		public:
			IRenderer() {}
			virtual ~IRenderer() {}
		};



		class __iberbarRendererApi__ CRenderer2d
			: public IRenderer
		{
		public:
			CRenderer2d();
			virtual ~CRenderer2d();

		public:
			void Init( RHI::IDevice* pDevice );
			void AddCommand( CRenderCommand* pCommand );
			void AddCommand( CRenderCommand* pCommand, int nQueueId );
			void CleanupCommands();
			int CreateRenderQueue();
			void PushRenderQueue( int nRenderQueueId );
			void PopRenderQueue();
			CRenderQueue* GetRenderQueue( int nQueueId ) { return &m_RenderQueue[ nQueueId ]; }

			inline RHI::IDevice* GetRHIDevice() { return m_pDevice; }
			inline CRenderGroupCommandManager* GetRenderGroupCommandManager() { return m_pRenderGroupCommandManager; }

		public:
			void Render();
			void Clear();
			void OnRhiLost();
			CResult OnRhiReset();
			
		protected:
			void ProcessGroupCommand( CRenderCommand* pCommand );

		protected:
			std::vector<CRenderQueue> m_RenderQueue;
			std::stack<int> m_CommandGroupStack;
			bool m_bIsRendering;
			RHI::IDevice* m_pDevice;
			//RHI::IVertexBuffer* m_pVertexBuffer;
			//RHI::IIndexBuffer* m_pIndexBuffer;
			//CRenderer2dState* m_pState;
			CRenderGroupCommandManager* m_pRenderGroupCommandManager;
			//std::function<void()> m_CallbackFinish;
			CBaseRendererProcessor* m_pCurrentRendererProcessor;
			CDefaultRendererProcessor* m_pDefaultRendererProcessor;


		private:
			static CRenderer2d* sm_pInstance;
		public:
			static CRenderer2d* sGetInstance() { return sm_pInstance; }
		};
	}
}
