#pragma once

#include <iberbar/Renderer/RenderCommand.h>
#include <unordered_map>

namespace iberbar
{
	namespace Renderer
	{
		class CRenderer2d;
		class CBaseRendererProcessor;

		class __iberbarRendererApi__ CRenderGroupCommandManager
		{
		public:
			CRenderGroupCommandManager( CRenderer2d* pRenderer );
			~CRenderGroupCommandManager();

			int AllocQueueId();
			void ReleaseQueueId( int nQueueId );

		private:
			CRenderer2d* m_pRenderer;
			std::unordered_map<int, bool> m_QueueIdMapping;
			std::vector<int> m_UnusedQueueId;
		};

		class __iberbarRendererApi__ CRenderGroupCommand
			: public CRenderCommand
		{
		public:
			CRenderGroupCommand( CRenderGroupCommandManager* pManager );
			CRenderGroupCommand( const CRenderGroupCommand& Other );
			virtual ~CRenderGroupCommand();

			void SetRendererProcessor( CBaseRendererProcessor* pRendererProcessor ) { m_pRendererProcessor = pRendererProcessor; }
			CBaseRendererProcessor* GetRendererProcessor() { return m_pRendererProcessor; }
			int GetQueueId() const { return m_nQueueId; }

			CRenderGroupCommand& operator=( const CRenderGroupCommand& Command ) = delete;

		protected:
			CRenderGroupCommandManager* m_pGroupCommandManager;
			int m_nQueueId;
			CBaseRendererProcessor* m_pRendererProcessor;
		};
	}
}