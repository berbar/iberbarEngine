#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Utility/Result.h>
#include <stack>

namespace iberbar
{
	namespace RHI
	{
		class IDevice;
		class ICommandContext;
		class IVertexBuffer;
		class IIndexBuffer;
	}


	namespace Renderer
	{
		class CRenderCommand;
		class CTrianglesCommand;
		class CRenderGroupCommand;
		class CRenderGroupCommandManager;
		class CRenderer2dState;

		typedef std::vector<CRenderCommand*> URenderCommandList;

		class __iberbarRendererApi__ CRenderQueue
		{
		public:
			enum class UQueueGroup
			{
				Zindex_Negative = 0,
				Zindex_Zero = 1,
				Zindex_Positive = 2,
				Count
			};

		public:
			void PushBack( CRenderCommand* command );
			void Sort();
			void Clear();
			const URenderCommandList& GetQueueGroup( UQueueGroup group ) { return m_CommandLists[ (int)group ]; }

		private:
			URenderCommandList m_CommandLists[ (int)UQueueGroup::Count ];
		};

		class __iberbarRendererApi__ CRenderer2d
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

			inline RHI::IDevice* GetRHIDevice() { return m_pDevice; }
			inline RHI::ICommandContext* GetRHIContext() { return m_pCommandContext; }
			inline CRenderGroupCommandManager* GetRenderGroupCommandManager() { return m_pRenderGroupCommandManager; }

		public:
			void Render();
			void Clear();
			void OnRhiLost();
			CResult OnRhiReset();
			
		protected:
			void VisitQueue( CRenderQueue& queue );
			void VisitCommandList( const URenderCommandList& commandList );
			void VisitCommand( CRenderCommand* pCommand );
			void DrawBatchTriangles();
			void DrawOneTriangles( CTrianglesCommand* pCommand );
			void Flush();
			void ProcessGroupCommand( CRenderGroupCommand* pCommand );

			void SetShaderBindings();

		protected:
			std::vector<CRenderQueue> m_RenderQueue;
			std::stack<int> m_CommandGroupStack;
			bool m_bIsRendering;
			RHI::IDevice* m_pDevice;
			RHI::ICommandContext* m_pCommandContext;
			RHI::IVertexBuffer* m_pVertexBuffer;
			RHI::IIndexBuffer* m_pIndexBuffer;
			CRenderer2dState* m_pState;
			CRenderGroupCommandManager* m_pRenderGroupCommandManager;
			//std::function<void()> m_CallbackFinish;
		};
	}
}
