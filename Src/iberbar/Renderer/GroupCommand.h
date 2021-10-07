#pragma once

#include <iberbar/Renderer/RenderCommand.h>
#include <unordered_map>

namespace iberbar
{
	namespace Renderer
	{
		class CRenderer2d;

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

			int GetQueueId() const { return m_nQueueId; }

			CRenderGroupCommand& operator=( const CRenderGroupCommand& Command ) = delete;

		protected:
			CRenderGroupCommandManager* m_pGroupCommandManager;
			int m_nQueueId;
		};
	}
}